#include <string.h>

#include "tea.h"

/*******************************加密解密函数************************************/
// 加密核心函数
// v为需要加密的数据
// k为加密的密钥
// round为加密的轮数，和解密相对应
void tea_encode(int32_t * v, const int32_t * k, int round)
{
	uint32_t y = 0, z = 0, sum = 0;
	uint32_t delta = 0x9e3779b9;
	// uint8_t n = 2, *DataP1;
	uint8_t n = round, *DataP1;
	DataP1 = (uint8_t *)v;
	y = *DataP1;
	DataP1++;
	y = y + ((uint32_t)*DataP1 << 8);
	DataP1++;
	y = y + ((uint32_t)*DataP1 << 16);
	DataP1++;
	y = y + ((uint32_t)*DataP1 << 24);

	DataP1++;
	z = *DataP1;
	DataP1++;
	z = z + ((uint32_t)*DataP1 << 8);
	DataP1++;
	z = z + ((uint32_t)*DataP1 << 16);
	DataP1++;
	z = z + ((uint32_t)*DataP1 << 24);

	while (n-- > 0)
	{
		sum += delta;
		y += ((z << 4) + k[0]) ^ (z + sum) ^ ((z >> 5) + k[1]);
		z += ((y << 4) + k[2]) ^ (y + sum) ^ ((y >> 5) + k[3]);
	}
	DataP1 = (uint8_t *)v;
	*DataP1 = y;
	DataP1++;
	*DataP1 = y >> 8;
	DataP1++;
	*DataP1 = y >> 16;
	DataP1++;
	*DataP1 = y >> 24;


	DataP1++;
	*DataP1 = z;
	DataP1++;
	*DataP1 = z >> 8;
	DataP1++;
	*DataP1 = z >> 16;
	DataP1++;
	*DataP1 = z >> 24;
}
/******************************************************************************/

// 少于64为数据加密函数
void tea_encode_byte(char * v, const int32_t * k, int32_t p, const char* y)
{
	//char y[] = "Wu&Tian"; //固定写的一个字符串，只要与解密相同即可，7字节长度
	if (y != NULL)
	{
		*v = *v ^ y[p] ^ (char)(k[p % 4] % 0xff);
	}
}

/******************************************************************************/
void tea_decode_byte(char * v, const int32_t * k, int32_t p, const char* y)
{
	//char y[] = "Wu&Tian";
	if (y != NULL)
	{
		*v = *v ^ (char)(k[p % 4] % 0xff) ^ y[p];
	}
}
/******************************************************************************/

// 解密核心函数
// v为需要解密的数据
// k为解密的密钥
// round为解密的轮数，和加密相对应
void tea_decode(int32_t * v, const int32_t * k, int round)
{
	uint32_t y = 0, z = 0, sum = 0, delta = 0x9e3779b9;
	// uint8_t n = 2, *DataP1;
	uint8_t n = round, *DataP1;

	DataP1 = (uint8_t *)v;
	y = *DataP1;
	DataP1++;
	y = y + ((uint32_t)*DataP1 << 8);
	DataP1++;
	y = y + ((uint32_t)*DataP1 << 16);
	DataP1++;
	y = y + ((uint32_t)*DataP1 << 24);

	DataP1++;
	z = *DataP1;
	DataP1++;
	z = z + ((uint32_t)*DataP1 << 8);
	DataP1++;
	z = z + ((uint32_t)*DataP1 << 16);
	DataP1++;
	z = z + ((uint32_t)*DataP1 << 24);

	sum = delta << 1;
	while (n-- > 0)
	{
		z -= ((y << 4) + k[2]) ^ (y + sum) ^ ((y >> 5) + k[3]);
		y -= ((z << 4) + k[0]) ^ (z + sum) ^ ((z >> 5) + k[1]);
		sum -= delta;
	}
	DataP1 = (uint8_t *)v;
	*DataP1 = y;
	DataP1++;
	*DataP1 = y >> 8;
	DataP1++;
	*DataP1 = y >> 16;
	DataP1++;
	*DataP1 = y >> 24;


	DataP1++;
	*DataP1 = z;
	DataP1++;
	*DataP1 = z >> 8;
	DataP1++;
	*DataP1 = z >> 16;
	DataP1++;
	*DataP1 = z >> 24;
}
/******************************************************************************/

//加密数据分组运算
void tea_encode_buffer(char * in_buffer, uint32_t in_size, const int32_t * key,
	int32_t cipherRemains, int round, const char* y)
{
	char * p;
	uint32_t remain = in_size % 8;//计算出数据8字节整数倍之外的数据

	uint32_t align_size = in_size - remain;

	for (p = in_buffer; p < in_buffer + align_size; p += 8)
		tea_encode((int32_t *)p, key, round);
	if (remain > 0 && cipherRemains)
		for (p = in_buffer + align_size; p < in_buffer + in_size; p += 1)
			tea_encode_byte(p, key, --remain, y);
}
/******************************************************************************/

//解密数据分组运算
void tea_decode_buffer(char * in_buffer, uint32_t in_size, const int32_t * key,
	int cipherRemains, int round, const char* y)
{
	char * p;
	uint32_t remain = in_size % 8;
	uint32_t align_size = in_size - remain;

	for (p = in_buffer; p < in_buffer + align_size; p += 8)
		tea_decode((int32_t *)p, key, round);//8字节分组加密
	//处理超出8字节整数倍数据的加密运算
	if (remain > 0 && cipherRemains)
		for (p = in_buffer + align_size; p < in_buffer + in_size; p += 1)
			tea_decode_byte(p, key, --remain, y);
}

// 加密
int tea_encrypt(char *buffer_in, int nlen, char *buffer_out,
	int32_t key[], int round, const char* y)
{
	if (buffer_in == NULL || buffer_out == NULL || nlen <= 0)
	{
		return -1;
	}

	char *pData = new char[nlen];
	memcpy(pData, buffer_in, nlen);

	// 加密数据块，加密后的数据存入pData中
	tea_encode_buffer((char*)pData, nlen, key, 1, round, y);

	memcpy(buffer_out, pData, nlen);

	delete pData;

	return 0;
}

// 解密
int tea_decrypt(char *buffer_in, int nlen, char *buffer_out,
	int32_t key[], int round, const char* y)
{
	if (buffer_in == NULL || buffer_out == NULL || nlen <= 0)
	{
		return -1;
	}

	char *pData = new char[nlen];
	memcpy(pData, buffer_in, nlen);

	// 解密数据块，解密后的数据存入pData中
	tea_decode_buffer((char*)pData, nlen, key, 1, round, y);

	memcpy(buffer_out, pData, nlen);

	delete pData;

	return 0;
}

// 加密
int tea_encrypt_string(std::string buffer_in, std::string buffer_out,
	int32_t key[], int round, const char* y)
{
	int nlen = buffer_in.length();
	char *pData = new char[nlen];
	memcpy(pData, buffer_in.data(), nlen);

	// 解密数据块，解密后的数据存入pData中
	tea_encode_buffer((char*)pData, nlen, key, 1, round, y);

	buffer_out = std::string(pData, nlen);

	delete pData;

	return 0;
}

// 解密
int tea_decrypt_string(std::string buffer_in, std::string buffer_out,
	int32_t key[], int round, const char* y)
{
	int nlen = buffer_in.length();
	char *pData = new char[nlen];
	memcpy(pData, buffer_in.data(), nlen);

	// 解密数据块，解密后的数据存入pData中
	tea_decode_buffer((char*)pData, nlen, key, 1, round, y);

	buffer_out = std::string(pData, nlen);

	delete pData;

	return 0;
}

// 加密
Uint8Array tea_encrypt_bytes(Uint8Array inBuff, int32_t key[], int round, const char* y)
{
	Uint8Array resBuf;

	if (inBuff.size() > 0)
	{
		// 加密数据块，加密后的数据存入buf_out中
		int nlen = inBuff.size();
		char *buf_out = new char[nlen];
		memcpy(buf_out, inBuff.data(), nlen);

		// 加密
		tea_encode_buffer((char*)buf_out, nlen, key, 1, round, y);

		resBuf.resize(nlen);

		memcpy(resBuf.data(), buf_out, nlen);

		delete buf_out;
	}

	return resBuf;
}

// 解密
Uint8Array tea_decrypt_bytes(Uint8Array inBuff, int32_t key[], int round, const char* y)
{
	Uint8Array resBuf;
	char strBuf[40960] = { 0 };

	if (inBuff.size() > 0)
	{
		// 加密数据块，加密后的数据存入buf_out中
		int nlen = inBuff.size();
		char *buf_out = new char[nlen];
		memcpy(buf_out, inBuff.data(), nlen);

		// 解密
		tea_decode_buffer((char*)buf_out, nlen, key, 1, round, y);

		resBuf.resize(nlen);

		memcpy(resBuf.data(), buf_out, nlen);

		delete buf_out;
	}

	return resBuf;
}
