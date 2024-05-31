#include "pch.h"
#include "MyPoint.h"

MY_POINT::~MY_POINT()
{
    if (name) {
        delete[] name;
    }
}

