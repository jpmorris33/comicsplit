
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.hpp"

extern int writePNG(const char* filename, Image *img);
extern Image *readPNG(const char* filename);
void process(const char *filein);
Image **SplitImageVertically(Image *img, int *rows);
Image **SplitImageHorizontally(Image *img, int *cols);



void process(const char *filein)	{
int vlen,hlen,filenum,x,y;
char fileout[1024];
char filename[1024];
char *extptr;

// Figure out where the extension is and remove it, since we want to insert something in front of it
strcpy(filename,filein);

extptr=strstr(filename,".png");
if(!extptr)
  extptr=strstr(filename,".PNG");
if(!extptr)
  extptr=strstr(filename,".Png");

if(!extptr)	{
  printf("Filename '%s' doesn't seem to have a PNG extension.\n",filein);
  return;
}

// Remove the extension
*extptr++=0;

// Now load the image in and start processing

Image *mainImage = readPNG(filein);
if(!mainImage)	{
    printf("Could not read png file '%s'\n",filein);
    return;
}

// First split it into rows

filenum = 1;

Image **vlist = SplitImageVertically(mainImage,&vlen);

if(!vlist)	{
  printf("Nothing to do\n");
  delete mainImage;
  return;
}

// Go through the rows and split them into columns

for(y=0;y<vlen;y++)	{
  Image **hlist = SplitImageHorizontally(vlist[y],&hlen);
  
  if(!hlist)	{
    continue;
  }
  
  // Split the columns into individual cels and save them

  for(x=0;x<hlen;x++)	{
    sprintf(fileout,"%s_%02d.%s",filename,filenum++,extptr);
    printf("Save file to %s\n",fileout);
    
    writePNG(fileout,hlist[x]);
    
  }
  
  // Now free the images
  for(x=0;x<hlen;x++)	{
    delete hlist[x];
  }
  free(hlist);
  hlist=NULL;
}
  

// Now free the rest of the images
for(y=0;y<vlen;y++)	{
    delete vlist[y];
}
free(vlist);
vlist=NULL;

delete mainImage;
printf("done\n");
}





Image **SplitImageVertically(Image *img, int *rows)	{
    
  Image **imagerow = NULL;
  int imagerows = img->getRowCount();
  printf("Image has %d rows\n",imagerows);

  *rows=0; // Error default
  
  if(imagerows < 1)	{
      printf("Nothing found, aborting\n");
      return NULL;
  }

  imagerow = (Image **)calloc(imagerows,sizeof(Image *));
  if(!imagerow)	{
	printf("Out of memory\n");
	return NULL;
  }

  // Split image vertically

  int start=0;
  int end=0;
  int rowctr=0;
  int h;
  Image *imgptr;
  
  for(int ctr=0;ctr<imagerows;ctr++)	{
    start=img->findRowStart(start);
    end=img->findRowEnd(start);
    h=end-start;
    
    imgptr = img->makeSubImage(0,start,img->width,h);
    
    imagerow[rowctr++] = imgptr;
    printf("Row %d, extent from %d to %d\n",ctr+1,start,end);
    
    start = end;
  }
  
  if(rowctr < imagerows)	{
      printf("Something went wrong\n");
      return NULL;
  }

  *rows = imagerows;
  
  return imagerow;
}



Image **SplitImageHorizontally(Image *img, int *cols)	{
    
  Image **imagecol = NULL;
  int imagecols = img->getColCount();
  printf("Image has %d cols\n",imagecols);

  *cols=0; // Error default
  
  if(imagecols < 1)	{
      printf("Nothing found, aborting\n");
      return NULL;
  }

  imagecol = (Image **)calloc(imagecols,sizeof(Image *));
  if(!imagecol)	{
	printf("Out of memory\n");
	return NULL;
  }

  // Split image horizontally

  int start=0;
  int end=0;
  int colctr=0;
  int w;
  Image *imgptr;
  
  for(int ctr=0;ctr<imagecols;ctr++)	{
    start=img->findColStart(start);
    end=img->findColEnd(start);
    w=end-start;
    
    imgptr = img->makeSubImage(start,0,w,img->height);
    
    imagecol[colctr++] = imgptr;
    printf("Col %d, extent from %d to %d\n",ctr+1,start,end);
    
    start = end;
  }
  
  if(colctr < imagecols)	{
      printf("Something went wrong\n");
      return NULL;
  }

  *cols = imagecols;
  
  return imagecol;
}
