/*
 * functions.h
 *
 *  Created on: 25 sie 2015
 *      Author: Dominik Sadko
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void foo_a() {}
uint16_t foo_b() { return 314; }
int32_t foo_c() { return 314 * 2; }
double foo_d() { return 3.14; }
const char* foo_e() { return "foo_e"; }
std::vector<std::string> foo_f() { return {"a", "b"}; }
int32_t foo_g(int32_t arg_1, int32_t arg_2) { return arg_1 + arg_2; }
int32_t print(lua_State* L)
{
    const char *s;
    lua_getglobal(L, "tostring");
    int n = lua_gettop(L);  // number of arguments

    for(int32_t i = 1; i < n; i++)
    {
        lua_pushvalue(L, -1);  // function to be called
        lua_pushvalue(L, i);   // value to print
        lua_call(L, 1, 1);
        s = lua_tostring(L, -1);

        if(!s)
            return luaL_error(L, "'tostring' must return a string to 'print'");

        LOGI("[Lua - print] %s\n", s);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);  // pop tostring
    return 0;
}

void LuaBinder_Functions()
{
	g_lua.staticFunction(&foo_a, "foo_a");
    g_lua.staticFunction(&foo_b, "foo_b");
    g_lua.staticFunction(&foo_c, "foo_c");
    g_lua.staticFunction(&foo_d, "foo_d");
    g_lua.staticFunction(&foo_e, "foo_e");
    g_lua.staticFunction(&foo_f, "foo_f");
    g_lua.staticFunction(&foo_g, "foo_g");
    lua_register(g_lua.state(), "print", &print);

    g_lua.callString("assert(foo_a() == nil)       \n"
    				 "assert(foo_b() == 314)       \n"
    				 "assert(foo_c() == 314 * 2)   \n"
			 	 	 "assert(foo_d() == 3.14)      \n"
    				 "assert(foo_e() == 'foo_e')   \n"
    				 "assert(foo_f()[1] == 'a')    \n"
			 	 	 "assert(foo_f()[2] == 'b')    \n"
			 	 	 "assert(foo_g(4, 8) == 4 + 8) \n"

	 	 	 	 	 "print('test printer') \n");
}


#endif /* FUNCTIONS_H_ */
