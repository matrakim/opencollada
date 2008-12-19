/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_BOUNDARYBASE_H__
#define __MayaDM_BOUNDARYBASE_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMAbstractBaseCreate.h"
namespace MayaDM
{
class BoundaryBase : public AbstractBaseCreate
{
public:
public:
	BoundaryBase(FILE* file,const std::string& name,const std::string& parent=""):AbstractBaseCreate(file, name, parent, "boundaryBase"){}
	virtual ~BoundaryBase(){}
	void setEndPointTolerance(double ept)
	{
		if(ept == 0) return;
		fprintf(mFile,"setAttr \".ept\" %f;\n", ept);

	}
	void getInputCurve1()
	{
		fprintf(mFile,"\"%s.ic1\"",mName.c_str());

	}
	void getInputCurve2()
	{
		fprintf(mFile,"\"%s.ic2\"",mName.c_str());

	}
	void getInputCurve3()
	{
		fprintf(mFile,"\"%s.ic3\"",mName.c_str());

	}
	void getInputCurve4()
	{
		fprintf(mFile,"\"%s.ic4\"",mName.c_str());

	}
	void getEndPointTolerance()
	{
		fprintf(mFile,"\"%s.ept\"",mName.c_str());

	}
	void getOutputSurface()
	{
		fprintf(mFile,"\"%s.os\"",mName.c_str());

	}
protected:
	BoundaryBase(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):AbstractBaseCreate(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_BOUNDARYBASE_H__
