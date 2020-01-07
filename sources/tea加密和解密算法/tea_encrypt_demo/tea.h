#ifndef _TEA_H_
#define _TEA_H_

#include <vector>
#include <string>

typedef unsigned char uint8_t;
typedef int int32_t;
typedef unsigned int uint32_t;

typedef std::vector<uint8_t> Uint8Array;

/*******************************加密解密函数************************************/
// 加密核心函数
// v为需要加密的数据
// k为加密的密钥
// round为加密的轮数，和解密相对应
void tea_encode(int32_t * v, const int32_t * k, int round);

// 少于64为数据加密函数
void tea_encode_byte(char * v, const int32_t * k, int32_t p, const char* y);

void tea_decode_byte(char * v, const int32_t * k, int32_t p, const char* y);

// 解密核心函数
// v为需要解密的数据
// k为解密的密钥
// round为解密的轮数，和加密相对应
void tea_decode(int32_t * v, const int32_t * k, int round);


//加密数据分组运算
void tea_encode_buffer(char * in_buffer, uint32_t in_size, const int32_t * key,
	int32_t cipherRemains, int round, const char* y);


//解密数据分组运算
void tea_decode_buffer(char * in_buffer, uint32_t in_size, const int32_t * key,
	int cipherRemains, int round, const char* y);

// 加密
int tea_encrypt(char *buffer_in, int nlen, char *buffer_out,
	int32_t key[], int round, const char* y);

// 解密
int tea_decrypt(char *buffer_in, int nlen, char *buffer_out,
	int32_t key[], int round, const char* y);

// 加密
int tea_encrypt_string(std::string buffer_in, std::string buffer_out,
	int32_t key[], int round, const char* y);

// 解密
int tea_decrypt_string(std::string buffer_in, std::string buffer_out,
	int32_t key[], int round, const char* y);

// 加密
Uint8Array tea_encrypt_bytes(Uint8Array inBuff, int32_t key[], int round, const char* y);

// 解密
Uint8Array tea_decrypt_bytes(Uint8Array inBuff, int32_t key[], int round, const char* y);

#endif
