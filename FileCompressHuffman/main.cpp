#include "FileCompressHuffman.h"

int main()
{
	FileCompressHuffman fH;
	string File = "1.txt";
	//fH.CompressFile(File);

	File = "1.Hzip";
	fH.UnCompressFile(File);
	return 0;
}