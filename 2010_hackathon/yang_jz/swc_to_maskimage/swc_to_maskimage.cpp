/*
 *  swc_to_maskimage .cpp
 *  swc_to_maskimage 
 *
 *  Created by Yang, Jinzhu, on 11/27/10.
 *
 */

#include "swc_to_maskimage.h"
#include "v3d_message.h" 
#include "../../../v3d_main/basic_c_fun/basic_surf_objs.h"


//Q_EXPORT_PLUGIN2 ( PluginName, ClassName )
//The value of PluginName should correspond to the TARGET specified in the plugin's project file.
Q_EXPORT_PLUGIN2(swc_to_maskimage, SWC_TO_MASKIMAGElugin);

void swc_to_maskimage(V3DPluginCallback &callback, QWidget *parent, int method_code);
void BoundNeuronCoordinates(NeuronTree & neuron, 
							bool b_subtractMinFromAllNonnegatives,
							double & output_xmin,
							double & output_xmax,
							double & output_ymin,
							double & output_ymax,
							double & output_zmin,
							double & output_zmax,
);

//plugin funcs
const QString title = "swc_to_maskimage";
QStringList SWC_TO_MASKIMAGElugin::menulist() const
{
    return QStringList() 
	<< tr("swc_to_maskimage")
	<<tr("multiple SWC_to_maskimage")
	<<tr("Help");
}

void SWC_TO_MASKIMAGElugin::domenu(const QString &menu_name, V3DPluginCallback &callback, QWidget *parent)
{
	if (menu_name == tr("swc_to_maskimage"))
	{
    	swc_to_maskimage(callback, parent,1 );
    }else if (menu_name == tr("multiple SWC_to_maskimage"))
	{
		swc_to_maskimage(callback, parent,2);
		
	}
	else if (menu_name == tr("Help"))
	{
		v3d_msg("(version 0.1) Convert a SWC file to a mask image, where the area of the swc tubes will have non-zero values, and other area will have 0s.");
		return;
	}
}

void BoundNeuronCoordinates(NeuronTree & neuron, 
							bool b_subtractMinFromAllNonnegatives,
							double & output_xmin,
							double & output_xmax,
							double & output_ymin,
							double & output_ymax,
							double & output_zmin,
							double & output_zmax,
)
{
	double xmin, ymin, zmin, xmax, ymax, zmax;
	NeuronSWC *p_cur = 0;
	
	//initial search
	for (V3DLONG ii=0; ii<neurons.listNeuron.size(); ii++)
	{
		p_cur = (NeuronSWC *)(&(neurons.listNeuron.at(ii)));
		
		if (ii==0)
		{
			xmin = xmax = p_cur->x; 
			ymin = ymax = p_cur->y;
			zmin = zmax = p_cur->z;
		}
		else
		{
			xmin = (p_cur->x < xmin) ? p_cur->x : xmin;
			ymin = (p_cur->y < ymin) ? p_cur->y : ymin;
			zmin = (p_cur->z < zmin) ? p_cur->z : zmin;
			
			xmax = (p_cur->x > xmax) ? p_cur->x : xmax;
			ymax = (p_cur->y > ymax) ? p_cur->y : ymax;
			zmax = (p_cur->z > zmax) ? p_cur->z : zmax;
		}
	}
	
	//subtraction
	for (V3DLONG ii=0; ii<neurons.listNeuron.size(); ii++)
	{
		p_cur = (NeuronSWC *)(&(neurons.listNeuron.at(ii)));
		
		if (xmin<0 || b_subtractMinFromAllNonnegatives) p_cur->x -= xmin;
		if (ymin<0 || b_subtractMinFromAllNonnegatives) p_cur->y -= ymin;
		if (zmin<0 || b_subtractMinFromAllNonnegatives) p_cur->z -= zmin;
	}
	
	//re-search the bounding box
	for (V3DLONG ii=0; ii<neurons.listNeuron.size(); ii++)
	{
		p_cur = (NeuronSWC *)(&(neurons.listNeuron.at(ii)));
		
		if (ii==0)
		{
			output_xmin = output_xmax = p_cur->x; 
			output_ymin = output_ymax = p_cur->y;
			output_zmin = output_zmax = p_cur->z;
		}
		else
		{
			output_xmin = (p_cur->x < output_xmin) ? p_cur->x : output_xmin;
			output_ymin = (p_cur->y < output_ymin) ? p_cur->y : output_ymin;
			output_zmin = (p_cur->z < output_zmin) ? p_cur->z : output_zmin;
			
			output_xmax = (p_cur->x > output_xmax) ? p_cur->x : output_xmax;
			output_ymax = (p_cur->y > output_ymax) ? p_cur->y : output_ymax;
			output_zmax = (p_cur->z > output_zmax) ? p_cur->z : output_zmax;
		}
	}
	
	return;
}

void ComputemaskImage(NeuronTree neurons, 
					  unsigned char* pImMask, //output mask image
					  unsigned char* ImMark,  //an indicator image to show whether or not a pixel has been visited/processed
					  V3DLONG sx, 
					  V3DLONG sy, 
					  V3DLONG sz, 
					  V3DLONG scale,  //not used now 
					  QString Filename //input swc filename
)
{
	NeuronSWC *p_cur = 0;
	//double check the data to ensure it is correct!
	for (V3DLONG ii=0; ii<neurons.listNeuron.size(); ii++)
	{
		p_cur = (NeuronSWC *)(&(neurons.listNeuron.at(ii)));
		
		xs = p_cur->x;
		ys = p_cur->y;		
		zs = p_cur->z;		
		rs = p_cur->r;
		if (xs<0 || ys<0 || zs<0 || rs<0)
		{
			v3d_msg("You have illeagal x,y,z coordinates or radius values. Check your data.");
			return;
		}
	}
	
	//the real computation
	double scalar = 1;
	double scalar2 = scalar*scalar;	
	
	//compute mask
	double xs = 0, ys = 0, zs = 0, xe = 0, ye = 0, ze = 0, rs = 0, re = 0;
	
	V3DLONG count1 = sx*sy;
	for (V3DLONG ii=0; ii<neurons.listNeuron.size(); ii++)
	{
		V3DLONG i,j,k;
		
		p_cur = (NeuronSWC *)(&(neurons.listNeuron.at(ii)));
		
		xs = p_cur->x;
		ys = p_cur->y;		
		zs = p_cur->z;		
		rs = p_cur->r;
		
		double ballx0, ballx1, bally0, bally1, ballz0, ballz1, tmpf;
		
		ballx0 = xs - rs; ballx0 = qBound(0, ballx0, sx-1);
		ballx1 = xs + rs; ballx1 = qBound(0, ballx1, sx-1);
		if (ballx0>ballx1) {tmpf = ballx0; ballx0 = ballx1; ballx1 = tmpf;}
		
		bally0 = ys - rs; bally0 = qBound(0, bally0, sy-1);
		bally1 = ys + rs; bally1 = qBound(0, bally1, sy-1);
		if (bally0>bally1) {tmpf = bally0; bally0 = bally1; bally1 = tmpf;}
		
		ballz0 = zs - rs; ballz0 = qBound(0, ballz0, sz-1);
		ballz1 = zs + rs; ballz1 = qBound(0, ballz1, sz-1);
		if (ballz0>ballz1) {tmpf = ballz0; ballz0 = ballz1; ballz1 = tmpf;}
		
		//marker all voxels close to the swc node(s)
	    for (k = ballz0; k < ballz1; k++)
		{
			for (j = bally0; j < bally1; j++)
			{
				for (i = ballx0; i < ballx1; i++)
				{
					double norms10 = (xs-i)*(xs-i) + (ys-j)*(ys-j) + (zs-k)*(zs-k);
					double dt = sqrt(norms10);
					
					V3DLONG ind = (k)*count1 + (j)*sx + i;
					
					if(dt < rs)
					{  
						int n = rand()%255 + 1;
						if(ImMark[ind]) //0 - not visited yet, 1 - already processed
						{
						}
						else
						{
							pImMask[ind] = n; //consider only one image at this moment
							//if there are more than 255 swc files, then this is not right! need fix data type, by PHC
							ImMark[ind] = 1;
						}
					}
				}
			}
		}
		
		//find previous node
		if (p_cur->pn < 0) //then it is root node already
		{
			continue;
		}
		
		NeuronSWC *pp=0;
		bool b_find = false;
		for (V3DLONG j=0; j<neurons.listNeuron.size(); j++)
		{
			pp = (NeuronSWC *)(&(neurons.listNeuron.at(j)));
			if(pp->n == p_cur->pn)
			{
				b_find = true;
				break;
			}
		}
		if (!b_find)
		{
			v3d_msg(QString("Cannot find the matching parent node [%1] for the current node [%2]\n").arg(p_cur->pn).arg(p_cur->n)), 0);
			continue;
		}
		else
		{
			xe = pp->x;
			ye = pp->y;
			ze = pp->z;
			re = pp->r;
		}
		
		//judge if two points overlap, if yes, then do nothing as the sphere has already been drawn
		if (xe==xs && ye==ys && ze==zs)
		{
			v3d_msg(QString("Detect overlapping coordinates of node [%1]\n").arg(p_cur->n)), 0);
			continue;
		}
		
		//finding the envelope of the current line segment
		double rbox = (rs>re) ? rs : re;
		double x_down = (xs < xe) ? xs : xe; x_down -= rbox; x_down = V3DLONG(x_down+0.5); qBound(0, x_down, sx-1);
		double x_top  = (xs > xe) ? xs : xe; x_top  += rbox; x_top  = V3DLONG(x_top +0.5); qBound(0, x_top,  sx-1);
		double y_down = (ys < ye) ? ys : ye; y_down -= rbox; y_down = V3DLONG(y_down+0.5); qBound(0, y_down, sy-1);
		double y_top  = (ys > ye) ? ys : ye; y_top  += rbox; y_top  = V3DLONG(y_top +0.5); qBound(0, y_top,  sy-1);
		double z_down = (zs < ze) ? zs : ze; z_down -= rbox; z_down = V3DLONG(z_down+0.5); qBound(0, z_down, sz-1);
		double z_top  = (zs > ze) ? zs : ze; z_top  += rbox; z_top  = V3DLONG(z_top +0.5); qBound(0, z_top,  sz-1);
		
		//compute cylinder and flag mask 
		
		for (k=z_down; k<z_top; k++)
		{
			for (j=y_down; j<y_top; j++)
			{
				for (i=x_down; i<x_top; i++)
				{
					V3DLONG ind = (k)*sx*sy + (j)*sx + i;
					
					double countxsi = (xs-i);
					double countysj = (ys-j);
					double countzsk = (zs-k);
					double countxes = (xe-xs);
					double countyes = (ye-ys);
					double countzes = (ze-zs);
					double norms10 = countxsi * countxsi + countysj * countysj + countzsk * countzsk;
					double norms21 = countxes * countxes + countyes * countyes + countzes * countzes;
					double dots1021 = countxsi * countxes + countysj * countyes + countzsk * countzes; 
					double dist = sqrt( norms10 - (dots1021*dots1021)/(norms21) );
					double t1 = -dots1021/norms21;
                    if(t1<0)
                        dist = sqrt(norms10);
                    else if(t1>1)
                        dist = sqrt((xe-i)*(xe-i) + (ye-j)*(ye-j) + (ze-k)*(ze-k));
					
				    int n=rand()%255+1;
					
					// compute point of intersection
					double v1 = xe - xs;
					double v2 = ye - ys;
					double v3 = ze - zs;
					double vpt = v1*v1 + v2*v2 +v3*v3;
					double t = (double(i-xs)*v1 +double(j-ys)*v2 + double(k-zs)*v3)/vpt;
					double temR,temr;
					double xc = xs + v1*t;
					double yc = ys + v2*t;
					double zc = zs + v3*t;
					double rr;
 					
					//compute rr
					double normssc = sqrt((xs-xc)*(xs-xc)+(ys-yc)*(ys-yc)+(zs-zc)*(zs-zc));
					double normsce = sqrt((xe-xc)*(xe-xc)+(ye-yc)*(ye-yc)+(ze-zc)*(ze-zc));
					rr = (rs > re) ? (rs - (rs - re)/sqrt(norms21)*normssc) : (re - (re-rs)/sqrt(norms21)*normsce);
					
					printf("rs=%lf rr=%lf dist=%lf re%lf \n",rs,rr,dist,re);
					
					//
					if (dist < rr)
						pImMask[indLoop] = n;
				}
			}
		}
	}
}

void swc_to_maskimage(V3DPluginCallback &callback, QWidget *parent, int method_code)
{
	NeuronTree neurons;
	double x_min,x_max,y_min,y_max,z_min,z_max,x_total,y_total,z_total;
	x_min = 0;
	x_max = 0;
	y_min = 0;
	y_max = 0;
	z_min = 0;
	z_max = 0;
	x_total = 0;
	y_total = 0;
	z_total = 0;
	V3DLONG alpha,beta,namesize,temp;
	V3DLONG sx,sy,sz;
	namesize = 0;
	unsigned char* pImMask;
	unsigned char* ImMark;
	
	if (method_code == 1)
	{
		QString filename;
		filename = QFileDialog::getOpenFileName(0, QObject::tr("Open File"),
												"",
												QObject::tr("Supported file (*.swc)"
															";;Neuron structure	(*.swc)"
															));
		if(filename.isEmpty()) 
		{
			v3d_msg("You don't have any image open in the main window.");
			return;
		}
		
		NeuronSWC *p_t=0;
		if (filename.size()>0)
		{
			neurons = readSWC_file(filename);
			
			
			for (V3DLONG ii=0; ii< neurons.listNeuron.size(); ii++)
			{
				p_t = (NeuronSWC *)(&(neurons.listNeuron.at(ii)));
				
				double xs = p_t->x+p_t->r;
				double ys = p_t->y+p_t->r;
				double zs = p_t->z+p_t->r;
				x_min = (xs<x_min)? xs:x_min;
				x_max = (xs>x_max)? xs:x_max;
				y_min = (ys<y_min)? ys:y_min;
				y_max = (ys>y_max)? ys:y_max;
				z_min = (zs<z_min)? zs:z_min;
				z_max = (zs>z_max)? zs:z_max;				 
			}		
			sx = abs((x_max  - x_min));
			sy = abs((y_max  - y_min));
			sz = abs((z_max  - z_min));
			V3DLONG pagesz = sx*sy*sz;
			pImMask = new unsigned char[pagesz];
			if (!pImMask) 
			{
				printf("Fail to allocate memory.\n");
				return ;
			}
			else
			{
				for(V3DLONG i=0; i<pagesz; i++)
					pImMask[i] = 0; 
			}
			ImMark = new unsigned char [pagesz];
			if (!ImMark) 
			{
				printf("Fail to allocate memory.\n");
				return;
			}
			else
			{
				for(V3DLONG i=0; i<pagesz; i++)
					ImMark[i] = 0; 
			}					
			ComputemaskImage(neurons,pImMask,ImMark,sx,sy,sz,temp,x_min,y_min,z_min,filename);
		}
		else 
		{
			v3d_msg("You don't have any image open in the main window.");
			return;
		}
		
	}
	else if (method_code ==2)
	{
		QString filename;
		QStringList filenames;
		filenames = QFileDialog::getOpenFileNames(0, 0,"","Supported file (*.swc)" ";;Neuron structure(*.swc)",0,0);
		if(filenames.isEmpty()) 
		{
			v3d_msg("You don't have any image open in the main window.");
			return;
		}
		namesize = filenames.size();
		NeuronSWC *p_t=0;
		for (V3DLONG i = 0; i < filenames.size();i++)
		{
			filename = filenames[i];
			if (filename.size()>0)
			{
				neurons = readSWC_file(filename);
				
				for (V3DLONG ii=0; ii<neurons.listNeuron.size(); ii++)
				{
					p_t = (NeuronSWC *)(&(neurons.listNeuron.at(ii)));
					
					double xs = p_t->x+p_t->r;
					double ys = p_t->y+p_t->r;
					double zs = p_t->z+p_t->r;
					x_min = (xs<x_min)? xs:x_min;
					x_max = (xs>x_max)? xs:x_max;
					y_min = (ys<y_min)? ys:y_min;
					y_max = (ys>y_max)? ys:y_max;
					z_min = (zs<z_min)? zs:z_min;
					z_max = (zs>z_max)? zs:z_max;				 
				}
			}else 
			{
				v3d_msg("You don't have any image open in the main window.");
				return;
			}
		}
		sx = abs((x_max  - x_min));
		sy = abs((y_max  - y_min));
		sz = abs((z_max  - z_min));
		V3DLONG pagesz = sx*sy*sz;
		pImMask = new unsigned char [pagesz];
		if (!pImMask) 
		{
			printf("Fail to allocate memory.\n");
			return;
		}
		else
		{
			for(V3DLONG i=0; i<pagesz; i++)
				pImMask[i] = 0; 
		}	
		ImMark = new unsigned char [pagesz];
		if (!ImMark) 
		{
			printf("Fail to allocate memory.\n");
			return;
		}
		else
		{
			for(V3DLONG i=0; i<pagesz; i++)
				ImMark[i] = 0; 
		}		
		for (V3DLONG i = 0; i < filenames.size();i++)
		{
			filename = filenames[i];
			if (filename.size()>0)
			{
				neurons = readSWC_file(filename);
				
				ComputemaskImage(neurons,pImMask,ImMark,sx,sy,sz,temp,x_min,y_min,z_min,filename);
			}
			else 
			{
				return;
			}
		}
	}
	/*********************************************************************/// coupute coordinate region 		
	Image4DSimple tmp;
	tmp.setData(pImMask,sx,sy,sz,1,V3D_UINT8);		
	v3dhandle newwin = callback.newImageWindow();
	callback.setImage(newwin, &tmp);
	callback.setImageName(newwin, QString("composition image"));
	callback.updateImageWindow(newwin);
	
}
