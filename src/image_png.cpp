/*
 * Author: Damian Kusnik
 *
 * Based on code from:
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * To build, use the next instruction on OS X.
 * $ brew install libpng
 * $ clang -lz -lpng15 libpng_test.c
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include "png.h"
#include "image.h"

Image *read_png_file(FILE *fp)
{
  int x, y;
  int width, height;
  png_byte color_type;
  png_byte bit_depth;
  png_bytep *row_pointers;
  png_infop info;
  png_structp png;

  Image *img_fourier;
  byte ***data;

  png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
    abort();

  info = png_create_info_struct(png);
  if (!info)
    abort();

  if (setjmp(png_jmpbuf(png)))
    abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width = png_get_image_width(png, info);
  height = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth = png_get_bit_depth(png, info);

  /* Read any color_type into 8bit depth, RGBA format.
   * See http://www.libpng.org/pub/png/libpng-manual.txt
   */

  if (bit_depth == 16)
    png_set_strip_16(png);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  /* PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth. */
  if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if (png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  /*
  // These color_type don't have an alpha channel then fill it with 0xff.
  */
  if (color_type == PNG_COLOR_TYPE_RGB ||
      color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  /*  png_set_filler(png, 0xFF, PNG_FILLER_AFTER);*/

  if (color_type == PNG_COLOR_TYPE_GRAY ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
  for (y = 0; y < height; y++)
  {
    row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
  }

  png_read_image(png, row_pointers);

  img_fourier = alloc_img(PIX_RGB, height, width);
  data = (byte***)get_img_data_nd(img_fourier);

  for (y = 0; y < height; y++)
  {
    png_bytep row = row_pointers[y];
    for (x = 0; x < width; x++)
    {
      /* this is a pointer to unsigned char */
      png_bytep px = &(row[x * 4]);
      data[y][x][0] = px[0];
      data[y][x][1] = px[1];
      data[y][x][2] = px[2];
    }
  }
  //fclose(fp);
  for (y = 0; y < height; y++)
  {
    free(row_pointers[y]);
  }
  free(row_pointers);

  png_destroy_read_struct(&png, &info, NULL);
  png = NULL;
  info = NULL;

  return img_fourier;
}

int write_png_file(const Image* _img, FILE* fp)
{
  int x, y;
  int width = _img->num_cols; 
  int height = _img->num_rows;

  //png_byte bit_depth;
  //png_byte color_type;
  png_bytep *row_pointers;
  byte ***data = (byte ***) get_img_data_nd(_img);

  png_structp png;
  png_infop info;
  if (!fp)
	  return 1;

  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
	  return 1;

  info = png_create_info_struct(png);
  if (!info)
	  return 1;

  if (setjmp(png_jmpbuf(png)))
	  return 1;

  png_init_io(png, fp);

  /* Output is 8bit depth, RGBA format. */
  png_set_IHDR(
      png,
      info,
      width, height,
      8,
      PNG_COLOR_TYPE_RGB,
      PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_DEFAULT,
      PNG_FILTER_TYPE_DEFAULT);
  png_write_info(png, info);

  /* To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
   * Use png_set_filler().
   */
  png_set_filler(png, 0, PNG_FILLER_AFTER);

  row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
  for (y = 0; y < height; y++)
  {
    row_pointers[y] = (png_byte *)malloc(sizeof(png_byte) * width * 4);
    //row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));

  }
  for (y = 0; y < height; y++)
  {
    png_bytep row = row_pointers[y];
    for (x = 0; x < width; x++)
    {
      png_bytep px = &(row[x*4]);
      px[0] = data[y][x][0];
      px[1] = data[y][x][1];
      px[2] = data[y][x][2];
    }
  }

  png_write_image(png, row_pointers);

  png_write_end(png, NULL);

  for (y = 0; y < height; y++)
  {
    free(row_pointers[y]);
  }
  free(row_pointers);

  //fclose(fp);
  if (png && info)
    png_destroy_write_struct(&png, &info);
  return 0;
}

