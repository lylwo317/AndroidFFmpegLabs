#ifndef _C_H2645_PACKET_PARSER_H_
#define _C_H2645_PACKET_PARSER_H_

struct NALU_t;
class CH2645PacketParser
{
public:
	CH2645PacketParser();
	~CH2645PacketParser();
	// return: -1, unknown, 0 h264, 1 h265
	static int ParserVideoType(unsigned char* pData, int iLength);
	// return: -1, unknown, 0 h264 key frame, 1 h265 key frame
	static int IsH2645KeyFrame(unsigned char* pData, int iLength);
	static bool IsH264KeyFrame(unsigned char* pData, int iLength);
	static bool IsH265KeyFrame(unsigned char* pData, int iLength);
	// return nalu type
	static int GetH264NALU(unsigned char* pData, int iLength);
	// return nalu length: nalu->startcodeprefix_len+nalu->len
	static int GetH264NALU(unsigned char* pData, int iLength, NALU_t *nalu);
	// return nalu type
	static int GetH265NALU(unsigned char* pData, int iLength);
	// return nalu length: nalu->startcodeprefix_len+nalu->len
	static int GetH265NALU(unsigned char* pData, int iLength, NALU_t *nalu);
};

#endif // _C_H2645_PACKET_PARSER_H_