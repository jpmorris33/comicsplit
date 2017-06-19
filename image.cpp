#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.hpp"


Image::Image(int w, int h)	{
  InitImage(w,h,NULL);
}

Image::Image(int w, int h, unsigned char *clonedata)	{
  InitImage(w,h,clonedata);
}

void Image::InitImage(int w, int h, unsigned char *clonedata)	{
    width=w;
    height=h;

    dataptr = clonedata;
    datastore = NULL;
    
    // If we're creating an image from scratch, use this
    if(!clonedata)	{
      datastore = (unsigned char *)calloc(w*3,h);
      if(!datastore)	{
	printf("Out of memory allocating %dx%d image\n",w,h);
	abort();
      }
      dataptr = datastore;
    }

    // Now build a list of row pointers
    rowlist = (unsigned char **)calloc(sizeof(char *),h);
    if(rowlist == NULL)	{
      printf("Out of memory allocating %d rows\n",h);
      abort();
    }

    // Set up the offsets
    long width3 = width*3;
    unsigned char *pos = dataptr;
    for(int ctr=0;ctr<h;ctr++)	{
      rowlist[ctr] = pos;
      pos += width3;
    }
    
}


Image::~Image()	{
    if(datastore)	{
      free(datastore);
      datastore=NULL;
    }
    if(rowlist)	{
      free(rowlist);
      rowlist=NULL;
    }
}





unsigned char *Image::getRowStart(int y)	{
  if(y<0 || y>=height)	{
    return NULL;
  }
  
  return rowlist[y];
}


int Image::isRowBlank(int y)	{
  unsigned char *ptr = getRowStart(y);
  unsigned char r,g,b;
  
  if(y<0 || y >= height)	{
    return 1;
  }
  
  for(int ctr=0;ctr<width;ctr++)	{
    r = *ptr++;
    g = *ptr++;
    b = *ptr++;
    
    if(r || g || b)	{
      return 0;
    }
  }
  
  return 1;
}


int Image::isColBlank(int x)	{
  unsigned char *ptr = NULL;
  unsigned char r,g,b;
  long offset = x*3;

  if(x<0 || x>= width)	{
    return 1;
  }
  
  for(int ctr=0;ctr<height;ctr++)	{
    ptr = getRowStart(ctr)+offset;	// Move to the X coordinate we're interested in
    r = *ptr++;
    g = *ptr++;
    b = *ptr++;
    
    if(r || g || b)	{
      return 0;
    }
  }
  
  return 1;
}


int Image::findRowStart(int y)	{
  
  int ctr;
  for(ctr=y;ctr<height;ctr++)	{
    if(!isRowBlank(ctr))	{
      return ctr;
    }
  }
  
  return height;
}


int Image::findRowEnd(int y)	{
  
  int ctr;
  for(ctr=y;ctr<height;ctr++)	{
    if(isRowBlank(ctr))	{
      return ctr;
    }
  }
  
  return height;
}

int Image::findColStart(int x)	{
  
  int ctr;
  for(ctr=x;ctr<width;ctr++)	{
    if(!isColBlank(ctr))	{
      return ctr;
    }
  }
  
  return width;
}


int Image::findColEnd(int x)	{
  
  int ctr;
  for(ctr=x;ctr<width;ctr++)	{
    if(isColBlank(ctr))	{
      return ctr;
    }
  }
  
  return width;
}



int Image::getRowCount()	{
  
  int pos=0;
  int rows=0;

  do	{
      pos=findRowStart(pos);
      if(pos < height)	{
	rows++;
	pos=findRowEnd(pos);
      }
      
  } while(pos < height);
  
  return rows;
}


int Image::getColCount()	{
  
  int pos=0;
  int cols=0;

  do	{
      pos=findColStart(pos);
      if(pos < width)	{
	cols++;
	pos=findColEnd(pos);
      }
      
  } while(pos < width);
  
  return cols;
}

//
//  Create a sub-image by address - DON'T free the main image until you're sure you're finished with it!
//
Image *Image::makeSubImage(int x, int y, int w, int h)	{

  int x2,y2,pos;
  long offset;
  
  if(x<0)
      x=0;
  if(y<0)
      y=0;
  if(x>=width)
      x=width-1;
  if(y>=height)
      y=height-1;
  
  x2=x+w;
  y2=y+h;

  if(x2 >= width)
    x2=width;
  if(y2 >= height)
    y2=height;
  
  if(x2<=x)
    return NULL;
  if(y2<=y)
    return NULL;

  // Now convert it back to relative coordinates in case it clipped
  w=x2-x;
  h=y2-y;
  
  Image *newImage = new Image(w,h,dataptr);	// Clone the image from our original datastore

  // Now simply rebuild the rowlist in the new image to point to bits of the old one
  
  offset = x*3;
  pos=0;
  for(int a=y;a<y2;a++)	{
    newImage->rowlist[pos++]=getRowStart(a)+offset;
  }
  
  return newImage;
  
}

//
// Create a sub-image by copy (not currently used anymore)
//
Image *Image::copySubImage(int x, int y, int w, int h)	{

  int x2,y2,wbytes;
  unsigned char *inptr,*outptr;
  long offset;
  
  if(x<0)
      x=0;
  if(y<0)
      y=0;
  if(x>=width)
      x=width-1;
  if(y>=height)
      y=height-1;
  
  x2=x+w;
  y2=y+h;

  if(x2 >= width)
    x2=width;
  if(y2 >= height)
    y2=height;
  
  if(x2<=x)
    return NULL;
  if(y2<=y)
    return NULL;

  // Now convert it back to relative coordinates in case it clipped
  w=x2-x;
  h=y2-y;
  wbytes = w*3;
  
  Image *newImage = new Image(w,h);
  
  outptr = newImage->datastore;
  inptr = getRowStart(y);
  inptr += (x*3); // Start of segment
  offset = width * 3;
  
  for(int a=y;a<y2;a++)	{
    memcpy(outptr,inptr,wbytes);
    outptr += wbytes;
    inptr += offset;
  }
  
  return newImage;
  
}
