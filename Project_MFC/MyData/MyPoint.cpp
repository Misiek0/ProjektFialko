#pragma once

#include "pch.h"
#include "MyPoint.h"
#include <cstring>

MY_POINT::~MY_POINT()
{
    if (name) {
        delete[] name;
        name = nullptr;
    }
}

//MY_POINT::MY_POINT(const MY_POINT& other)
//{
//    if (other.name) {
//        //name = new char[std::strlen(other.name) + 1];
//        //strcpy_s(name, std::strlen(other.name), other.name);
//
//        size_t len = std::strlen(other.name) + 1; 
//        name = new char[len];
//        strcpy_s(name, len, other.name);
//    }
//    else {
//        name = nullptr;
//    }
//}

