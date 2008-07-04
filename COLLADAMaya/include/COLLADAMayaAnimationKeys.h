/*
    Copyright (c) 2008 NetAllied Systems GmbH
	
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __COLLADA_MAYA_ANIMATION_KEYS_H__
#define __COLLADA_MAYA_ANIMATION_KEYS_H__

#include "ColladaMayaPrerequisites.h"
#include "COLLADAMayaTangentPoint.h"
#include "COLLADALibraryAnimations.h"

namespace COLLADAMaya
{
    /**
    * This file contains the AnimationKey, AnimationKeyBezier and AnimationKeyTCB classes.
    */

    // ----------------------------------------------------------
    /**
    This class is the base for every animation key.
    */

    class BaseAnimationKey
    {

    public:
        BaseAnimationKey() {}

        /** The key input. Typically, this will be a time value, in seconds.
        For driven curves, the dimension of this value will depend on the driver. */
        float input;

        /** The key interpolation type. */
        COLLADA::LibraryAnimations::InterpolationType interpolation;
    };

    // ----------------------------------------------------------
    /**
    A simple animation key.
    This class is the base for the more complex one-dimensional keys
    and it is used directly for linear and step keys.

    Do not create directly.
    Instead call AnimationCurve::AddKey(LINEAR)
    or AnimationCurve::AddKey(STEP).
    */

    class AnimationKey : public BaseAnimationKey
    {

    public:
        AnimationKey() {}

        /** The key output. */
        float output;
    };

    /** Animation key list */
    typedef std::vector<AnimationKey*> AnimationKeyList;

    // ----------------------------------------------------------
    /**
    An animation key with tangents values.
    This class is used for bezier keys and soon: for hermite keys as well.

    Do not create directly.
    Instead call AnimationCurve::AddKey(BEZIER).
    */

    class AnimationKeyBezier : public AnimationKey
    {

    public:
        TangentPoint inTangent;  /**< The incoming tangent value. */
        TangentPoint outTangent; /**< The outcoming tangent value. */
    };

    // ----------------------------------------------------------
    /**
    An animation key with tension, continuity and bias values.
    This class is used for 3dsMax TCB keys.

    Do not create directly.
    Instead call AnimationCurve::AddKey(TCB).
    */

    class AnimationKeyTCB : public AnimationKey
    {

    public:
        float tension;  /**< The tension. */
        float continuity; /**< The continuity. */
        float bias;   /**< The bias. */

        float easeIn;  /**< The ease-in factor. */
        float easeOut;  /**< The ease-out factor. */
    };

    // ----------------------------------------------------------
    /**
    A simple multi-dimensional animation key.
    This class is the base for the more complex multi-dimensional keys
    and it is used directly for linear and step multi-dimensional keys.
    */

    class AnimationMKey : public BaseAnimationKey
    {

    private:
        uint dimension;

    public:
        /** Constructor. Do not use directly.
        Instead call AnimationMultiCurve::AddKey(FUDaeInterpolation::LINEAR)
        or AnimationMultiCurve::AddKey(FUDaeInterpolation::STEP).
        @param dimension The number of dimension to the key output. */
        AnimationMKey ( uint dimension );

        /** Destructor. */
        virtual ~AnimationMKey();

        /** Retrieves the number of dimensions for this key.
        @return The number of dimensions. */
        uint getDimension() const
        {
            return dimension;
        };

        /** The multi-dimensional key output. */
        float* output;
    };

    /** A array of multi-dimensional animation keys. */
    typedef std::vector<AnimationMKey*> AnimationMKeyList;

    // ----------------------------------------------------------
    /**
    A multi-dimensional animation key with tangents values.
    This class is used for bezier keys and soon: for hermite keys as well.
    */

    class AnimationMKeyBezier : public AnimationMKey
    {

    public:
        /** Constructor: do not use directly.
        Instead call AnimationCurve::AddKey(FUDaeInterpolation::BEZIER).
        @param dimension The number of dimension to the key output. */
        AnimationMKeyBezier ( uint dimension );

        /** Destructor. */
        virtual ~AnimationMKeyBezier();

        TangentPoint* inTangent; /**< The incoming tangent value. */
        TangentPoint* outTangent; /**< The outcoming tangent value. */
    };

    // ----------------------------------------------------------
    /**
    An animation key with tension, continuity and bias values.
    This class is used for 3dsMax TCB keys.
    */

    class AnimationMKeyTCB : public AnimationMKey
    {

    public:
        /** Constructor: do not use directly.
        Instead call AnimationMultiCurve::AddKey(FUDaeInterpolation::TCB).
        @param dimension The number of dimension to the key output. */
        AnimationMKeyTCB ( uint dimension );

        /** Destructor. */
        virtual ~AnimationMKeyTCB();

        float* tension; /**< The multi-dimensional tensions. */
        float* continuity; /**< The multi-dimensional continuities. */
        float* bias; /**< The multi-dimensional biases. */

        float* easeIn; /**< The multi-dimensional ease-in factors. */
        float* easeOut; /**< The multi-dimensional ease-out factors. */
    };

}

#endif // __COLLADA_MAYA_ANIMATION_KEYS_H__

