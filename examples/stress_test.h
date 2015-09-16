/*
 * stress_test.h
 *
 *      Author: Dominik Sadko
 */

#ifndef STRESS_TEST_H_
#define STRESS_TEST_H_

#include <sys/timeb.h>
#include <inttypes.h>

int64_t getTime()
{
	timeb t;
	ftime(&t);
	return ((int64_t)t.millitm) + ((int64_t)t.time) * 1000;
}

int32_t stressTest(int32_t a, int32_t b)
{
	return a + b;
}

void LuaBinder_StressTest()
{
	g_lua.staticFunction(&stressTest, "stressTest");
	g_lua.staticFunction(&stressTest, "time");

	int64_t now = getTime();
    g_lua.callString("for i = 1, 100000 do\n"
    				 "    stressTest(0, 2)\n"
    				 "end\n");
    LOGI("Stress test time: %I64d\n", getTime() - now);

	now = getTime();
    g_lua.callString("function stressTest2(a, b) return a + b end");
    auto foo = g_lua["stressTest2"];
    for(int32_t i = 1; i <= 100000; i++)
    	foo(0, 2);
    LOGI("Stress test time: %I64d\n", getTime() - now);
}


#endif // STRESS_TEST_H_ //
