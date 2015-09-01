/*
 * sandbox.h
 *
 *  Created on: 26 sie 2015
 *      Author: Dominik Sadko
 */

#ifndef SANDBOX_H_
#define SANDBOX_H_

void LuaBinder_SandBox()
{
    g_lua.doString("SandBox",
    		"variable = 'Number_1' \n"
    		""
    		"function testFoo() \n"
    		"    assert(variable == 'Number_1') \n"
    		"    assert(OtherBox.variable == 'Number_2')\n"
    		"end\n"
    );

    g_lua.doString("OtherBox",
    		"variable = 'Number_2' \n"
    		"assert(SandBox.variable == 'Number_1')\n"
    		""
    		"function testFoo() \n"
    		"    assert(variable == 'Number_2') \n"
    		"end\n"
    );

    assert(g_lua["SandBox"]["variable"].get<std::string>() == "Number_1");
    assert(g_lua["OtherBox"]["variable"].get<std::string>() == "Number_2");

    g_lua["SandBox"]["testFoo"].call();
    g_lua["OtherBox"]["testFoo"].call();
}


#endif /* SANDBOX_H_ */
