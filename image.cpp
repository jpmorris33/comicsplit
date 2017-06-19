#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.hpp"


Image::Image(int w, int h)	{
    width=w;
    height=h;
    
    data = (unsigned char *)calloc(w*3,h);
    if(data == NULL)	{
      printf("Out of memory allocating %dx%d images\n",w,h);
      abort();
    }
}

Image::~Image()	{
    if(data)	{
      free(data);
      data=NULL;
    }
}





unsigned char *Image::getRowStart(int y)	{
  long offset = y*width*3;
  return data+offset;
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
  unsigned char *ptr = getRowStart(0);
  unsigned char r,g,b;
  long offset = x*3;

  if(x<0 || x>= width)	{
    return 1;
  }
  
  ptr += offset;	// Move to the X coordinate we're interested in
  
  offset = (width-1)*3;	// this is now the offset to move the pointer to the next row
  
  for(int ctr=0;ctr<height;ctr++)	{
    r = *ptr++;
    g = *ptr++;
    b = *ptr++;
    
    if(r || g || b)	{
      return 0;
    }
    
    ptr += offset;
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


Image *Image::makeSubImage(int x, int y, int w, int h)	{

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
  
  outptr = newImage->data;
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
