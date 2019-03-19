#include "FileCompressHuffman.h"
#include "lz77.h"

#include <ctime>
#include <iostream>

int main()
{
	time_t start, stop1, stop2, stop3, stop4;
	start = time(NULL);
	
	FileCompressHuffman fH;
	LZ77 lz;

	string File = "1.txt";
	lz.CompressFile(File); //��һ��ѹ��
	stop1 = time(NULL);
	std::cout << stop1 - start << endl;

	File = "1.zipL";
	fH.CompressFile(File);//�ڶ���ѹ��
	stop2 = time(NULL);
	std::cout << stop2 - start << endl;

	File = "1.Hzip"; 
	fH.UnCompressFile(File);//��һ�ν�ѹ��
	stop3 = time(NULL);
	std::cout << stop3 - start << endl;

	File = "1.zipLH"; 
	lz.UnCompressFile(File); //�ڶ��ν�ѹ��
	stop4 = time(NULL);
	std::cout << stop4 - start << endl;

	system("pause");
	return 0;
}