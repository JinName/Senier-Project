#include "Crypt.h"

Crypt* g_pCrypt = nullptr;

Crypt::Crypt()
	: m_hProv(NULL)
	, m_hHash(NULL)
	, m_hKey(NULL)
{
}

int Crypt::Init()
{
	if (CryptAcquireContext(&m_hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0) == FALSE)
	{
		return 1;
	}

	if (CryptCreateHash(m_hProv, CALG_SHA, 0, 0, &m_hHash) == FALSE)
	{
		return 2;
	}

	if (CryptHashData(m_hHash, (BYTE*)PASSKEY, (DWORD)strlen(PASSKEY), 0) == FALSE)
	{
		return 3;
	}

	if (CryptDeriveKey(m_hProv, CALG_RC4, m_hHash, CRYPT_EXPORTABLE, &m_hKey) == FALSE)
	{
		return 4;
	}

	return 0;
}

void Crypt::Clean()
{
	if (m_hKey)
		CryptDestroyKey(m_hKey);
	if (m_hHash)
		CryptDestroyHash(m_hHash);
	if (m_hProv)
		CryptReleaseContext(m_hProv, 0);

	m_hProv = NULL;
	m_hHash = NULL;
	m_hKey = NULL;
}

bool Crypt::Encrypt(char* _pStr)
{
	if (m_hProv == NULL ||
		m_hHash == NULL ||
		m_hKey == NULL)
	{
		cout << "Encrypt() >> crypt got nullptr!!" << endl;
		return false;
	}

	BYTE tempByte[MAX_BUF_LEN];
	DWORD dwStrLen = (DWORD)strlen(_pStr) + 1;

	ZeroMemory(tempByte, MAX_BUF_LEN);
	CopyMemory(tempByte, _pStr, dwStrLen);

	if (FALSE == CryptEncrypt(m_hKey, 0, TRUE, 0, tempByte, &dwStrLen, MAX_BUF_LEN))
		return false;

	CopyMemory(_pStr, tempByte, dwStrLen);

	return true;
}

bool Crypt::Decrypt(char* _pStr)
{
	if (m_hProv == NULL ||
		m_hHash == NULL ||
		m_hKey == NULL)
	{
		cout << "Decrypt() >> crypt got nullptr!!" << endl;
		return false;
	}

	BYTE tempByte[MAX_BUF_LEN];
	DWORD dwStrLen = (DWORD)strlen(_pStr) + 1;

	ZeroMemory(tempByte, MAX_BUF_LEN);
	CopyMemory(tempByte, _pStr, dwStrLen);

	if (FALSE == CryptDecrypt(m_hKey, NULL, TRUE, 0, tempByte, &dwStrLen))
		return false;

	CopyMemory(_pStr, tempByte, dwStrLen);

	return true;
}