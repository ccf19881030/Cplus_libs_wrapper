#pragma once

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

#ifdef _WIN32
	#ifndef LUA_EXPORT
	#define DLL_LUA_API __declspec(dllexport)
	#else
	#define DLL_LUA_API __declspec(dllimport)
	#endif
#else
	#define DLL_LUA_API extern "C"
#endif

// 定义导出函数
DLL_LUA_API int luaopen_mLualib(lua_State* L);