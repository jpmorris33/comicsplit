//
//	PNG read, based on an PNG writing example by A. Greensted and other sources
//

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "image.hpp"

static void freePNGstruct(FILE *fp, png_structpp pngptr, png_infopp infoptr);

void freePNGstruct(FILE *fp, png_structpp pngptr, png_infopp infoptr)	{
	if(fp)
	    fclose(fp);
	if (infoptr)
	  png_free_data(*pngptr, *infoptr, PNG_FREE_ALL, -1);
	if (pngptr)
	    png_destroy_read_struct(pngptr,infoptr,NULL);
}



Image *readPNG(const char* filename)
{
	FILE *fp;
	png_structp pngptr=NULL;
	png_infop infoptr=NULL;
	int readlen=0;

	unsigned char header[8];
	
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		printf("Could not open file %s\n", filename);
		freePNGstruct(fp,&pngptr,&infoptr);
		return 0;
	}

        readlen=fread(header, 1, 8, fp);
        if (readlen < 8 || png_sig_cmp(header, 0, 8))	{
		printf("File %s is not a PNG file\n", filename);
		freePNGstruct(fp,&pngptr,&infoptr);
		return 0;
	}
	
	// Initialize write structure
	pngptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (pngptr == NULL) {
		printf("Could not allocate read struct\n");
		freePNGstruct(fp,&pngptr,&infoptr);
		return 0;
	}

	// Initialize info structure
	infoptr = png_create_info_struct(pngptr);
	if (infoptr == NULL) {
		printf("Could not allocate info struct\n");
		freePNGstruct(fp,&pngptr,&infoptr);
		return 0;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(pngptr))) {
		printf( "Error during png read\n");
		freePNGstruct(fp,&pngptr,&infoptr);
		return 0;
	}

	png_init_io(pngptr, fp);
        png_set_sig_bytes(pngptr, 8);
        png_read_info(pngptr, infoptr);

        int width = png_get_image_width(pngptr, infoptr);
        int height = png_get_image_height(pngptr, infoptr);
        int color_type = png_get_color_type(pngptr, infoptr);
        int bit_depth = png_get_bit_depth(pngptr, infoptr);


	// Ensure it's converted to straight 24-bit RGB

	if (bit_depth == 16)
	    png_set_strip_16(pngptr);
	if (color_type == PNG_COLOR_TYPE_GRAY ||
	    color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	      png_set_gray_to_rgb(pngptr);	
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	    png_set_palette_to_rgb(pngptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	    png_set_expand_gray_1_2_4_to_8(pngptr);
	if (png_get_valid(pngptr, infoptr, PNG_INFO_tRNS))
	    png_set_tRNS_to_alpha(pngptr);
	png_set_strip_alpha(pngptr);

        png_read_update_info(pngptr, infoptr);

        /* read file */
        if (setjmp(png_jmpbuf(pngptr)))	{
                printf("Error during read_image\n");
		freePNGstruct(fp,&pngptr,&infoptr);
		return NULL;
	}

	// Now build a list of pointers
	png_bytep *row = (png_bytep *)calloc(sizeof(png_bytep), height);
	if(!row)	{
	  freePNGstruct(fp,&pngptr,&infoptr);
	  return NULL;
	}
	
	Image *newImage = new Image(width,height);
	
        for (int y=0; y<height; y++)	{
	  row[y] = newImage->getRowStart(y);
	}

        png_read_image(pngptr, row);
	png_read_end(pngptr, NULL);

	freePNGstruct(fp,&pngptr,&infoptr);
	
	free(row);
	
	return newImage;
}
