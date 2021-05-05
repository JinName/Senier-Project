#pragma once
#include <iostream>
#include <typeinfo>

// template 을 활용한 에러 위치, 원인 출력 함수
template <typename T>
void printError(T thisClass, const char* thisFunc, const char* msg)
{
	std::cout << "[ERROR] " << typeid(*thisClass).name() << " >> " << thisFunc << "() : " << msg << std::endl;
}