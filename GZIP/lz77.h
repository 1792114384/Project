#pragma once
#include "HashTable.hpp"

class LZ77
{
public:
	LZ77();
	~LZ77();

	void CompressFile(const std::string& filePath);
	void UnCompressFile(const std::string& filePath);
	UCH LongMatch(USH matchHead, USH& curMatchDist);
	void WriteFlag(FILE* fOutF, UCH& chFlag, UCH& bitCount, bool IsChar, unsigned char buf_flag[], int& pos_flag);
	void GetLine(FILE* fIn, char* filePostFix);
	bool IsChar(UCH& chFlag, UCH& bitCount);
	void FillWindow(FILE* fIn);
private:
	USH _start;
	UCH* _pWin;
	size_t _lookAhead;
	HashTable _ht;
};
