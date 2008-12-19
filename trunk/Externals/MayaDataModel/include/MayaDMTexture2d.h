/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_TEXTURE2D_H__
#define __MayaDM_TEXTURE2D_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMDependNode.h"
namespace MayaDM
{
class Texture2d : public DependNode
{
public:
public:
	Texture2d(FILE* file,const std::string& name,const std::string& parent=""):DependNode(file, name, parent, "texture2d"){}
	virtual ~Texture2d(){}
	void setFilter(float f)
	{
		if(f == 1.0) return;
		fprintf(mFile,"setAttr \".f\" %f;\n", f);

	}
	void setFilterOffset(float fo)
	{
		if(fo == 0.0) return;
		fprintf(mFile,"setAttr \".fo\" %f;\n", fo);

	}
	void setInvert(bool i)
	{
		if(i == false) return;
		fprintf(mFile,"setAttr \".i\" %i;\n", i);

	}
	void setAlphaIsLuminance(bool ail)
	{
		if(ail == false) return;
		fprintf(mFile,"setAttr \".ail\" %i;\n", ail);

	}
	void setColorGain(const float3& cg)
	{
		if(cg == float3(1.0f,1.0f,1.0f)) return;
		fprintf(mFile,"setAttr \".cg\" -type \"float3\" ");
		cg.write(mFile);
		fprintf(mFile,";\n");

	}
	void setColorGainR(float cgr)
	{
		if(cgr == 0.0) return;
		fprintf(mFile,"setAttr \".cg.cgr\" %f;\n", cgr);

	}
	void setColorGainG(float cgg)
	{
		if(cgg == 0.0) return;
		fprintf(mFile,"setAttr \".cg.cgg\" %f;\n", cgg);

	}
	void setColorGainB(float cgb)
	{
		if(cgb == 0.0) return;
		fprintf(mFile,"setAttr \".cg.cgb\" %f;\n", cgb);

	}
	void setColorOffset(const float3& co)
	{
		if(co == float3(0.0f,0.0f,0.0f)) return;
		fprintf(mFile,"setAttr \".co\" -type \"float3\" ");
		co.write(mFile);
		fprintf(mFile,";\n");

	}
	void setColorOffsetR(float cor)
	{
		if(cor == 0.0) return;
		fprintf(mFile,"setAttr \".co.cor\" %f;\n", cor);

	}
	void setColorOffsetG(float cog)
	{
		if(cog == 0.0) return;
		fprintf(mFile,"setAttr \".co.cog\" %f;\n", cog);

	}
	void setColorOffsetB(float cob)
	{
		if(cob == 0.0) return;
		fprintf(mFile,"setAttr \".co.cob\" %f;\n", cob);

	}
	void setAlphaGain(float ag)
	{
		if(ag == 1.0) return;
		fprintf(mFile,"setAttr \".ag\" %f;\n", ag);

	}
	void setAlphaOffset(float ao)
	{
		if(ao == 0.0) return;
		fprintf(mFile,"setAttr \".ao\" %f;\n", ao);

	}
	void setDefaultColor(const float3& dc)
	{
		if(dc == float3(0.5f,0.5f,0.5f)) return;
		fprintf(mFile,"setAttr \".dc\" -type \"float3\" ");
		dc.write(mFile);
		fprintf(mFile,";\n");

	}
	void setDefaultColorR(float dcr)
	{
		if(dcr == 0.0) return;
		fprintf(mFile,"setAttr \".dc.dcr\" %f;\n", dcr);

	}
	void setDefaultColorG(float dcg)
	{
		if(dcg == 0.0) return;
		fprintf(mFile,"setAttr \".dc.dcg\" %f;\n", dcg);

	}
	void setDefaultColorB(float dcb)
	{
		if(dcb == 0.0) return;
		fprintf(mFile,"setAttr \".dc.dcb\" %f;\n", dcb);

	}
	void getUvCoord()
	{
		fprintf(mFile,"\"%s.uv\"",mName.c_str());

	}
	void getUCoord()
	{
		fprintf(mFile,"\"%s.uv.u\"",mName.c_str());

	}
	void getVCoord()
	{
		fprintf(mFile,"\"%s.uv.v\"",mName.c_str());

	}
	void getUvFilterSize()
	{
		fprintf(mFile,"\"%s.fs\"",mName.c_str());

	}
	void getUvFilterSizeX()
	{
		fprintf(mFile,"\"%s.fs.fsx\"",mName.c_str());

	}
	void getUvFilterSizeY()
	{
		fprintf(mFile,"\"%s.fs.fsy\"",mName.c_str());

	}
	void getFilter()
	{
		fprintf(mFile,"\"%s.f\"",mName.c_str());

	}
	void getFilterOffset()
	{
		fprintf(mFile,"\"%s.fo\"",mName.c_str());

	}
	void getInvert()
	{
		fprintf(mFile,"\"%s.i\"",mName.c_str());

	}
	void getAlphaIsLuminance()
	{
		fprintf(mFile,"\"%s.ail\"",mName.c_str());

	}
	void getColorGain()
	{
		fprintf(mFile,"\"%s.cg\"",mName.c_str());

	}
	void getColorGainR()
	{
		fprintf(mFile,"\"%s.cg.cgr\"",mName.c_str());

	}
	void getColorGainG()
	{
		fprintf(mFile,"\"%s.cg.cgg\"",mName.c_str());

	}
	void getColorGainB()
	{
		fprintf(mFile,"\"%s.cg.cgb\"",mName.c_str());

	}
	void getColorOffset()
	{
		fprintf(mFile,"\"%s.co\"",mName.c_str());

	}
	void getColorOffsetR()
	{
		fprintf(mFile,"\"%s.co.cor\"",mName.c_str());

	}
	void getColorOffsetG()
	{
		fprintf(mFile,"\"%s.co.cog\"",mName.c_str());

	}
	void getColorOffsetB()
	{
		fprintf(mFile,"\"%s.co.cob\"",mName.c_str());

	}
	void getAlphaGain()
	{
		fprintf(mFile,"\"%s.ag\"",mName.c_str());

	}
	void getAlphaOffset()
	{
		fprintf(mFile,"\"%s.ao\"",mName.c_str());

	}
	void getDefaultColor()
	{
		fprintf(mFile,"\"%s.dc\"",mName.c_str());

	}
	void getDefaultColorR()
	{
		fprintf(mFile,"\"%s.dc.dcr\"",mName.c_str());

	}
	void getDefaultColorG()
	{
		fprintf(mFile,"\"%s.dc.dcg\"",mName.c_str());

	}
	void getDefaultColorB()
	{
		fprintf(mFile,"\"%s.dc.dcb\"",mName.c_str());

	}
	void getOutColor()
	{
		fprintf(mFile,"\"%s.oc\"",mName.c_str());

	}
	void getOutColorR()
	{
		fprintf(mFile,"\"%s.oc.ocr\"",mName.c_str());

	}
	void getOutColorG()
	{
		fprintf(mFile,"\"%s.oc.ocg\"",mName.c_str());

	}
	void getOutColorB()
	{
		fprintf(mFile,"\"%s.oc.ocb\"",mName.c_str());

	}
	void getOutAlpha()
	{
		fprintf(mFile,"\"%s.oa\"",mName.c_str());

	}
protected:
	Texture2d(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):DependNode(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_TEXTURE2D_H__
