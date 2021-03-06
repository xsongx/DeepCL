#pragma once

#include "BackpropErrorsv2.h"
#include "OpenCLHelper.h"

#define STATIC static
#define VIRTUAL virtual

class BackpropErrorsv2Cached : public BackpropErrorsv2 {
public:
    CLKernel *kernel;
    CLKernel *applyActivationDeriv;

    // [[[cog
    // import cog_addheaders
    // cog_addheaders.add()
    // ]]]
    // generated, using cog:
    VIRTUAL ~BackpropErrorsv2Cached();
    VIRTUAL void backpropErrors( int batchSize,
    CLWrapper *inputDataWrapper, CLWrapper *errorsWrapper, CLWrapper *weightsWrapper,
    CLWrapper *errorsForUpstreamWrapper );
    BackpropErrorsv2Cached( OpenCLHelper *cl, LayerDimensions dim, ActivationFunction const *upstreamFn );

    // [[[end]]]
};

