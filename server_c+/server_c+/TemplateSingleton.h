// template singleton for single object in global
/*
--- how to use ---
class CObject : pubilc TemplateSingleton<cobject>
{
public:
	void method();
	...
}

CObject::GetInstance()->method();
*/

#pragma once
#include "stdafx.h"

template < typename T >
class TemplateSingleton
{
protected:
	TemplateSingleton() {}
	~TemplateSingleton() {}

public:
	static T* GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new T;

		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

private:
	static T* m_pInstance;
};

template < typename T >
T* TemplateSingleton<T>::m_pInstance = NULL;