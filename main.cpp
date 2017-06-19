
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void process(const char *filein);


int main(int argc, char *argv[])	{
  
printf("comicsplit 1.51\n");

if(argc < 2)	{
    printf("Usage: comicsplit <file.png>\n");
    exit(1);
}  

process(argv[1]);

return 0;
}

