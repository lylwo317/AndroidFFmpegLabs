#include "CH2645PacketParser.h"
#include <iostream>
#include <cstring>
#include "h2645_paramter.h"
//#include "CGWEWindowsApis.h"

CH2645PacketParser::CH2645PacketParser()
{
}

CH2645PacketParser::~CH2645PacketParser()
{
}

static int FindStartCode2(unsigned char *Buf) {
	unsigned int* pCode = (unsigned int*)Buf;
	return (*pCode<<8) == 16777216;
	//if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 1) return 0; //0x000001?
	//else
	//{
	//	return 1;
	//}
}

static int FindStartCode3(unsigned char *Buf) {
	unsigned int* pCode = (unsigned int*)Buf;
	return *pCode == 16777216;
	//if (Buf[0] != 0 || Buf[1] != 0 || Buf[2] != 0 || Buf[3] != 1)
	//	return 0;//0x00000001?
	//else
	//{
	//	return 1;
	//}
}
/*
int GetAnnexbNALU(unsigned char* pData, int iLength, NALU_t *nalu, int& iNuluType, int& iVideoType)
{
	int info2 = 0, info3 = 0;
	int pos = 0;
	int StartCodeFound, rewind;
	unsigned char *Buf = pData;
	int m_start_code_size = 0;
	int iTempLen = iLength;

	if (nalu)
	{
		nalu->startcodeprefix_len = 3;
	}

	if (iLength < 3)
	{
		return 0;
	}
	//int d = get_one_Byte(Buf,m_h264_buffer);
	info2 = FindStartCode2(Buf);
	if (info2 != 1) {
		if (iLength < 4)
		{
			return 0;
		}
		info3 = FindStartCode3(Buf);
		if (info3 != 1) {
			return -1;
		}
		else {
			pos = 4;
			if (nalu)
				nalu->startcodeprefix_len = 4;
		}
	}
	else {
		if (nalu)
			nalu->startcodeprefix_len = 3;
		pos = 3;
	}
	StartCodeFound = 0;
	info2 = 0;
	info3 = 0;
	m_start_code_size = pos;
	iTempLen -= m_start_code_size;
	while (!StartCodeFound) {
		if (0 == iTempLen) {
			iNuluType = Buf[m_start_code_size] & 0x1f;// 5 bit
			if (iNuluType >= 0 && iNuluType <= 31)		// h264 nalu type
			{
				if (nalu)
				{
					nalu->len = (pos)-nalu->startcodeprefix_len;
					nalu->buf = new char[nalu->len * sizeof(char)];
					memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
					nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
					nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
					nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;// 5 bit
				}
				iVideoType = 0;
			}
			else
			{
				iNuluType = (Buf[m_start_code_size] & 0x7E) >> 1;// 6 bit
				if (iNuluType >=0 && iNuluType <= 63)	// h265 nalu type
				{
					if (nalu)
					{
						nalu->len = (pos)-nalu->startcodeprefix_len;
						nalu->buf = new char[nalu->len * sizeof(char)];
						memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
						nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
						nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
						nalu->nal_unit_type = (nalu->buf[0] & 0x7E) >> 1;// 6 bit
					}
					iVideoType = 1;
				}
				else
				{
					iVideoType = -1;
					return -1; // not h264 or h265
				}
			}
			return pos;
		}
		if (iTempLen > 0)
		{
			pos++;
			--iTempLen;
		}
		info3 = FindStartCode3(&Buf[pos - 4]);
		if (info3 != 1)
			info2 = FindStartCode2(&Buf[pos - 3]);
		StartCodeFound = (info2 == 1 || info3 == 1);
	}

	rewind = (info3 == 1) ? -4 : -3;

	iNuluType = Buf[m_start_code_size] & 0x1f;// 5 bit
	if (iNuluType >= 0 && iNuluType <= 31)		// h264 nalu type
	{
		if (nalu)
		{
			nalu->len = (pos)-nalu->startcodeprefix_len;
			nalu->buf = new char[nalu->len * sizeof(char)];
			memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
			nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
			nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
			nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;// 5 bit
		}
		iVideoType = 0;
	}
	else
	{
		iNuluType = (Buf[m_start_code_size] & 0x7E) >> 1;// 6 bit
		if (iNuluType >= 0 && iNuluType <= 63)	// h265 nalu type
		{
			if (nalu)
			{
				nalu->len = (pos)-nalu->startcodeprefix_len;
				nalu->buf = new char[nalu->len * sizeof(char)];
				memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
				nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
				nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
				nalu->nal_unit_type = (nalu->buf[0] & 0x7E) >> 1;// 6 bit
			}
			iVideoType = 1;
		}
		else
		{
			iVideoType = -1;
			return -1; // not h264 or h265
		}
	}

	return (pos + rewind);//���ص�������NALU�ĳ��ȣ�nalu.len+StartCodeSizeLen��
}
*/
int GetH264NALU(unsigned char* pData, int iLength, NALU_t *nalu, int& iNuluType)
{
	unsigned int* pCode;
	int info2 = 0, info3 = 0;
	int pos = 0;
	int StartCodeFound, rewind;
	unsigned char *Buf = pData;
	int m_start_code_size = 0;
	int iTempLen = iLength;

	if (nalu)
	{
		nalu->startcodeprefix_len = 3;
	}

	if (iLength < 3)
	{
		return 0;
	}
	//int d = get_one_Byte(Buf,m_h264_buffer);
	info2 = FindStartCode2(Buf);
	if (info2 != 1) {
		if (iLength < 4)
		{
			return 0;
		}
		info3 = FindStartCode3(Buf);
		if (info3 != 1) {
			return -1;
		}
		else {
			pos = 4;
			if(nalu)
				nalu->startcodeprefix_len = 4;
		}
	}
	else {
		if (nalu)
			nalu->startcodeprefix_len = 3;
		pos = 3;
	}
	StartCodeFound = 0;
	info2 = 0;
	info3 = 0;
	m_start_code_size = pos;
	iTempLen -= m_start_code_size;
	while (!StartCodeFound) {
		if (0 == iTempLen) {
			iNuluType = Buf[m_start_code_size] & 0x1f;// 5 bit
			if (nalu)
			{
				nalu->len = (pos)-nalu->startcodeprefix_len;
				nalu->buf = new char[nalu->len * sizeof(char)];
				memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
				nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
				nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
				nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;// 5 bit
			}
			return pos;
		}
		if (iTempLen > 0)
		{
			pos++;
			--iTempLen;
		}
		pCode = (unsigned int*)(&Buf[pos - 4]);
		info3 = (*pCode == 16777216);// FindStartCode3(&Buf[pos - 4]);//
		if (info3 != 1)
		{
			pCode = (unsigned int*)(&Buf[pos - 3]);
			info2 = ((*pCode << 8) == 16777216);// FindStartCode2(&Buf[pos - 3]);//
		}
		StartCodeFound = (info2 == 1 || info3 == 1);
	}

	rewind = (info3 == 1) ? -4 : -3;

	iNuluType = Buf[m_start_code_size] & 0x1f;// 5 bit
	if (nalu)
	{
		nalu->len = (pos)-nalu->startcodeprefix_len;
		nalu->buf = new char[nalu->len * sizeof(char)];
		memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
		nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
		nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
		nalu->nal_unit_type = (nalu->buf[0]) & 0x1f;// 5 bit
	}

	return (pos + rewind);//���ص�������NALU�ĳ��ȣ�nalu.len+StartCodeSizeLen��
}

int GetH265NALU(unsigned char* pData, int iLength, NALU_t *nalu, int& iNuluType)
{
	unsigned int* pCode;
	int info2 = 0, info3 = 0;
	int pos = 0;
	int StartCodeFound, rewind;
	unsigned char *Buf = pData;
	int m_start_code_size = 0;
	int iTempLen = iLength;

	if (nalu)
	{
		nalu->startcodeprefix_len = 3;
	}

	if (iLength < 3)
	{
		return 0;
	}
	//int d = get_one_Byte(Buf,m_h264_buffer);
	info2 = FindStartCode2(Buf);
	if (info2 != 1) {
		if (iLength < 4)
		{
			return 0;
		}
		info3 = FindStartCode3(Buf);
		if (info3 != 1) {
			return -1;
		}
		else {
			pos = 4;
			if (nalu)
				nalu->startcodeprefix_len = 4;
		}
	}
	else {
		if (nalu)
			nalu->startcodeprefix_len = 3;
		pos = 3;
	}
	StartCodeFound = 0;
	info2 = 0;
	info3 = 0;
	m_start_code_size = pos;
	iTempLen -= m_start_code_size;
	while (!StartCodeFound) {
		if (0 == iTempLen) {
			iNuluType = (Buf[m_start_code_size] & 0x7E) >> 1;// 6 bit
			if (nalu)
			{
				nalu->len = (pos)-nalu->startcodeprefix_len;
				nalu->buf = new char[nalu->len * sizeof(char)];
				memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
				nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
				nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
				nalu->nal_unit_type = (nalu->buf[0] & 0x7E) >> 1;// 6 bit
			}
			return pos;
		}
		if (iTempLen > 0)
		{
			pos++;
			--iTempLen;
		}
		pCode = (unsigned int*)(&Buf[pos - 4]);
		info3 = (*pCode == 16777216);// FindStartCode3(&Buf[pos - 4]);//
		if (info3 != 1)
		{
			pCode = (unsigned int*)(&Buf[pos - 3]);
			info2 = ((*pCode << 8) == 16777216);// FindStartCode2(&Buf[pos - 3]);//
		}
		StartCodeFound = (info2 == 1 || info3 == 1);
	}

	rewind = (info3 == 1) ? -4 : -3;

	iNuluType = (Buf[m_start_code_size] & 0x7E) >> 1;// 6 bit
	if (nalu)
	{
		nalu->len = (pos)-nalu->startcodeprefix_len;
		nalu->buf = new char[nalu->len * sizeof(char)];
		memcpy(nalu->buf, &Buf[nalu->startcodeprefix_len], nalu->len);
		nalu->forbidden_bit = nalu->buf[0] & 0x80; //1 bit
		nalu->nal_reference_idc = nalu->buf[0] & 0x60; // 2 bit
		nalu->nal_unit_type = (nalu->buf[0] & 0x7E) >> 1;// 6 bit
	}

	return (pos + rewind);//���ص�������NALU�ĳ��ȣ�nalu.len+StartCodeSizeLen��
}

int CH2645PacketParser::ParserVideoType(unsigned char * pData, int iLength)
{
	int iVideoType = -1;
	iVideoType = IsH2645KeyFrame(pData, iLength);
	return iVideoType;
}

int CH2645PacketParser::IsH2645KeyFrame(unsigned char * pData, int iLength)
{
	int iRet = -1;
	if (IsH264KeyFrame(pData, iLength))
	{
		iRet = 0;
	}
	else if (IsH265KeyFrame(pData, iLength))
	{
		iRet = 1;
	}
	return iRet;
}

bool CH2645PacketParser::IsH264KeyFrame(unsigned char * pData, int iLength)
{
	bool bRet = false;
	int iNaluType = -1;
	int iNaluLength = 0;
	int iLeftLength = iLength;
	while (iLeftLength > 0)
	{
		iNaluLength = ::GetH264NALU(pData + (iLength - iLeftLength), iLeftLength, NULL, iNaluType);
		if (iNaluLength <= 0)
		{
			break;
		}
		iLeftLength -= iNaluLength;
		if (1 == iNaluType)	// h264 NALU_TYPE_SLICE
		{
			break;
		}
		if (5 == iNaluType || 7 == iNaluType)	// NALU_TYPE_IDR || NALU_TYPE_SPS
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}

bool CH2645PacketParser::IsH265KeyFrame(unsigned char * pData, int iLength)
{
	bool bRet = false;
	int iNaluType = -1;
	int iNaluLength = 0;
	int iLeftLength = iLength;
	while (iLeftLength > 0)
	{
		iNaluLength = ::GetH265NALU(pData + (iLength - iLeftLength), iLeftLength, NULL, iNaluType);
		if (iNaluLength <= 0)
		{
			break;
		}
		iLeftLength -= iNaluLength;
		if (iNaluType >= 0 && iNaluType <= 9)	// h265 NALU_TYPE_SLICE
		{
			break;
		}
		if (33 == iNaluType)	// H265_NAL_UNIT_SPS
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}

int CH2645PacketParser::GetH264NALU(unsigned char * pData, int iLength)
{
	int iType = 0;
	::GetH264NALU(pData, iLength, NULL, iType);
	return iType;
}

int CH2645PacketParser::GetH264NALU(unsigned char * pData, int iLength, NALU_t * nalu)
{
	int iType = 0;
	return ::GetH264NALU(pData, iLength, nalu, iType);
}

int CH2645PacketParser::GetH265NALU(unsigned char * pData, int iLength)
{
	int iType = 0;
	::GetH265NALU(pData, iLength, NULL, iType);
	return iType;
}

int CH2645PacketParser::GetH265NALU(unsigned char * pData, int iLength, NALU_t * nalu)
{
	int iType = 0;
	return ::GetH265NALU(pData, iLength, nalu, iType);
}
