/*
    Copyright (c) 2008-2009 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_GEOMETRYCONSTRAINT_H__
#define __MayaDM_GEOMETRYCONSTRAINT_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMConstraint.h"
namespace MayaDM
{
class GeometryConstraint : public Constraint
{
public:
	struct Target{
		double targetWeight;
		void write(FILE* file) const
		{
			fprintf(file,"%f", targetWeight);
		}
	};
public:
	GeometryConstraint():Constraint(){}
	GeometryConstraint(FILE* file,const std::string& name,const std::string& parent="",bool create=true)
		:Constraint(file, name, parent, "geometryConstraint", create){}
	virtual ~GeometryConstraint(){}
	void setTarget(size_t tg_i,const Target& tg)
	{
		fprintf(mFile,"\tsetAttr \".tg[%i]\" ",tg_i);
		tg.write(mFile);
		fprintf(mFile,";\n");

	}
	void setTargetWeight(size_t tg_i,double tw)
	{
		if(tw == 1.0) return;
		fprintf(mFile,"\tsetAttr \".tg[%i].tw\" %f;\n", tg_i,tw);

	}
	void setConstraintParentInverseMatrix(const matrix& cpim)
	{
		if(cpim == identity) return;
		fprintf(mFile,"\tsetAttr \".cpim\" -type \"matrix\" ");
		cpim.write(mFile);
		fprintf(mFile,";\n");

	}
	void getTarget(size_t tg_i)const
	{
		fprintf(mFile,"\"%s.tg[%i]\"",mName.c_str(),tg_i);

	}
	void getTargetGeometry(size_t tg_i)const
	{
		fprintf(mFile,"\"%s.tg[%i].tgm\"",mName.c_str(),tg_i);

	}
	void getTargetWeight(size_t tg_i)const
	{
		fprintf(mFile,"\"%s.tg[%i].tw\"",mName.c_str(),tg_i);

	}
	void getConstraintParentInverseMatrix()const
	{
		fprintf(mFile,"\"%s.cpim\"",mName.c_str());

	}
	void getConstraintGeometry()const
	{
		fprintf(mFile,"\"%s.cgm\"",mName.c_str());

	}
protected:
	GeometryConstraint(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType,bool create=true)
		:Constraint(file, name, parent, nodeType, create) {}

};
}//namespace MayaDM
#endif//__MayaDM_GEOMETRYCONSTRAINT_H__
