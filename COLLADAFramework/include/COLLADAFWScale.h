/*
Copyright (c) 2008 NetAllied Systems GmbH

This file is part of COLLADAFramework.

Licensed under the MIT Open Source License, 
for details please see LICENSE file or the website
http://www.opensource.org/licenses/mit-license.php
*/

#ifndef __COLLADAFW_SCALE_H__
#define __COLLADAFW_SCALE_H__

#include "COLLADAFWPrerequisites.h"
#include "COLLADAFWTransformation.h"
#include "COLLADAFWMathVector3.h"


namespace COLLADAFW
{
	class Scale : public Transformation 
	{
	private:
		/** Vector representing the scales in three directions.*/
		Math::Vector3 mScaleVector;
	
	public:
		/** Constructor. Creates identity scale.*/
		Scale();

		/** Constructor. Sets scale to @a scaleVector.*/ 
		Scale(const Math::Vector3& scaleVector);

		/** Constructor. Sets the components of the scale to @a x, @a y, @a z.*/
		Scale(double x, double y, double z);

		virtual ~Scale();

		/** Returns the scale vector.*/
		const Math::Vector3& getScale() const { return mScaleVector; }

		/** Sets the scale vector.*/
		void setScale(const Math::Vector3& scaleVector) { mScaleVector = scaleVector; }

		/**  Sets the components of the scale to @a x, @a y, @a z.*/
		void setScale(double x, double y, double z) { mScaleVector = Math::Vector3(x, y, z); }
	};

} // namespace COLLADAFW

#endif // __COLLADAFW_SCALE_H__
