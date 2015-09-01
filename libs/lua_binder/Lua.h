#ifndef __LUA_LIB_H__
#define __LUA_LIB_H__

#ifndef lua_h
    #include <lua52/lua.h>
    #include <lua52/lauxlib.h>
    #include <lua52/lualib.h>
#endif

#include <vector>
#include <list>
#include <map>
#include <functional>

#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

#include <typeinfo>
#include <cxxabi.h>

#include <assert.h>


#include "Basic.h"
#include "Shared.h"
#include "Parser.h"
#include "Tupler.h"
#include "Object.h"
#include "Binder.h"

namespace Lua
{
class State
{
    public:
        State()
        {
            L = NULL;
        }

        void init()
        {
            L = luaL_newstate();
            luaL_openlibs(L);

            lua_newtable(L);
            lua_setglobal(L, "__refs");

            lua_newtable(L);
            lua_setglobal(L, "__shared");
        }

        void close()
        {
        	lua_close(L);
        }

        lua_State* state() { return L; }

        ////////////////      Load & Call     ////////////////
        bool call(int nargs = 0, int nresults = 0, int ctx = 0)
        {
            if(lua_pcall(L, nargs, nresults, ctx))
            {
                lua_error(L);
                return false;
            }

            return true;
        }

        bool loadFile(const char* filename, const char* mode = nullptr)
        {
            if(luaL_loadfilex(L, filename, mode))
            {
                lua_error(L);
                return false;
            }

            return true;
        }

        bool loadString(const char* script)
        {
            if(luaL_loadstring(L, script))
            {
                lua_error(L);
                return false;
            }

            return true;
        }

        bool callString(const char* script)
            { return loadString(script) && call(); }

        bool callFile(const char* filename, const char* mode = nullptr)
            { return loadFile(filename, mode) && call(); }

        bool doString(const std::string& name, const char* script)
        {
        	if(strlen(script) <= 0)
        	{
        		LOGE("script is empty: %s", name.c_str());
        		return false;
        	}

        	int ret = luaL_loadstring(L, script);
            if(ret != LUA_OK) // 1
            {
                lua_error(L);
                LOGE("name: %s", name.c_str());
                return false;
            }

            if(!name.empty())
            {
				lua_newtable(L); // script env  // 21
				lua_setglobal(L, name.c_str()); // set global name for new table // 1

				lua_getglobal(L, name.c_str()); // 21
				lua_newtable(L); // metatable // 321
				lua_getglobal(L, "_G"); // 4321
				lua_setfield(L, -2, "__index"); // set __index in metatable to _G // 321
				lua_setmetatable(L, -2); // set metatable for script env // 21

				lua_setupvalue(L, -2, 1); // set env for state // 21
            }

            ret = lua_pcall(L, 0, LUA_MULTRET, 0);
            if(ret != LUA_OK) // run script // 1
            {
                lua_error(L);
                LOGE("name: %s", name.c_str());
                return false;
            }

            return true;
        }

        bool doFile(const std::string& name, const std::string& path)
        {
            int ret = luaL_loadfile(L, path.c_str());
            if(ret != LUA_OK) // 1
            {
                lua_error(L);
                LOGE("name: %s", name.c_str());
                return false;
            }

            if(!name.empty())
            {
				lua_newtable(L); // script env  // 21
				lua_setglobal(L, name.c_str()); // set global name for new table // 1

				lua_getglobal(L, name.c_str()); // 21
				lua_newtable(L); // metatable // 321
				lua_getglobal(L, "_G"); // 4321
				lua_setfield(L, -2, "__index"); // set __index in metatable to _G // 321
				lua_setmetatable(L, -2); // set metatable for script env // 21

				lua_setupvalue(L, -2, 1); // set env for state // 21
            }

            ret = lua_pcall(L, 0, LUA_MULTRET, 0);
            if(ret != LUA_OK) // run script // 1
            {
                lua_error(L);
                LOGE("name: %s", name.c_str());
                return false;
            }

            return true;
        }

        ////////////////      Register functions     ////////////////
        template <typename Ret, typename... Args>
        Binder<void, Ret*, Args...>* constructor(const std::string& name)
            { return staticFunction(std::function<Ret*(Args...)>( [](Args... args){ return new Ret(args...); }), name); }

        template <typename T>
        Binder<void, void, T*>* destructor(const std::string& name)
            { return staticFunction(std::function<void(T*)>( [] (T* in) { delete in; } ), name); }



        template <typename Ret, typename... Args>
        Binder<void, Ret, Args...>* staticFunction(Ret(*fn)(Args... args), const std::string& name)
            { return new Binder<void, Ret, Args...>(L, fn, name); }

        template <typename Ret, typename... Args>
        Binder<void, Ret, Args...>* staticFunction(std::function<Ret(Args...)> fn, const std::string& name)
            { return new Binder<void, Ret, Args...>(L, fn, name); }




        template <class FC, typename Ret, typename... Args>
        Binder<FC*, Ret, Args...>* instanceFunction(Ret (FC::*fn)(Args... args), FC* in, const std::string& name)
            { return new Binder<FC*, Ret, Args...>(L, fn, in, name); }

        template <class FC, typename Ret, typename... Args>
        Binder<FC*, Ret, Args...>* instanceFunction(Ret (FC::*fn)(Args... args) const, FC* in, const std::string& name)
            { return new Binder<FC*, Ret, Args...>(L, fn, in, name); }


        template <class FC, typename Ret, typename... Args>
        Binder<FC*, Ret, Args...>* sharedFunction(Ret (FC::*fn)(Args... args), const std::string& name)
            { return new Binder<FC*, Ret, Args...>(L, fn, nullptr, name); }

        template <class FC, typename Ret, typename... Args>
        Binder<FC*, Ret, Args...>* sharedFunction(Ret (FC::*fn)(Args... args) const, const std::string& name)
            { return new Binder<FC*, Ret, Args...>(L, fn, nullptr, name); }


        ////////////////      Operators     ////////////////
        Object operator[](const char* name) { return Object(L, name); }
        Object operator[](const std::string& name) { return Object(L, name); }

        ////////////////      Lua functions     ////////////////

        template <typename T>
        void set(const std::string& name, const T& value) { Lua::set(L, name, value); }

        template <typename T>
        T get(const std::string& name) { return Lua::get<T>(L, name); }

        void remove(const std::string& name)
        {
            lua_pushnil(L);
            lua_setglobal(L, name.c_str());
        }

        void bindTable(const std::string& table)
        {
            lua_newtable(L);
            lua_setglobal(L, table.c_str());
        }

        static int32_t getGlobal(lua_State* state)
        {
            std::string field = Lua::Parser<std::string>::get(state, -1);
            Lua::pop(state);
            lua_getglobal(state, field.c_str());
            return 1;
        }

        void newMetatable(const std::string& className, const std::string& baseClass = "")
        {
            // className = {}
            lua_newtable(L);
            lua_pushvalue(L, -1);
            lua_setglobal(L, className.c_str());
            int32_t methods = lua_gettop(L);

            // methodsTable = {}
            lua_newtable(L);

            // setmetatable(className, methodsTable)
            lua_setmetatable(L, methods);

            if(!baseClass.empty())
            {
                luaL_getmetatable(L, baseClass.c_str());
                lua_setmetatable(L, methods);
            }

            // className.metatable = {}
            luaL_newmetatable(L, className.c_str());
            int32_t metatable = lua_gettop(L);

            // className.metatable.__metatable = methodsTable
            lua_pushvalue(L, methods);
            lua_setfield(L, metatable, "__metatable");

            // className.metatable.__index = methodsTable
            lua_pushvalue(L, methods);
            lua_setfield(L, metatable, "__index");

            lua_pop(L, 2);

            ASSERT_TOP(L);
        }

    private:
        lua_State* L;
};





////////////////      Parser function     ////////////////

template <typename Ret, typename... Args> struct Parser<Binder_Template<Ret, Args...>*>
{
    static Binder_Template<Ret, Args...>* get(lua_State* L, int32_t idx)
    {
        return lua_touserdata(L, lua_upvalueindex(1));
    }

    static void put(lua_State* L, Binder_Template<Ret, Args...>* v)
    {
        lua_pushlightuserdata(L, v);
        lua_pushcclosure(L, Binder_Template<Ret, Args...>::lua_callback, 1);
    }

    static bool is(lua_State* L, int32_t idx) { return lua_isfunction(L, idx); }
};

template <> struct Parser<Lua::Object*>
{
    static Lua::Object* get(lua_State* L, int32_t idx)
    {
        return new Lua::Object(L, idx);
    }

    static void put(lua_State* L, Lua::Object* object)
    {
        Lua::rawget(L, object->getRef());
        delete object;
    }

    static bool is(lua_State*, int32_t) { return true; }
};

}


#endif
