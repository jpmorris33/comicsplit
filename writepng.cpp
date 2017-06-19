//
//	PNG writer, based on an example by A. Greensted
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
	    png_destroy_write_struct(pngptr,infoptr);
}



int writePNG(const char* filename, Image *img)
{
	FILE *fp;
	png_structp pngptr=NULL;
	png_infop infoptr=NULL;
	
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		printf("Could not write file %s\n", filename);
		freePNGstruct(fp,&pngptr,&infoptr);
		return 0;
	}

	// Initialize write structure
	pngptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (pngptr == NULL) {
		printf("Could not allocate write struct\n");
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
		printf( "Error during png creation\n");
		freePNGstruct(fp,&pngptr,&infoptr);
		return 0;
	}

	png_init_io(pngptr, fp);

	// Write header (8 bit colour depth)
	png_set_IHDR(pngptr, infoptr, img->width, img->height,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_write_info(pngptr, infoptr);

	
	
	// Write image data
	for (int y=0 ; y<img->height ; y++) {
		png_write_row(pngptr, img->getRowStart(y));
	}

	// End write
	png_write_end(pngptr, NULL);

	freePNGstruct(fp,&pngptr,&infoptr);
	return 1;
}
