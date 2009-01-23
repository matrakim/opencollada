/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_AVGSURFACEPOINTS_H__
#define __MayaDM_AVGSURFACEPOINTS_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMAbstractBaseCreate.h"
namespace MayaDM
{
class AvgSurfacePoints : public AbstractBaseCreate
{
public:
	struct Result{
		void write(FILE* file) const
		{
		}
	};
public:
	AvgSurfacePoints(FILE* file,const std::string& name,const std::string& parent=""):AbstractBaseCreate(file, name, parent, "avgSurfacePoints"){}
	virtual ~AvgSurfacePoints(){}
	void setWeight(size_t wt_i,double wt)
	{
		if(wt == 0.5) return;
		fprintf(mFile,"setAttr \".wt[%i]\" %f;\n", wt_i,wt);

	}
	void setWeight(size_t wt_start,size_t wt_end,double* wt)
	{
		fprintf(mFile,"setAttr \".wt[%i:%i]\" ", wt_start,wt_end);
		size_t size = (wt_end-wt_start)*1+1;
		for(size_t i=0;i<size;++i)
		{
			fprintf(mFile,"%f",wt[i]);
			if(i+1<size) fprintf(mFile," ");
		}
		fprintf(mFile,";\n");

	}
	void startWeight(size_t wt_start,size_t wt_end)
	{
		fprintf(mFile,"setAttr \".wt[%i:%i]\"",wt_start,wt_end);

	}
	void appendWeight(double wt)
	{
		fprintf(mFile," %f",wt);

	}
	void endWeight()
	{
		fprintf(mFile,";\n");

	}
	void setParameterU(size_t u_i,double u)
	{
		if(u == 0.0) return;
		fprintf(mFile,"setAttr \".u[%i]\" %f;\n", u_i,u);

	}
	void setParameterU(size_t u_start,size_t u_end,double* u)
	{
		fprintf(mFile,"setAttr \".u[%i:%i]\" ", u_start,u_end);
		size_t size = (u_end-u_start)*1+1;
		for(size_t i=0;i<size;++i)
		{
			fprintf(mFile,"%f",u[i]);
			if(i+1<size) fprintf(mFile," ");
		}
		fprintf(mFile,";\n");

	}
	void startParameterU(size_t u_start,size_t u_end)
	{
		fprintf(mFile,"setAttr \".u[%i:%i]\"",u_start,u_end);

	}
	void appendParameterU(double u)
	{
		fprintf(mFile," %f",u);

	}
	void endParameterU()
	{
		fprintf(mFile,";\n");

	}
	void setParameterV(size_t v_i,double v)
	{
		if(v == 0.0) return;
		fprintf(mFile,"setAttr \".v[%i]\" %f;\n", v_i,v);

	}
	void setParameterV(size_t v_start,size_t v_end,double* v)
	{
		fprintf(mFile,"setAttr \".v[%i:%i]\" ", v_start,v_end);
		size_t size = (v_end-v_start)*1+1;
		for(size_t i=0;i<size;++i)
		{
			fprintf(mFile,"%f",v[i]);
			if(i+1<size) fprintf(mFile," ");
		}
		fprintf(mFile,";\n");

	}
	void startParameterV(size_t v_start,size_t v_end)
	{
		fprintf(mFile,"setAttr \".v[%i:%i]\"",v_start,v_end);

	}
	void appendParameterV(double v)
	{
		fprintf(mFile," %f",v);

	}
	void endParameterV()
	{
		fprintf(mFile,";\n");

	}
	void setTurnOnPercentage(bool top)
	{
		if(top == false) return;
		fprintf(mFile,"setAttr \".top\" %i;\n", top);

	}
	void getInputSurfaces(size_t is_i)
	{
		fprintf(mFile,"\"%s.is[%i]\"",mName.c_str(),is_i);

	}
	void getWeight(size_t wt_i)
	{
		fprintf(mFile,"\"%s.wt[%i]\"",mName.c_str(),wt_i);

	}
	void getParameterU(size_t u_i)
	{
		fprintf(mFile,"\"%s.u[%i]\"",mName.c_str(),u_i);

	}
	void getParameterV(size_t v_i)
	{
		fprintf(mFile,"\"%s.v[%i]\"",mName.c_str(),v_i);

	}
	void getTurnOnPercentage()
	{
		fprintf(mFile,"\"%s.top\"",mName.c_str());

	}
	void getResult()
	{
		fprintf(mFile,"\"%s.r\"",mName.c_str());

	}
	void getPosition()
	{
		fprintf(mFile,"\"%s.r.p\"",mName.c_str());

	}
	void getPositionX()
	{
		fprintf(mFile,"\"%s.r.p.px\"",mName.c_str());

	}
	void getPositionY()
	{
		fprintf(mFile,"\"%s.r.p.py\"",mName.c_str());

	}
	void getPositionZ()
	{
		fprintf(mFile,"\"%s.r.p.pz\"",mName.c_str());

	}
	void getNormal()
	{
		fprintf(mFile,"\"%s.r.n\"",mName.c_str());

	}
	void getNormalX()
	{
		fprintf(mFile,"\"%s.r.n.nx\"",mName.c_str());

	}
	void getNormalY()
	{
		fprintf(mFile,"\"%s.r.n.ny\"",mName.c_str());

	}
	void getNormalZ()
	{
		fprintf(mFile,"\"%s.r.n.nz\"",mName.c_str());

	}
protected:
	AvgSurfacePoints(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):AbstractBaseCreate(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_AVGSURFACEPOINTS_H__
