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

template < typename T >
class TemplateSingleton
{
protected:
	TemplateSingleton() : m_bClean(false) {}
	~TemplateSingleton() {}

	bool m_bClean;

public:
	static T* GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new T;
		//atexit(DestroyInstance);

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

	bool GetClean() { return m_bClean; }

	virtual void Init() = 0;
	virtual void Clean() = 0;

private:
	static T* m_pInstance;
};

template < typename T >
T* TemplateSingleton<T>::m_pInstance = NULL;