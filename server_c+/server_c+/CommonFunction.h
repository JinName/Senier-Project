#pragma once
#include <iostream>
#include <typeinfo>

// template �� Ȱ���� ���� ��ġ, ���� ��� �Լ�
template <typename T>
void printError(T thisClass, const char* thisFunc, const char* msg)
{
	std::cout << "[ERROR] " << typeid(*thisClass).name() << " >> " << thisFunc << "() : " << msg << std::endl;
}