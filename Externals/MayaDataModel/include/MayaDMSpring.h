/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_SPRING_H__
#define __MayaDM_SPRING_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMShape.h"
namespace MayaDM
{
class Spring : public Shape
{
public:
	struct IdMapping{
		void write(FILE* file) const
		{
		}
	};
public:
	Spring():Shape(){}
	Spring(FILE* file,const std::string& name,const std::string& parent="",bool create=true)
		:Shape(file, name, parent, "spring", create){}
	virtual ~Spring(){}
	void setEnd1Weight(double f)
	{
		if(f == 1) return;
		fprintf(mFile,"\tsetAttr \".f\" %f;\n", f);

	}
	void setEnd2Weight(double t)
	{
		if(t == 1) return;
		fprintf(mFile,"\tsetAttr \".t\" %f;\n", t);

	}
	void setUseStiffnessPS(bool usps)
	{
		if(usps == true) return;
		fprintf(mFile,"\tsetAttr \".usps\" %i;\n", usps);

	}
	void setUseDampingPS(bool udps)
	{
		if(udps == true) return;
		fprintf(mFile,"\tsetAttr \".udps\" %i;\n", udps);

	}
	void setUseRestLengthPS(bool urps)
	{
		if(urps == true) return;
		fprintf(mFile,"\tsetAttr \".urps\" %i;\n", urps);

	}
	void setStiffness(double s)
	{
		if(s == 1) return;
		fprintf(mFile,"\tsetAttr \".s\" %f;\n", s);

	}
	void setDamping(double d)
	{
		if(d == .2) return;
		fprintf(mFile,"\tsetAttr \".d\" %f;\n", d);

	}
	void setRestLength(double r)
	{
		if(r == 0) return;
		fprintf(mFile,"\tsetAttr \".r\" %f;\n", r);

	}
	void setStiffnessPS(const doubleArray& sps)
	{
		if(sps.size == 0) return;
		fprintf(mFile,"\tsetAttr \".sps\" -type \"doubleArray\" ");
		sps.write(mFile);
		fprintf(mFile,";\n");

	}
	void setDampingPS(const doubleArray& dps)
	{
		if(dps.size == 0) return;
		fprintf(mFile,"\tsetAttr \".dps\" -type \"doubleArray\" ");
		dps.write(mFile);
		fprintf(mFile,";\n");

	}
	void setRestLengthPS(const doubleArray& rps)
	{
		if(rps.size == 0) return;
		fprintf(mFile,"\tsetAttr \".rps\" -type \"doubleArray\" ");
		rps.write(mFile);
		fprintf(mFile,";\n");

	}
	void setObjectPositions(size_t opos_i,const vectorArray& opos)
	{
		if(opos.size == 0) return;
		fprintf(mFile,"\tsetAttr \".opos[%i]\" -type \"vectorArray\" ",opos_i);
		opos.write(mFile);
		fprintf(mFile,";\n");

	}
	void setObjectPositions(size_t opos_start,size_t opos_end,vectorArray* opos)
	{
		fprintf(mFile,"\tsetAttr \".opos[%i:%i]\" ", opos_start,opos_end);
		size_t size = (opos_end-opos_start)*1+1;
		for(size_t i=0;i<size;++i)
		{
			opos[i].write(mFile);
			fprintf(mFile,"\n");
		}
		fprintf(mFile,";\n");

	}
	void startObjectPositions(size_t opos_start,size_t opos_end)const
	{
		fprintf(mFile,"\tsetAttr \".opos[%i:%i]\"",opos_start,opos_end);
		fprintf(mFile," -type \"vectorArray\" ");

	}
	void appendObjectPositions(const vectorArray& opos)const
	{
		fprintf(mFile,"\n");
		opos.write(mFile);

	}
	void endObjectPositions()const
	{
		fprintf(mFile,";\n");

	}
	void setObjectVelocities(size_t ovel_i,const vectorArray& ovel)
	{
		if(ovel.size == 0) return;
		fprintf(mFile,"\tsetAttr \".ovel[%i]\" -type \"vectorArray\" ",ovel_i);
		ovel.write(mFile);
		fprintf(mFile,";\n");

	}
	void setObjectVelocities(size_t ovel_start,size_t ovel_end,vectorArray* ovel)
	{
		fprintf(mFile,"\tsetAttr \".ovel[%i:%i]\" ", ovel_start,ovel_end);
		size_t size = (ovel_end-ovel_start)*1+1;
		for(size_t i=0;i<size;++i)
		{
			ovel[i].write(mFile);
			fprintf(mFile,"\n");
		}
		fprintf(mFile,";\n");

	}
	void startObjectVelocities(size_t ovel_start,size_t ovel_end)const
	{
		fprintf(mFile,"\tsetAttr \".ovel[%i:%i]\"",ovel_start,ovel_end);
		fprintf(mFile," -type \"vectorArray\" ");

	}
	void appendObjectVelocities(const vectorArray& ovel)const
	{
		fprintf(mFile,"\n");
		ovel.write(mFile);

	}
	void endObjectVelocities()const
	{
		fprintf(mFile,";\n");

	}
	void setObjectMass(size_t omas_i,const doubleArray& omas)
	{
		if(omas.size == 0) return;
		fprintf(mFile,"\tsetAttr \".omas[%i]\" -type \"doubleArray\" ",omas_i);
		omas.write(mFile);
		fprintf(mFile,";\n");

	}
	void setObjectMass(size_t omas_start,size_t omas_end,doubleArray* omas)
	{
		fprintf(mFile,"\tsetAttr \".omas[%i:%i]\" ", omas_start,omas_end);
		size_t size = (omas_end-omas_start)*1+1;
		for(size_t i=0;i<size;++i)
		{
			omas[i].write(mFile);
			fprintf(mFile,"\n");
		}
		fprintf(mFile,";\n");

	}
	void startObjectMass(size_t omas_start,size_t omas_end)const
	{
		fprintf(mFile,"\tsetAttr \".omas[%i:%i]\"",omas_start,omas_end);
		fprintf(mFile," -type \"doubleArray\" ");

	}
	void appendObjectMass(const doubleArray& omas)const
	{
		fprintf(mFile,"\n");
		omas.write(mFile);

	}
	void endObjectMass()const
	{
		fprintf(mFile,";\n");

	}
	void setValidIndex(const intArray& vali)
	{
		if(vali.size == 0) return;
		fprintf(mFile,"\tsetAttr \".vali\" -type \"intArray\" ");
		vali.write(mFile);
		fprintf(mFile,";\n");

	}
	void setObject0(const intArray& obz)
	{
		if(obz.size == 0) return;
		fprintf(mFile,"\tsetAttr \".obz\" -type \"intArray\" ");
		obz.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPoint0(const intArray& ptz)
	{
		if(ptz.size == 0) return;
		fprintf(mFile,"\tsetAttr \".ptz\" -type \"intArray\" ");
		ptz.write(mFile);
		fprintf(mFile,";\n");

	}
	void setObject1(const intArray& obo)
	{
		if(obo.size == 0) return;
		fprintf(mFile,"\tsetAttr \".obo\" -type \"intArray\" ");
		obo.write(mFile);
		fprintf(mFile,";\n");

	}
	void setPoint1(const intArray& pto)
	{
		if(pto.size == 0) return;
		fprintf(mFile,"\tsetAttr \".pto\" -type \"intArray\" ");
		pto.write(mFile);
		fprintf(mFile,";\n");

	}
	void setMinSprings(int ms)
	{
		if(ms == 0) return;
		fprintf(mFile,"\tsetAttr \".ms\" %i;\n", ms);

	}
	void setManageParticleDeath(bool mpd)
	{
		if(mpd == true) return;
		fprintf(mFile,"\tsetAttr \".mpd\" %i;\n", mpd);

	}
	void setLengths(const doubleArray& lns)
	{
		if(lns.size == 0) return;
		fprintf(mFile,"\tsetAttr \".lns\" -type \"doubleArray\" ");
		lns.write(mFile);
		fprintf(mFile,";\n");

	}
	void getEnd1Weight()const
	{
		fprintf(mFile,"\"%s.f\"",mName.c_str());

	}
	void getEnd2Weight()const
	{
		fprintf(mFile,"\"%s.t\"",mName.c_str());

	}
	void getStiffness()const
	{
		fprintf(mFile,"\"%s.s\"",mName.c_str());

	}
	void getDamping()const
	{
		fprintf(mFile,"\"%s.d\"",mName.c_str());

	}
	void getRestLength()const
	{
		fprintf(mFile,"\"%s.r\"",mName.c_str());

	}
	void getStiffnessPS()const
	{
		fprintf(mFile,"\"%s.sps\"",mName.c_str());

	}
	void getDampingPS()const
	{
		fprintf(mFile,"\"%s.dps\"",mName.c_str());

	}
	void getRestLengthPS()const
	{
		fprintf(mFile,"\"%s.rps\"",mName.c_str());

	}
	void getObjectPositions(size_t opos_i)const
	{
		fprintf(mFile,"\"%s.opos[%i]\"",mName.c_str(),opos_i);

	}
	void getObjectVelocities(size_t ovel_i)const
	{
		fprintf(mFile,"\"%s.ovel[%i]\"",mName.c_str(),ovel_i);

	}
	void getObjectMass(size_t omas_i)const
	{
		fprintf(mFile,"\"%s.omas[%i]\"",mName.c_str(),omas_i);

	}
	void getDeltaTime(size_t dt_i)const
	{
		fprintf(mFile,"\"%s.dt[%i]\"",mName.c_str(),dt_i);

	}
	void getOutputForce(size_t of_i)const
	{
		fprintf(mFile,"\"%s.of[%i]\"",mName.c_str(),of_i);

	}
	void getIdMapping(size_t idm_i)const
	{
		fprintf(mFile,"\"%s.idm[%i]\"",mName.c_str(),idm_i);

	}
	void getSortedId(size_t idm_i)const
	{
		fprintf(mFile,"\"%s.idm[%i].sid\"",mName.c_str(),idm_i);

	}
	void getIdIndex(size_t idm_i)const
	{
		fprintf(mFile,"\"%s.idm[%i].idix\"",mName.c_str(),idm_i);

	}
	void getCount()const
	{
		fprintf(mFile,"\"%s.cnt\"",mName.c_str());

	}
	void getObjects(size_t obj_i)const
	{
		fprintf(mFile,"\"%s.obj[%i]\"",mName.c_str(),obj_i);

	}
	void getObjCount()const
	{
		fprintf(mFile,"\"%s.obc\"",mName.c_str());

	}
	void getLengths()const
	{
		fprintf(mFile,"\"%s.lns\"",mName.c_str());

	}
	void getMinUsed()const
	{
		fprintf(mFile,"\"%s.mnu\"",mName.c_str());

	}
	void getMaxUsed()const
	{
		fprintf(mFile,"\"%s.mxu\"",mName.c_str());

	}
	void getPt0Index()const
	{
		fprintf(mFile,"\"%s.pzi\"",mName.c_str());

	}
	void getPt1Index()const
	{
		fprintf(mFile,"\"%s.poi\"",mName.c_str());

	}
	void getObj0Index()const
	{
		fprintf(mFile,"\"%s.ozi\"",mName.c_str());

	}
	void getObj1Index()const
	{
		fprintf(mFile,"\"%s.ooi\"",mName.c_str());

	}
protected:
	Spring(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType,bool create=true)
		:Shape(file, name, parent, nodeType, create) {}

};
}//namespace MayaDM
#endif//__MayaDM_SPRING_H__
