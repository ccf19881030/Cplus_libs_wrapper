#include <stdio.h>

#include "tea.h"

/****************************************************************************
函数名称: str_to_hex
函数功能: 字符串转换为十六进制
输入参数: string 字符串 cbuf 十六进制 len 字符串的长度。
输出参数: 无
*****************************************************************************/
int str_to_hex(char *string, unsigned char *cbuf, int len)
{
	uint8_t high, low;
	int idx, ii = 0;
	for (idx = 0; idx < len; idx += 2)
	{
		high = string[idx];
		low = string[idx + 1];

		if (high >= '0' && high <= '9')
			high = high - '0';
		else if (high >= 'A' && high <= 'F')
			high = high - 'A' + 10;
		else if (high >= 'a' && high <= 'f')
			high = high - 'a' + 10;
		else
			return -1;

		if (low >= '0' && low <= '9')
			low = low - '0';
		else if (low >= 'A' && low <= 'F')
			low = low - 'A' + 10;
		else if (low >= 'a' && low <= 'f')
			low = low - 'a' + 10;
		else
			return -1;

		cbuf[ii++] = high << 4 | low;
	}

	return 0;
}

/****************************************************************************
函数名称: hex_to_str
函数功能: 十六进制转字符串
输入参数: ptr 字符串 buf 十六进制 len 十六进制字符串的长度。
输出参数: 无
*****************************************************************************/
void hex_to_str(char *ptr, unsigned char *buf, int len)
{
	for (int i = 0; i < len; i++)
	{
		sprintf(ptr, "%02x", buf[i]);
		ptr += 2;
	}
}

// 测试tea_encrypt和tea_decrypt接口
// 输入的数据为字符串
void test1()
{
	int key[] = { 0x37777931, 0x73444148, 0x4148314F, 0x59484170 }; // 128位密钥
	char* pTestStr = "E2DDFA5D00E0FF6880B0924100000000000000002D48190080A2190030000000000040004CFF000000000000000000000000000000000000750200000000";
	
	int nlen = strlen(pTestStr) + 1;
	char* pData = new char[nlen];
	memcpy(pData, pTestStr, nlen);

	printf("加密前的字符串为:%s\n", pData);

	// 加密数据块，加密后的数据存入strEncodeData中
	char strEncodeData[1024] = { 0 };
	int iResult = tea_encrypt((char*)pData, nlen, strEncodeData,
		key, 2, "Wu&Tian");
	printf("加密后的字符串为:%s\n", strEncodeData);

	char strDecodeData[1024] = { 0 };
	iResult = tea_decrypt((char*)strEncodeData, nlen, strDecodeData,
		key, 2, "Wu&Tian");
	printf("解密后的字符串为:%s\n", strDecodeData);
}

// 测试tea_encrypt_bytes和tea_decrypt_bytes接口
// 输入的数据为16进制字节数组
void test2()
{
	// 测试tea_encrypt和tea_decrypt接口
	Uint8Array inBuf = {
		0xE2, 0xDD, 0xFA, 0x5D, 0x00, 0xE0, 0xFF, 0x68, 0x80, 0xB0, 0x92, 0x41, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x48, 0x19, 0x00, 0x80, 0xA2, 0x19,
		0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x4C, 0xFF, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x75, 0x02, 0x00, 0x00, 0x00, 0x00
	};

	//int *key = (int *)"1yw7HADsO1HApAHY";
	int key[] = { 0x37777931, 0x73444148, 0x4148314F, 0x59484170 };

	int nlen = inBuf.size();

	char strBuf[40960] = { 0 };
	hex_to_str(strBuf, inBuf.data(), nlen);
	printf("加密前的16进制数据为: %s\n", strBuf);

	// 加密
	Uint8Array encodeArr = tea_encrypt_bytes(inBuf, key, 2, "Wu&Tian");

	hex_to_str(strBuf, (unsigned char*)encodeArr.data(), encodeArr.size());
	printf("加密后的16进制数据为: %s\n", strBuf);

	// 解密
	Uint8Array decodeArr = tea_decrypt_bytes(encodeArr, key, 2, "Wu&Tian");

	hex_to_str(strBuf, (unsigned char*)decodeArr.data(), decodeArr.size());
	printf("解密后的16进制数据为: %s\n", strBuf);
}

int main(int argc, char* argv[])
{
	//printf("测试1:字符串 E2DDFA5D00E0FF6880B0924100000000000000002D48190080A2190030000000000040004CFF000000000000000000000000000000000000750200000000 \n");
	test1();

	printf("--------------------------------------------------------------------------------------------------\n");
	
	//printf("测试2: 16进制数据 E2DDFA5D00E0FF6880B0924100000000000000002D48190080A2190030000000000040004CFF000000000000000000000000000000000000750200000000 \n");
	test2();

	printf("--------------------------------------------------------------------------------------------------\n");

	getchar();

	return 0;
}
