/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of MayaDataModel.

    Licensed under the MIT Open Source License,
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/
#ifndef __MayaDM_FFD_H__
#define __MayaDM_FFD_H__
#include "MayaDMTypes.h"
#include "MayaDMConnectables.h"
#include "MayaDMGeometryFilter.h"
namespace MayaDM
{
class Ffd : public GeometryFilter
{
public:
	struct DeformedLattice{
		lattice deformedLatticePoints;
		matrix deformedLatticeMatrix;
		void write(FILE* file) const
		{
			deformedLatticePoints.write(file);
			fprintf(file, " ");
			deformedLatticeMatrix.write(file);
		}
	};
	struct BaseLattice{
		lattice baseLatticePoints;
		matrix baseLatticeMatrix;
		void write(FILE* file) const
		{
			baseLatticePoints.write(file);
			fprintf(file, " ");
			baseLatticeMatrix.write(file);
		}
	};
	struct StuCacheList{
		double* stuCache;
		void write(FILE* file) const
		{
			size_t size = sizeof(stuCache)/sizeof(double);
			for(size_t i=0; i<size; ++i)
			{
				fprintf(file,"%f", stuCache[i]);
				if(i+1<size) fprintf(file," ");
			}
		}
	};
public:
	Ffd(FILE* file,const std::string& name,const std::string& parent=""):GeometryFilter(file, name, parent, "ffd"){}
	virtual ~Ffd(){}
	void setDeformedLattice(const DeformedLattice& dl)
	{
		fprintf(mFile,"setAttr \".dl\" ");
		dl.write(mFile);
		fprintf(mFile,";\n");

	}
	void setDeformedLatticePoints(const lattice& dlp)
	{
		fprintf(mFile,"setAttr \".dl.dlp\" -type \"lattice\" ");
		dlp.write(mFile);
		fprintf(mFile,";\n");

	}
	void setDeformedLatticeMatrix(const matrix& dlm)
	{
		if(dlm == identity) return;
		fprintf(mFile,"setAttr \".dl.dlm\" -type \"matrix\" ");
		dlm.write(mFile);
		fprintf(mFile,";\n");

	}
	void setBaseLattice(const BaseLattice& bl)
	{
		fprintf(mFile,"setAttr \".bl\" ");
		bl.write(mFile);
		fprintf(mFile,";\n");

	}
	void setBaseLatticePoints(const lattice& blp)
	{
		fprintf(mFile,"setAttr \".bl.blp\" -type \"lattice\" ");
		blp.write(mFile);
		fprintf(mFile,";\n");

	}
	void setBaseLatticeMatrix(const matrix& blm)
	{
		if(blm == identity) return;
		fprintf(mFile,"setAttr \".bl.blm\" -type \"matrix\" ");
		blm.write(mFile);
		fprintf(mFile,";\n");

	}
	void setStuCacheList(size_t scl_i,const StuCacheList& scl)
	{
		fprintf(mFile,"setAttr \".scl[%i]\" ",scl_i);
		scl.write(mFile);
		fprintf(mFile,";\n");

	}
	void setStuCache(size_t scl_i,size_t stu_i,double stu)
	{
		if(stu == 0) return;
		fprintf(mFile,"setAttr \".scl[%i].stu[%i]\" %f;\n", scl_i,stu_i,stu);

	}
	void setPartialResolution(double ptr)
	{
		if(ptr == 0.01) return;
		fprintf(mFile,"setAttr \".ptr\" %f;\n", ptr);

	}
	void setLocalInfluenceS(short lis)
	{
		if(lis == 2) return;
		fprintf(mFile,"setAttr \".lis\" %i;\n", lis);

	}
	void setLocalInfluenceT(short lit)
	{
		if(lit == 2) return;
		fprintf(mFile,"setAttr \".lit\" %i;\n", lit);

	}
	void setLocalInfluenceU(short liu)
	{
		if(liu == 2) return;
		fprintf(mFile,"setAttr \".liu\" %i;\n", liu);

	}
	void setFreezeGeometry(bool fg)
	{
		if(fg == 0) return;
		fprintf(mFile,"setAttr \".fg\" %i;\n", fg);

	}
	void setLocal(bool lo)
	{
		if(lo == 0) return;
		fprintf(mFile,"setAttr \".lo\" %i;\n", lo);

	}
	void setUsePartialResolution(unsigned int upr)
	{
		if(upr == 0) return;
		fprintf(mFile,"setAttr \".upr\" %i;\n", upr);

	}
	void setOutsideLattice(unsigned int ot)
	{
		if(ot == 0) return;
		fprintf(mFile,"setAttr \".ot\" %i;\n", ot);

	}
	void setOutsideFalloffDist(double ofd)
	{
		if(ofd == 1.0) return;
		fprintf(mFile,"setAttr \".ofd\" %f;\n", ofd);

	}
	void getDeformedLattice()
	{
		fprintf(mFile,"\"%s.dl\"",mName.c_str());

	}
	void getDeformedLatticePoints()
	{
		fprintf(mFile,"\"%s.dl.dlp\"",mName.c_str());

	}
	void getDeformedLatticeMatrix()
	{
		fprintf(mFile,"\"%s.dl.dlm\"",mName.c_str());

	}
	void getBaseLattice()
	{
		fprintf(mFile,"\"%s.bl\"",mName.c_str());

	}
	void getBaseLatticePoints()
	{
		fprintf(mFile,"\"%s.bl.blp\"",mName.c_str());

	}
	void getBaseLatticeMatrix()
	{
		fprintf(mFile,"\"%s.bl.blm\"",mName.c_str());

	}
	void getStuCacheList(size_t scl_i)
	{
		fprintf(mFile,"\"%s.scl[%i]\"",mName.c_str(),scl_i);

	}
	void getStuCache(size_t scl_i,size_t stu_i)
	{
		fprintf(mFile,"\"%s.scl[%i].stu[%i]\"",mName.c_str(),scl_i,stu_i);

	}
	void getPartialResolution()
	{
		fprintf(mFile,"\"%s.ptr\"",mName.c_str());

	}
	void getLocalInfluenceS()
	{
		fprintf(mFile,"\"%s.lis\"",mName.c_str());

	}
	void getLocalInfluenceT()
	{
		fprintf(mFile,"\"%s.lit\"",mName.c_str());

	}
	void getLocalInfluenceU()
	{
		fprintf(mFile,"\"%s.liu\"",mName.c_str());

	}
	void getFreezeGeometry()
	{
		fprintf(mFile,"\"%s.fg\"",mName.c_str());

	}
	void getLocal()
	{
		fprintf(mFile,"\"%s.lo\"",mName.c_str());

	}
	void getUsePartialResolution()
	{
		fprintf(mFile,"\"%s.upr\"",mName.c_str());

	}
	void getOutsideLattice()
	{
		fprintf(mFile,"\"%s.ot\"",mName.c_str());

	}
	void getOutsideFalloffDist()
	{
		fprintf(mFile,"\"%s.ofd\"",mName.c_str());

	}
protected:
	Ffd(FILE* file,const std::string& name,const std::string& parent,const std::string& nodeType):GeometryFilter(file, name, parent, nodeType) {}

};
}//namespace MayaDM
#endif//__MayaDM_FFD_H__
