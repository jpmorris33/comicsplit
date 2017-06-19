
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void process(const char *filein);


int main(int argc, char *argv[])	{
  
if(argc < 2)	{
    printf("Usage: pngsplit <file.png>\n");
    exit(1);
}  

process(argv[1]);
}

