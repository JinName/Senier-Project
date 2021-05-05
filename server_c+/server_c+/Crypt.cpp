#include "Crypt.h"

Crypt* g_pCrypt = nullptr;

Crypt::Crypt()
	: m_Prov(NULL)
	, m_Hash(NULL)
	, m_Key(NULL)
{
}

int Crypt::Init()
{
	if (CryptAcquireContext(&m_Prov, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0) == FALSE)
	{
		return 1;
	}

	if (CryptCreateHash(m_Prov, CALG_SHA, 0, 0, &m_Hash) == FALSE)
	{
		return 2;
	}

	if (CryptHashData(m_Hash, (BYTE*)PASSKEY, (DWORD)strlen(PASSKEY), 0) == FALSE)
	{
		return 3;
	}

	if (CryptDeriveKey(m_Prov, CALG_RC4, m_Hash, CRYPT_EXPORTABLE, &m_Key) == FALSE)
	{
		return 4;
	}

	return 0;
}

void Crypt::Clean()
{
	if (m_Key)
		CryptDestroyKey(m_Key);
	if (m_Hash)
		CryptDestroyHash(m_Hash);
	if (m_Prov)
		CryptReleaseContext(m_Prov, 0);

	m_Prov = NULL;
	m_Hash = NULL;
	m_Key = NULL;
}

bool Crypt::Encrypt(char* pStr)
{
	if (m_Prov == NULL ||
		m_Hash == NULL ||
		m_Key == NULL)
	{
		cout << "Encrypt() >> crypt got nullptr!!" << endl;
		return false;
	}

	BYTE tempByte[MAX_BUF_LEN];
	DWORD dwStrLen = (DWORD)strlen(pStr) + 1;

	ZeroMemory(tempByte, MAX_BUF_LEN);
	CopyMemory(tempByte, pStr, dwStrLen);

	if (FALSE == CryptEncrypt(m_Key, 0, TRUE, 0, tempByte, &dwStrLen, MAX_BUF_LEN))
		return false;

	CopyMemory(pStr, tempByte, dwStrLen);

	return true;
}

bool Crypt::Decrypt(char* pStr)
{
	if (m_Prov == NULL ||
		m_Hash == NULL ||
		m_Key == NULL)
	{
		cout << "Decrypt() >> crypt got nullptr!!" << endl;
		return false;
	}

	BYTE tempByte[MAX_BUF_LEN];
	DWORD dwStrLen = (DWORD)strlen(pStr) + 1;

	ZeroMemory(tempByte, MAX_BUF_LEN);
	CopyMemory(tempByte, pStr, dwStrLen);

	if (FALSE == CryptDecrypt(m_Key, NULL, TRUE, 0, tempByte, &dwStrLen))
		return false;

	CopyMemory(pStr, tempByte, dwStrLen);

	return true;
}