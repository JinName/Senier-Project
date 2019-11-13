#include "TemplateSingleton.h"

template < typename T >
T* TemplateSingleton<T>::m_pInstance = 0;

template < typename T >
TemplateSingleton<T>::TemplateSingleton()
{
}

template < typename T >
TemplateSingleton<T>::~TemplateSingleton()
{
}

template < typename T >
T* TemplateSingleton<T>::GetInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new T;

	return m_pInstance;
}

template < typename T >
void TemplateSingleton<T>::DestroyInstance()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}