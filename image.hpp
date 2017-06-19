class Image	{
  
public:
	int width;
	int height;
	unsigned char **rowlist;
	unsigned char *dataptr;		// Pointer to image data, may be from another image
	unsigned char *datastore;	// Actual allocated memory, if any
	
	Image(int w, int h);
	Image(int w, int h, unsigned char *clonedata);
	~Image();
	void InitImage(int w, int h, unsigned char *clonedata);
	unsigned char *getRowStart(int y);
	int isRowBlank(int y);
	int isColBlank(int x);
	int getRowCount();
	int getColCount();
	int findRowStart(int y);
	int findRowEnd(int y);
	int findColStart(int x);
	int findColEnd(int x);
	
	Image *makeSubImage(int x, int y, int w, int h);
	Image *copySubImage(int x, int y, int w, int h);
};





