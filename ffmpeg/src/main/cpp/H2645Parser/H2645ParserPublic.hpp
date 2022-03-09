#ifndef _H_2645_PARSER_PUBLIC_H_
#define _H_2645_PARSER_PUBLIC_H_

#include "H264Parser.h"
#include "h265Parser.h"
enum IType{
	H264 = 0,
	H265 = 1
};

struct SH2645ParserInfo;
static SH2645ParserInfo* ParseH2645(IType iType, void* pData, int iLength);
static int IsH2645KeyFrame(void* pData, int iLength);

struct SH2645ParserInfo 
{
	SH2645ParserInfo()
		: m_pExtraData(NULL)
		, m_iExtraDataLength(0)
		, m_pSps(NULL)
		, m_iSpsLength(0)
		, m_pVps(NULL)
		, m_iVpsLength(0)
		, m_pPps(NULL)
		, m_iPpsLength(0)
		, m_iWidth(0)
		, m_iHeight(0)
		, m_iYUVFormat(-1)
	{

	}
	~SH2645ParserInfo()
	{
		if (m_pExtraData)
		{
			delete[]m_pExtraData;
		}
		if (m_pSps)
		{
			delete[]m_pSps;
		}
		if (m_pVps)
		{
			delete[]m_pVps;
		}
		if (m_pPps)
		{
			delete[]m_pPps;
		}
	}
	void AddExtraData(void* pData, int iDataLength)
	{
		if (NULL == pData || iDataLength <= 0)
		{
			return;
		}
		if (m_pExtraData)
		{
			delete[] m_pExtraData;
			m_pExtraData = NULL;
			m_iExtraDataLength = 0;
		}
		m_pExtraData = new char[iDataLength];
		memcpy(m_pExtraData, pData, iDataLength);
		m_iExtraDataLength = iDataLength;
	}
	void AddSps(void* pData, int iDataLength)
	{
		if (NULL == pData || iDataLength <= 0)
		{
			return;
		}
		if (m_pSps)
		{
			delete[] m_pSps;
			m_pSps = NULL;
			m_iSpsLength = 0;
		}
		m_pSps = new char[iDataLength];
		memcpy(m_pSps, pData, iDataLength);
		m_iSpsLength = iDataLength;
	}
	void AddVps(void* pData, int iDataLength)
	{
		if (NULL == pData || iDataLength <= 0)
		{
			return;
		}
		if (m_pVps)
		{
			delete[] m_pVps;
			m_pVps = NULL;
			m_iVpsLength = 0;
		}
		m_pVps = new char[iDataLength];
		memcpy(m_pVps, pData, iDataLength);
		m_iVpsLength = iDataLength;
	}
	void AddPps(void* pData, int iDataLength)
	{
		if (NULL == pData || iDataLength <= 0)
		{
			return;
		}
		if (m_pPps)
		{
			delete[] m_pPps;
			m_pPps = NULL;
			m_iPpsLength = 0;
		}
		m_pPps = new char[iDataLength];
		memcpy(m_pPps, pData, iDataLength);
		m_iPpsLength = iDataLength;
	}
	int m_iWidth;
	int m_iHeight;
	int m_iYUVFormat;
	void* m_pExtraData;
	int m_iExtraDataLength;
	void* m_pSps;
	int m_iSpsLength;
	void* m_pVps;
	int m_iVpsLength;
	void* m_pPps;
	int m_iPpsLength;
};
// iType: 0 h264, 1 h265
static SH2645ParserInfo* ParseH2645(IType iType, void* pData, int iLength)
{
	unsigned char* pTemp = new unsigned char[iLength];
	memcpy(pTemp, pData, iLength);
	SH2645ParserInfo* pRet = new SH2645ParserInfo;
	do
	{
		if (0 == iType)
		{
			h264Parser ch264Parser;
			ch264Parser.h264_parser((unsigned char*)pTemp, iLength, pRet);
			if (1 != ch264Parser.m_isSps)
			{
				delete pRet;
				pRet = NULL;
				break;
			}
		}
		else if (1 == iType)
		{
			h265Parser ch265Parser;
			ch265Parser.h265_parser((unsigned char*)pTemp, iLength, pRet);
			if (1 != ch265Parser.m_isSps)
			{
				delete pRet;
				pRet = NULL;
				break;
			}
		}
	} while (false);
	delete[] pTemp;
	return pRet;
}

static int IsH2645KeyFrame(void* pData, int iLength)
{
	unsigned char* pTemp = new unsigned char[iLength];
	memcpy(pTemp, pData, iLength);
	int iRet = -1;
	do
	{
		//if (0 == iType)
		{
			h264Parser ch264Parser;
			ch264Parser.h264_parser((unsigned char*)pTemp, iLength, NULL);
			if (1 == ch264Parser.m_isSps)
			{
				iRet = 0;
				break;
			}
		}
		//else if (1 == iType)
		{
			memcpy(pTemp, pData, iLength);
			h265Parser ch265Parser;
			ch265Parser.h265_parser((unsigned char*)pTemp, iLength, NULL);
			if (1 == ch265Parser.m_isSps)
			{
				iRet = 1;
				break;
			}
		}
	} while (false);
	delete[] pTemp;
	return iRet;
}

#endif