/*
 * hello.h
 *
 *      Author: Dominik Sadko
 */

#ifndef HELLO_H_
#define HELLO_H_

Lua::State g_lua;

void LuaBinder_Hello()
{
    g_lua.init();
    g_lua.callString("print('Hello World!')");
}


#endif /* HELLO_H_ */
