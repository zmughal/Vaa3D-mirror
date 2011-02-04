/*
 *  montage_image_sections .cpp
 *  montage_image_sections 
 *
 *  Created by Yang, Jinzhu and Hanchuan Peng, on 11/22/10.
 *
 */

#include "montage_image_sections.h"
#include "v3d_message.h" 

//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(Montage, MONTAGEPlugin);

template <class T> 
void montage_image_sections (T *apsInput, T * aspOutput, V3DLONG iImageWidth, V3DLONG iImageHeight, V3DLONG iImageLayer)
{
	V3DLONG i, j,k,n,count,m,row,column;
	V3DLONG mCount = iImageHeight * iImageWidth;
	
	column = (V3DLONG)sqrt((double)iImageLayer);
	//v3d_msg("1");
	//printf("column1=%d,",column);
	
	for (i=0; i<iImageLayer; i++)
	{
		for (j=0; j<iImageHeight; j++)
		{
			for (k=0; k<iImageWidth; k++)
			{
				if (i < column) 
				{
					//aspOutput[(j+1)* column *iImageWidth + (k+1)  + (i * iImageWidth) ]= apsInput[i * mCount + j*iImageWidth + k];
					aspOutput[(j)* column *iImageWidth + (k)  + (i * iImageWidth) ]= apsInput[i * mCount + j*iImageWidth + k];
				}else
				{
					//aspOutput[(((i/column)*iImageHeight)+ (j+1) )* column* iImageWidth + (k+1) + ((i%column) * iImageWidth)]= apsInput[i * mCount + j*iImageWidth + k];	
					aspOutput[(((i/column)*iImageHeight)+ j )* column* iImageWidth + k + ((i%column) * iImageWidth)]= apsInput[i * mCount + j*iImageWidth + k];	
				}
			}
		}
	}
	//v3d_msg("2");	
}

void do_computation(V3DPluginCallback2 &callback, QWidget *parent, int method_code);

//plugin funcs
const QString title = "montage_image_sections";
QStringList MONTAGEPlugin::menulist() const
{
    return QStringList() 
	<< tr("montage_image_sections")
	<< tr("Help");
}

void MONTAGEPlugin::domenu(const QString &menu_name, V3DPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("montage_image_sections"))
	{
    	do_computation(callback, parent,1 );
    }
	else if (menu_name == tr("Help"))
	{
		v3d_msg("This plugin produces a montage view of all z slices.");
		return;
	}

}

void do_computation(V3DPluginCallback2 &callback, QWidget *parent, int method_code)
{
	v3dhandle curwin = callback.currentImageWindow();
	V3DLONG h;
	V3DLONG d;
	V3DLONG channelsz,channelsz1;
	V3DLONG column,row;
	channelsz =channelsz1=column = row = 0;
	if (!curwin)
	{
		v3d_msg("You don't have any image open in the main window.");
		return;
	}
	int start_t = clock(); // record time point
	
	Image4DSimple* subject = callback.getImage(curwin);
	QString m_InputFileName = callback.getImageName(curwin);
	
	if (!subject)
	{
		QMessageBox::information(0, title, QObject::tr("No image is open."));
		return;
	}	
	Image4DProxy<Image4DSimple> pSub(subject);
	
	V3DLONG sz0 = subject->getXDim();
    V3DLONG sz1 = subject->getYDim();
    V3DLONG sz2 = subject->getZDim();
	V3DLONG sz3 = subject->getCDim();
	V3DLONG pagesz_sub = sz0*sz1*sz2;
	
	//----------------------------------------------------------------------------------------------------------------------------------

//	column = (V3DLONG)(sqrt(sz2)+ 0.5);
//	V3DLONG remainder= sz2 - column*column;
//	float num = sqrt(sz0*sz1*sz2);
//    column = num/sz0;
//	row = (sz2/column+0.5);
//	channelsz1 = (V3DLONG)column*sz0*(V3DLONG)row*sz1;
	//row = sz2/column;
	
	column = (V3DLONG)(sqrt((double)sz2)+ 0.5);
	V3DLONG remainder= sz2 - column*column;
	if (remainder == 0)
	{
		row = column;
		channelsz1 = (sz0)*(sz1)*sz2;
	}else 
	{
		row = column+1;
		channelsz1 = (sz0)*(sz1)*column*(column+1);
	}	
	channelsz = sz0*sz1*sz2;
  //  printf("column=%d,remainder=%d,row=%d sz3=%d\n",column,remainder,row,sz3);
	void *pData=NULL;
	V3DLONG sz_data[4]; sz_data[0]=sz0; sz_data[1]=sz1; sz_data[2]=sz2; sz_data[3]=1;
	switch (subject->getDatatype()) 
	{
		case V3D_UINT8:
			
			try
			{
				pData = (void *)(new unsigned char [sz3*channelsz1]()); 
			}
				catch (...)
			{
				v3d_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}
			
			{
				unsigned char * pSubtmp_uint8 = pSub.begin();
			
				for (V3DLONG ich=0; ich<sz3; ich++)
					montage_image_sections(pSubtmp_uint8+ich*channelsz, (unsigned char *)pData+ich*channelsz1, sz0, sz1, sz2);
			}
			break;
			
		case V3D_UINT16:
			try
			{
				pData = (void *)(new short int [sz3*channelsz1]()); 
			}
				catch (...)
			{
				v3d_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}
			
			{
				short int * pSubtmp_uint16 = (short int *)pSub.begin();
			
				for (V3DLONG ich=0; ich<sz3; ich++)
					montage_image_sections(pSubtmp_uint16+ich*channelsz, (short int *)pData+ich*channelsz1, sz0, sz1, sz2 );
			}
			break;
		case V3D_FLOAT32:
			
			try
			{
				pData = (void *)(new float [sz3*channelsz1]()); 
			}
				catch (...)
			{
				v3d_msg("Fail to allocate memory in data combination.");
				if (pData) {delete []pData; pData=0;}
				return;
			}
			
			{
				float * pSubtmp_float32 = (float *)pSub.begin();
				
				for (V3DLONG ich=0; ich<sz3; ich++)
					montage_image_sections(pSubtmp_float32+ich*channelsz, (float *)pData+ich*channelsz, sz0, sz1, sz2 );
			}
			
			break;
		default:
			break;
	}
	
	//V3DLONG width= column*sz0;
	//V3DLONG Height = row*sz1;
	
	for (V3DLONG j=0; j<row; j++)
	{
		for(V3DLONG k=0; k<column; k++)
		{
			(unsigned char*)pData[j*column+k] = 255;	
		}
	}
	
	//----------------------------------------------------------------------------------------------------------------------------------
	
	int end_t = clock();
//	printf("time eclapse %d s for dist computing!\n", (end_t-start_t)/1000000);	
	///v3d_msg("2");
	//printf("column=%d,remainder=%d,row=%d\n",column,remainder,row);
	Image4DSimple p4DImage;
	
	p4DImage.setData((unsigned char*)pData, (V3DLONG)column*sz0, (V3DLONG)row*sz1, 1, sz3, subject->getDatatype());	
	
	//p4DImage.setData((unsigned char*)pData, (V3DLONG)column*(sz0), (V3DLONG)row*(sz1), 1, sz3, subject->getDatatype());
	//	printf("sz0= %d sz1=%d sz2=%d dd=%d vv=%d\n", sz0,sz1,sz2,dd,vv);
	v3dhandle newwin;
	if(QMessageBox::Yes == QMessageBox::question (0, "", QString("Do you want to use the existing window?"), QMessageBox::Yes, QMessageBox::No))
		newwin = callback.currentImageWindow();
	else
		newwin = callback.newImageWindow();
	
	callback.setImage(newwin, &p4DImage);
	callback.setImageName(newwin, QString("montage_image_sections"));
	callback.updateImageWindow(newwin);
}
