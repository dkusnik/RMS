#include "image.h"


#ifdef WIN64
int main_ms_rlsf (int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
	float elapsed_time;
	clock_t start_time;
	Image* in_img;
	Image* noisy_img;
	Image* out_img;
	int iter;
	int r;
	float sigma;
	int alpha;
	char* alg = (char *) "CUDA_MS_RLSF";

	if (argc < 3)
	{
		printf("argc: %d\n", argc);
		fprintf(stderr, "Usage: %s <reference image { rgb }> <noisy image {rgb}> <block_radius> <alpha> <sigma> <iter>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 7)
	{
		r = atoi(argv[3]);
		alpha = atof(argv[4]);
		sigma = atoi(argv[5]);
		iter = atoi(argv[6]);
	}
	else {
		r = 2;
		alpha = 3;
		sigma = 50;
		iter = 10;
	}

	printf("Testing Robust MeanShift (RMS) Filter...\n");
	/* Read the input image */
	in_img = read_img(argv[1]);
	noisy_img = read_img(argv[2]);

	/* Make sure it's an rgb image */
	if (is_gray_img(in_img))
	{
		fprintf(stderr, "Input image ( %s ) must not be grayscale !", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Start the timer */
	start_time = start_timer();
	#ifdef CUDA
	    out_img = CUDA_filter_ms_rlsf(noisy_img, r, alpha, sigma, iter);
	#else
		out_img = filter_ms_rlsf(noisy_img, r, alpha, sigma, iter);
	#endif

    elapsed_time = stop_timer(start_time);

    write_img(out_img, "out.png", FMT_PNG);

    printf("Used parameters: r, alpha, sigma, iter: %d, %d, %f, %d, %f\n\n=========== \n\n", r, alpha, sigma, iter,  elapsed_time);

    printf("Measures: \n \n");

	#ifdef CUDA
        printf("Prat: %f\n", calculate_prat(in_img, out_img));
	#endif
    calculate_snr(in_img, out_img, NULL);
    calculate_ssim(in_img, out_img, NULL);

	#ifdef CUDA
        printf("\n\nCUDA Robust MeanShift (RMS) time = %f\n", elapsed_time);
	#else
        printf("\n\nRobust MeanShift (RMS) time = %f\n", elapsed_time);
	#endif

	/* Calculate and print various error measures */

	free_img(in_img);
    free_img(out_img);
	free_img(noisy_img);
	return EXIT_SUCCESS;
}
