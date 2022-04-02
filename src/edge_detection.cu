#ifdef CUDA
/** 
 * @file edge_detection.cu
 * Routines for VR edge detection
 */
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda_profiler_api.h"
#include "math_constants.h"

#include <stdio.h>
#include "image.h"
#include "math.h"

 /** 
 * @brief Implements the VR edge detector. Eq.2 from :
DOI: 10.1109/83.217230
SourcePubMed
Authors:
Panos E. Trahanias
32.18Foundation for Research and Technology - Hellas
A N Venetsanopoulos
 * @param[in] in_img Image pointer { rgb }
 * @param[in] treshold Cut off for edge or not
 *
 * @return Pointer to the binary edge image or NULL
 *
 * @author Kusnik Damian 
 * @date 26.10.2020
 */
__global__
void CUDA_detect_edge_VR(int* in_data, float* out_data, int width, int height, int threshold) {
	int ic = blockIdx.y * blockDim.y + threadIdx.y;
	int ir = blockIdx.x * blockDim.x + threadIdx.x;
	if (ic >= width - 1 || ir >= height - 1 || ic < 1 || ir < 1)
		return;

	int pos = ir * width + ic;

	int f = 1;
	int iwr, iwc;
	int win_count = 9;
	int center_pix = win_count / 2;
	int offset;
	int min_dist;
	int max_dist;
	int min_dist_index, max_dist_index;
	float dist_sum = 0;

	// this should be dynamic, speed reasons
	int red[9];
	int blue[9];
	int green[9];
	int count = 0;

	float dist_mat[9][9];
	
	for (iwr = 0; iwr < win_count; iwr++)
		for (iwc = 0; iwc < win_count; iwc++)
			dist_mat[9][9] = 0;
	
	/* For each window row */
	for (iwr = -f; iwr <= f; iwr++)
	{
		/* For each window column */
		for (iwc = -f; iwc <= f; iwc++)
		{
			/* Store the red, green, blue values */
			offset = pos +iwr * width + iwc;
			red[count] = (in_data[offset] & 0XFF0000) >> 16;
			green[count] = (in_data[offset] & 0XFF00) >> 8;
			blue[count] = (in_data[offset] & 0XFF) ;
			count++;
		}
	}

	/* Calculate the distances between pairwise color vectors */
	for (iwr = 0; iwr < win_count; iwr++)
	{
		for (iwc = iwr + 1; iwc < win_count; iwc++)
		{
			dist_mat[iwr][iwc] = sqrtf(
				(red[iwr] - red[iwc]) * (red[iwr] - red[iwc]) +
				(green[iwr] - green[iwc]) * (green[iwr] - green[iwc]) +
				(blue[iwr] - blue[iwc]) * (blue[iwr] - blue[iwc])
			);
		}
	}

	/* Calculate the cumulative distance for each pixel and find the minimum */
	min_dist = INT_MAX;
	min_dist_index = center_pix;
	max_dist = INT_MIN;
	max_dist_index = center_pix;
	for (iwr = 0; iwr < win_count; iwr++)
	{
		dist_sum = 0.0;

		for (iwc = 0; iwc < iwr; iwc++)
		{
			dist_sum += dist_mat[iwc][iwr];
		}

		for (iwc = iwr + 1; iwc < win_count; iwc++)
		{
			dist_sum += dist_mat[iwr][iwc];
		}

		if (dist_sum < min_dist)
		{
			min_dist = dist_sum;
			min_dist_index = iwr;
		}
		if (dist_sum > max_dist)
		{
			max_dist = dist_sum;
			max_dist_index = iwr;
		}
	}
	
	out_data[pos] = sqrtf(
			(red[min_dist_index] - red[max_dist_index]) * (red[min_dist_index] - red[max_dist_index]) +
			(green[min_dist_index] - green[max_dist_index]) * (green[min_dist_index] - green[max_dist_index]) +
			(blue[min_dist_index] - blue[max_dist_index]) * (blue[min_dist_index] - blue[max_dist_index])
		);
	return;
}


Image*
detect_edge_VR ( const Image * in_img, const int threshold)
{
 SET_FUNC_NAME ( "detect_edge_VR" );

 byte*** in_data;
 byte** out_data;
 int num_rows, num_cols;
 Image* out_img;
 if ( !is_rgb_img ( in_img ) )
  {
   ERROR_RET ( "Not a color image !", NULL );
  }

 if ( !IS_POS ( threshold ) )
  {
   ERROR ( "theshold ( %d ) must be positive !", threshold );
   return NULL;
  }

 num_rows = get_num_rows(in_img);
 num_cols = get_num_cols(in_img);

 in_data = (byte***)get_img_data_nd(in_img);
 out_img = alloc_img(PIX_GRAY, num_rows, num_cols);
 out_data = (byte**)get_img_data_nd(out_img);

 size_t size_i = size_t(num_rows * num_cols) * sizeof(int);
 size_t size_f = size_t(num_rows * num_cols) * sizeof(float);

 int* int_in_data = (int*)malloc(size_i);

 for (int i = 0; i < num_rows; i++) 
	 for (int j = 0; j < num_cols; j++)
		 int_in_data[i * num_cols + j] = (((int)in_data[i][j][0]) << 16) | ((int)in_data[i][j][1] << 8) | ((int)in_data[i][j][2]);

 int* d_in_data;
 cudaMalloc((void**)&d_in_data, size_i);
 cudaMemcpy(d_in_data, int_in_data, size_i, cudaMemcpyHostToDevice);

 float* d_out_data;
 cudaMalloc((void**)&d_out_data, size_f);

 dim3 blockDim(1, 128, 1);
 dim3 gridDim((unsigned int)ceil((float)num_rows / (float)blockDim.x),
	 (unsigned int)ceil((float)num_cols / (float)blockDim.y),
	 1);

 CUDA_detect_edge_VR << < gridDim, blockDim >> > (d_in_data, d_out_data, num_cols, num_rows, threshold);

 cudaDeviceSynchronize();

 float* float_out_data = (float*)malloc(size_f);
 cudaMemcpy(float_out_data, d_out_data, size_f, cudaMemcpyDeviceToHost);

 for (int i = 0; i < num_rows; i++)
	 for (int j = 0; j < num_cols; j++)
		 out_data[i][j] = (int)(float_out_data[i * num_cols + j])&0xFF;

 // Free device memory

 cudaFree(d_in_data);
 cudaFree(d_out_data);
 cudaDeviceSynchronize();

 free(int_in_data);
 free(float_out_data);

 return out_img;
}
#endif
