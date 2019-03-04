#define _CRT_SECURE_NO_WARNINGS
#include "lz77.h"
#include <iostream>
#include <assert.h>
using namespace std;

const USH MIN_LOOKAHEAD = MAX_MATCH + 1;
const USH MAX_DIST = WSIZE - MIN_LOOKAHEAD;

LZ77::LZ77()
	:_start(0)
	,_pWin(new UCH[WSIZE * 2])
	,_lookAhead(0)
	,_ht(WSIZE)
{}

LZ77::~LZ77()
{
	if (_pWin)
	{
		delete[] _pWin;
	}
}

void LZ77::CompressFile(const std::string& filePath)
{
	FILE* fIn = fopen(filePath.c_str(), "rb");
	if (nullptr == fIn)
	{
		cout << "���ļ�ʧ��" << endl;
		return;
	}

	fseek(fIn, 0, SEEK_END);
	ULL fileSize = ftell(fIn);
	fseek(fIn, 0, SEEK_SET);

	if (fileSize <= MIN_MATCH)
	{
		fclose(fIn);
		return;
	}
	//��ȡһ��������������
	//lookAhead:��ѹ�����ݵĸ���
	_lookAhead = fread(_pWin, 1, WSIZE * 2, fIn);
	USH hashAddr = 0;
	//��ϣ��ַ�Ǹ��������ַ�����ģ��Ȱ�ǰ�����������
	//������ֻ��Ҫ�ٴ�һ���ַ��Ϳ�����
	for (size_t i = 0; i < MIN_MATCH - 1; ++i)
		_ht.HashFunc(hashAddr, _pWin[i]);

	//���ڱ���ѹ������
	FILE* fOutD = fopen("1.zipL", "wb");
	if (nullptr == fOutD)
	{
		cout << "д��ѹ���ļ�ʧ��" << endl;
		return;
	}

	//�����׺
	string postFix = filePath.substr(filePath.rfind('.'));
	postFix += '\n';
	fwrite(postFix.c_str(), 1, postFix.size(), fOutD);
	//���ڱ���ѹ�����
	FILE *fOutF = fopen("2.zipL", "wb");
	if (nullptr == fOutF)
	{
		cout << "д�����ļ�ʧ��" << endl;
		return;
	}

	USH matchHead = 0;
	UCH chFlag = 0;
	UCH bitCount = 0;
	while (_lookAhead)
	{
		_ht.Insert(hashAddr, _pWin[_start + 2], _start, matchHead);

		USH curMatchDist = 0;
		UCH curMatchLen =0;
		if (matchHead && _lookAhead > MIN_MATCH)
		{
			curMatchLen = LongMatch(matchHead, curMatchDist);
		}

		if (curMatchLen < MIN_MATCH)
		{
			//û��ƥ��,дԴ�ַ�
			fputc(_pWin[_start], fOutD);
			++_start;
			--_lookAhead;
			//д���,Դ�ַ���0��ʾ���������1��ʾ
			WriteFlag(fOutF, chFlag, bitCount, false);
		}
		else
		{
			//д�볤�Ⱦ����
			//fputc(curMatchDist, fOutD);
			fwrite(&curMatchDist, 2, 1, fOutD);
			fputc(curMatchLen, fOutD);
			//д���
			WriteFlag(fOutF, chFlag, bitCount, true);
			//����lookAhead
			_lookAhead -= curMatchLen;
			//���¹�ϣ��
			while (curMatchLen--)
			{
				++_start;
				_ht.Insert(hashAddr, _pWin[_start + 2], _start, matchHead);
			}
		}
		//���������ݲ�������ʼ�������
		if (_lookAhead <= MIN_LOOKAHEAD)
			FillWindow(fIn);
	}
	//���һ�����ݲ���8��bitλ���������⴦��
	if (bitCount > 0 && bitCount < 8)
	{
		chFlag <<= (8 - bitCount);
		fputc(chFlag, fOutF);
	}
	fclose(fOutF);
	fclose(fIn);

	FILE* fInF = fopen("2.zipL", "rb");
	assert(fInF);
	UCH* pReadBuff = new UCH[1024];
	size_t flagSize = 0;
	while (true)
	{
		size_t rdSize = fread(pReadBuff, 1, 1024, fInF);
		if (rdSize == 0)
			break;
		flagSize += rdSize;
		fwrite(pReadBuff, 1, rdSize, fOutD);
	}
	fclose(fInF);

	fwrite(&fileSize, sizeof(fileSize), 1, fOutD);
	fwrite(&flagSize, sizeof(flagSize), 1, fOutD);

	fclose(fOutD);

	remove("2.zipL");
}
//�ƥ��
UCH LZ77::LongMatch(USH matchHead, USH& curMatchDist)
{
	UCH MaxLength = 0;
	USH pos = 0;
	int MatchchainLen = 255;
	//������ǰ��������Զ����
	USH limit = _start > MAX_DIST ? _start - MAX_DIST : 0;  
	do
	{
		USH CurMatchLen = 0;
		UCH* pStart = _pWin + _start;
		UCH* pEnd = pStart + MAX_MATCH;
		//�ڲ��һ���������ƥ�䴮����ʼλ��
		UCH* pCurStart = _pWin + matchHead;
		
		while (pStart < pEnd && *pStart == *pCurStart)
		{
			pStart++;
			pCurStart++;
			CurMatchLen++;
		}
		if (CurMatchLen > MaxLength) 
		{
			MaxLength = CurMatchLen;
			pos = matchHead;
		}

	} while ((matchHead = _ht.GetNextPos(matchHead)) > limit
		&& MatchchainLen--);

	curMatchDist = _start - pos;
	return MaxLength;
}

void LZ77::WriteFlag(FILE* fOutF, UCH& chFlag, UCH& bitCount, bool IsChar)
{
	chFlag <<= 1;
	//�ж��Ƿ���Դ�ַ�
	if (IsChar)
		chFlag |= 1;
	bitCount++;
	if (8 == bitCount)
	{
		fputc(chFlag, fOutF);
		bitCount = 0;
		chFlag = 0;
	}
}

void LZ77::FillWindow(FILE* fIn)
{
	//���Ҵ��ڵ����ݰ��Ƶ��󴰿�
	if (_start >= WSIZE + MAX_DIST)
	{
		memcpy(_pWin, _pWin + WSIZE, WSIZE);
		memset(_pWin + WSIZE, 0, WSIZE);
		_start -= WSIZE;
		_ht.UpdateHashTable();
	}

	//��ʼ�������
	if (!feof(fIn))
	{
		_lookAhead += fread(_pWin + WSIZE, 1, WSIZE, fIn);
	}

}

void LZ77::UnCompressFile(const std::string& filePath)
{
	string strPostFix = filePath.substr(filePath.rfind('.'));
	if (strPostFix != ".zipL")
	{
		cout << "ѹ���ļ���ʽ��֧��" << endl;
		return;
	}

	FILE* fInD = fopen(filePath.c_str(), "rb");
	if (fInD == nullptr)
	{
		cout << "��ѹ���ļ�ʧ��" << endl;
		return;
	}

	//��ȡ��Ǵ�С
	fseek(fInD, -4, SEEK_END);
	size_t flagSize = 0;
	fread(&flagSize, 4, 1, fInD);
	//��ȡԴ�ļ���С
	fseek(fInD, -12, SEEK_END);
	ULL fileSize = 0;
	fread(&fileSize, 8, 1, fInD);
	fseek(fInD, 0, SEEK_SET);

	FILE* fInF = fopen(filePath.c_str(), "rb");
	if (fInF == nullptr)
	{
		cout << "ѹ���ļ���ʧ��" << endl;
		return;
	}
	int offSet = 0 - (sizeof(fileSize) + sizeof(flagSize) + flagSize);
	fseek(fInF, offSet, SEEK_END);
	//��ȡԴ�ļ���׺
	char filePostFix[20];
	GetLine(fInD, filePostFix);
	//��ѹ������ļ���
	string fileName = "2";
	fileName += (string)filePostFix;

	FILE* fOut = fopen(fileName.c_str(), "wb");
	if (fOut == nullptr)
	{
		cout << "��ѹ���ļ���ʧ��" << endl;
		return;
	}

	FILE* fWin = fopen(fileName.c_str(), "rb");
	if (fOut == nullptr)
	{
		cout << "��ѹ���ļ���ʧ��" << endl;
		return;
	}

	UCH chFlag = 0;
	UCH bitCount = 0;

	while (fileSize)
	{
		if (bitCount == 0)
			fread(&chFlag, 1, 1, fInF);
		bool isChar = IsChar(chFlag, bitCount);
		//��ʾ0��ǣ�ֱ��д��
		if (isChar == true)
		{
			UCH ch = fgetc(fInD);
			fputc(ch, fOut);
			fileSize -= 1;
		}
		//1��ǣ���ʾ���Ⱦ����
		else
		{
			USH dist = 0;
			fread(&dist, 2, 1, fInD);
			//UCH dist = fgetc(fInD);
			UCH len = fgetc(fInD);
			fflush(fOut);
			//�ӽ�ѹ���ļ�ĩβǰ��dist���ֽڿ�ʼ��
			fseek(fWin, 0 - dist, SEEK_END);
			fileSize -= len;
			while (len--)
			{
				UCH ch = fgetc(fWin);
				fputc(ch, fOut);
				fflush(fOut);
			}
		}
	}
	fclose(fInD);
	fclose(fInF);
	fclose(fOut);
	fclose(fWin);
}

void LZ77::GetLine(FILE* fIn, char* filePostFix)
{
	int index = 0;
	while (true)
	{
		fread(filePostFix + index, 1, 1, fIn);
		if (index != 0 && *(filePostFix + index) == '\n')
			break;
		index++;
	}
	*(filePostFix + index) = '\0';
}

bool LZ77::IsChar(UCH& chFlag, UCH& bitCount)
{
	bool tmp;
	if ((chFlag & 0x80) == 0x80)
		tmp = false;
	else
		tmp = true;
	chFlag <<= 1;
	bitCount++;
	if (bitCount == 8)
	{
		chFlag = 0;
		bitCount = 0;
	}
	return tmp;
}