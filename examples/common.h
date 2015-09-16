/*
 * common.h
 *
 *      Author: Dominik Sadko
 */

#ifndef COMMON_H_
#define COMMON_H_

void LuaBinder_Common()
{
	g_lua.set("var_a", true);			// boolean
	g_lua.set("var_b", 2);				// integer
	g_lua.set("var_c", 3.14f);			// number
	g_lua.set("var_d", (char*)"txt");	// string

	assert(g_lua["var_a"] == true);
	assert(g_lua["var_b"] == 2);
	assert(g_lua["var_c"] == 3.14f);
	assert(g_lua["var_d"] == "txt");
}


#endif /* COMMON_H_ */
