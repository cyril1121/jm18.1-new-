
/*!
 ************************************************************************
 * \file
 *    blk_prediction.h
 *
 * \brief
 *    block prediction header
 *
 * \author
 *      Main contributors (see contributors.h for copyright, 
 *                         address and affiliation details)
 *      - Alexis Michael Tourapis  <alexismt@ieee.org>
 *
 *************************************************************************************
 */

#ifndef _BLK_PREDICTION_H_
#define _BLK_PREDICTION_H_
#include "mbuffer.h"
extern void compute_residue_p16x16 (Macroblock *currMB,int block,int yuv,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int   list_mode[2],int width, int height);

extern void compute_residue    (imgpel **curImg, imgpel **mb_pred, int **mb_rres, int mb_x, int opix_x, int width, int height);
extern void sample_reconstruct (imgpel **curImg, imgpel **mb_pred, int **mb_rres, int mb_x, int opix_x, int width, int height, int max_imgpel_value, int dq_bits);
extern void sample_reconstruct_p8x8 (Macroblock *currMB,int block,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height, int max_imgpel_value, int dq_bits);
extern void sample_reconstruct_p4x4 (Macroblock *currMB,int block,int block_x,int block_y,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height, int max_imgpel_value, int dq_bits);
extern void compute_residue_p8x8 (Macroblock *currMB,int block,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height);
extern void compute_residue_p4x4 (Macroblock *currMB,int block,int block_x,int block_y,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height);
extern void sample_reconstruct_p4x4forzero (Macroblock *currMB,int block,int bx,int by,imgpel **curImg, imgpel **mpr, int **mb_rres, int mb_x, int opix_x, int width, int height);

#endif

