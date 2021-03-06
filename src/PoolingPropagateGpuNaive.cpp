// Copyright Hugh Perkins 2014 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License, 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#include <iostream>
#include <cstring>

#include "OpenCLHelper.h"

#include "StatefulTimer.h"
#include "stringhelper.h"

#include "PoolingPropagateGpuNaive.h"

//#include "test/PrintBuffer.h"

using namespace std;

#undef VIRTUAL
#define VIRTUAL 
#undef STATIC
#define STATIC

VIRTUAL PoolingPropagateGpuNaive::~PoolingPropagateGpuNaive() {
    delete kernel;
}
VIRTUAL void PoolingPropagateGpuNaive::propagate( int batchSize, CLWrapper *inputWrapper, CLWrapper *selectorsWrapper, CLWrapper *outputWrapper ) {
//    cout << StatefulTimer::instance()->prefix << "PoolingPropagateGpuNaive::propagate( CLWrapper * )" << endl;
    StatefulTimer::instance()->timeCheck("PoolingPropagateGpuNaive::propagate start" );

    kernel->input( batchSize )->input( inputWrapper )->output( selectorsWrapper )->output( outputWrapper );
    int globalSize = batchSize * numPlanes * outputImageSize * outputImageSize;
    int workgroupsize = cl->getMaxWorkgroupSize();
    globalSize = ( ( globalSize + workgroupsize - 1 ) / workgroupsize ) * workgroupsize;
//    cout << "PoolingPropagateGpuNaive::propagate batchsize=" << batchSize << " g=" << globalSize << " w=" << workgroupsize << endl;
    kernel->run_1d(globalSize, workgroupsize);
    cl->finish();

//    cout << "PoolingPropagateGpuNaive::propagate selectorswrapper:" << endl;
//    PrintBuffer::printInts( cl, selectorsWrapper, outputImageSize, outputImageSize );

    StatefulTimer::instance()->timeCheck("PoolingPropagateGpuNaive::propagate end" );
}
PoolingPropagateGpuNaive::PoolingPropagateGpuNaive( OpenCLHelper *cl, bool padZeros, int numPlanes, int inputImageSize, int poolingSize ) :
        PoolingPropagate( cl, padZeros, numPlanes, inputImageSize, poolingSize ) {
    string options = "";
    options += " -DgOutputImageSize=" + toString( outputImageSize );
    options += " -DgOutputImageSizeSquared=" + toString( outputImageSize * outputImageSize );
    options += " -DgInputImageSize=" + toString( inputImageSize );
    options += " -DgInputImageSizeSquared=" + toString( inputImageSize * inputImageSize );
    options += " -DgPoolingSize=" + toString( poolingSize );
    options += " -DgNumPlanes=" + toString( numPlanes );

    // [[[cog
    // import stringify
    // stringify.write_kernel2( "kernel", "cl/pooling.cl", "propagateNaive", 'options' )
    // ]]]
    // generated using cog, from cl/pooling.cl:
    const char * kernelSource =  
    "// Copyright Hugh Perkins 2014 hughperkins at gmail\n" 
    "//\n" 
    "// This Source Code Form is subject to the terms of the Mozilla Public License,\n" 
    "// v. 2.0. If a copy of the MPL was not distributed with this file, You can\n" 
    "// obtain one at http://mozilla.org/MPL/2.0/.\n" 
    "\n" 
    "// every plane is independent\n" 
    "// every example is independent\n" 
    "// so, globalid can be: [n][plane][outputRow][outputCol]\n" 
    "kernel void propagateNaive( const int batchSize, global const float *input, global int *selectors, global float *output ) {\n" 
    "    const int globalId = get_global_id(0);\n" 
    "\n" 
    "    const int intraImageOffset = globalId % gOutputImageSizeSquared;\n" 
    "    const int outputRow = intraImageOffset / gOutputImageSize;\n" 
    "    const int outputCol = intraImageOffset % gOutputImageSize;\n" 
    "\n" 
    "    const int image2dIdx = globalId / gOutputImageSizeSquared;\n" 
    "    const int plane = image2dIdx % gNumPlanes;\n" 
    "    const int n = image2dIdx / gNumPlanes;\n" 
    "\n" 
    "    if( n >= batchSize ) {\n" 
    "        return;\n" 
    "    }\n" 
    "\n" 
    "    const int inputRow = outputRow * gPoolingSize;\n" 
    "    const int inputCol = outputCol * gPoolingSize;\n" 
    "    const int inputImageOffset = ( n * gNumPlanes + plane ) * gInputImageSizeSquared;\n" 
    "    int selector = 0;\n" 
    "    int poolInputOffset = inputImageOffset + inputRow * gInputImageSize + inputCol;\n" 
    "    float maxValue = input[ poolInputOffset ];\n" 
    "    for( int dRow = 0; dRow < gPoolingSize; dRow++ ) {\n" 
    "        for( int dCol = 0; dCol < gPoolingSize; dCol++ ) {\n" 
    "            bool process = ( inputRow + dRow < gInputImageSize ) && ( inputCol + dCol < gInputImageSize );\n" 
    "            if( process ) {\n" 
    "                float thisValue = input[ poolInputOffset + dRow * gInputImageSize + dCol ];\n" 
    "                if( thisValue > maxValue ) {\n" 
    "                    maxValue = thisValue;\n" 
    "                    selector = dRow * gPoolingSize + dCol;\n" 
    "                }\n" 
    "            }\n" 
    "        }\n" 
    "    }\n" 
    "    output[ globalId ] = maxValue;\n" 
    "    selectors[ globalId ] = selector;\n" 
    "//    selectors[globalId] = 123;\n" 
    "}\n" 
    "\n" 
    "";
    kernel = cl->buildKernelFromString( kernelSource, "propagateNaive", options, "cl/pooling.cl" );
    // [[[end]]]
//    kernel = cl->buildKernel( "pooling.cl", "propagateNaive", options );
}

