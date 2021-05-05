#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <Windows.h>
#include <wincrypt.h>
using namespace std;

// ERROR DEFINE
#define ERR_PROVIDER 1
#define ERR_CREATE_HASH 2
#define ERR_HASHDATA 3
#define ERR_DERIVE_KEY 4
#define ERR_SET_KEYPARAM 5
#define ERR_CRYPTING 6

// PERSONAL DEFINE
#define MAX_BUF_LEN 32
#define PASSKEY "parksw"

class Crypt
{
public:
	Crypt();
	~Crypt() {}

	int Init();
	void Clean();

	// encrypt
	bool Encrypt(char* pStr);

	// decrypt
	bool Decrypt(char* pStr);

private:
	HCRYPTKEY	m_Key;
	HCRYPTPROV	m_Prov;
	HCRYPTHASH	m_Hash;
};

extern Crypt* g_pCrypt;