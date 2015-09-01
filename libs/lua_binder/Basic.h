#ifndef __BASIC_LIB_H__
#define __BASIC_LIB_H__

#ifndef LOGE
	#define LOGE(...) ({ printf(__VA_ARGS__); })
#endif

#ifndef LOGI
	#define LOGI(...) ({ printf(__VA_ARGS__); })
#endif

#ifndef LOGD
	#define LOGD(...) ({ printf(__VA_ARGS__); })
#endif

#define LUA_ERROR(...) ({ LOGE(__VA_ARGS__); })
#define ASSERT_TOP(L) ({ assert(lua_gettop(L) == 0); })
#define ASSERT_TOPS(L, N) ({ assert(lua_gettop(L) == N); })
#define DEBUG_TOP(L) ({ std::cout << "top: " << lua_gettop(L) << std::endl; })

#define lua_error(L) ({ LOGE("%s\n", lua_tostring(L, -1)); })


#define CLASS_NAME(T) int status; \
        char* n = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status); \
        std::string class_name(n); \
        delete n;


enum class LuaType {
    Nil = 0,
    Boolean = 1,
    LightUserData = 2,
    Number = 3,
    String = 4,
    Table = 5,
    Function = 6,
    UserData = 7,
    Thread = 8,
};

namespace Lua
{
    inline std::vector<std::string> explodeString(const std::string& str)
    {
        std::vector<std::string> explode;

        std::size_t lastpos = 0, pos = str.find(".");
        if(pos == std::string::npos)
            explode.push_back(str);

        while(pos != std::string::npos)
        {
            explode.push_back(std::string(str, lastpos, pos - lastpos).c_str());

            lastpos = ++pos;
            if((pos = str.find(".", pos)) == std::string::npos)
                explode.push_back(std::string(str, lastpos, pos - lastpos).c_str());
        }

        return explode;
    }

    inline void pop(lua_State* L, int idx = 1) { if(lua_gettop(L)) lua_pop(L, idx); }
    inline void getTable(lua_State* L, int idx = -2) { lua_gettable(L, idx); }
    inline void setTable(lua_State* L, int idx = -3) { lua_settable(L, idx); }
/*
    inline int  ref(lua_State* L, int idx = LUA_REGISTRYINDEX) { return luaL_ref(L, idx); }
    inline void unref(lua_State* L, int t, int idx = LUA_REGISTRYINDEX) { luaL_unref(L, idx, t); }
    inline void rawget(lua_State* L, int t, int idx = LUA_REGISTRYINDEX) { lua_rawgeti(L, idx, t); }
*/

    inline int  refShared(lua_State* L)
    {
        static int __refid = 0;
        if(__refid == 2147483647)
            __refid = 0;

        lua_getglobal(L, "__shared");

        lua_pushvalue(L, -2);
        lua_rawseti(L, -2, ++__refid);
        Lua::pop(L, 2);

        return __refid;
    }

    inline void unrefShared(lua_State* L, int t)
    {
        lua_getglobal(L, "__shared");
        lua_pushnil(L);
        lua_rawseti(L, -2, t);
        Lua::pop(L, 1);
    }

    inline void rawgetShared(lua_State* L, int t)
    {
        lua_getglobal(L, "__shared");
        lua_rawgeti(L, -1, t);
        lua_remove(L, -2);
    }

    //////////////////////////////////////////////////////

    inline int  ref(lua_State* L)
    {
        static int __refid = 0;
        if(__refid == 2147483647)
            __refid = 0;

        lua_getglobal(L, "__refs");

        lua_pushvalue(L, -2);
        lua_rawseti(L, -2, ++__refid);
        Lua::pop(L, 2);

        return __refid;
    }

    inline void unref(lua_State* L, int t)
    {
        lua_getglobal(L, "__refs");
        lua_pushnil(L);
        lua_rawseti(L, -2, t);
        Lua::pop(L, 1);
    }

    inline void rawget(lua_State* L, int t)
    {
        lua_getglobal(L, "__refs");
        lua_rawgeti(L, -1, t);
        lua_remove(L, -2);
    }



    static std::vector<int32_t> m_weakFunctions;
    static std::map<int32_t, void*> m_functions;

    template <typename Ret, typename... Args>
    inline void addRefFunction(int32_t id, std::function<Ret(Args...)>* fn)
    {
        Lua::m_functions[id] = (void*)fn;
    }

    template <typename Ret, typename... Args>
    inline bool remove(lua_State* L, std::function<Ret(Args...)>* fn)
    {
        for(auto it = Lua::m_functions.begin(); it != Lua::m_functions.end(); it++)
        {
            if(it->second == (void*)fn)
            {
                Lua::unref(L, it->first);
                Lua::m_functions.erase(it);
                delete fn;
                return true;
            }
        }

        return false;
    }
}


#endif
