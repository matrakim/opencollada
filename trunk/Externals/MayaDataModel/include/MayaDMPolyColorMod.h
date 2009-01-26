/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_POLYCOLORMOD_H__
#define __MayaDM_POLYCOLORMOD_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMPolyModifier.h"
namespace MayaDM
{
class PolyColorMod : public PolyModifier
{
public:
	struct RedScale{
		float redScale_Position;
		float redScale_FloatValue;
		unsigned int redScale_Interp;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", redScale_Position);
			fprintf(file,"%f ", redScale_FloatValue);
			fprintf(file,"%i", redScale_Interp);
		}
	};
	struct GreenScale{
		float greenScale_Position;
		float greenScale_FloatValue;
		unsigned int greenScale_Interp;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", greenScale_Position);
			fprintf(file,"%f ", greenScale_FloatValue);
			fprintf(file,"%i", greenScale_Interp);
		}
	};
	struct BlueScale{
		float blueScale_Position;
		float blueScale_FloatValue;
		unsigned int blueScale_Interp;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", blueScale_Position);
			fprintf(file,"%f ", blueScale_FloatValue);
			fprintf(file,"%i", blueScale_Interp);
		}
	};
	struct AlphaScale{
		float alphaScale_Position;
		float alphaScale_FloatValue;
		unsigned int alphaScale_Interp;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", alphaScale_Position);
			fprintf(file,"%f ", alphaScale_FloatValue);
			fprintf(file,"%i", alphaScale_Interp);
		}
	};
	struct IntensityScale{
		float intensityScale_Position;
		float intensityScale_FloatValue;
		unsigned int intensityScale_Interp;
		void write(FILE* file) const
		{
			fprintf(file,"%f ", intensityScale_Position);
			fprintf(file,"%f ", intensityScale_FloatValue);
			fprintf(file,"%i", intensityScale_Interp);
		}
	};
public:
	PolyColorMod(FILE* file,const std::string& name,const std::string& parent=""):PolyModifier(file, name, parent, "polyColorMod"){}
	virtual ~PolyColorMod(){}
	void setBaseColorName(const string& bcn)
	{
		if(bcn == "NULL") return;
		fprintf(mFile,"setAttr \".bcn\" -type \"string\" ");
		bcn.write(mFile);
		fprintf(mFile,";\n");

	}
	void setRedScale(size_t r_i,const RedScale& r)
	{
		fprintf(mFile,"setAttr \".r[%i]\" ",r_i);
		r.write(mFile);
		fprintf(mFile,";\n");

	}
	void setRedScale_Position(size_t r_i,float rp)
	{
		if(rp == 0.0) return;
		fprintf(mFile,"setAttr \".r[%i].rp\" %f;\n", r_i,rp);

	}
	void setRedScale_FloatValue(size_t r_i,float rfv)
	{
		if(rfv == 0.0) return;
		fprintf(mFile,"setAttr \".r[%i].rfv\" %f;\n", r_i,rfv);

	}
	void setRedScale_Interp(size_t r_i,unsigned int ri)
	{
		if(ri == 0) return;
		fprintf(mFile,"setAttr \".r[%i].ri\" %i;\n", r_i,ri);

	}
	void setGreenScale(size_t g_i,const GreenScale& g)
	{
		fprintf(mFile,"setAttr \".g[%i]\" ",g_i);
		g.write(mFile);
		fprintf(mFile,";\n");

	}
	void setGreenScale_Position(size_t g_i,float gp)
	{
		if(gp == 0.0) return;
		fprintf(mFile,"setAttr \".g[%i].gp\" %f;\n", g_i,gp);

	}
	void setGreenScale_FloatValue(size_t g_i,float gfv)
	{
		if(gfv == 0.0) return;
		fprintf(mFile,"setAttr \".g[%i].gfv\" %f;\n", g_i,gfv);

	}
	void setGreenScale_Interp(size_t g_i,unsigned int gi)
	{
		if(gi == 0) return;
		fprintf(mFile,"setAttr \".g[%i].gi\" %i;\n", g_i,gi);

	}
	void setBlueScale(size_t b_i,const BlueScale& b)
	{
		fprintf(mFile,"setAttr \".b[%i]\" ",b_i);
		b.write(mFile);
		fprintf(mFile,";\n");

	}
	void setBlueScale_Position(size_t b_i,float bp)
	{
		if(bp == 0.0) return;
		fprintf(mFile,"setAttr \".b[%i].bp\" %f;\n", b_i,bp);

	}
	void setBlueScale_FloatValue(size_t b_i,float bfv)
	{
		if(bfv == 0.0) return;
		fprintf(mFile,"setAttr \".b[%i].bfv\" %f;\n", b_i,bfv);

	}
	void setBlueScale_Interp(size_t b_i,unsigned int bi)
	{
		if(bi == 0) return;
		fprintf(mFile,"setAttr \".b[%i].bi\" %i;\n", b_i,bi);

	}
	void setAlphaScale(size_t a_i,const AlphaScale& a)
	{
		fprintf(mFile,"setAttr \".a[%i]\" ",a_i);
		a.write(mFile);
		fprintf(mFile,";\n");

	}
	void setAlphaScale_Position(size_t a_i,float ap)
	{
		if(ap == 0.0) return;
		fprintf(mFile,"setAttr \".a[%i].ap\" %f;\n", a_i,ap);

	}
	void setAlphaScale_FloatValue(size_t a_i,float afv)
	{
		if(afv == 0.0) return;
		fprintf(mFile,"setAttr \".a[%i].afv\" %f;\n", a_i,afv);

	}
	void setAlphaScale_Interp(size_t a_i,unsigned int ai)
	{
		if(ai == 0) return;
		fprintf(mFile,"setAttr \".a[%i].ai\" %i;\n", a_i,ai);

	}
	void setIntensityScale(size_t n_i,const IntensityScale& n)
	{
		fprintf(mFile,"setAttr \".n[%i]\" ",n_i);
		n.write(mFile);
		fprintf(mFile,";\n");

	}
	void setIntensityScale_Position(size_t n_i,float np)
	{
		if(np == 0.0) return;
		fprintf(mFile,"setAttr \".n[%i].np\" %f;\n", n_i,np);

	}
	void setIntensityScale_FloatValue(size_t n_i,float nfv)
	{
		if(nfv == 0.0) return;
		fprintf(mFile,"setAttr \".n[%i].nfv\" %f;\n", n_i,nfv);

	}
	void setIntensityScale_Interp(size_t n_i,unsigned int ni)
	{
		if(ni == 0) return;
		fprintf(mFile,"setAttr \".n[%i].ni\" %i;\n", n_i,ni);

	}
	void setHuev(float h)
	{
		if(h == 0.0) return;
		fprintf(mFile,"setAttr \".h\" %f;\n", h);

	}
	void setSatv(float s)
	{
		if(s == 1.0) return;
		fprintf(mFile,"setAttr \".s\" %f;\n", s);

	}
	void setValue(float v)
	{
		if(v == 1.0) return;
		fprintf(mFile,"setAttr \".v\" %f;\n", v);

	}
	void getBaseColorName()
	{
		fprintf(mFile,"\"%s.bcn\"",mName.c_str());

	}
	void getRedScale(size_t r_i)
	{
		fprintf(mFile,"\"%s.r[%i]\"",mName.c_str(),r_i);

	}
	void getRedScale_Position(size_t r_i)
	{
		fprintf(mFile,"\"%s.r[%i].rp\"",mName.c_str(),r_i);

	}
	void getRedScale_FloatValue(size_t r_i)
	{
		fprintf(mFile,"\"%s.r[%i].rfv\"",mName.c_str(),r_i);

	}
	void getRedScale_Interp(size_t r_i)
	{
		fprintf(mFile,"\"%s.r[%i].ri\"",mName.c_str(),r_i);

	}
	void getGreenScale(size_t g_i)
	{
		fprintf(mFile,"\"%s.g[%i]\"",mName.c_str(),g_i);

	}
	void getGreenScale_Position(size_t g_i)
	{
		fprintf(mFile,"\"%s.g[%i].gp\"",mName.c_str(),g_i);

	}
	void getGreenScale_FloatValue(size_t g_i)
	{
		fprintf(mFile,"\"%s.g[%i].gfv\"",mName.c_str(),g_i);

	}
	void getGreenScale_Interp(size_t g_i)
	{
		fprintf(mFile,"\"%s.g[%i].gi\"",mName.c_str(),g_i);

	}
	void getBlueScale(size_t b_i)
	{
		fprintf(mFile,"\"%s.b[%i]\"",mName.c_str(),b_i);

	}
	void getBlueScale_Position(size_t b_i)
	{
		fprintf(mFile,"\"%s.b[%i].bp\"",mName.c_str(),b_i);

	}
	void getBlueScale_FloatValue(size_t b_i)
	{
		fprintf(mFile,"\"%s.b[%i].bfv\"",mName.c_str(),b_i);

	}
	void getBlueScale_Interp(size_t b_i)
	{
		fprintf(mFile,"\"%s.b[%i].bi\"",mName.c_str(),b_i);

	}
	void getAlphaScale(size_t a_i)
	{
		fprintf(mFile,"\"%s.a[%i]\"",mName.c_str(),a_i);

	}
	void getAlphaScale_Position(size_t a_i)
	{
		fprintf(mFile,"\"%s.a[%i].ap\"",mName.c_str(),a_i);

	}
	void getAlphaScale_FloatValue(size_t a_i)
	{
		fprintf(mFile,"\"%s.a[%i].afv\"",mName.c_str(),a_i);

	}
	void getAlphaScale_Interp(size_t a_i)
	{
		fprintf(mFile,"\"%s.a[%i].ai\"",mName.c_str(),a_i);

	}
	void getIntensityScale(size_t n_i)
	{
		fprintf(mFile,"\"%s.n[%i]\"",mName.c_str(),n_i);

	}
	void getIntensityScale_Position(size_t n_i)
	{
		fprintf(mFile,"\"%s.n[%i].np\"",mName.c_str(),n_i);

	}
	void getIntensityScale_FloatValue(size_t n_i)
	{
		fprintf(mFile,"\"%s.n[%i].nfv\"",mName.c_str(),n_i);

	}
	void getIntensityScale_Interp(size_t n_i)
	{
		fprintf(mFile,"\"%s.n[%i].ni\"",mName.c_str(),n_i);

	}
	void getHuev()
	{
		fprintf(mFile,"\"%s.h\"",mName.c_str());

	}
	void getSatv()
	{
		fprintf(mFile,"\"%s.s\"",mName.c_str());

	}
	void getValue()
	{
		fprintf(mFile,"\"%s.v\"",mName.c_str());

	}
protected:
	PolyColorMod(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):PolyModifier(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_POLYCOLORMOD_H__