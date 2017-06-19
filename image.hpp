class Image	{
  
public:
	int width;
	int height;
	unsigned char *data;
	
	Image(int w, int h);
	~Image();
	unsigned char *getRowStart(int y);
	int isRowBlank(int y);
	int isColBlank(int x);
	int getRowCount();
	int getColCount();
	int findRowStart(int y);
	int findRowEnd(int y);
	int findColStart(int x);
	int findColEnd(int x);
	
	Image * makeSubImage(int x, int y, int w, int h);
};





