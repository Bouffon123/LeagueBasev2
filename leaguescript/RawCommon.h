#pragma once

//#define MANAGED_BUILD

#ifdef MANAGED_BUILD
#define SCRIPT_API __declspec( dllimport )
#else
#define SCRIPT_API __declspec( dllexport )
#endif