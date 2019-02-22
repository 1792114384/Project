#pragma once

#include "Huffman.hpp"
#include <string>
using namespace std;

struct CharInfo
{
	CharInfo()
		:_chCount(0)
		,_strCode()
	{}
	friend bool operator>(const CharInfo &left, const CharInfo &right)
	{
		return left._chCount > right._chCount;
	}
	friend CharInfo operator+(const CharInfo &left, const CharInfo &right)
	{
		CharInfo tmp;
		tmp._chCount = left._chCount + right._chCount;
		return tmp;
	}
	unsigned char _ch;
	long long _chCount;
	string _strCode;
};

class FileCompressHuffman
{
public:
	void CompressFile(string& FilePath);
	void UnCompressFile(string& FilePath);
private:
	//��ȡ����
	void GetCode(const HuffmanTreeNode<CharInfo>* hT);
	//��ȡ�ļ���
	string GetFileName(string& FilePath);
	//��ȡ�ļ���Ϣ
	void GetFileInfo(FILE* fOut, string& FilePath);
	//����ѹ���������
	void CompressData(FILE *fIn, FILE *fOut);
	//��ȡ�ļ�ͷ����Ϣ
	void GetHead(FILE *fIn, string& FileSuffixName);
	//��ȡѹ���ļ�ÿһ�е���Ϣ
	void GetLine(FILE *fIn, unsigned char* buf);
	//����ת��Ϊԭ����
	void CodeToData(FILE* fIn, FILE *fOut, HuffmanTreeNode<CharInfo>* hT);
private:
	CharInfo info[256];
};