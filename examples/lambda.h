/*
 * lambda.h
 *
 *      Author: Dominik Sadko
 */

#ifndef LAMBDA_H
#define LAMBDA_H

void callbackTest1(std::function<void(int32_t, int32_t, int32_t)> callback, int32_t arg)
{
	callback(arg, 8, arg + 8); // Important: You have to call this function only one time! Memory leak problem
	//callback(9, arg, 9 + arg); <- got error: "attempt to call a nil value"
}

void callbackTest2(std::function<void(int32_t, int32_t, int32_t)>* callback)
{
	(*callback)(5, 8, 5 + 8); // You can call more than one time
	(*callback)(9, 1, 9 + 1);

    if(!Lua::remove(g_lua.state(), callback)) // Important: Do not forget to free! Memory leak problem
        delete callback;
}

void LuaBinder_Lambda()
{
	std::function<int32_t(int32_t, int32_t)> lambda = [](int32_t arg1, int32_t arg2) { return arg1 + arg2; };

	g_lua.staticFunction(lambda, "lambda");
	g_lua.staticFunction(&callbackTest1, "callbackTest1");
	g_lua.staticFunction(&callbackTest2, "callbackTest2");

    g_lua.callString("assert(lambda(32, 64) == 32 + 64)     \n");
    g_lua.callString("function callback(arg1, arg2, arg3)   \n"
    				 "    assert(arg1 + arg2 == arg3)       \n"
    				 "end                                   \n"
    				 "callbackTest1(callback, 20)           \n"
    				 "callbackTest2(callback)               \n");


    g_lua["callback"](1, 1, 2);
}




#endif /* LAMBDA_H */
