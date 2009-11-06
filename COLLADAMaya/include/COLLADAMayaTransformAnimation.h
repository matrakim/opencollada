/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of COLLADAMaya.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#ifndef __COLLADAMAYA_TRANSFORMANIMATION_H__
#define __COLLADAMAYA_TRANSFORMANIMATION_H__

#include "COLLADAMayaPrerequisites.h"
#include "COLLADAMayaBaseAnimation.h"
#include "COLLADAMayaMayaTransform.h"

#include "COLLADAFWTransformation.h"


namespace COLLADAMaya
{

    /** Stores the transform node id, the mayaTransform node and the transformation type. */
    class TransformAnimation : public BaseAnimation
    {
	private:
        
        const COLLADAFW::Transformation* mTransformation;

        /** Need to know, which rotation is animted. */
        MayaTransform::TransformPhase mTransformPhase;

        bool mIsJointTransform;

    public:

        /** Constructor. */
		TransformAnimation();

        /** Destructor. */
		virtual ~TransformAnimation();

        const COLLADAFW::Transformation* getTransformation () const { return mTransformation; }
        void setTransformation ( const COLLADAFW::Transformation* val );
        const COLLADAFW::Transformation::TransformationType getTransformType () const { return mTransformation->getTransformationType (); }

        const MayaTransform::TransformPhase& getTransformPhase () const { return mTransformPhase; }
        void setTransformPhase ( const MayaTransform::TransformPhase& val ) { mTransformPhase = val; }

        const bool& getIsJointTransform () const { return mIsJointTransform; }
        void setIsJointTransform ( const bool& val ) { mIsJointTransform = val; }

	};

} // namespace COLLADAMAYA

#endif // __COLLADAMAYA_TRANSFORMANIMATION_H__
