// Copyright Hugh Perkins 2015 hughperkins at gmail
//
// This Source Code Form is subject to the terms of the Mozilla Public License, 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "Layer.h"
#include "ActivationFunction.h"
#include "stringhelper.h"

#define VIRTUAL virtual

class ForceBackpropLayerMaker;

// This layer is very simple.  It has one role in life: it forces the layer after it
// to do backprop of gradients, to this layer
// otherwise, it is just a 'pass-thru' layer, does nothing
// It's usecase is very simple: in benchmarking, we can put this layer just
// before the layer we want to benchmark, to force that layer to calc
// gradients for this layer
//
// It runs only on cpu, since it will sit just after the input layer, which is
// cpu too
class ForceBackpropLayer : public Layer, IHasToString {
public:
    const int outputPlanes;
    const int outputImageSize;

    int batchSize;
    int allocatedSize;
    float *results;

    inline int getResultIndex( int n, int outPlane, int outRow, int outCol ) const {
        return ( ( n
            * outputPlanes + outPlane )
            * outputImageSize + outRow )
            * outputImageSize + outCol;
    }
    inline float getResult( int n, int outPlane, int outRow, int outCol ) const {
        return results[ getResultIndex(n,outPlane, outRow, outCol ) ];
    }

    // [[[cog
    // import cog_addheaders
    // cog_addheaders.add()
    // ]]]
    // generated, using cog:
    ForceBackpropLayer( Layer *previousLayer, ForceBackpropLayerMaker *maker );
    VIRTUAL ~ForceBackpropLayer();
    VIRTUAL std::string getClassName() const;
    VIRTUAL float *getResults();
    VIRTUAL ActivationFunction const *getActivationFunction();
    VIRTUAL int getPersistSize() const;
    VIRTUAL bool needsBackProp();
    VIRTUAL void printOutput() const;
    VIRTUAL void print() const;
    VIRTUAL bool needErrorsBackprop();
    VIRTUAL void setBatchSize( int batchSize );
    VIRTUAL void propagate();
    VIRTUAL void backPropErrors( float learningRate, float const *errors );
    VIRTUAL int getOutputImageSize() const;
    VIRTUAL int getOutputPlanes() const;
    VIRTUAL int getOutputCubeSize() const;
    VIRTUAL int getResultsSize() const;
    VIRTUAL std::string toString();
    VIRTUAL std::string asString() const;

    // [[[end]]]
};

std::ostream &operator<<( std::ostream &os, ForceBackpropLayer &layer );
std::ostream &operator<<( std::ostream &os, ForceBackpropLayer const*layer );

