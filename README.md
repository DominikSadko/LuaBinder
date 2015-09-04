# LuaBinder
Lua Binder in C++11

Used MinGW GNU 4.7.2 to compile.

Example:
```
#include <stdio.h>
#include <stdlib.h>
#include <lua_binder/Lua.h>

using namespace std;
Lua::State g_lua;

int32_t main(void)
{
    g_lua.init();
    g_lua.loadFile("test.lua");
    g_lua["foo"](3.14);

    return EXIT_SUCCESS;
}
```
