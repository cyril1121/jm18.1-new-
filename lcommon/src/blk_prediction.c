/*!
 *************************************************************************************
 * \file blk_prediction.c
 *
 * \brief
 *    Block Prediction related functions
 *
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *    - Alexis Michael Tourapis         <alexismt@ieee.org>
 *
 *************************************************************************************
 */

#include "contributors.h"

#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <math.h>

#include "block.h"
#include "global.h"

#include "macroblock.h"
#include "mc_prediction.h"
#include "image.h"
#include "mb_access.h"
void compute_residue_p16x16 (Macroblock *currMB,int block,int yuv,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int   list_mode[2],int width, int height)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;
	MotionVector ***** mv_array = currSlice->all_mv;
	MotionVector *curr_mv = NULL;
	//PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j,num=0;
  double rms,chun,sumR=0,sumRD=0,sumD2=0,sumD=0;
  double offset;
  float scale;
  //int  block_x = currMB->block_x + (block & 0x01)*2;
 // int	 block_y = currMB->block_y + (block & 0x02);
  int  block_x = (block & 0x01)*2;
  int	 block_y =  (block & 0x02);
   curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
  //MotionVector *mv = &motion[block_y][block_x].mv[LIST_0];
  num=height*width;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];    
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i = 0; i < width; i++)
    {
    	sumR+=*imgOrg;
		sumD+=*imgPred;
		sumRD+= (*imgOrg)*(*imgPred);
		sumD2+=(*imgPred)*(*imgPred);
		*imgOrg++ ;
		*imgPred++;
    }
  }
  
  
  scale=(num*sumRD-sumD*sumR)/(num*sumD2-sumD*sumD);
  if(0)
  {
	  if((scale>=0.995)&&(scale<=1.005))
	  {
	  	scale=1.00;
	  }
  	  //mv->offset[yuv]=(sumR-scale*sumD)/num;
  }
  else
  {
	  scale=1.00;
	  //mv->offset[yuv]=0;

  }
  curr_mv->scale[yuv]=scale*100;
  scale=(float)curr_mv->scale[yuv]/100;
  curr_mv->offset[yuv]=(sumR-scale*sumD)/num;

  for(j=0;j<4;j++)
  {
	  for(i=0;i<4;i++)
	  {
		  mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y+j][block_x+i].scale[yuv]=curr_mv->scale[yuv];
		  mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y+j][block_x+i].offset[yuv]=curr_mv->offset[yuv];
	  }
  }
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];    
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i = 0; i < width; i++)
    {
		/*
    	sumR+=*imgOrg;
		sumD+=*imgPred;
		sumRD+= (*imgOrg)*(*imgPred);
		sumD2+=(*imgPred)*(*imgPred);*/
		*m7++ = (*imgOrg++ - (int)(scale*(*imgPred++))-curr_mv->offset[yuv]);
    }
  }   
}

void compute_residue_p8x8 (Macroblock *currMB,int block,int yuv,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x,int   list_mode[2], int width, int height)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;
	MotionVector ***** mv_array = currSlice->all_mv;
	MotionVector *curr_mv = NULL;
	//PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j,num=0;
  double rms,chun,sumR=0,sumRD=0,sumD2=0,sumD=0;
  double offset;
  float scale;
  //int  block_x = currMB->block_x + (block & 0x01)*2;
  //int	 block_y = currMB->block_y + (block & 0x02);
  int  block_x =  (block & 0x01)*2;
  int	 block_y = (block & 0x02);
  curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
 // MotionVector *mv = &motion[block_y][block_x].mv[LIST_0];
  num=height*width;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];    
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i = 0; i < width; i++)
    {
    	sumR+=*imgOrg;
		sumD+=*imgPred;
		sumRD+= (*imgOrg)*(*imgPred);
		sumD2+=(*imgPred)*(*imgPred);
		*imgOrg++ ;
		*imgPred++;
    }
  }
  
  
  scale=(num*sumRD-sumD*sumR)/(num*sumD2-sumD*sumD);
  if(0)
  {
	  if((scale>=0.995)&&(scale<=1.005))
	  {
	  	scale=1.00;
	  }
  	  //mv->offset[yuv]=(sumR-scale*sumD)/num;
  }
  else
  {
	  scale=1.00;
	 // mv->offset[yuv]=0;

  }
  curr_mv->scale[yuv]=scale*100;
  scale=(float)curr_mv->scale[yuv]/100;
  curr_mv->offset[yuv]=(sumR-scale*sumD)/num;

  for(j=0;j<2;j++)
  {
	  for(i=0;i<2;i++)
	  {
		  mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y+j][block_x+i].scale[yuv]=curr_mv->scale[yuv];
		  mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y+j][block_x+i].offset[yuv]=curr_mv->offset[yuv];
	  }
  }

  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];    
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i = 0; i < width; i++)
    {
		/*
    	sumR+=*imgOrg;
		sumD+=*imgPred;
		sumRD+= (*imgOrg)*(*imgPred);
		sumD2+=(*imgPred)*(*imgPred);
		*/
		*m7++ = (*imgOrg++ - (int)(scale*(*imgPred++))-curr_mv->offset[yuv]);
    }
  }   
}
void compute_residue_p4x4 (Macroblock *currMB,int block,int yuv,int bx,int by,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x,int   list_mode[2], int width, int height)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;

	//PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j,num=0;
  double rms,chun,sumR=0,sumRD=0,sumD2=0,sumD=0;
  float scale;
  double offset;

 // int  block_x = currMB->block_x +bx;
 // int	 block_y = currMB->block_y+by;	
  int  block_x = bx;
  int	 block_y = by;	
  MotionVector ***** mv_array = currSlice->all_mv;
  MotionVector *curr_mv = NULL;
  curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
  //MotionVector *mv = &motion[block_y][block_x].mv[LIST_0];
  num=height*width;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];    
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i = 0; i < width; i++)
    {
    	sumR+=*imgOrg;
		sumD+=*imgPred;
		sumRD+= (*imgOrg)*(*imgPred);
		sumD2+=(*imgPred)*(*imgPred);
		*imgOrg++ ;
		*imgPred++;
    }
  }
  
  
  scale=(num*sumRD-sumD*sumR)/(num*sumD2-sumD*sumD);
   if(0)
  {
	  if((scale>=0.995)&&(scale<=1.005))
	  {
	  	scale=1.00;
	  }
  	 // mv->offset[yuv]=(sumR-scale*sumD)/num;
  }
  else
  {
	  scale=1.00;
	 // mv->offset[yuv]=0;

  }
  curr_mv->scale[yuv]=scale*100;
  scale=(float)curr_mv->scale[yuv]/100;
  curr_mv->offset[yuv]=(sumR-scale*sumD)/num;
  mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x].scale[yuv]=curr_mv->scale[yuv];
  mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x].offset[yuv]=curr_mv->offset[yuv];
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];    
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i = 0; i < width; i++)
    {
		/*
    	sumR+=*imgOrg;
		sumD+=*imgPred;
		sumRD+= (*imgOrg)*(*imgPred);
		sumD2+=(*imgPred)*(*imgPred);
		*/
		*m7++ = (*imgOrg++ - (int)(scale*(*imgPred++))-curr_mv->offset[yuv]);
    }
  }   
}

void sample_reconstruct_p4x4 (Macroblock *currMB,int block,int yuv,int bx,int by,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height,int   list_mode[2], int max_imgpel_value, int dq_bits)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;
	PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j;
  float scale;
 // int  block_x = currMB->block_x +(block&0x01)*2+bx;
 // int	 block_y = currMB->block_y+(block&0x02)+by;
  int  block_x = (block&0x01)*2+bx;
  int	 block_y =(block&0x02)+ by;
   MotionVector ***** mv_array = currSlice->all_mv;
  MotionVector *curr_mv = NULL;
  curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
  scale=(float)curr_mv->scale[yuv]/100;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i=0;i<width;i++)
      *imgOrg++ = (imgpel) iClip1( max_imgpel_value, (int)(scale*(*imgPred++))+curr_mv->offset[yuv]+rshift_rnd_sf(*m7++, dq_bits) );
  }
}
void sample_reconstruct_p4x4forzero (Macroblock *currMB,int block,int yuv,int bx,int by,imgpel **curImg, imgpel **mpr,  int mb_x, int opix_x,int   list_mode[2], int width, int height)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;
	PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j;
  float scale;
  //int  block_x = currMB->block_x +(block&0x01)*2+bx;
 // int	 block_y = currMB->block_y+(block&0x02)+by;
  int  block_x = (block&0x01)*2+bx;
  int	 block_y = (block&0x02)+by;
  MotionVector ***** mv_array = currSlice->all_mv;
  MotionVector *curr_mv = NULL;
  curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
  scale=(float)curr_mv->scale[yuv]/100;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];
    imgPred = &mpr[j][mb_x];
    for (i=0;i<width;i++)
      *imgOrg++ = (imgpel) ((int)(scale*(*imgPred++))+curr_mv->offset[yuv] );
  }
}
void sample_reconstruct_p16x16 (Macroblock *currMB,int block,int yuv,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height,int   list_mode[2], int max_imgpel_value, int dq_bits)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;
	

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j;
  float scale;
 // int  block_x = currMB->block_x + (block & 0x01)*2;
 // int  block_y = currMB->block_y + (block & 0x02);
  int  block_x = (block & 0x01)*2;
  int  block_y = (block & 0x02);
  MotionVector ***** mv_array = currSlice->all_mv;
  MotionVector *curr_mv = NULL;
  curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
  scale=(float)curr_mv->scale[yuv]/100;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i=0;i<width;i++)
      *imgOrg++ = (imgpel) iClip1( max_imgpel_value, (int)(scale*(*imgPred++))+curr_mv->offset[yuv]+rshift_rnd_sf(*m7++, dq_bits) );
  }
}

void sample_reconstruct_p8x8 (Macroblock *currMB,int block,int yuv,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height, int   list_mode[2],int max_imgpel_value, int dq_bits)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;
	PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j;
  float scale;
 // int  block_x = currMB->block_x + (block & 0x01)*2;
 // int  block_y = currMB->block_y + (block & 0x02);
  int  block_x = (block & 0x01)*2;
  int  block_y = (block & 0x02);

  MotionVector ***** mv_array = currSlice->all_mv;
  MotionVector *curr_mv = NULL;
  curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
  scale=(float)curr_mv->scale[yuv]/100;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i=0;i<width;i++)
      *imgOrg++ = (imgpel) iClip1( max_imgpel_value, (int)(scale*(*imgPred++))+curr_mv->offset[yuv]+rshift_rnd_sf(*m7++, dq_bits) );
  }
}
void sample_reconstruct_p8x8forzero (Macroblock *currMB,int block,int yuv,imgpel **curImg, imgpel **mpr,  int mb_x, int opix_x, int width, int height, int   list_mode[2],int max_imgpel_value, int dq_bits)
{
	VideoParameters *p_Vid = currMB->p_Vid;
	InputParameters *p_Inp = currMB->p_Inp;
	Slice *currSlice = currMB->p_Slice;
	PicMotionParams **motion = p_Vid->enc_picture->mv_info;

  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j;
  float scale;
 // int  block_x = currMB->block_x + (block & 0x01)*2;
  //int  block_y = currMB->block_y + (block & 0x02);
  int  block_x =  (block & 0x01)*2;
  int  block_y =  (block & 0x02);
  MotionVector ***** mv_array = currSlice->all_mv;
  MotionVector *curr_mv = NULL;
  curr_mv = &mv_array[LIST_0][LIST_0][list_mode[LIST_0]][block_y][block_x];
  scale=(float)curr_mv->scale[yuv]/100;
  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];
    imgPred = &mpr[j][mb_x];
    for (i=0;i<width;i++)
      *imgOrg++ = (imgpel) iClip1( max_imgpel_value, (int)(scale*(*imgPred++))+curr_mv->offset[yuv]);
  }
}


void compute_residue (imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height)
{
  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j;


  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];    
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i = 0; i < width; i++)
    {
      	*m7++ = *imgOrg++ - *imgPred++;
    }
  }
}

void sample_reconstruct (imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height, int max_imgpel_value, int dq_bits)
{
  imgpel *imgOrg, *imgPred;
  int    *m7;
  int i, j;

  for (j = 0; j < height; j++)
  {
    imgOrg = &curImg[j][opix_x];
    imgPred = &mpr[j][mb_x];
    m7 = &mb_rres[j][mb_x]; 
    for (i=0;i<width;i++)
      *imgOrg++ = (imgpel) iClip1( max_imgpel_value, rshift_rnd_sf(*m7++, dq_bits) + *imgPred++);
  }
}

