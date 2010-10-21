/*
 * Copyright (c)2006-2010  Hanchuan Peng (Janelia Farm, Howard Hughes Medical Institute).
 * All rights reserved.
 */


/************
                                            ********* LICENSE NOTICE ************

This folder contains all source codes for the V3D project, which is subject to the following conditions if you want to use it.

You will ***have to agree*** the following terms, *before* downloading/using/running/editing/changing any portion of codes in this package.

1. This package is free for non-profit research, but needs a special license for any commercial purpose. Please contact Hanchuan Peng for details.

2. You agree to appropriately cite this work in your related studies and publications.

Peng, H., Ruan, Z., Long, F., Simpson, J.H., and Myers, E.W. (2010) “V3D enables real-time 3D visualization and quantitative analysis of large-scale biological image data sets,” Nature Biotechnology, Vol. 28, No. 4, pp. 348-353, DOI: 10.1038/nbt.1612. ( http://penglab.janelia.org/papersall/docpdf/2010_NBT_V3D.pdf )

Peng, H, Ruan, Z., Atasoy, D., and Sternson, S. (2010) “Automatic reconstruction of 3D neuron structures using a graph-augmented deformable model,” Bioinformatics, Vol. 26, pp. i38-i46, 2010. ( http://penglab.janelia.org/papersall/docpdf/2010_Bioinfo_GD_ISMB2010.pdf )

3. This software is provided by the copyright holders (Hanchuan Peng), Howard Hughes Medical Institute, Janelia Farm Research Campus, and contributors "as is" and any express or implied warranties, including, but not limited to, any implied warranties of merchantability, non-infringement, or fitness for a particular purpose are disclaimed. In no event shall the copyright owner, Howard Hughes Medical Institute, Janelia Farm Research Campus, or contributors be liable for any direct, indirect, incidental, special, exemplary, or consequential damages (including, but not limited to, procurement of substitute goods or services; loss of use, data, or profits; reasonable royalties; or business interruption) however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software, even if advised of the possibility of such damage.

4. Neither the name of the Howard Hughes Medical Institute, Janelia Farm Research Campus, nor Hanchuan Peng, may be used to endorse or promote products derived from this software without specific prior written permission.

*************/




/*
 *  renderer_tex2.cpp
 *
 *  Created by Ruan Zongcai on 8/6/08.
 *
 * Last update: 20080812, Hanchuan Peng, update some of the message function calls to Qt
 * Last update: 20090421, Zongcai Ruan. move code of object hit processing to renderer_hit2.cpp
 * Last update: 20091120. Hanchuan Peng. fix a bug in the subvolume cut image processing
 *
 *  Copyright Hanchuan Peng. All rights reserved.
 *
 */

//#include "../v3d/v3d_compile_constraints.h"

#include "renderer_tex2.h"
#include "v3dr_glwidget.h"

#define SIM_DIM1 756	//X
#define SIM_DIM2 756	//Y
#define SIM_DIM3 200	//Z
#define SIM_DIM4 3		//C
#define SIM_DIM5 1		//T
#define MAX_3DTEX_SIZE  256


// if error then close
// clean memory before MessageBox, otherwise MessageBox maybe could not be created correctly
#define ERROR_MessageBox(title, type, what) { \
	b_error = true; \
	cleanData(); \
	QMessageBox::critical( 0, title, QObject::tr("%1: OUT OF MEMORY or GL ERROR.\n---%2 exception: %3")\
			.arg(title).arg(type).arg(what) + "\n\n" + \
		QObject::tr("3D View: Please close some images or views to release memory, then try again.\n\n") ); \
}
#define CATCH_handler( func_name ) \
	catch (std::exception & e) { \
		\
		qDebug("   catch: data4dp  = @%0p", data4dp); \
		qDebug("   catch: rgbaBuf = @%0p", rgbaBuf); \
		\
		ERROR_MessageBox( func_name, "std", e.what() ); \
		return; \
		\
	} catch (const char* s) { \
		\
		ERROR_MessageBox( func_name, "GL", s ); \
		return; \
		\
	} catch (...) { \
		\
		ERROR_MessageBox( func_name, "UNKOWN", "unknown exception" ); \
		return; \
		\
	}



////////////////////////////////////////////////////////////////////////////////

Renderer_tex2::Renderer_tex2(void* widget)
	: Renderer(widget)
{
	qDebug("  Renderer_tex2::Renderer_tex2");
	init_members();
}

////////////////////////////////////////////////////////////////////////////////

Renderer_tex2::~Renderer_tex2()
{
	qDebug("  Renderer_tex2::~Renderer_tex2");

	cleanVol();
	cleanObj();

	cleanData();
}

////////////////////////////////////////////////////////////////////////////////

void Renderer_tex2::setupData(void* idep)
{
	cleanData(); //090705
	qDebug("  Renderer_tex2::setupData");

	PROGRESS_DIALOG("", widget);

	this->_idep = idep;
	isSimulatedData = false;
	bool bLocal = false;
	b_limitedsize = (tryTexStream==0); //091022, 100720: down-sampling only if tryTexStream==0
	if (b_limitedsize)
	{
		qDebug("	Down-sampling to 512x512x256 ");
	}

	try
	{
#ifndef test_main_cpp

		My4DImage* image4d = v3dr_getImage4d(_idep);
		if (image4d && image4d->getCDim()>0)
		{
			bLocal = ((iDrawExternalParameter*)_idep)->b_local;
			//bLimited = ((iDrawExternalParameter*)_idep)->b_use_512x512x256; //091015: no need this, because always can use stream texture

			data_unitbytes = image4d->getUnitBytes();
			data4dp = image4d->getRawData();
			data4d_uint8 = image4d->data4d_uint8;

			size1=dim1 = image4d->getXDim();
			size2=dim2 = image4d->getYDim();
			size3=dim3 = image4d->getZDim();
			size4=dim4 = image4d->getCDim();
			size5=dim5 = 1;
			if (image4d->getTDim()>1 && image4d->getTimePackType()==TIME_PACK_C)
			{
				MESSAGE_ASSERT(image4d->getCDim() >= image4d->getTDim());

				size4=dim4 = image4d->getCDim()/image4d->getTDim();
				size5=dim5 = image4d->getTDim();
			}
			start1 = 0;
			start2 = 0;
			start3 = 0;
			start4 = 0;
			start5 = 0;

			if (bLocal)
			{
				size1 = ((iDrawExternalParameter*)_idep)->local_size.x;
				size2 = ((iDrawExternalParameter*)_idep)->local_size.y;
				size3 = ((iDrawExternalParameter*)_idep)->local_size.z;
				sampleScale[0] = sampleScale[1] =sampleScale[2] = 1;

				start1 = ((iDrawExternalParameter*)_idep)->local_start.x;
				start2 = ((iDrawExternalParameter*)_idep)->local_start.y;
				start3 = ((iDrawExternalParameter*)_idep)->local_start.z;
				//data4dp += start3*(dim2*dim1) + start2*(dim1) + start1;
			}
		}
		else // image4d==0  default coordinate frame for surface
		{
			size1=dim1 = 0; //DEFAULT_DIM1;
			size2=dim2 = 0; //DEFAULT_DIM2;
			size3=dim3 = 0; //DEFAULT_DIM3;
			size4=dim4 = 0; // this make no rgbaBuf allocated
			size5=dim5 = 0; // packed time
			start1 = 0;
			start2 = 0;
			start3 = 0;
			start4 = 0;
			start5 = 0;
			data4dp = 0; // this make no rgbaBuf allocated
		}
#else // then _idep==0
		{
			PROGRESS_TEXT( QObject::tr("new a simulated Data") );
			PROGRESS_PERCENT(10);
			{
				isSimulatedData = true;
				size1=dim1 = SIM_DIM1;
				size2=dim2 = SIM_DIM2;
				size3=dim3 = SIM_DIM3;
				size4=dim4 = SIM_DIM4;
				size5=dim5 = SIM_DIM5;
				start1 = 0;
				start2 = 0;
				start3 = 0;
				start4 = 0;
				start5 = 0;
				data4dp = createSimulatedData(dim1, dim2, dim3, dim4, dim5);
				qDebug("   new data4dp = @%0p", data4dp);
			}
			PROGRESS_PERCENT(100); //auto close when reach 100%
		}
#endif

		if (b_limitedsize)
		{
			getLimitedSampleScaleBufSize(size1, size2, size3, size4, size5, sampleScale, bufSize);
		}
		else
		{
			bufSize[0] = size1;
			bufSize[1] = size2;
			bufSize[2] = size3;
			bufSize[3] = size4;
			bufSize[4] = size5;
			sampleScale[0]=sampleScale[1]=sampleScale[2]=sampleScale[3]=sampleScale[4] = 1;
		}

		total_rgbaBuf = rgbaBuf = 0; //(RGBA*)-1; //test whether the new sets pointer to 0 when failed
		if (data4dp && size4>0)
		{
			// only RGB, first 3 channels of original image
			total_rgbaBuf = rgbaBuf = new RGBA8[ bufSize[0] * bufSize[1] * bufSize[2] * 1 * bufSize[4] ];
		}

		qDebug("   data4dp = %0p \t(start %dx%dx%d_%d_%d, size %dx%dx%d_%d_%d)", data4dp,
				start1,start2,start3,start4,start5,  size1,size2,size3,size4, size5);

		qDebug("   rgbaBuf = %0p \t(%dx%dx%d_%d_%d)", rgbaBuf, bufSize[0],bufSize[1],bufSize[2],bufSize[3],bufSize[4]);


		dataViewProcBox = dataBox = BoundingBox(start1, start2, start3, start1+(size1-1), start2+(size2-1), start3+(size3-1));

		qDebug("   data box in original image space @\t(%g %g %g)--(%g %g %g)", dataBox.x0,dataBox.y0,dataBox.z0, dataBox.x1,dataBox.y1,dataBox.z1);

	} CATCH_handler( "Renderer_tex2::setupData" );


	QTime qtime;  qtime.start();
	{
#ifndef test_main_cpp

		My4DImage* image4d = v3dr_getImage4d(_idep);
		if (image4d)
		{
			Image4DProxy<Image4DSimple> img4dp( image4d );
			img4dp.vmin.clear();
			img4dp.vmax.clear();
			//qDebug()<<"before vmin vmax";
			switch(img4dp.su)
			{
			case 4:
			case 2:
				for (int i=0; i<img4dp.sc; i++)
				{
					img4dp.vmin.push_back( image4d->p_vmin[i] );
					img4dp.vmax.push_back( image4d->p_vmax[i] );
				}
			}
			//qDebug()<<"after vmin vmax";
			data4dp_to_rgba3d(img4dp,  dim5,
					start1, start2, start3, start4,
					size1, size2, size3, size4,
					total_rgbaBuf, bufSize);
		}
#else // then _idep==0
		data4dp_to_rgba3d(data4dp,
				dim1, dim2, dim3, dim4, dim5,
				start1, start2, start3, start4,
				size1, size2, size3, size4,
				total_rgbaBuf, bufSize);
#endif

		if (dim4==1)   rgba3d_r2gray(total_rgbaBuf, bufSize); //081103
	}
	qDebug("   data4dp_to_rgba3d .................. cost time = %g sec", qtime.elapsed()*0.001);

}

////////////////////////////////////////////////////////////////////////////////

void Renderer_tex2::cleanData()
{
	qDebug("   Renderer_tex2::cleanData");

	if (isSimulatedData && data4dp) {
		isSimulatedData = false;
		DELETE_AND_ZERO( data4dp );
		dim1=dim2=dim3=dim4=0;
		start1=start2=start3=start4=0;
		size1=size2=size3=size4=0;
	}
	for (int i=0; i<5; i++)
	{
	    sampleScale[i]=1; bufSize[i]=0;
	}

	DELETE_AND_ZERO(total_rgbaBuf);
	rgbaBuf = 0;
	DELETE_AND_ZERO(rgbaBuf_Yzx);
	DELETE_AND_ZERO(rgbaBuf_Xzy);
}


////////////////////////////////////////////////////////////////////////////////

void Renderer_tex2::initialize(int version)
{
	qDebug("  Renderer_tex2::initialize (%d)", version);
	if (b_error) return; //080924 try to catch the memory error

	try {

		Renderer::initialize(); //call GLdetect(), ..., loadObj()
		if (version<1) return;

		loadVol();

	} CATCH_handler( "Renderer_tex2::initialize" );

	BoundingBox& sBB =surfBoundingBox;
	BoundingBox& BB  =boundingBox;

	BB.x0 = 0;
	BB.y0 = 0;
	BB.z0 = 0;
	BB.x1 = imageX / sampleScaleX * thicknessX;
	BB.y1 = imageY / sampleScaleY * thicknessY;
	BB.z1 = imageZ / sampleScaleZ * thicknessZ;
	qDebug("	BoundingBox surface (%g %g %g)--(%g %g %g)", sBB.x0,sBB.y0,sBB.z0, sBB.x1,sBB.y1,sBB.z1 );
	qDebug("	BoundingBox default (%g %g %g)--(%g %g %g)", BB.x0,BB.y0,BB.z0, BB.x1,BB.y1,BB.z1 );

	swcBB = NULL_BoundingBox;
	apoBB = NULL_BoundingBox;
	labelBB = NULL_BoundingBox;
}

void Renderer_tex2::reinitializeVol(int version)
{
	qDebug("  Renderer_tex2::reinitializeVol (%d)", version);
	if (b_error) return; //080924 try to catch the memory error

	try {

		if (version<1) return;

		loadVol();

	} CATCH_handler( "Renderer_tex2::reinitializeVol" );

	BoundingBox& sBB =surfBoundingBox;
	BoundingBox& BB  =boundingBox;

	BB.x0 = 0;
	BB.y0 = 0;
	BB.z0 = 0;
	BB.x1 = imageX / sampleScaleX * thicknessX;
	BB.y1 = imageY / sampleScaleY * thicknessY;
	BB.z1 = imageZ / sampleScaleZ * thicknessZ;
	qDebug("	BoundingBox surface (%g %g %g)--(%g %g %g)", sBB.x0,sBB.y0,sBB.z0, sBB.x1,sBB.y1,sBB.z1 );
	qDebug("	BoundingBox default (%g %g %g)--(%g %g %g)", BB.x0,BB.y0,BB.z0, BB.x1,BB.y1,BB.z1 );
}

////////////////////////////////////////////////////////////////////////////////

void Renderer_tex2::paint()
{
	//qDebug(" Renderer_tex2::paint(renderMode=%i)", renderMode);

	if (b_error) return; //080924 try to catch the memory error

	glClearColor(color_background.r, color_background.g, color_background.b, 0);
	glClearStencil(0);
	glClearDepth(1);
	glDepthRange(0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
				//GL_LEQUAL);

	glMatrixMode(GL_MODELVIEW);
	// here, MUST BE normalized space of [-1,+1]^3;

	glGetDoublev(GL_MODELVIEW_MATRIX, volumeViewMatrix); //no scale here, used for drawUnitVolume()

	glPushMatrix();
		setMarkerSpace(); // space to define marker & curve
		glGetIntegerv(GL_VIEWPORT,         viewport);            // used for selectObj(smMarkerCreate)
		glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);    // used for selectObj(smMarkerCreate)
		glGetDoublev(GL_MODELVIEW_MATRIX,  markerViewMatrix);    // used for selectObj(smMarkerCreate)
	glPopMatrix();

	bShowCSline = bShowAxes;
	bShowFSline = bShowBoundingBox;

	if (has_image())
	{
		glPushMatrix(); //===================================================== Volume {

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (renderMode==rmAlphaBlending || renderMode==rmMaxIntensityProjection) // not for rmCrossSection
			enableViewClipPlane(); //front-cut-plane

		// unit image space ==>fit in [-1,+1]^3
		setUnitVolumeSpace();
		glPushName(dcVolume);
			drawVol();
		glPopName();

		if (! b_selecting) if (bShowCSline && renderMode==rmCrossSection)
		{
			drawCrossLine(2);
		}
		if (! b_selecting) if (bShowFSline && bFSlice) //renderMode==rmCrossSection)
		{
			drawUnitFrontSlice(1); // just draw bound Line of F-Slice
		}

		if (renderMode==rmAlphaBlending || renderMode==rmMaxIntensityProjection)
			disableViewClipPlane();

		glPopMatrix(); //============================================================== }
	}

	if (sShowMarkers>0 || sShowSurfObjects>0)
	{
		if (polygonMode==1)	      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else if (polygonMode==2)  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		else                      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		setObjLighting();

		if (sShowSurfObjects>0)
		{
			glPushMatrix(); //================================================= SurfObject {

			// original surface object space ==>fit in [-1,+1]^3
			setSurfaceStretchSpace();
			glPushName(dcSurface);
				drawObj();  // neuron-swc, cell-apo, label-surf, etc
			glPopName();

			glPopMatrix(); //============================================================= }
		}

		if (sShowMarkers>0)
		{
			glPushMatrix(); //===================================================== Marker {

			// marker defined in original image space ==>fit in [-1,+1]^3
			setMarkerSpace();
			glPushName(dcSurface);
				drawMarker();  // just markers
			glPopName();

			glPopMatrix(); //============================================================= }
		}

		disObjLighting();
	}

	if (! b_selecting)
	{
		if (bShowBoundingBox || bShowAxes)
		{
			glPushMatrix(); //========================== default bounding frame & axes {

			// bounding box space ==>fit in [-1,+1]^3
			setObjectSpace();
			drawBoundingBoxAndAxes(boundingBox, 1, 3);

			glPopMatrix(); //========================================================= }
		}

		if (bShowBoundingBox2 && has_image() && !surfBoundingBox.isNegtive() )
		{
			glPushMatrix(); //============================ surface object bounding box {

			setSurfaceStretchSpace();
			drawBoundingBoxAndAxes(surfBoundingBox, 1, 0);

			glPopMatrix(); //========================================================= }
		}

	    if (bOrthoView)
	    {
			glPushMatrix(); //============================================== scale bar {

			drawScaleBar();

			glPopMatrix(); //========================================================= }
	    }
	}

	// must be at last
	if (! b_selecting && sShowTrack)
	{
		blendTrack();
	}

	return;
}


void Renderer_tex2::drawBackFillVolCube()
{
	if ((VOL_X1-VOL_X0<0)||(VOL_Y1-VOL_Y0<0)||(VOL_Z1-VOL_Z0<0)) return;

	glPushAttrib(GL_DEPTH_BUFFER_BIT);
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glBegin(GL_QUADS);
	{
		//yx0
		glVertex3f(VOL_X0, VOL_Y0, VOL_Z0);
		glVertex3f(VOL_X0, VOL_Y1, VOL_Z0);
		glVertex3f(VOL_X1, VOL_Y1, VOL_Z0);
		glVertex3f(VOL_X1, VOL_Y0, VOL_Z0);
		//x0z
		glVertex3f(VOL_X0, VOL_Y0, VOL_Z0);
		glVertex3f(VOL_X1, VOL_Y0, VOL_Z0);
		glVertex3f(VOL_X1, VOL_Y0, VOL_Z1);
		glVertex3f(VOL_X0, VOL_Y0, VOL_Z1);
		//0zy
		glVertex3f(VOL_X0, VOL_Y0, VOL_Z0);
		glVertex3f(VOL_X0, VOL_Y0, VOL_Z1);
		glVertex3f(VOL_X0, VOL_Y1, VOL_Z1);
		glVertex3f(VOL_X0, VOL_Y1, VOL_Z0);
		//xy1
		glVertex3f(VOL_X0, VOL_Y0, VOL_Z1);
		glVertex3f(VOL_X1, VOL_Y0, VOL_Z1);
		glVertex3f(VOL_X1, VOL_Y1, VOL_Z1);
		glVertex3f(VOL_X0, VOL_Y1, VOL_Z1);
		//z1x
		glVertex3f(VOL_X0, VOL_Y1, VOL_Z0);
		glVertex3f(VOL_X0, VOL_Y1, VOL_Z1);
		glVertex3f(VOL_X1, VOL_Y1, VOL_Z1);
		glVertex3f(VOL_X1, VOL_Y1, VOL_Z0);
		//1yz
		glVertex3f(VOL_X1, VOL_Y0, VOL_Z0);
		glVertex3f(VOL_X1, VOL_Y1, VOL_Z0);
		glVertex3f(VOL_X1, VOL_Y1, VOL_Z1);
		glVertex3f(VOL_X1, VOL_Y0, VOL_Z1);
	}
	glEnd();
	glPopAttrib();
}

void Renderer_tex2::drawCrossLine(float lineWidth)
{
	glPushAttrib(GL_LINE_BIT | GL_POLYGON_BIT);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//	glEnable(GL_POLYGON_OFFSET_LINE);
//	glPolygonOffset(-1, -1); // deal z-fighting, 081121

//	float d = 0.0001f; // here is unit volume
//	glDepthRange(0, 1-d);

	glLineStipple(1, 0x7070);
	glEnable(GL_LINE_STIPPLE);

	glLineWidth(lineWidth); // work only before glBegin(GL_LINES), by RZC 080827
	//glBegin(GL_QUADS);
	glBegin(GL_LINES);
	{
		glColor3fv(color_line.c);
		//x
		glVertex3f(0, VOL_Y0, VOL_Z0);		glVertex3f(1, VOL_Y0, VOL_Z0);
		//glVertex3f(1, VOL_Y0+d, VOL_Z0+d);	glVertex3f(0, VOL_Y0+d, VOL_Z0+d);
		//y
		glVertex3f(VOL_X0, 0, VOL_Z0);		glVertex3f(VOL_X0, 1, VOL_Z0);
		//glVertex3f(VOL_X0+d, 1, VOL_Z0+d);	glVertex3f(VOL_X0+d, 0, VOL_Z0+d);
		//z
		glVertex3f(VOL_X0, VOL_Y0, 0);		glVertex3f(VOL_X0, VOL_Y0, 1);
		//glVertex3f(VOL_X0+d, VOL_Y0+d, 1);	glVertex3f(VOL_X0+d, VOL_Y0+d, 0);
	}
	glEnd();

//	glDepthRange(0, 1);
	//glLineWidth(1);
	//glDisable(GL_LINE_STIPPLE);
	glPopAttrib();
}

void Renderer_tex2::equAlphaBlending()
{
	glBlendEquationEXT(GL_FUNC_ADD_EXT);//this is important
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// back to front when depth-test on, A for all of RGB
	//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);	// back to front when depth-test on, RGBA separated, strange color
}

void Renderer_tex2::equMaxIntensityProjection()
{
	glBlendEquationEXT(GL_MAX_EXT);    //seems not be controlled by GL_BLEND
}

void Renderer_tex2::equCrossSection()
{
	glBlendEquationEXT(GL_FUNC_ADD_EXT);//this is important
	glBlendColorEXT(1, 1, 1, 1-CSbeta);
	glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); // constant Alpha
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// back to front when depth-test on, Alpha for all of RGB
}

void Renderer_tex2::drawVol()
{
	if (volTimeOffset)
	{
		qDebug(" volTimeOffset = %d, volTimePoint = %d", volTimeOffset, volTimePoint);

		volTimePoint = CLAMP(0, imageT-1, volTimePoint);
		volTimeOffset = 0;
		subloadTex(volTimePoint, false);
	}

	//---------------------------------------
	if (CSbeta >=1) return; // 081202, avoid be hit when invisible

	float af = 3.0f;
				//1.0f;
	alpha_threshold = pow(double(CSbeta), double(af));
	color_proxy.a = pow(1.0-double(CSbeta), double(1.0/af)); //change to double() by PHC, 2010-05-20
	SLICE_COLOR = color_proxy;

	switch (renderMode)
	{
	case rmAlphaBlending:
		glEnable(GL_BLEND);      equAlphaBlending();
		glEnable(GL_ALPHA_TEST); glAlphaFunc(GL_GREATER, alpha_threshold); // > threshold Alpha
		break;

	case rmMaxIntensityProjection:
		if (has_image())
		{
			glColor3f(0, 0, 0);
			drawBackFillVolCube(); // clear the project region to zero for MIP
		}
		glEnable(GL_BLEND);      equMaxIntensityProjection();
		glEnable(GL_ALPHA_TEST); glAlphaFunc(GL_GEQUAL, alpha_threshold); // >= threshold Alpha, 080930
		break;

	case rmCrossSection:
		if (GLEE_EXT_blend_color)
		{
			glEnable(GL_BLEND);  equCrossSection();
		}
		else
			glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
//		if (CSbeta >=0.5)
//			glDisable(GL_DEPTH_TEST);
//		else
//			glEnable(GL_DEPTH_TEST);
		break;

	default: // unknown ?
		break;
	}

	// modulate by color_proxy, by RZC 080930
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // this is default, but make sure;
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glColor4fv(SLICE_COLOR.c);

	glDisable(GL_LIGHTING);

	glShadeModel(GL_FLAT);
	{
		updateVolCutRange();

		glEnable(GL_TEXTURE_3D); //glBindTexture(GL_TEXTURE_3D, 0);
		glEnable(GL_TEXTURE_2D); //glBindTexture(GL_TEXTURE_2D, 0);

		glPushName(renderMode);  //100729 make nameLength>2 to skip the Intel GL ICD bug
		{
			drawUnitVolume(); //100729 select name in drawStackX/Y/Z

			glPushName(vsFslice);  //100729 add select name of vsFslice
				drawUnitFrontSlice(0); // F-Slice
			glPopName();
		}
		glPopName();

		glDisable(GL_TEXTURE_3D);
		glDisable(GL_TEXTURE_2D);

	}
	glShadeModel(GL_SMOOTH);

	glDisable(GL_BLEND); //090429 RZC: no effect to glBlendEquationEXT(GL_MAX_EXT), must set to GL_FUNC_ADD_EXT
	glBlendEquationEXT(GL_FUNC_ADD_EXT);
	glDisable(GL_ALPHA_TEST);

}

/////////////////////////////////////////////////////////////////////////////
#define __renderer_detail__

void Renderer_tex2::setRenderMode(RenderMode rm)
{
	//qDebug(" Renderer_tex2::changeRenderMode = %i", rm);
	renderMode = rm;
}

void Renderer_tex2::updateVolCutRange()
{
	Renderer::updateVolCutRange();

	// [0,1] range cut box
	VOL_X0 = (imageX-1<=0)? 0: float(xCut0)/(imageX-1);
	VOL_X1 = (imageX-1<=0)? 0: float(xCut1)/(imageX-1);
	VOL_Y0 = (imageY-1<=0)? 0: float(yCut0)/(imageY-1);
	VOL_Y1 = (imageY-1<=0)? 0: float(yCut1)/(imageY-1);
	VOL_Z0 = (imageZ-1<=0)? 0: float(zCut0)/(imageZ-1);
	VOL_Z1 = (imageZ-1<=0)? 0: float(zCut1)/(imageZ-1);

	if (renderMode==rmCrossSection)
	{
		VOL_X1=VOL_X0;
		VOL_Y1=VOL_Y0;
		VOL_Z1=VOL_Z0;
		//100801
		dataViewProcBox = BoundingBox(start1, start2, start3,
								start1+(size1-1), start2+(size2-1), start3+(size3-1));
	}
	else
	{
//	//091120. 100405 PHC: add qMax to fix the bugs of unable to define marker on one slide, and fail to trace for one slice
//	dataViewProcBox = BoundingBox(start1+VOL_X0*size1, start2+VOL_Y0*size2, start3+VOL_Z0*size3,
//								  qMax(start1+VOL_X1*size1-1, float(0)),
//								  qMax(start2+VOL_Y1*size2-1, float(0)),
//								  qMax(start3+VOL_Z1*size3-1, float(0)));
		//100721: correct box bounding
		dataViewProcBox = BoundingBox(start1+VOL_X0*(size1-1), start2+VOL_Y0*(size2-1), start3+VOL_Z0*(size3-1),
								start1+VOL_X1*(size1-1), start2+VOL_Y1*(size2-1), start3+VOL_Z1*(size3-1));
	}
	//qDebug("	dataViewProcBox X(%g-%g)Y(%g-%g)Z(%g-%g)", dataViewProcBox.x0,dataViewProcBox.x1, dataViewProcBox.y0,dataViewProcBox.y1, dataViewProcBox.z0,dataViewProcBox.z1);
}

////////////////////////////////////////////////////////////////////////////////

void Renderer_tex2::toggleTexFilter()
{
	VOLUME_FILTER = (VOLUME_FILTER+1)%2;
	//qDebug( "	volume filter = %d", VOLUME_FILTER);
}

void Renderer_tex2::toggleTex2D3D()
{
	tryTex3D = !tryTex3D;
	//qDebug( "	tryTex3D = %d", tryTex3D);
	try	{
		PROGRESS_DIALOG( QObject::tr((tryTex3D)? "Try Texture 3D Format": "Try Texture 2D Format"), widget);
		PROGRESS_PERCENT(30);

		loadVol();

		PROGRESS_PERCENT(100);
	} CATCH_handler( "Renderer_tex2::toggleTex2D3D" );
}

void Renderer_tex2::toggleTexCompression()
{
	tryTexCompress = !tryTexCompress;
	//qDebug( "	tryTexCompress = %d", tryTex3D);
	try	{
		PROGRESS_DIALOG( QObject::tr((tryTexCompress)? "Try Texture Compression Format": "Try Texture UnCompression Format"), widget);
		PROGRESS_PERCENT(30);

		loadVol();

		PROGRESS_PERCENT(100);
	} CATCH_handler( "Renderer_tex2::toggleTexCompression" );
}

////////////////////////////////////////////////////////////////////////////////

int  Renderer_tex2::_getBufFillSize(int w)
{
#if BUFFER_NPT
	return w;
#else
	return power_of_two_ceil(w); //always use power_of_two
#endif
}
int  Renderer_tex2::_getTexFillSize(int w)
{
	if (tryTexNPT)  return w;
	else            return power_of_two_ceil(w);
}

void Renderer_tex2::cleanVol()
{
	qDebug("   Renderer_tex2::cleanVol");
	makeCurrent(); //ensure right context when multiple views animation or mouse drop, 081105

	if (Zslice_data) delete[] Zslice_data;	Zslice_data = 0;
	if (Yslice_data) delete[] Yslice_data;	Yslice_data = 0;
	if (Xslice_data) delete[] Xslice_data;	Xslice_data = 0;
	if (Fslice_data) delete[] Fslice_data;	Fslice_data = 0;

	// solved by makeCurrent: explicit delete of texture-ids causes other view's texture-ids may be deleted. 081021
	if (tex3D) {
		glDeleteTextures(1, &tex3D);
		tex3D = 0;
	}
	if (Ztex_list) { //+1 for pbo tex
		glDeleteTextures(imageZ+1, Ztex_list);
		delete[] Ztex_list;	Ztex_list = 0;
	}
	if (Ytex_list) {
		glDeleteTextures(imageY+1, Ytex_list);
		delete[] Ytex_list;	Ytex_list = 0;
	}
	if (Xtex_list) {
		glDeleteTextures(imageX+1, Xtex_list);
		delete[] Xtex_list;	Xtex_list = 0;
	}
	if (texFslice) {
		glDeleteTextures(1, &texFslice);
		texFslice = 0;
	}

	texture_format = image_format = image_type = -1;
	imageX = imageY = imageZ = imageT = 0;
	safeX = safeY = safeZ = 0;
	fillX = fillY = fillZ = fillF = 0;
	realX = realY = realZ = realF = 0;
}

void Renderer_tex2::loadVol()
{
	cleanVol(); // 081006: move to before setting imageX/Y/Z, 090705 move to first line
	cleanTexStreamBuffer(); //091012

	qDebug("  Renderer_tex2::loadVol");
	makeCurrent(); //ensure right context when multiple views animation or mouse drop, 081105

	if (! rgbaBuf || bufSize[3]<1 ) return; // no image data, 081002

	////////////////////////////////////////////////////////////////
	// set coordinate frame size
	sampleScaleX = sampleScale[0];
	sampleScaleY = sampleScale[1];
	sampleScaleZ = sampleScale[2];
	imageX = bufSize[0];
	imageY = bufSize[1];
	imageZ = bufSize[2];
	imageT = bufSize[4];

	bool ok;
	if ( !(ok = supported_TexNPT()) )
		tryTexNPT = 0;
	//qDebug()<< QString("	ARB_texture_non_power_of_two          %1 supported ").arg(ok?"":"NOT");

	if ( !(ok = supported_TexCompression()) )
		tryTexCompress = 0;
	//qDebug()<< QString("	ARB_texture_compression               %1 supported ").arg(ok?"":"NOT");

	if ( !(ok = supported_Tex3D()) )
		tryTex3D = 0;
	//qDebug()<< QString("	EXT_texture3D (or OpenGL 2.0)         %1 supported ").arg(ok?"":"NOT");

	if ( !(ok = supported_TexStream()) )
		if (tryTexStream != -1)
			tryTexStream = 0;
	//qDebug()<< QString("	texture stream (need PBO and GLSL)    %1 supported ").arg(ok?"":"NOT");

	ok = supported_GL2();
	//qDebug()<< QString("	GLSL (and OpenGL 2.0)                 %1 supported ").arg(ok?"":"NOT");


	if (imageT>1) //090802: TexSubImage conflicts against compressed texture2D, but is good for compressed texture3D
	{
		//tryTex3D = 1; 			qDebug("	Turn on tryTex3D for Time series");
		tryTexCompress = 0;		qDebug("		Turn off tryTexCompress for time series");
		tryTexStream = 0;		qDebug("		Turn off tryTexStream for time series");
	}

//	// comment for easy test on small volume
//	if (IS_FITTED_VOLUME(imageX,imageY,imageZ))
//	{
//		if (tryTexStream==1)
//		{
//			qDebug("	No need texture stream for small volume (fitted in %dx%dx%d)", LIMIT_VOLX,LIMIT_VOLY,LIMIT_VOLZ);
//			tryTexStream = 0;  // no need stream, because volume can be fitted in video memory
//		}
//	}

	////////////////////////////////////////////////////////////////
	// coordinate system
	//
	//     y_slice[z][x]
	//      |
	//      |
	//      |_______ x_slice[z][y]
	//     /
	//    /z_slice[y][x]
	//
	////////////////////////////////////////////////////////////////
	QTime qtime;  qtime.start();
	qDebug("   setupStack start --- try %s", try_vol_state());

	fillX = _getTexFillSize(imageX);
	fillY = _getTexFillSize(imageY);
	fillZ = _getTexFillSize(imageZ);
	qDebug("   sampleScale = %gx%gx%g""   sampledImage = %dx%dx%d""   fillTexture = %dx%dx%d",
			sampleScaleX, sampleScaleY, sampleScaleZ,  imageX, imageY, imageZ,  fillX, fillY, fillZ);

	if (tryTex3D && supported_Tex3D())
	{
		glGenTextures(1, &tex3D);		//qDebug("	tex3D = %u", tex3D);
	}
	if (!tex3D || tryTexStream !=0) //stream = -1/1/2
	{
		//tryTex3D = 0; //091015: no need, because tex3D & tex_stream_buffer is not related now.

		Ztex_list = new GLuint[imageZ+1]; //+1 for pbo tex
		Ytex_list = new GLuint[imageY+1];
		Xtex_list = new GLuint[imageX+1];
		memset(Ztex_list, 0, sizeof(GLuint)*(imageZ+1));
		memset(Ytex_list, 0, sizeof(GLuint)*(imageY+1));
		memset(Xtex_list, 0, sizeof(GLuint)*(imageX+1));
		glGenTextures(imageZ+1, Ztex_list);
		glGenTextures(imageY+1, Ytex_list);
		glGenTextures(imageX+1, Xtex_list);

		CHECK_GLErrorString_throw(); // can throw const char* exception, RZC 080925

		int X = _getBufFillSize(imageX);
		int Y = _getBufFillSize(imageY);
		int Z = _getBufFillSize(imageZ);
		Zslice_data = new RGBA8 [Y * X];//[Z][y][x] //base order
		Yslice_data = new RGBA8 [Z * X];//[Y][z][x]
		Xslice_data = new RGBA8 [Z * Y];//[X][z][y]
		memset(Zslice_data, 0, sizeof(RGBA8)* (Y * X));
		memset(Yslice_data, 0, sizeof(RGBA8)* (Z * X));
		memset(Xslice_data, 0, sizeof(RGBA8)* (Z * Y));

		// optimized copy slice data in setupStackTexture, by RZC 2008-10-04
	}

	qDebug("   setupStack: id & buffer ....................... cost time = %g sec", qtime.elapsed()*0.001);


	///////////////////////////////////////
	if (texture_format==-1)
	{
		texture_format = GL_RGBA;
		//Call TexImage with a generic compressed internal format. The texture image will be compressed by the GL, if possible.
		//Call CompressedTexImage to Load pre-compressed image.
		//S3TC: DXT1(1bit alpha), DXT3(sharp alpha), DXT5(smooth alpha)
		//glHint(GL_TEXTURE_COMPRESSION_HINT_ARB, GL_NICEST); // seems no use, choice DXT3, but DXT5 is better, 081020
		if (tryTexCompress && GLEE_ARB_texture_compression)
			texture_format = GL_COMPRESSED_RGBA_ARB;
		if (texture_format==GL_COMPRESSED_RGBA_ARB && GLEE_EXT_texture_compression_s3tc)
			texture_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}
	if (image_format==-1)
	{
		image_format = GL_RGBA;
	}
	if (image_type==-1)
	{
		image_type = GL_UNSIGNED_BYTE;
	}

	subloadTex(volTimePoint, true);   // first loading
	///////////////////////////////////////

	//091013
	V3dR_GLWidget* w = (V3dR_GLWidget*)widget;
	if (w)  w->updateControl();
}

void Renderer_tex2::subloadTex(V3DLONG timepoint, bool bfirst)
{
	if (texture_format==-1)  return; // not done by loadVol
	if (! rgbaBuf || bufSize[3]<1 ) return; // no image data, 081002

	QTime qtime;  qtime.start();
	{
		timepoint = CLAMP(0, imageT-1, timepoint);
		rgbaBuf = total_rgbaBuf + timepoint*(imageZ*imageY*imageX);

		//if (tryTexStream<=0) 			// 091014: mix down-sampled & streamed method
			setupStackTexture(bfirst);  // use a temporary buffer, so first

		if (tryTexStream >0 && bfirst)
			setupTexStreamBuffer();

	}
	qDebug("   subloadTex [%d]: %s ......... cost time = %g sec", timepoint, try_vol_state(), qtime.elapsed()*0.001);
}

////////////////////////////////////////////////////////////////////////////////
void _copySliceFromStack(RGBA8* rgbaBuf, int bufX, int bufY, int bufZ,
						RGBA8* slice, int copyW, int stack_i, int slice_i,
						RGBA8* rgbaYzx, RGBA8* rgbaXzy)
{
	V3DLONG x, y, z;
	switch (stack_i)
	{
	case 1: //Zslice_data[y][x]
		z = slice_i;
		if (copyW == bufX)
			memcpy( slice,  rgbaBuf + z*(bufY*bufX), sizeof(RGBA8)*bufY*bufX);
		else
			for (y = 0; y < bufY; y++)
			{
				memcpy( slice + y*(copyW),  rgbaBuf + z*(bufY*bufX) + y*(bufX), sizeof(RGBA8)*bufX);
			}
		break;

	case 2: //Yslice_data[z][x]
		y = slice_i;
		if (rgbaYzx  && copyW == bufX)
			memcpy( slice,  rgbaYzx + y*(bufZ*bufX), sizeof(RGBA8)*bufZ*bufX);
		else
			for (z = 0; z < bufZ; z++)
			{
				memcpy( slice + z*(copyW),  rgbaBuf + z*(bufY*bufX) + y*(bufX), sizeof(RGBA8)*bufX);
			}
		break;

	case 3: //Xslice_data[z][y]
		x = slice_i;
		if (rgbaXzy  && copyW == bufY)
			memcpy( slice,  rgbaXzy + x*(bufZ*bufY), sizeof(RGBA8)*bufZ*bufY);
		else if (rgbaXzy)
			for (z = 0; z < bufZ; z++)
			{
				memcpy( slice + z*(copyW),  rgbaXzy + x*(bufZ*bufY) + z*(bufY), sizeof(RGBA8)*bufY);
			}
		else
			for (z = 0; z < bufZ; z++)
			{
				for (y = 0; y < bufY; y++)
				{
					slice[ z*(copyW) + y] = rgbaBuf[ z*(bufY*bufX) + y*(bufX) + x];
				}
			}
		break;
	}
}

void _copyXzyFromZyx(RGBA8* rgbaXzy, RGBA8* rgbaZyx, int bufX, int bufY, int bufZ)
{
	V3DLONG x, y, z;
	for (z = 0; z < bufZ; z++)
	{
		for (y = 0; y < bufY; y++)
		{
			for (x = 0; x < bufX; x++)
			{
				rgbaXzy[ x*(bufZ * bufY) + z*(bufY) + y] = rgbaZyx[ z*(bufY * bufX) + y*(bufX) + x];
			}
		}
	}
}
void _copyYzxFromZyx(RGBA8* rgbaYzx, RGBA8* rgbaZyx, int bufX, int bufY, int bufZ)
{
	V3DLONG x, y, z;
	for (z = 0; z < bufZ; z++)
	{
		for (y = 0; y < bufY; y++)
		{
			for (x = 0; x < bufX; x++)
			{
				rgbaYzx[ y*(bufZ * bufX) + z*(bufX) + x] = rgbaZyx[ z*(bufY * bufX) + y*(bufX) + x];
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
static RGBA8* _safe3DBuf = 0;  /// (256^3)*4==64M  (512x512x256)*4==256M
static void _safeRelease3DBuf()
{
	if (_safe3DBuf) delete[] _safe3DBuf;  _safe3DBuf = 0;
}
static RGBA8* _safeReference3DBuf(RGBA8* rgbaBuf, int bufX, int bufY, int bufZ,
		 int &safeX, int &safeY, int &safeZ)
{
	_safeRelease3DBuf();
//	V3DLONG limitX = MAX_3DTEX_SIZE;
//	V3DLONG limitY = MAX_3DTEX_SIZE;
//	V3DLONG limitZ = MAX_3DTEX_SIZE;
	V3DLONG limitX = LIMIT_VOLX;
	V3DLONG limitY = LIMIT_VOLY;
	V3DLONG limitZ = LIMIT_VOLZ;
	V3DLONG fillX = power_of_two_ceil(bufX);
	V3DLONG fillY = power_of_two_ceil(bufY);
	V3DLONG fillZ = power_of_two_ceil(bufZ);
	if (fillX<=limitX && fillY<=limitY && fillZ<=limitZ)
	{
		safeX = bufX;
		safeY = bufY;
		safeZ = bufZ;
		return rgbaBuf;
	}

	float sx, sy, sz;
	V3DLONG dx, dy, dz;
	sx = float(bufX)/MIN(limitX, bufX);
	sy = float(bufY)/MIN(limitY, bufY);
	sz = float(bufZ)/MIN(limitZ, bufZ);
	dx = V3DLONG(sx);
	dy = V3DLONG(sy);
	dz = V3DLONG(sz);
	MESSAGE_ASSERT(dx*dy*dz >=1); // down sampling
	limitX = V3DLONG(bufX/sx);
	limitY = V3DLONG(bufY/sy);
	limitZ = V3DLONG(bufZ/sz);
	safeX = limitX;
	safeY = limitY;
	safeZ = limitZ;
	//qDebug("	safe = %dx%dx%d", limitX, limitY, limitZ);

	_safe3DBuf = new RGBA8[safeX*safeY*safeZ];
	//memset(_safe3DBuf, 0, sizeof(RGBA8)*safeX*safeY*safeZ);
	V3DLONG ox, oy, oz;
	V3DLONG ix, iy, iz;
	for (oz = 0; oz < safeZ; oz++)
		for (oy = 0; oy < safeY; oy++)
			for (ox = 0; ox < safeX; ox++)
			{
				ix = CLAMP(0,bufX-1, IROUND(ox*sx));
				iy = CLAMP(0,bufY-1, IROUND(oy*sy));
				iz = CLAMP(0,bufZ-1, IROUND(oz*sz));

				RGBA32f rgbaf;
				RGBA8 rgba8;
				rgbaf = sampling3dRGBA8( rgbaBuf, bufX, bufY, bufZ, ix, iy, iz, dx, dy, dz);
				rgba8.r = (unsigned char)rgbaf.r;
				rgba8.g = (unsigned char)rgbaf.g;
				rgba8.b = (unsigned char)rgbaf.b;
				rgba8.a = (unsigned char)rgbaf.a;

				_safe3DBuf[ oz*(safeY*safeX) + oy*(safeX) + ox] = rgba8;
			}
	return _safe3DBuf;
}
//////////////////////////////////////////////////////////////

//#define TEXTURE_CLAMP GL_CLAMP
#define TEXTURE_CLAMP GL_CLAMP_TO_EDGE

void Renderer_tex2::setTexParam3D()
{
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, TEXTURE_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, TEXTURE_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, TEXTURE_CLAMP);
	if (VOLUME_FILTER == 1) {
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}
void Renderer_tex2::setTexParam2D()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TEXTURE_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TEXTURE_CLAMP);
	if (VOLUME_FILTER == 1) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}


#define BIND_TEXTURE_0()  //{glBindTexture(GL_TEXTURE_3D, 0); glBindTexture(GL_TEXTURE_2D, 0);}

#define TEXTURE_UNIT0_3D(b) \
if (b) \
{ \
	texture_unit0_3D = true;  glDisable(GL_TEXTURE_2D);  glEnable(GL_TEXTURE_3D); \
} \
else \
{ \
	texture_unit0_3D = false; glDisable(GL_TEXTURE_3D);  glEnable(GL_TEXTURE_2D); \
}


void Renderer_tex2::setupStackTexture(bool bfirst)
{
	// In OpenGL 1.0, storing texture maps in display lists was the preferred method for optimizing performance.
	// However, a better solution, texture objects, introduced in OpenGL 1.1, are preferred.
	//glistTexs1 = glGenLists(imageZ);
	//glistTexs2 = glGenLists(imageY);
	//glistTexs3 = glGenLists(imageX);
	//CHECK_GLErrorString_throw(); // can throw const char* exception, RZC 080925

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//090802: seems glSubImage conflicts against all compression texture2D, and large compression texture3D
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	RGBA8* tex3DBuf = NULL;
	if (tryTexStream == -1) //091016
	{
		tex3DBuf = rgbaBuf;
		realX = imageX;
		realY = imageY;
		realZ = imageZ;
	}
	else
	{
		tex3DBuf = _safeReference3DBuf(rgbaBuf, imageX, imageY, imageZ,  safeX, safeY, safeZ); //081008
		realX = safeX;
		realY = safeY;
		realZ = safeZ;
	}
	fillX = _getTexFillSize(realX);
	fillY = _getTexFillSize(realY);
	fillZ = _getTexFillSize(realZ);

	qDebug("	texture:   real = %dx%dx%d   fill = %dx%dx%d",  realX, realY, realZ,  fillX, fillY, fillZ);

	if (tex3D) // all in 1 texture
	{
		glBindTexture(GL_TEXTURE_3D, tex3D);
		if (bfirst)
		{
			setTexParam3D();
			glTexImage3D(GL_TEXTURE_3D, // target
				0, // level
				texture_format, // texture format
				fillX, // width
				fillY, // height
				fillZ, // depth
				0, // border
				image_format, // image format
				image_type, // image type
				NULL);
			CHECK_GLErrorString_throw(); // can throw const char* exception, RZC 080925
		}
		///else  // compressed texture cannot using TexSubImage2D but TexSubImage3D !!!
		{
			glTexSubImage3D(GL_TEXTURE_3D, // target
				0, // level
				0,0,0,  // offset
				realX, // sub width
				realY, // sub height
				realZ, // sub depth
				image_format, // image format
				image_type, // image type
				tex3DBuf);
		}
		CHECK_GLErrorString_throw(); // can throw const char* exception, RZC 080925

	}
	else
	// 3 stacks of 2D texture ===================================================================
	for (int stack_i=1; stack_i<=3; stack_i++)
	{

		int n_slice = 0;
		RGBA8* p_slice = 0;
		GLuint* p_tex = 0;
		int w = 0, h =0;
		int sw = 0, sh = 0;

		switch (stack_i)
		{
		case 1: //Z[y][x]
			n_slice = realZ;
			p_slice = Zslice_data;
			p_tex = Ztex_list;
			w = fillX, h = fillY;
			sw = COPY_X, sh = COPY_Y;
			break;
		case 2: //Y[z][x]
			n_slice = realY;
			p_slice = Yslice_data;
			p_tex = Ytex_list;
			w = fillX, h = fillZ;
			sw = COPY_X, sh = COPY_Z;
			break;
		case 3: //X[z][y]
			n_slice = realX;
			p_slice = Xslice_data;
			p_tex = Xtex_list;
			w = fillY, h = fillZ;
			sw = COPY_Y, sh = COPY_Z;
			break;
		}

		MESSAGE_ASSERT(imageX>=realX && imageY>=realY && imageZ>=realZ);
		MESSAGE_ASSERT(COPY_X>=realX && COPY_Y>=realY && COPY_Z>=realZ);
//		sw+=1, sh+=1;  // to get rid of artifacts at sub-tex border // or use NPT tex
//		if (sw>w) sw = w;
//		if (sh>h) sh = h;

		for (int i = 0; i < n_slice; i++)
		{
			glBindTexture(GL_TEXTURE_2D, p_tex[i+1]); //[0] reserved for pbo tex
			RGBA8* p_first = NULL;

			if (bfirst)
			{
				p_first = p_slice;
				if (p_first) _copySliceFromStack(tex3DBuf, realX,realY,realZ,  p_first, w,  stack_i, i);

				setTexParam2D();
				glTexImage2D(GL_TEXTURE_2D, // target
					0, // level
					texture_format, // texture format
					w, // width
					h, // height
					0, // border
					image_format, // image format
					image_type, // image type
					p_first);
				CHECK_GLErrorString_throw(); // can throw const char* exception, RZC 080925
			}
			// Notice: compressed texture is corrupted with TexSubImage2D but TexSubImage3D !!!
			//else
			if (p_first==NULL)
			{
				_copySliceFromStack(tex3DBuf, realX,realY,realZ,  p_slice, sw,  stack_i, i);

				glTexSubImage2D(GL_TEXTURE_2D, // target
					0, // level
					0,0,  // offset
					sw, // sub width
					sh, // sub height
					image_format, // image format
					image_type, // image type
					p_slice);
				CHECK_GLErrorString_throw(); // can throw const char* exception, RZC 080925
			}
		}

	}//tex2D

	_safeRelease3DBuf();

	BIND_TEXTURE_0();
}

////////////////////////////////////////////////////////////////////////////////

void Renderer_tex2::_drawStack( double ts, double th, double tw,
		double s0, double s1, double h0, double h1, double w0, double w1,
		double ds, int slice0, int slice1, int thickness,
		GLuint tex3D, GLuint texs[], int stack_i,
		float direction, int section, bool b_tex3d, bool b_stream)
{
	//qDebug("		s(%g-%g)h(%g-%g)w(%g-%g)", s0,s1, h0,h1, w0,w1);
	if ((s1-s0<0)||(h1-h0<0)||(w1-w0<0)) return; // no draw
	if (thickness <1) return; // only support thickness>=1

	if (section >0) { // cross-section
		h0 = 0;		h1 = 1;
		w0 = 0;		w1 = 1;
		s1 = s0;
		slice1 = slice0;
	}

//	double moreslice = ((tex3D)? 4 : 1); // 081009: more slice for tex3D
	double step, slice, s;
	if (direction <0) {
		step = (+1); ///moreslice);
		slice = slice0;
		s = s0;
	} else {
		step = (-1); ///moreslice);
		slice = slice1;
		s = s1;
	}

	double tw0 = tw*w0;  double tw1 = tw*w1;
	double th0 = th*h0;  double th1 = th*h1;

	for (;
		slice0 <= slice && slice <= slice1;
		slice += step, s += step * ds
		)
	{
		if (!b_tex3d)
		{
			if (b_stream)
			{
				_streamTex(stack_i, int(slice), int(step), slice0, slice1);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texs[int(slice)+1]); //[0] reserved for pbo tex
				setTexParam2D();
			}
		}
		else // (btex_3d && !btex_stream)
		{
			glBindTexture(GL_TEXTURE_3D, tex3D);
			setTexParam3D();
		}

		double tss = ts*s;
		int k_repeat = thickness;
		if ((thickness==1)||(step>0 && slice==slice1)||(step<0 && slice==slice0)) k_repeat = 1; // 081106

		for (int k=0; k<k_repeat; k++) // 081105
		{
			double ids = step * k*ds/thickness;
			double idts = ts*ids;

			glBegin(GL_QUADS);

			if (!b_tex3d)  glTexCoord2d(tw0, th0);
			else if (stack_i==1) glTexCoord3d(tw0, th0, tss +idts);
			else if (stack_i==2) glTexCoord3d(tw0, tss +idts, th0);
			else if (stack_i==3) glTexCoord3d(tss +idts, tw0, th0);
			if      (stack_i==1) glVertex3d(w0, h0, s +ids);
			else if (stack_i==2) glVertex3d(w0, s +ids, h0);
			else if (stack_i==3) glVertex3d(s +ids, w0, h0);

			if (!b_tex3d)  glTexCoord2d(tw1, th0);
			else if (stack_i==1) glTexCoord3d(tw1, th0, tss +idts);
			else if (stack_i==2) glTexCoord3d(tw1, tss +idts, th0);
			else if (stack_i==3) glTexCoord3d(tss +idts, tw1, th0);
			if      (stack_i==1) glVertex3d(w1, h0, s +ids);
			else if (stack_i==2) glVertex3d(w1, s +ids, h0);
			else if (stack_i==3) glVertex3d(s +ids, w1, h0);

			if (!b_tex3d)  glTexCoord2d(tw1, th1);
			else if (stack_i==1) glTexCoord3d(tw1, th1, tss +idts);
			else if (stack_i==2) glTexCoord3d(tw1, tss +idts, th1);
			else if (stack_i==3) glTexCoord3d(tss +idts, tw1, th1);
			if      (stack_i==1) glVertex3d(w1, h1, s +ids);
			else if (stack_i==2) glVertex3d(w1, s +ids, h1);
			else if (stack_i==3) glVertex3d(s +ids, w1, h1);

			if (!b_tex3d)  glTexCoord2d(tw0, th1);
			else if (stack_i==1) glTexCoord3d(tw0, th1, tss +idts);
			else if (stack_i==2) glTexCoord3d(tw0, tss +idts, th1);
			else if (stack_i==3) glTexCoord3d(tss +idts, tw0, th1);
			if      (stack_i==1) glVertex3d(w0, h1, s +ids);
			else if (stack_i==2) glVertex3d(w0, s +ids, h1);
			else if (stack_i==3) glVertex3d(s +ids, w0, h1);

			glEnd();
		}
	}
	if (b_stream) _streamTex_end();
}

#define SET_Priority( stack_i ) // \
//{ \
//	GLclampf Priority; \
//	if (tex3D) { \
//		Priority = .5f; \
//		glPrioritizeTextures(1, &tex3D, &Priority); \
//	} else { \
//		Priority = (.1f + .5f*(stack_i==1)); \
//		for (int i=0; i<realZ; i++) glPrioritizeTextures(1, Ztex_list+i, &Priority); \
//		Priority = (.1f + .5f*(stack_i==2)); \
//		for (int i=0; i<realY; i++) glPrioritizeTextures(1, Ytex_list+i, &Priority); \
//		Priority = (.1f + .5f*(stack_i==3)); \
//		for (int i=0; i<realX; i++) glPrioritizeTextures(1, Ztex_list+i, &Priority); \
//	} \
//}

void Renderer_tex2::drawStackZ(float direction, int section, bool t3d, bool stream)
{
	double ts = double(realZ) /fillZ;
	double th = double(realY) /fillY;
	double tw = double(realX) /fillX;
	double s0 = VOL_Z0;
	double s1 = VOL_Z1;
	double h0 = VOL_Y0;
	double h1 = VOL_Y1;
	double w0 = VOL_X0;
	double w1 = VOL_X1;
	double ds = (realZ<=1)? 0 : (1.f / (realZ-1));
	int slice0 = s0*((realZ<=1)? 0 : realZ-1); //zCut0;
	int slice1 = s1*((realZ<=1)? 0 : realZ-1); //zCut1;
	//slice0 = 0; 	Zslice_data = imageZ-1;

	SET_Priority( 1 );
	glPushName(vsZslice);
	_drawStack( ts,th,tw,  s0,s1, h0,h1, w0,w1,  ds, slice0,slice1, int(thicknessZ),
			tex3D, Ztex_list, (1),
			direction, section, t3d, stream);
	glPopName();
}
void Renderer_tex2::drawStackY(float direction, int section, bool t3d, bool stream)
{
	double ts = double(realY) /fillY;
	double th = double(realZ) /fillZ;
	double tw = double(realX) /fillX;
	double s0 = VOL_Y0;
	double s1 = VOL_Y1;
	double h0 = VOL_Z0;
	double h1 = VOL_Z1;
	double w0 = VOL_X0;
	double w1 = VOL_X1;
	double ds = (realY<=1)? 0 : (1.f / (realY-1));
	int slice0 = s0*((realY<=1)? 0 : realY-1); //yCut0;
	int slice1 = s1*((realY<=1)? 0 : realY-1); //yCut1;
	//slice0 = 0; 	Zslice_data = imageY-1;

	SET_Priority( 2 );
	glPushName(vsYslice);
	_drawStack( ts,th,tw,  s0,s1, h0,h1, w0,w1,  ds, slice0,slice1, int(thicknessY),
			tex3D, Ytex_list, (2),
			direction, section, t3d, stream);
	glPopName();
}
void Renderer_tex2::drawStackX(float direction, int section, bool t3d, bool stream)
{
	double ts = double(realX) /fillX;
	double th = double(realZ) /fillZ;
	double tw = double(realY) /fillY;
	double s0 = VOL_X0;
	double s1 = VOL_X1;
	double h0 = VOL_Z0;
	double h1 = VOL_Z1;
	double w0 = VOL_Y0;
	double w1 = VOL_Y1;
	double ds = (realX<=1)? 0 : (1.f / (realX-1));
	int slice0 = s0*((realX<=1)? 0 : realX-1); //xCut0;
	int slice1 = s1*((realX<=1)? 0 : realX-1); //xCut1;
	//slice0 = 0; 	Zslice_data = imageX-1;

	SET_Priority( 3 );
	glPushName(vsXslice);
	_drawStack( ts,th,tw,  s0,s1, h0,h1, w0,w1,  ds, slice0,slice1, int(thicknessX),
			tex3D, Xtex_list, (3),
			direction, section, t3d, stream);
	glPopName();
}

////////////////////////////////////////////////////

void Renderer_tex2::setUnitVolumeSpace()
{
	BoundingBox BB = boundingBox;  // a copy
	float DX = BB.Dx();
	float DY = BB.Dy();
	float DZ = BB.Dz();
	float maxD = BB.Dmax();

	float s[3];
	s[0] = DX /maxD *2;
	s[1] = DY /maxD *2;
	s[2] = DZ /maxD *2;

	// form unit volume space ==> fit in [-1, +1]^3
	glScaled(s[0], s[1], s[2]);
	//qDebug("Scale from [0,1]: x=%f, y=%f, z=%f", s[0],s[1],s[2]);
	glTranslated(-.5, -.5, -.5);
}

void Renderer_tex2::drawUnitVolume()
{
	if (! rgbaBuf || bufSize[3]<1 ) return; // no image data, 081002
	if ((VOL_X1<VOL_X0) || (VOL_Y1<VOL_Y0) || (VOL_Z1<VOL_Z0)) return; // all clipped, no draw

	bool b_stream = _streamingTex();
	bool b_tex3d = tex3D>0;

	if (b_stream   //091014: for streamed method
		|| tryTexStream == -1) //091016
	{
		realX = imageX;
		realY = imageY;
		realZ = imageZ;
	}
	else
	{
		realX = safeX;
		realY = safeY;
		realZ = safeZ;
	}
//	realX = ABS(realX); //???
//	realY = ABS(realY);
//	realZ = ABS(realZ);
	fillX = _getTexFillSize(realX);
	fillY = _getTexFillSize(realY);
	fillZ = _getTexFillSize(realZ);

	//qDebug("	drawUnitVolume:   real = %dx%dx%d   fill = %dx%dx%d,  stream = %d",  realX, realY, realZ,  fillX, fillY, fillZ, b_stream);
	if (realX<=0 || realY<=0 || realZ<=0) return; //081026 to prevent drawing before subloadTex


	double mm[4][4];
	//glGetDoublev(GL_MODELVIEW_MATRIX, &mm[0][0]); //have scale & shift when here, cannot treat as pure rotation
	MAT16_TO_MAT4x4( volumeViewMatrix, mm );

	//mm = [u,v,w,t], a = [w-t](4d->3d)   object z-direction vector in View space
	double ax, ay, az;
	ax = (mm[0][2] - mm[0][3]) / (mm[3][2] - mm[3][3]);
	ay = (mm[1][2] - mm[1][3]) / (mm[3][2] - mm[3][3]);
	az = (mm[2][2] - mm[2][3]) / (mm[3][2] - mm[3][3]);


	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	if (b_stream || !b_tex3d)
	{
		TEXTURE_UNIT0_3D(false);
		b_tex3d = false; ////
	}
	else // (b_tex3d && !b_stream)
	{
		TEXTURE_UNIT0_3D(true);
	}
	shaderTexBegin(b_stream);
	{

		int section = (renderMode == rmCrossSection);
		// cross section =============================================
		if (renderMode == rmCrossSection)
		{
			if (bXSlice)	drawStackX(-1, section, b_tex3d, b_stream);
			if (bYSlice)	drawStackY(-1, section, b_tex3d, b_stream);
			if (bZSlice)	drawStackZ(-1, section, b_tex3d, b_stream);
			//if (bFSlice)	;//in drawUnitFrontSlice

		}
		// only for one slice //090601: or 2 slice (texture compressed) //100721: cut slice
		else if (xCut1-xCut0<=1)
		{
			drawStackX(-1, section, b_tex3d, b_stream);
		}
		else if (yCut1-yCut0<=1)
		{
			drawStackY(-1, section, b_tex3d, b_stream);
		}
		else if (zCut1-zCut0<=1)
		{
			drawStackZ(-1, section, b_tex3d, b_stream);
		}
		// volume //=================================================
		else if ((fabs(ax) >= fabs(ay)) && (fabs(ax) > fabs(az)))  //100813 |ax|==|ay|>|az|
		{
			//SLICE_COLOR.a *= 1+fabs(d.x);		glColor4fv(SLICE_COLOR.c);
			drawStackX(ax, section, b_tex3d, b_stream);
		}
		else if ((fabs(ay) > fabs(ax)) && (fabs(ay) > fabs(az)))
		{
			//SLICE_COLOR.a *= 1+fabs(d.y);		glColor4fv(SLICE_COLOR.c);
			drawStackY(ay, section, b_tex3d, b_stream);
		}
		else
		{
			//SLICE_COLOR.a *= 1+fabs(d.z);		glColor4fv(SLICE_COLOR.c);
			drawStackZ(az, section, b_tex3d, b_stream);
		}

	}
	shaderTexEnd();

	BIND_TEXTURE_0();
}

/////////////////////////////////////////////////////////////////////////////////////////
static void _frontSliceFromStack(RGBA8* rgbaBuf, int imageX, int imageY, int imageZ,
							RGBA8* slice, int copyX, int copyY,
							XYZ& S0, XYZ& Sx, XYZ& Sy)
{
	memset(slice, 0, sizeof(RGBA8)* (copyX * copyY)); // clear to black

	XYZ Tx = Sx - S0;
	XYZ Ty = Sy - S0;

	int ox, oy;
	int ix, iy, iz;
	for (oy = 0; oy < copyY; oy++)
		for (ox = 0; ox < copyX; ox++)
	{
		float tx = float(ox)/(copyX-1);
		float ty = float(oy)/(copyY-1);

		XYZ Tf = Tx * tx + Ty * ty + S0;

		ix = IROUND(Tf.x * (imageX-1));
		iy = IROUND(Tf.y * (imageY-1));
		iz = IROUND(Tf.z * (imageZ-1));

		if (BETWEENEQ(0,imageX-1,ix) && BETWEENEQ(0,imageY-1,iy) && BETWEENEQ(0,imageZ-1,iz))
		{
			slice[oy * (copyX) + ox] = rgbaBuf[iz * (imageY * imageX) + iy * (imageX) + ix];
		}
	}
}

void Renderer_tex2::setupFrontSliceBuffer()
{
	if (Fslice_data==0 && texFslice==0)
	{
		realF = MAX(imageX, MAX(imageY, imageZ));
		fillF = _getTexFillSize(realF);
		Fslice_data = new RGBA8[(realF * realF)];
//	}
//	if (texFslice==0)
//	{
		glGenTextures(1, &texFslice);
		glBindTexture(GL_TEXTURE_2D, texFslice);
		setTexParam2D();
		glTexImage2D(GL_TEXTURE_2D, // target
				0, // level
				GL_RGBA, //texture_format, // glSubImage conflicts against compression format
				fillF, // width
				fillF, // height
				0, // border
				image_format, // image format
				image_type, // image type
				NULL); // load texture later
		CHECK_GLError_print();

		qDebug("	frontSlice:   real = %d^2   fill = %d^2", realF, fillF);
	}
}

void Renderer_tex2::drawUnitFrontSlice(int line)
{
	if (!rgbaBuf || bufSize[3]<1 ) return; // no image data, 081231
	if (boundingBox.Dmin()<=1) return; //081231

	if (renderMode==rmCrossSection)
	{
		if (!bFSlice) return;
	}
	else
	{
		//;      // always used in MIP & Alpha mode
		if (!bFSlice) return;     // toggled by bFslice
		//if (!(bFSlice && line==1)) return;     // toggled by bFslice & only bound line
	}


	// here in Model space (object space)
	///////////////////////////////////////////////////////////////////////////
	BoundingBox box(VOL_X0,VOL_Y0,VOL_Z0, VOL_X1,VOL_Y1,VOL_Z1); //100730
	if (line==1 || renderMode==rmCrossSection)
	{
		box = BoundingBox(0,0,0, 1,1,1);
	}
	enableClipBoundingBox(box, false, 0.001); //081231

	GLdouble mvMatrix[16]; // view to unit model space
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glGetDoublev(GL_MODELVIEW_MATRIX, mvMatrix);
	glPushMatrix();
	glLoadIdentity();
	// here in View space (eye space)
	////////////////////////////////////////////////////////////////////////////
	//glEnable(GL_ALPHA_TEST); glAlphaFunc(GL_GREATER, 0);

	double fZ = viewClip;
	double fs = 1.415; // >= sqrt(2)

	// slice corner [-1, +1]^2 * fs
	double c0[] = { -fs, -fs, fZ, 1 };
	double cx[] = {  fs, -fs, fZ, 1 };
	double cy[] = { -fs,  fs, fZ, 1 };
	double c1[] = {  fs,  fs, fZ, 1 };

	if (line==1) // 090714 RZC
	{
		// just draw bounding line
		glPushAttrib(/*GL_ENABLE_BIT |*/ GL_LINE_BIT | GL_POLYGON_BIT);
		//glDisable(GL_TEXTURE_2D);
		//glDisable(GL_BLEND);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(0, -1); // deal z-fighting, 081120

		glLineWidth(1); // work only before glBegin(), by RZC 080827
		glBegin(GL_QUADS);
			glColor3fv(color_line.c);
			glVertex3dv(c0);
			glVertex3dv(cx);
			glVertex3dv(c1);
			glVertex3dv(cy);
		glEnd();

		glPopAttrib();

		////////////////////////////////////////
		disableClipBoundingBox(); //081231
		glPopMatrix();
		return;
		////////////////////////////////////////
	}


	double s0[4];	memcpy(s0, c0, sizeof(s0));
	double sx[4];	memcpy(sx, cx, sizeof(sx));
	double sy[4];	memcpy(sy, cy, sizeof(sy));
	double s1[4];	memcpy(s1, c1, sizeof(s1));

	ViewToModel(mvMatrix, s0[0], s0[1], s0[2]);
	ViewToModel(mvMatrix, sx[0], sx[1], sx[2]);
	ViewToModel(mvMatrix, sy[0], sy[1], sy[2]);
	ViewToModel(mvMatrix, s1[0], s1[1], s1[2]);

	XYZ S0(s0[0], s0[1], s0[2]);
	XYZ Sx(sx[0], sx[1], sx[2]);
	XYZ Sy(sy[0], sy[1], sy[2]);
	XYZ S1(sy[0], s1[1], s1[2]);


	////////////////////////////////////////////////////////////////////////////////
	setupFrontSliceBuffer(); //here set fillF

	bool use_tex2D = (Fslice_data && texFslice);
	if (use_tex2D)
	{
		// dynamic sampling slice image in unit volume space
		_frontSliceFromStack(rgbaBuf, imageX, imageY, imageZ,
							Fslice_data, realF, realF,
							S0, Sx, Sy);

		glBindTexture(GL_TEXTURE_2D, texFslice);
		setTexParam2D();
		glTexSubImage2D(GL_TEXTURE_2D, // target
				0, // level
				0,0, // offset
				realF, // width
				realF, // height
				image_format, // image format
				image_type, // image type
				Fslice_data);
		CHECK_GLError_print();

		TEXTURE_UNIT0_3D(false);
	}
	else
	{
		TEXTURE_UNIT0_3D(true); // not implemented
	}

	// draw slice
	double tw = float(realF) /fillF;
	double tz = float(imageZ) /_getTexFillSize(imageZ);
	double ty = float(imageY) /_getTexFillSize(imageY);
	double tx = float(imageX) /_getTexFillSize(imageX);

	drawing_fslice = true;
	shaderTexBegin(false); // no stream
	glBegin(GL_QUADS);
	{
		if (use_tex2D) 	glTexCoord2d(0,  0);
		else			glTexCoord3d(S0.x*tx, S0.y*ty, S0.z*tz);
		glVertex3dv(c0);

		if (use_tex2D) 	glTexCoord2d(tw, 0);
		else 			glTexCoord3d(Sx.x*tx, Sx.y*ty, Sx.z*tz);
		glVertex3dv(cx);

		if (use_tex2D) 	glTexCoord2d(tw, tw);
		else 			glTexCoord3d(S1.x*tx, S1.y*ty, S1.z*tz);
		glVertex3dv(c1);

		if (use_tex2D) 	glTexCoord2d(0,  tw);
		else 			glTexCoord3d(Sy.x*tx, Sy.y*ty, Sy.z*tz);
		glVertex3dv(cy);
	}
	glEnd();
	shaderTexEnd();
	drawing_fslice = false;

	BIND_TEXTURE_0();

	///////////////////////////////////////////
	disableClipBoundingBox(); //081231
	glPopMatrix();
	///////////////////////////////////////////
}

void Renderer_tex2::blendTrack()
{
	if (listMarkerPos.size()<1)  return;

	int N = listMarkerPos.size();

	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_BLEND);
	glBlendEquationEXT(GL_FUNC_ADD_EXT);
	glBlendColorEXT(1, 1, 1, 0.3);
	glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA); // constant Alpha
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1, 1, 0);  // OpenGL is bottom to top
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix(); //100801
		glLoadIdentity();

	glPointSize(10);
	glBegin(GL_POINTS);

//	//for debug
//	glColor3ub(255,0,255);
//	glVertex2d( 0, 0 );
//	glVertex2d( 1, 1 );

	glColor3ub(0,255,255);
	for (int i=0; i<N; i++)
	{
		const MarkerPos & pos = listMarkerPos.at(i);
		double x = (pos.x   - pos.view[0])/pos.view[2];
		double y = (pos.y   - pos.view[1])/pos.view[3];
		glVertex2d( x, y );
		//qDebug("\t blend track (%g, %g)", pos.x, pos.y);
	}

	glEnd();
	glPointSize(1);

	glMatrixMode(GL_PROJECTION);
		glPopMatrix(); //100801
	glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glPopAttrib();
	//glFlush();
}

