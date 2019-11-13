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
	TemplateSingleton();
	~TemplateSingleton();

public:
	static T* GetInstance();
	static void DestroyInstance();

private:
	static T* m_pInstance;
};