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




/****************************************************************************
**
V3D main program

By Hanchuan Peng

July 21, 2006
060924: v3d v1.2

Last update: 2008-04-25: try to add command line based utilities
Last update: 2010-04-12: add a global try-catch to catch all exceptions
 
****************************************************************************/

#define COMPILE_TO_COMMANDLINE 1

#include "v3d_compile_constraints.h"

#include <QApplication>

#include <iostream>

#include "mainwindow.h"

#include <string>
using namespace std;

#include "v3d_core.h"
//#include "idrawmain.h"

void printHelp_v3d();
void printHelp_align();
void printHelp_straight();
void printHelp_trace();

void printHelp_v3d()
{
	printf("\nV3D: a 3D image visualization and processing tool developed by Hanchuan Peng and colleagues.\n");
	printf("\nUsage: v3d -h -M moduleCode [all other options specific to different modules]\n");
	printf("\t -h/H         help information. \n");
	printf("\t -M module    a string indicate which module will be used for processing. \n");
	printf("\t Available/future modules/codes (no difference for capital): \n");
	printf("\t   ALIGN      3D image elastic registration for objects like brains.\n");
	printf("\t   BLEND      3D image blending.\n");
	printf("\t   CROP       3D image cropping.\n");
	printf("\t   LANDMARK   operation on the landmark files.\n");
	printf("\t   MASK       3D image masking.\n");
	printf("\t   RESAMPLE   3D image resampling (reslicing).\n");
	printf("\t   ROT        3D image rotation.\n");
	printf("\t   SAVEAS     3D image file format conversion.\n");
	printf("\t   SEG        3D image segmentation for spherical objects (e.g. cells, nuclei).\n");
	printf("\t   STITCH     3D image stitching.\n");
	printf("\t   STRAIGHT   3D image straightening (e.g. for C. elegans worm body).\n");
	printf("\t   TRACE      3D image tracing (e.g. for neurons).\n");

	printf("\n");
	printHelp_align();

	printf("\n");
	printHelp_straight();

	printf("\n");
	printHelp_trace();
}

void printHelp_align()
{
	printf("\nUsage: v3d -M ALIGN -t <rawImgFile_template/target> -s <rawImgFile_subject> -o <rawImgFile_warped_subject> -c <channalNo_reference> -w <warp_type> -n <nloop>\n");
	printf("\t -t <rawImgFile_template/target>    .raw/.tif file containing 3D template/target stack to which a subject image will be warped/mapped. \n");
	printf("\t -s <rawImgFile_subject>            .raw/.tif file containing 3D subject which will deform/warp to best appriximate the geometry of the template/target. \n");
	printf("\t -o <rawImgFile_warped_subject>     .raw/.tif file containing warped result stack.\n");
	printf("\t -F <rawImgFile_DF_file>            .raw file (4-byte float) containing the displacement field (DF), chanel 0, 1, 2 for sx, sy, ans sz in terms of pixels.\n");
	printf("\t                                    Note: specify -t will prohibit -F.\n");
	printf("\t -L <landmark_file_target>          a plain text csv file containing landmarks of only the target_pos (if -w 9).\n");
	printf("\t -l <landmark_file_subject>         a plain text csv file containing landmarks of only the subject_pos (if -w 9). If un-specified, then assume -w 6\n");
	printf("\t [-C]                               the ID of the reference channel of the target image (default 0) which is used in computing warping. \n");
	printf("\t [-c]                               the ID of the reference channel of the subject image (default 0) which is used in computing warping. All other channels will use the same warp.\n");
	printf("\t [-w]                               the type of warp, 0 -- global, 1 -- local (without global), and 2 -- local (with global), 6-target landmark in file. 9 - landmark pairs in file. \n");
	printf("\t [-n]                               number of loops of global/local estimation. \n");
	printf("\t [-m]                               number of loops of local smoothing. \n");
	printf("\t [-z]                               minimal xy size in the pyrimid (e.g. 64 or 32). \n");
	printf("\t [-r]                               the half window size in defining the local block (e.g. 5 means a 11x11 block, and 3 means a 7x7 block). \n");
	printf("\t [-X]                               target image: X/Y direction pixel size (resolution) in microns. Also assume the X and Y pixel size are always the same. \n");
	printf("\t [-Z]                               target image: Z direction resolution in microns. \n");
	printf("\t [-U]                               subject image: X/Y direction pixel size (resolution) in microns. Also assume the X and Y pixel size are always the same. \n");
	printf("\t [-V]                               subject image: Z direction resolution in microns. \n");
	printf("\t [-B]                               XY plane size of image mapping (default is 512, can be 1024, 2048, or 256, - note: if use 1024 and above, should use the cluster node with big memory). \n");
	printf("\t [-e]                               multiplication factor for control points in an existing landmark file (e.g. if image size is 1024, and existing landmarks are defined using 512 image, then use -e 2. Default is 1.). \n");
	printf("\t \n");    
	printf("\t [-T]                               only convert the file format of the -s file to the -o file (if they have the same suffixes, then just a save as). \n");
	printf("\t \n");
	printf("\t [-v]                               verbose printing enabled. \n");
	printf("\t [-h]                               print this message.\n");
	return;
}

void printHelp_straight()
{
}

void printHelp_trace()
{
}


int main(int argc, char **argv)
{

#ifdef COMPILE_TO_COMMANDLINE
	if (argc<=1)
	{
#endif
		Q_INIT_RESOURCE(v3d);

		QApplication app(argc, argv);

		//090812 RZC: best solution for QMainWindow is using new, then deleteLater itself in its closeEvent.
		MainWindow* mainWin = new MainWindow;

        // On Mac, allow mainWin to get QFileOpen events, such as when a tif
        // file is dragged onto the application icon.
        // CMB Nov-12-2010
        app.installEventFilter(mainWin);

		mainWin->show();

		//*************************
		// DO NOT USE THE FOLLOWING AS I CHANGED TO MDI APPLICATIONS
		//*************************

		//XFormWidget xformWidget(0);
		//XFormWidget xformWidget;
		//XFormWidget xformWidget1(0); //useful to display multiple stacks

		/*
		//set style

		QStyle *arthurStyle = new ArthurStyle();
		xformWidget.setStyle(arthurStyle);

		QList<QWidget *> widgets = qFindChildren<QWidget *>(&xformWidget);
		foreach (QWidget *w, widgets)
			w->setStyle(arthurStyle);
		*/

		//display

		//xformWidget.show();
		//xformWidget1.show(); //useful to display multiple stacks

		//iDrawMainWindow my3ddraw;
		//my3ddraw.show();

		try 
		{
			return app.exec();
		}
		catch (...) {
			v3d_msg("Catch an exception at the main application level. Basically you should never see this. Please click Ok to quit and send the error log to the V3D developers to figure out the problem.");
			return 1;
		}

#ifdef COMPILE_TO_COMMANDLINE
	}
	else
	{
		//string s1, s2;
		//s1 = argv[1];
		//if (s1=="-h" || s1=="-H")
		//{
		//	if (argc<=2)
		//	{
		//		printHelp_v3d();
		//		return 0;
		//	}
		//	else
		//	{
		//		s2 = argv[2];
		//		if (s2=="ALIGN") {return 0;}
		//		else if (s2=="BLEND") {return 0;}
		//		else if (s2=="CROP") {return 0;}
		//		else if (s2=="LANDMARK") {return 0;}
		//		else if (s2=="MASK") {return 0;}
		//		else if (s2=="RESAMPLE") {return 0;}
		//		else if (s2=="ROT") {return 0;}
		//		else if (s2=="SAVEAS") {return 0;}
		//		else if (s2=="SEG") {return 0;}
		//		else if (s2=="STITCH") {return 0;}
		//		else if (s2=="STRAIGHT") {return 0;}
		//		else if (s2=="TRACE") {return 0;}
		//		else
		//		{
		//			printf("Your module code is illegal. Please follow the instruction of the help page below.\n\n");
		//			printHelp_v3d();
		//			return 1;
		//		}
		//	}
		//}
		//else if (s1=="-M") //must be capital
		//{
		//	return 0;
		//}
		//else
		//{
		//	printf("Your command line input is illegal. Please follow the instruction of the help page below.\n\n");
		//	printHelp_v3d();
		//	return 1;
		//}

		// -------------------------------------------------------
		// predefine -f load image/object (swc, apo) file into V3D
		// YuY: Nov. 18, 2010. 
		
		// command arguments parsing
		char* filename;

		if(argc<=2)
		{
			if(string(argv[1]) == "-h" || string(argv[1]) == "-H")
			{
				printHelp_v3d();
				return 0;
			}
			else if(string(argv[1]) == "-M") //must be capital
			{
				return 0;
			}
			else
			{
				cout<<"Your module code is illegal. Please follow the instruction of the help page below."<<endl<<endl;
				printHelp_v3d();
				return 1;
			}
					
		}
		else
		{
			for(int i=1; i<argc; i++)
			{
				if(i+1 != argc) // check that we haven't finished parsing yet
				{
					if(string(argv[i]) == "-f")
					{
						filename = argv[i+1];
						//cout<<filename<<endl;
					}
					else
					{
						cout<<"Your module code is illegal. Please follow the instruction of the help page below."<<endl<<endl;
						printHelp_v3d();
						return -1;
					}
				}
			
			}
		
		}
		
		

		// image/object handling module
		QString qFile(filename);

		if(!QFile(qFile).exists()) // supporting both local and web files. Nov. 18, 2010. YuY
		{
			// judge whether the file exist in the web
			// "://" like "smb://" "http://" "ftp://" 

			if(qFile.contains("://"))
			{
				QUrl url(filename);

				if(!url.isValid()) // valid or invalid url
				{
					cout<<"The file does not exist! Exit."<<endl;
					return -1;	
				}

				// V3D GUI handling module
				Q_INIT_RESOURCE(v3d);

				QApplication app(argc, argv);

				MainWindow* mainWin = new MainWindow;

				mainWin->loadV3DUrl(QUrl(filename), true);

				app.installEventFilter(mainWin);

				mainWin->show();

				try 
				{
					return app.exec();
				}
				catch (...) 
				{
					v3d_msg("Catch an exception at the main application level. Basically you should never see this. Please click Ok to quit and send the error log to the V3D developers to figure out the problem.");
					return 1;
				}
			
			}
			else // impossible be a url
			{
				cout<<"The file does not exist! Exit."<<endl;
				return -1;	
			}
			
		}
		else
		{
			//QString curSuffix = QFileInfo(qFile).suffix();

			// V3D GUI handling module
			Q_INIT_RESOURCE(v3d);

			QApplication app(argc, argv);

			MainWindow* mainWin = new MainWindow;

			mainWin->loadV3DFile(filename, true, false);

			app.installEventFilter(mainWin);

			mainWin->show();

			try 
			{
				return app.exec();
			}
			catch (...) 
			{
				v3d_msg("Catch an exception at the main application level. Basically you should never see this. Please click Ok to quit and send the error log to the V3D developers to figure out the problem.");
				return 1;
			}
		}


		// -------------------------------------------------------
		

	}
#endif

}
