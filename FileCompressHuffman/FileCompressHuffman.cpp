#define _CRT_SECURE_NO_WARNINGS
#include "FileCompressHuffman.h"
#include "Huffman.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

//ѹ���ļ�
void FileCompressHuffman::CompressFile(string& FilePath)
{
	//�Զ����Ƶ��ļ���
	FILE* FIn = fopen(FilePath.c_str(), "rb");
	if (nullptr == FIn)
	{
		cout << "���ļ�ʧ��" << endl;
		return;
	}
	//����ַ�
	for (int i = 0; i < 256; ++i)
	{
		info[i]._ch = i;
	}
	//����ַ����ֵĴ���
	unsigned char buf[1024];
	size_t n;
	do
	{
		n = fread(buf, 1, 1024, FIn);
		for (size_t i = 0; i < n; i++)
			info[buf[i]]._chCount++;
	} while (n > 0);

	//����Huffman��
	HuffmanTree<CharInfo> hT(info, 0);
	//��ȡ����
	GetCode(hT.GetRoot());
	//��ȡ�ļ���
	string FileName = GetFileName(FilePath);
	FileName += ".Hzip";

	FILE* fOut = fopen(FileName.c_str(), "wb");
	if (fOut == nullptr)
	{
		cout << "ѹ���ļ�ʧ��" << endl;
		return;
	}
	//����ļ���Ϣ
	GetFileInfo(fOut, FilePath);
	//��FIn����ָ���ļ�ͷ
	fseek(FIn, 0, SEEK_SET);
	//����ļ�ѹ���������
	CompressData(FIn, fOut);

	fclose(FIn);
	fclose(fOut);
}

//��ѹ���ļ�
void FileCompressHuffman::UnCompressFile(string& FilePath)
{
	//��ȡ�ļ���
	string FileName;
	int end = FilePath.find_last_of('.');
	FileName = FilePath.substr(0, end);
	//��ȡ�ļ�ͷ����Ϣ
	FILE* fIn = fopen(FilePath.c_str(), "rb");
	if (fIn == nullptr)
	{
		cout << "ѹ���ļ���ʧ��" << endl;
		return;
	}
	string FileSuffixName;
	GetHead(fIn, FileSuffixName);
	FileName += FileSuffixName;
	FileName += 'H'; //��H��Ϊ�˺�ԭ�ļ�����
	FILE* fOut = fopen(FileName.c_str(), "wb");
	if (fOut == nullptr)
	{
		cout << "ѹ���ļ�д��ʧ��" << endl;
	}
	//����ַ�
	for (int i = 0; i < 256; i++)
	{
		info[i]._ch = i;
	}
	//�ؽ�Huffman��
	HuffmanTree<CharInfo> hT(info, 0);
	//����ת��Ϊ�ֽ�
	CodeToData(fIn, fOut, hT.GetRoot());

	fclose(fIn);
	fclose(fOut);
}

//��ȡ����
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

//��ȡ�ļ���
string FileCompressHuffman::GetFileName(string& FilePath)
{
	size_t begin = 0;
	size_t end = FilePath.find_last_of(".");
	return FilePath.substr(begin, end);
}

//��ȡ�ļ���Ϣ
void FileCompressHuffman::GetFileInfo(FILE* fOut, string& FilePath)
{
	string  FileSuffixName;
	size_t begin = FilePath.find_last_of(".");
	FileSuffixName = FilePath.substr(begin);
	FileSuffixName += '\n';
	fputs(FileSuffixName.c_str(), fOut);
	int cnt = 0;  //���ַ���������ͬ���ַ���
	
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
			string buf;  //buf�д��  ������
			fputc(info[i]._ch, fOut);//�����Ȱ�ch�Ž�ȥ�������ch��Ϊstring���ַ����ת��ΪC���ַ����ᵼ��'\0'û�д���
			buf = ',';
			_itoa(info[i]._chCount, countStr, 10);
			buf += countStr;
			fputs(buf.c_str(), fOut);
			fputc('\n', fOut);
		}
	}
}

//����ѹ���������
void FileCompressHuffman::CompressData(FILE *fIn, FILE *fOut)
{
	unsigned char ch = 0;
	unsigned char buf[1024];
	unsigned char out[1024];
	size_t out_idx = 0;
	size_t n = 0; //n��ʾÿ�ζ������ֽ���
	size_t pos = 0; //��ʾ��ǰһ���ֽ��м�����Чλ
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
				//�ȷ���out�У���out����ֱ��д���ļ���
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

	if (pos < 8 && pos > 0) //��ʾû��8��bitλ
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
//��ȡͷ����Ϣ
void FileCompressHuffman::GetHead(FILE *fIn, string& FileSuffixName)
{
	//��ȡ�ļ���׺
	unsigned char buf[256];
	GetLine(fIn, buf);
	FileSuffixName = (char *)buf;
	//��ȡ�к�
	GetLine(fIn, buf);
	int line = atoi((char *)buf);
	//��ȡÿ���ַ������Ӧ���ֵĴ���
	while (line--)
	{
		GetLine(fIn, buf);
		info[*buf]._chCount = atoi((char *)(buf + 2));
	}
}
//��ȡÿһ�е���Ϣ
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
//������ת��ΪԪ����
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
				//ת��ÿ���ֽ�
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
