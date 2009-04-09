/*
Copyright (c) 2008 NetAllied Systems GmbH

This file is part of COLLADAFramework.

Licensed under the MIT Open Source License, 
for details please see LICENSE file or the website
http://www.opensource.org/licenses/mit-license.php
*/

#ifndef __COLLADAFW_ANIMATION_H__
#define __COLLADAFW_ANIMATION_H__

#include "COLLADAFWPrerequisites.h"
#include "COLLADAFWObject.h"


namespace COLLADAFW
{

	/** The base class for animation. Currently we only support animation curves, but in future probably 
	formulas, too. */
	class Animation : public ObjectTemplate<COLLADA_TYPE::ANIMATION>
	{
	public:
		enum AnimationType
		{
			ANIMATION_CURVE,
			ANIMATION_FORMULA   // for the future
		};

	private:
		/** The type of the animation.*/
		AnimationType mAnimationType;

	public:

		/** Destructor. */
		virtual ~Animation(){}

		/** The type of the animation.*/
		AnimationType getAnimationType() const { return mAnimationType; }

	protected:
		/** Constructor. */
		Animation( ObjectId objectId, AnimationType animationType)
			: ObjectTemplate<COLLADA_TYPE::ANIMATION>(objectId)
			, mAnimationType(animationType)
		{}

	private:

		/** Disable default copy ctor. */
		Animation( const Animation& pre );

		/** Disable default assignment operator. */
		const Animation& operator= ( const Animation& pre );

	};

} // namespace COLLADAFW

#endif // __COLLADAFW_ANIMATION_H__
