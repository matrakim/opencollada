/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_AIRFIELD_H__
#define __MayaDM_AIRFIELD_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMField.h"
namespace MayaDM
{
class AirField : public Field
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
	AirField(FILE* file,const std::string& name,const std::string& parent=""):Field(file, name, parent, "airField"){}
	virtual ~AirField(){}
	void setDirection(const Direction& d)
	{
		fprintf(mFile,"setAttr \".d\" ");
		d.write(mFile);
		fprintf(mFile,";\n");

	}
	void setDirectionX(double dx)
	{
		if(dx == 0) return;
		fprintf(mFile,"setAttr \".d.dx\" %f;\n", dx);

	}
	void setDirectionY(double dy)
	{
		if(dy == 0) return;
		fprintf(mFile,"setAttr \".d.dy\" %f;\n", dy);

	}
	void setDirectionZ(double dz)
	{
		if(dz == 0) return;
		fprintf(mFile,"setAttr \".d.dz\" %f;\n", dz);

	}
	void setSpeed(double spd)
	{
		if(spd == 0) return;
		fprintf(mFile,"setAttr \".spd\" %f;\n", spd);

	}
	void setInheritVelocity(double iv)
	{
		if(iv == 0) return;
		fprintf(mFile,"setAttr \".iv\" %f;\n", iv);

	}
	void setComponentOnly(bool co)
	{
		if(co == false) return;
		fprintf(mFile,"setAttr \".co\" %i;\n", co);

	}
	void setSpread(double spr)
	{
		if(spr == 0) return;
		fprintf(mFile,"setAttr \".spr\" %f;\n", spr);

	}
	void setEnableSpread(bool es)
	{
		if(es == false) return;
		fprintf(mFile,"setAttr \".es\" %i;\n", es);

	}
	void setInheritRotation(bool ir)
	{
		if(ir == false) return;
		fprintf(mFile,"setAttr \".ir\" %i;\n", ir);

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
	void getSpeed()
	{
		fprintf(mFile,"\"%s.spd\"",mName.c_str());

	}
	void getInheritVelocity()
	{
		fprintf(mFile,"\"%s.iv\"",mName.c_str());

	}
	void getComponentOnly()
	{
		fprintf(mFile,"\"%s.co\"",mName.c_str());

	}
	void getSpread()
	{
		fprintf(mFile,"\"%s.spr\"",mName.c_str());

	}
	void getEnableSpread()
	{
		fprintf(mFile,"\"%s.es\"",mName.c_str());

	}
	void getInheritRotation()
	{
		fprintf(mFile,"\"%s.ir\"",mName.c_str());

	}
protected:
	AirField(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):Field(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_AIRFIELD_H__
