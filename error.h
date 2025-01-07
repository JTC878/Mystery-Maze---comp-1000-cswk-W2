#pragma once
#ifndef _ERROR_H
#define _ERROR_H

#include"wncurses.h"
#include<stdio.h>

#ifdef _DEBUG

#define AN_ERROR() {\
    __debugbreak();\
}
#define ASSERT(x) {\
	if (!(x))\
		__debugbreak();\
}
#else
#define AN_ERROR(x) { }
#define ASSERT(x) { }
#endif
#endif