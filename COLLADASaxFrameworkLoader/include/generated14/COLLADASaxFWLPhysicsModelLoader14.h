/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of COLLADASaxFrameworkLoader.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/


#ifndef __COLLADASAXFWL_PHYSICSMODELLOADER14_H__
#define __COLLADASAXFWL_PHYSICSMODELLOADER14_H__


#include "COLLADASaxFWLPrerequisites.h"
#include "COLLADASaxFWLPhysicsModelLoader.h"
#include "COLLADASaxFWLIParserImpl14.h"


namespace COLLADASaxFWL
{


class IFilePartLoader;


class PhysicsModelLoader14 : public IParserImpl14
{
private:
PhysicsModelLoader* mLoader;

public:
PhysicsModelLoader14(PhysicsModelLoader* loader)
 : mLoader(loader)
{}


virtual bool begin__physics_model( const COLLADASaxFWL14::physics_model__AttributeData& attributeData );

virtual bool end__physics_model();

virtual bool begin__rigid_body( const COLLADASaxFWL14::rigid_body__AttributeData& attributeData );

virtual bool end__rigid_body();

virtual bool begin__rigid_body__technique_common();

virtual bool end__rigid_body__technique_common();

virtual bool begin__rigid_body__technique_common__dynamic( const COLLADASaxFWL14::rigid_body__technique_common__dynamic__AttributeData& attributeData );

virtual bool end__rigid_body__technique_common__dynamic();

virtual bool data__rigid_body__technique_common__dynamic( bool value );

virtual bool begin__mass( const COLLADASaxFWL14::mass__AttributeData& attributeData );

virtual bool end__mass();

virtual bool data__mass( float value );

virtual bool begin__rigid_body__technique_common__mass_frame(){return true; }

virtual bool end__rigid_body__technique_common__mass_frame(){return true; }

virtual bool begin__inertia( const COLLADASaxFWL14::inertia__AttributeData& attributeData ){return true; }

virtual bool end__inertia(){return true; }

virtual bool data__inertia( const float* value, size_t length ){return true; }

virtual bool begin__instance_physics_material( const COLLADASaxFWL14::instance_physics_material__AttributeData& attributeData ){return true; }

virtual bool end__instance_physics_material(){return true; }

virtual bool begin__rigid_body__technique_common__shape(){return true; }

virtual bool end__rigid_body__technique_common__shape(){return true; }

virtual bool begin__rigid_body__technique_common__shape__hollow( const COLLADASaxFWL14::rigid_body__technique_common__shape__hollow__AttributeData& attributeData ){return true; }

virtual bool end__rigid_body__technique_common__shape__hollow(){return true; }

virtual bool data__rigid_body__technique_common__shape__hollow(  bool value ){return true; }

virtual bool begin__density( const COLLADASaxFWL14::density__AttributeData& attributeData ){return true; }

virtual bool end__density(){return true; }

virtual bool data__density(  float value ){return true; }

virtual bool begin__plane(){return true; }

virtual bool end__plane(){return true; }

virtual bool begin__equation(){return true; }

virtual bool end__equation(){return true; }

virtual bool data__equation( const float* data, size_t length ){return true; }

virtual bool begin__box(){return true; }

virtual bool end__box(){return true; }

virtual bool begin__half_extents(){return true; }

virtual bool end__half_extents(){return true; }

virtual bool data__half_extents( const float* data, size_t length ){return true; }

virtual bool begin__sphere(){return true; }

virtual bool end__sphere(){return true; }

virtual bool begin__radius____float(){return true; }

virtual bool end__radius____float(){return true; }

virtual bool data__radius____float(  float value ){return true; }

virtual bool begin__cylinder(){return true; }

virtual bool end__cylinder(){return true; }

virtual bool begin__height(){return true; }

virtual bool end__height(){return true; }

virtual bool data__height(  float value ){return true; }

virtual bool begin__radius____float2(){return true; }

virtual bool end__radius____float2(){return true; }

virtual bool data__radius____float2( const float* data, size_t length ){return true; }

virtual bool begin__capsule(){return true; }

virtual bool end__capsule(){return true; }

virtual bool begin__radius____float3(){return true; }

virtual bool end__radius____float3(){return true; }

virtual bool data__radius____float3( const float* data, size_t length ){return true; }

virtual bool begin__instance_geometry( const COLLADASaxFWL14::instance_geometry__AttributeData& attributeData ){return true; }

virtual bool end__instance_geometry(){return true; }

virtual bool begin__rigid_constraint( const COLLADASaxFWL14::rigid_constraint__AttributeData& attributeData ){return true; }

virtual bool end__rigid_constraint(){return true; }

virtual bool begin__ref_attachment( const COLLADASaxFWL14::ref_attachment__AttributeData& attributeData ){return true; }

virtual bool end__ref_attachment(){return true; }

virtual bool begin__attachment( const COLLADASaxFWL14::attachment__AttributeData& attributeData ){return true; }

virtual bool end__attachment(){return true; }

virtual bool begin__translate( const COLLADASaxFWL14::translate__AttributeData& attributeData ){return true; }

virtual bool end__translate(){return true; }

virtual bool data__translate( const float* data, size_t length ){return true; }

virtual bool begin__rotate( const COLLADASaxFWL14::rotate__AttributeData& attributeData ){return true; }

virtual bool end__rotate(){return true; }

virtual bool data__rotate( const float* data, size_t length ){return true; }

virtual bool begin__rigid_constraint__technique_common(){return true; }

virtual bool end__instance_rigid_body__technique_common(){return true; }

virtual bool begin__enabled( const COLLADASaxFWL14::enabled__AttributeData& attributeData ){return true; }

virtual bool end__enabled(){return true; }

virtual bool data__enabled(  bool value ){return true; }

virtual bool begin__interpenetrate( const COLLADASaxFWL14::interpenetrate__AttributeData& attributeData ){return true; }

virtual bool end__interpenetrate(){return true; }

virtual bool data__interpenetrate(  bool value ){return true; }

virtual bool begin__limits(){return true; }

virtual bool end__limits(){return true; }

virtual bool begin__swing_cone_and_twist(){return true; }

virtual bool end__swing_cone_and_twist(){return true; }

virtual bool begin__min( const COLLADASaxFWL14::min__AttributeData& attributeData ){return true; }

virtual bool end__min(){return true; }

virtual bool data__min( const float* value, size_t length ){return true; }

virtual bool begin__max( const COLLADASaxFWL14::max__AttributeData& attributeData ){return true; }

virtual bool end__max(){return true; }

virtual bool data__max( const float* value, size_t length ){return true; }

virtual bool begin__spring(){return true; }

virtual bool end__spring(){return true; }

virtual bool begin__limits__linear(){return true; }

virtual bool end__limits__linear(){return true; }

virtual bool begin__angular(){return true; }

virtual bool end__angular(){return true; }

virtual bool begin__stiffness( const COLLADASaxFWL14::stiffness__AttributeData& attributeData ){return true; }

virtual bool end__stiffness(){return true; }

virtual bool data__stiffness(  float value ){return true; }

virtual bool begin__damping( const COLLADASaxFWL14::damping__AttributeData& attributeData ){return true; }

virtual bool end__damping(){return true; }

virtual bool data__damping(  float value ){return true; }

virtual bool begin__target_value( const COLLADASaxFWL14::target_value__AttributeData& attributeData ){return true; }

virtual bool end__target_value(){return true; }

virtual bool data__target_value(  float value ){return true; }

virtual bool begin__spring__linear(){return true; }

virtual bool end__spring__linear(){return true; }

virtual bool begin__instance_physics_model( const COLLADASaxFWL14::instance_physics_model__AttributeData& attributeData ){return true; }

virtual bool end__instance_physics_model(){return true; }

virtual bool begin__instance_force_field( const COLLADASaxFWL14::instance_force_field__AttributeData& attributeData ){return true; }

virtual bool end__instance_force_field(){return true; }

virtual bool begin__instance_rigid_body( const COLLADASaxFWL14::instance_rigid_body__AttributeData& attributeData ){return true; }

virtual bool end__instance_rigid_body(){return true; }

virtual bool begin__angular_velocity(){return true; }

virtual bool end__angular_velocity(){return true; }

virtual bool data__angular_velocity( const float* data, size_t length ){return true; }

virtual bool begin__velocity(){return true; }

virtual bool end__velocity(){return true; }

virtual bool data__velocity( const float* data, size_t length ){return true; }

virtual bool begin__instance_rigid_body__technique_common__dynamic( const COLLADASaxFWL14::instance_rigid_body__technique_common__dynamic__AttributeData& attributeData ){return true; }

virtual bool end__instance_rigid_body__technique_common__dynamic(){return true; }

virtual bool data__instance_rigid_body__technique_common__dynamic(  bool value ){return true; }

virtual bool begin__instance_rigid_body__technique_common__mass_frame(){return true; }

virtual bool end__instance_rigid_body__technique_common__mass_frame(){return true; }

virtual bool begin__instance_rigid_body__technique_common__shape(){return true; }

virtual bool end__instance_rigid_body__technique_common__shape(){return true; }

virtual bool begin__instance_rigid_body__technique_common__shape__hollow( const COLLADASaxFWL14::instance_rigid_body__technique_common__shape__hollow__AttributeData& attributeData ){return true; }

virtual bool end__instance_rigid_body__technique_common__shape__hollow(){return true; }

virtual bool data__instance_rigid_body__technique_common__shape__hollow(  bool value ){return true; }

virtual bool begin__instance_rigid_constraint( const COLLADASaxFWL14::instance_rigid_constraint__AttributeData& attributeData ){return true; }



private:
/** Disable default copy ctor. */
PhysicsModelLoader14(const PhysicsModelLoader14&);
/** Disable default assignment operator. */
const PhysicsModelLoader14& operator=(const PhysicsModelLoader14&);
};
}
#endif // __COLLADASAXFWL_PHYSICSMODELLOADER14_H__
