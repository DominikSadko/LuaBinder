/*
 * Program name: Lua Binder
 * Auto: Dominik Sadko
 * Contact: Dominik.Sadko@gmail.com
 *
 * todo:
 * 		Lua::Parser<std::tuple>
 * 		problem with auto lambda
 * 		fix destructor: Lua::Shared::isRemoved
 * 		register lambda scalar
*/

#include <stdio.h>
#include <stdlib.h>
#include <lua_binder/Lua.h>

#include "examples/hello.h"
#include "examples/common.h"
#include "examples/table.h"
#include "examples/functions.h"
#include "examples/lambda.h"
#include "examples/classes.h"
#include "examples/inheritanced_classes.h"
#include "examples/shared_classes.h"
#include "examples/parser.h"
#include "examples/sandbox.h"
#include "examples/stress_test.h"



int32_t main(void)
{
	LuaBinder_Hello();
	LuaBinder_Common();
	LuaBinder_Table();
	LuaBinder_Functions();
	LuaBinder_Lambda();
	LuaBinder_Classes();
	LuaBinder_InheritancedClasses();
	LuaBinder_SharedClasses();
	LuaBinder_Parser();
	LuaBinder_SandBox();
	LuaBinder_StressTest();

	ASSERT_TOP(g_lua.state());

	return EXIT_SUCCESS;
}
