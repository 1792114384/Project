#include "lz77.h"
#include <iostream>

int main()
{
	LZ77 lz;
	lz.CompressFile("1.txt");
	
	lz.UnCompressFile("1.zipL");
	system("pause");
	return 0;
}