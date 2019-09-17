#include "mLualib.h"
#include <stdio.h>

static int averageFunc(lua_State *L)
{
	int n = lua_gettop(L);
	double sum = 0;
	int i;

	/* ѭ�������֮�� */
	for (i = 1; i <= n; i++)
		sum += lua_tonumber(L, i);

	lua_pushnumber(L, sum / n);     //ѹ��ƽ��ֵ  
	lua_pushnumber(L, sum);         //ѹ���  

	return 2;                       //�����������  
}

static int sayHelloFunc(lua_State* L)
{
	printf("hello world!\n");
	return 0;
}

static const struct luaL_Reg myLib[] = 
{
	{"average", averageFunc},
	{"sayHello", sayHelloFunc},
	{NULL, NULL}  //���������һ�Ա�����{NULL, NULL}��������ʾ����      
};

int luaopen_mLualib(lua_State* L)
{
	//luaL_register(L, "ss", myLib);
	luaL_newlib(L, myLib);
	return 1;  // ��myLib��ѹ����ջ�У����Ծ���Ҫ����1  
}
