#define _CRT_SECURE_NO_WARNINGS
#include "FileCompressHuffman.h"
#include "Huffman.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

//压缩文件
void FileCompressHuffman::CompressFile(string& FilePath)
{
	//以二进制的文件打开
	FILE* FIn = fopen(FilePath.c_str(), "rb");
	if (nullptr == FIn)
	{
		cout << "打开文件失败" << endl;
		return;
	}
	//填充字符
	for (int i = 0; i < 256; ++i)
	{
		info[i]._ch = i;
	}
	//填充字符出现的次数
	unsigned char buf[1024];
	size_t n;
	do
	{
		n = fread(buf, 1, 1024, FIn);
		for (size_t i = 0; i < n; i++)
			info[buf[i]]._chCount++;
	} while (n > 0);

	//创建Huffman树
	HuffmanTree<CharInfo> hT(info, 0);
	//获取编码
	GetCode(hT.GetRoot());
	//获取文件名
	string FileName = GetFileName(FilePath);
	FileName += ".Hzip";

	FILE* fOut = fopen(FileName.c_str(), "wb");
	if (fOut == nullptr)
	{
		cout << "压缩文件失败" << endl;
		return;
	}
	//存放文件信息
	GetFileInfo(fOut, FilePath);
	//让FIn重新指向文件头
	fseek(FIn, 0, SEEK_SET);
	//存放文件压缩后的数据
	CompressData(FIn, fOut);

	fclose(FIn);
	fclose(fOut);
}

//解压缩文件
void FileCompressHuffman::UnCompressFile(string& FilePath)
{
	//获取文件名
	string FileName;
	int end = FilePath.find_last_of('.');
	FileName = FilePath.substr(0, end);
	//获取文件头部信息
	FILE* fIn = fopen(FilePath.c_str(), "rb");
	if (fIn == nullptr)
	{
		cout << "压缩文件打开失败" << endl;
		return;
	}
	string FileSuffixName;
	GetHead(fIn, FileSuffixName);
	FileName += FileSuffixName;
	FileName += 'H'; //加H是为了和原文件区别
	FILE* fOut = fopen(FileName.c_str(), "wb");
	if (fOut == nullptr)
	{
		cout << "压缩文件写入失败" << endl;
	}
	//填充字符
	for (int i = 0; i < 256; i++)
	{
		info[i]._ch = i;
	}
	//重建Huffman树
	HuffmanTree<CharInfo> hT(info, 0);
	//编码转换为字节
	CodeToData(fIn, fOut, hT.GetRoot());

	fclose(fIn);
	fclose(fOut);
}

//获取编码
void FileCompressHuffman::GetCode(const HuffmanTreeNode<CharInfo>* root)
{
	if (root != nullptr)
	{
		GetCode(root->pLeft);
		GetCode(root->pRight);

		if (root->pLeft == nullptr && root->pRight == nullptr)
		{
			HuffmanTreeNode<CharInfo>* Parent = root->pParent;
			string code;
			const HuffmanTreeNode<CharInfo>* tmp = root;
			while (Parent != nullptr)
			{
				if (Parent->pLeft == tmp)
				{
					code += "0";
				}
				if (Parent->pRight == tmp)
				{
					code += "1";
				}
				tmp = Parent;
				Parent = Parent->pParent;
			}
			reverse(code.begin(), code.end());
			info[root->_weight._ch]._strCode = code;
		}
	}
}

//获取文件名
string FileCompressHuffman::GetFileName(string& FilePath)
{
	size_t begin = 0;
	size_t end = FilePath.find_last_of(".");
	return FilePath.substr(begin, end);
}

//获取文件信息
void FileCompressHuffman::GetFileInfo(FILE* fOut, string& FilePath)
{
	string  FileSuffixName;
	size_t begin = FilePath.find_last_of(".");
	FileSuffixName = FilePath.substr(begin);
	FileSuffixName += '\n';
	fputs(FileSuffixName.c_str(), fOut);
	int cnt = 0;  //总字符个数（不同的字符）
	
	unsigned char tmp[50];
	for (size_t i = 0; i < 256; i++)
	{
		if (info[i]._chCount != 0)
		{
			cnt++;
		}
	}
	sprintf((char *)tmp, "%d", cnt);
	string output;
	output = (char *)tmp;
	output += '\n';
	fputs(output.c_str(), fOut);
	char countStr[20];
	for (size_t i = 0; i < 256; i++)
	{
		if (info[i]._chCount != 0)
		{
			string buf;  //buf中存放  ，次数
			fputc(info[i]._ch, fOut);//必须先把ch放进去，如果把ch作为string的字符最后转换为C的字符，会导致'\0'没有处理
			buf = ',';
			_itoa(info[i]._chCount, countStr, 10);
			buf += countStr;
			fputs(buf.c_str(), fOut);
			fputc('\n', fOut);
		}
	}
}

//处理压缩后的数据
void FileCompressHuffman::CompressData(FILE *fIn, FILE *fOut)
{
	unsigned char ch = 0;
	unsigned char buf[1024];
	unsigned char out[1024];
	size_t out_idx = 0;
	size_t n = 0; //n表示每次读到的字节数
	size_t pos = 0; //表示当前一个字节有几个有效位
	size_t count = 0;
	do
	{
		size_t i = 0;
		memset(buf, '\0', 1024);
		n = fread(buf, 1, 1024, fIn);
		while (i < n)
		{
			string& code = info[buf[i++]]._strCode;
			size_t len = code.length();
			size_t i_len = 0;
			while (i_len < len)
			{
				for (; pos < 8 && i_len < len; pos++)
				{
					ch <<= 1;
					if (code[i_len++] == '1')
					{
						ch |= 1;
					}
				}
				//先放入out中，当out满了直接写入文件中
				if (pos == 8)
				{
					out[out_idx++] = ch;
					if (out_idx == 1024)
					{
						fwrite(out, 1, 1024, fOut);
						out_idx = 0;
					}
					pos = 0;
					ch = 0;
				}
			}
		}
	} while (n);

	if (pos < 8 && pos > 0) //表示没到8个bit位
	{
		size_t j = 0;
		while (j++ < 8 - pos)
		{
			ch <<= 1;
		}
		out[out_idx++] = ch;
	}
	if (out_idx > 0)
		fwrite(out, 1, out_idx, fOut);
}
//获取头部信息
void FileCompressHuffman::GetHead(FILE *fIn, string& FileSuffixName)
{
	//获取文件后缀
	unsigned char buf[256];
	GetLine(fIn, buf);
	FileSuffixName = (char *)buf;
	//获取行号
	GetLine(fIn, buf);
	int line = atoi((char *)buf);
	//获取每个字符及其对应出现的次数
	while (line--)
	{
		GetLine(fIn, buf);
		info[*buf]._chCount = atoi((char *)(buf + 2));
	}
}
//获取每一行的信息
void FileCompressHuffman::GetLine(FILE *fIn, unsigned char* buf)
{
	int index = 0;
	while (fread(buf + index, 1, 1, fIn))
	{
		if (index != 0 && buf[index] == '\n')
			break;
		index++;
	}
	buf[index] = '\0';
}
//将编码转化为元数据
void FileCompressHuffman::CodeToData(FILE *fIn, FILE *fOut, HuffmanTreeNode<CharInfo>* root)
{
	assert(root);
	unsigned char readBuf[1024];
	unsigned char writeBuf[1024];
	memset(writeBuf, '\0', 1024);
	size_t n = 0;
	size_t w_idx = 0;
	long long fileLen = root->_weight._chCount;
	HuffmanTreeNode<CharInfo>* pRoot = root;
	while (!feof(fIn))
	{
		do
		{
			size_t r_idx = 0;
			memset(readBuf, '\0', 1024);
			n = fread(readBuf, 1, 1024, fIn);
			for (; r_idx < n; r_idx++)
			{
				//转换每个字节
				unsigned char ch = readBuf[r_idx];
				for (size_t pos = 0; pos < 8; pos++)
				{
					if ((ch & 0x80) == 0x80)
					{
						pRoot = pRoot->pRight;
					}
					else
					{
						pRoot = pRoot->pLeft;
					}
					if (pRoot->pLeft == nullptr && pRoot->pRight == nullptr)
					{
						writeBuf[w_idx++] = pRoot->_weight._ch;
						if (w_idx == 1024)
						{
							fwrite(writeBuf, 1, 1024, fOut);
							memset(writeBuf, '\0', 1024);
							w_idx = 0;
						}
						pRoot = root;
						fileLen--;
					}
					if (fileLen == 0)
					{
						break;
					}
					ch <<= 1;
				}
			}
		} while (n);
		if (w_idx < 1024 && w_idx > 0)
		{
			fwrite(writeBuf, 1, w_idx, fOut);
		}
	}
}
