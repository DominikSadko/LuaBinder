/*
 * table.h
 *
 *  Created on: 25 sie 2015
 *      Author: Dominik Sadko
 */

#ifndef TABLE_H_
#define TABLE_H_

enum e_numbers : uint8_t
{
	First = 0,
	Second = 1,
	Last = e_numbers::Second
};

void LuaBinder_Table()
{
	std::vector<int> table_a = {100, 200, 300};
	g_lua.set("table_a", table_a);
	assert(g_lua["table_a"][1] == 100);
	assert(g_lua["table_a"][2] == 200);
	assert(g_lua["table_a"][3] == 300);


	std::list<float> table_b = {1.1f, 2.2f};
	g_lua.set("table_b", table_b);
	assert(g_lua["table_b"][1] == 1.1f);
	assert(g_lua["table_b"][2] == 2.2f);


    std::map<std::string, uint8_t> table_c;
    table_c["First"]          = e_numbers::First;
    table_c["Second"]         = e_numbers::Second;
    table_c["Last"]           = e_numbers::Last;
    g_lua.set("e_numbers", table_c);

	assert(g_lua["e_numbers"]["First"] == e_numbers::First);
	assert(g_lua["e_numbers"]["Second"] == e_numbers::Second);
	assert(g_lua["e_numbers"]["Last"] == e_numbers::Last);


	// set value methods
	g_lua["table_b"].set(1, 3.14f);
	assert(g_lua["table_b"][1] == 3.14f);
}


#endif /* TABLE_H_ */
