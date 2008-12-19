/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_UNIFORMFIELD_H__
#define __MayaDM_UNIFORMFIELD_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMField.h"
namespace MayaDM
{
class UniformField : public Field
{
public:
	struct Direction{
		double directionX;
		double directionY;
		double directionZ;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", directionX);
			fprintf(file,"%f ", directionY);
			fprintf(file,"%f", directionZ);
		}
	};
public:
	UniformField(FILE* file,const std::string& name,const std::string& parent=""):Field(file, name, parent, "uniformField"){}
	virtual ~UniformField(){}
	void setDirection(const Direction& d)
	{
		fprintf(mFile,"setAttr \".d\" ");
		d.write(mFile);
		fprintf(mFile,";\n");

	}
	void setDirectionX(double dx)
	{
		if(dx == 0.0) return;
		fprintf(mFile,"setAttr \".d.dx\" %f;\n", dx);

	}
	void setDirectionY(double dy)
	{
		if(dy == 0.0) return;
		fprintf(mFile,"setAttr \".d.dy\" %f;\n", dy);

	}
	void setDirectionZ(double dz)
	{
		if(dz == 0.0) return;
		fprintf(mFile,"setAttr \".d.dz\" %f;\n", dz);

	}
	void getDirection()
	{
		fprintf(mFile,"\"%s.d\"",mName.c_str());

	}
	void getDirectionX()
	{
		fprintf(mFile,"\"%s.d.dx\"",mName.c_str());

	}
	void getDirectionY()
	{
		fprintf(mFile,"\"%s.d.dy\"",mName.c_str());

	}
	void getDirectionZ()
	{
		fprintf(mFile,"\"%s.d.dz\"",mName.c_str());

	}
	void getInheritFactor()
	{
		fprintf(mFile,"\"%s.inh\"",mName.c_str());

	}
protected:
	UniformField(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):Field(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_UNIFORMFIELD_H__
