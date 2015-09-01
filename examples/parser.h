/*
 * parser.h
 *
 *  Created on: 25 sie 2015
 *      Author: Dominik Sadko
 */

#ifndef PARSER_H_
#define PARSER_H_

struct Point
{
	Point() : x(0), y(0) {}
	Point(int32_t _x, int32_t _y) : x(_x), y(_y) {}
	int32_t x, y;
};


namespace Lua
{
	template <> struct Parser<Point>
	{
		static Point get(lua_State* L, int32_t idx);
		static void  put(lua_State* L, const Point& v);
		static bool  is(lua_State* L, int32_t idx);
	};

	Point Parser<Point>::get(lua_State* L, int32_t idx)
	{
		Point point;
		Lua::Object object(L, idx);

		point.x = object["x"].get<int32_t>();
		point.y = object["y"].get<int32_t>();
		return point;
	}

	void Parser<Point>::put(lua_State* L, const Point& point)
	{
		std::map<std::string, int32_t> pointMap;
		pointMap["x"] = point.x;
		pointMap["y"] = point.y;

		Lua::Parser<std::map<std::string, int32_t>>::put(L, pointMap);
	}

	bool Parser<Point>::is(lua_State* L, int32_t idx)  { return lua_istable(L, idx); }
}

Point movePointX(const Point& point, int32_t x) { return {point.x + x, point.y}; }

void LuaBinder_Parser()
{
	Point point(80, 120);
	g_lua.set("point", point);

    g_lua.callString("assert(point.x == 80)");
    g_lua.callString("assert(point.y == 120)");

    g_lua.staticFunction(movePointX, "movePointX");
    g_lua.callString("local point = movePointX(point, 20) \n"
    				 "assert(point.x == 100)");
}

#endif /* PARSER_H_ */
