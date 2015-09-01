#ifndef __PARSER_LIB_H__
#define __PARSER_LIB_H__

namespace Lua
{


template<typename T>
struct HasMetatableMethod
{
    template<typename U, std::string (U::*)() const> struct TYPE {};
    template<typename U> static char CHECK(TYPE<U, &U::getMetatable>*);
    template<typename U> static int CHECK(...);
    static const bool Has = sizeof(CHECK<T>(0)) == sizeof(char);
};

template<typename T>
inline std::string getMetatableMethod(const T* t, std::true_type)
{
    return t->getMetatable();
}

template<typename T>
inline std::string getMetatableMethod(const T*, std::false_type)
{
    return "";
}

template<typename T>
inline std::string getMetatableMethod(const T* t)
{
    return getMetatableMethod(t, std::integral_constant<bool, HasMetatableMethod<T>::Has>());
}

template <typename T, class Enable = void>
struct Parser { };

template <> struct Parser<void>
{
    static void push(lua_State*) {}

    template<typename T, typename... Args>
    static void push(lua_State* L, const T& v, const Args&... args)
    {
        Parser<T>::put(L, v);
        Parser<void>::push(L, args...);
    }
};

template <> struct Parser<std::nullptr_t>
{
    static std::nullptr_t get(lua_State*, int32_t) { return nullptr; }
    static void put(lua_State* L, std::nullptr_t) { lua_pushnil(L); }
    static bool is(lua_State* L, int32_t idx) { return lua_isnil(L, idx); }
};

////////////////      Integers     ////////////////

template <> struct Parser<bool>
{
    static bool    get(lua_State* L, int32_t idx)  { return lua_toboolean(L, idx); }
    static void    put(lua_State* L, bool v)       { lua_pushboolean(L, v); }
    static bool     is(lua_State* L, int32_t idx)  { return lua_isboolean(L, idx); }
};

template <> struct Parser<const bool>
{
    static bool get(lua_State* L, int32_t idx)  { return lua_toboolean(L, idx); }
    static void put(lua_State* L, const bool v) { lua_pushboolean(L, v); }
    static bool is(lua_State* L, int32_t idx)   { return lua_isboolean(L, idx); }
};

template <> struct Parser<uint8_t>
{
    static uint8_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, uint8_t v)    { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)  { return lua_isnumber(L, idx); }
};

template <> struct Parser<const uint8_t>
{
    static uint8_t get(lua_State* L, int32_t idx)      { return lua_tointeger(L, idx); }
    static void    put(lua_State* L, const uint8_t v)  { lua_pushinteger(L, v); }
    static bool     is(lua_State* L, int32_t idx)      { return lua_isnumber(L, idx); }
};

template <> struct Parser<int8_t>
{
    static int8_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, int8_t v)    { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const int8_t>
{
    static int8_t get(lua_State* L, int32_t idx)     { return lua_tointeger(L, idx); }
    static void   put(lua_State* L, const int8_t v)  { lua_pushinteger(L, v); }
    static bool   is(lua_State* L, int32_t idx)      { return lua_isnumber(L, idx); }
};

template <> struct Parser<int16_t>
{
    static int16_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, int16_t v)    { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const int16_t>
{
    static int16_t get(lua_State* L, int32_t idx)      { return lua_tointeger(L, idx); }
    static void    put(lua_State* L, const int16_t v)  { lua_pushinteger(L, v); }
    static bool     is(lua_State* L, int32_t idx)      { return lua_isnumber(L, idx); }
};

template <> struct Parser<uint16_t>
{
    static uint16_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, uint16_t v)    { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const uint16_t>
{
    static uint16_t get(lua_State* L, int32_t idx)      { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, const uint16_t v) { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)      { return lua_isnumber(L, idx); }
};

template <> struct Parser<int32_t>
{
    static int32_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void    put(lua_State* L, int32_t v)     { lua_pushinteger(L, v); }
    static bool     is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const int32_t>
{
    static int32_t get(lua_State* L, int32_t idx)     { return lua_tointeger(L, idx); }
    static void    put(lua_State* L, const int32_t v) { lua_pushinteger(L, v); }
    static bool     is(lua_State* L, int32_t idx)     { return lua_isnumber(L, idx); }
};

template <> struct Parser<uint32_t>
{
    static uint32_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, uint32_t v)    { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const uint32_t>
{
    static uint32_t get(lua_State* L, int32_t idx)      { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, const uint32_t v) { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)      { return lua_isnumber(L, idx); }
};

template <> struct Parser<int64_t>
{
    static int64_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void    put(lua_State* L, int64_t v)     { lua_pushinteger(L, v); }
    static bool     is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const int64_t>
{
    static int64_t get(lua_State* L, int32_t idx)     { return lua_tointeger(L, idx); }
    static void    put(lua_State* L, const int64_t v) { lua_pushinteger(L, v); }
    static bool     is(lua_State* L, int32_t idx)     { return lua_isnumber(L, idx); }
};

template <> struct Parser<uint64_t>
{
    static uint64_t get(lua_State* L, int32_t idx)   { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, uint64_t v)    { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const uint64_t>
{
    static uint64_t get(lua_State* L, int32_t idx)      { return lua_tointeger(L, idx); }
    static void     put(lua_State* L, const uint64_t v) { lua_pushinteger(L, v); }
    static bool      is(lua_State* L, int32_t idx)      { return lua_isnumber(L, idx); }
};

template <> struct Parser<float>
{
    static float   get(lua_State* L, int32_t idx)   { return (float)lua_tonumber(L, idx); }
    static void    put(lua_State* L, float v)       { lua_pushnumber(L, (double)v); }
    static bool     is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const float>
{
    static float get(lua_State* L, int32_t idx)   { return (float)lua_tonumber(L, idx); }
    static void  put(lua_State* L, const float v) { lua_pushnumber(L, (double)v); }
    static bool  is(lua_State* L, int32_t idx)    { return lua_isnumber(L, idx); }
};

template <> struct Parser<double>
{
    static double  get(lua_State* L, int32_t idx)   { return lua_tonumber(L, idx); }
    static void    put(lua_State* L, double v)      { lua_pushnumber(L, v); }
    static bool     is(lua_State* L, int32_t idx)   { return lua_isnumber(L, idx); }
};

template <> struct Parser<const double>
{
    static double get(lua_State* L, int32_t idx)    { return lua_tonumber(L, idx); }
    static void   put(lua_State* L, const double v) { lua_pushnumber(L, v); }
    static bool   is(lua_State* L, int32_t idx)     { return lua_isnumber(L, idx); }
};

////////////////      Strings     ////////////////
template <> struct Parser<char[]>
{
    static char*   get(lua_State* L, int32_t idx)    { char* ret = const_cast<char*>(luaL_tolstring(L, idx, NULL)); Lua::pop(L); return ret; }
    static void    put(lua_State* L, char* v)        { lua_pushstring(L, v); }
    static bool     is(lua_State* L, int32_t idx)    { return lua_isstring(L, idx); }
};

template <> struct Parser<char*>
{
    static char*   get(lua_State* L, int32_t idx)    { char* ret = const_cast<char*>(luaL_tolstring(L, idx, NULL)); Lua::pop(L); return ret; }
    static void    put(lua_State* L, char* v)        { lua_pushstring(L, v); }
    static bool     is(lua_State* L, int32_t idx)    { return lua_isstring(L, idx); }
};

template <> struct Parser<const char*>
{
    static const char* get(lua_State* L, int32_t idx)   { const char* ret = luaL_tolstring(L, idx, NULL); Lua::pop(L); return ret; }
    static void        put(lua_State* L, const char* v) { lua_pushstring(L, v); }
    static bool        is(lua_State* L, int32_t idx)    { return lua_isstring(L, idx); }
};

template <> struct Parser<std::string>
{
    static std::string get(lua_State* L, int32_t idx)   { const char* ret = luaL_tolstring(L, idx, NULL); Lua::pop(L); return std::string(ret); }
    static void        put(lua_State* L, const std::string v) { lua_pushstring(L, v.c_str()); }
    static bool        is(lua_State* L, int32_t idx)    { return lua_isstring(L, idx); }
};

template <> struct Parser<const std::string>
{
    static const std::string get(lua_State* L, int32_t idx)   { return std::string(lua_tostring(L, idx)); }
    static void              put(lua_State* L, const std::string v) { lua_pushstring(L, v.c_str()); }
    static bool              is(lua_State* L, int32_t idx)    { return lua_isstring(L, idx); }
};

////////////////      Classes     ////////////////
template <class T> struct Parser<T*>
{
    static int32_t LUA_INDEX(lua_State* L)
    {
        // -2 self
        // -1 key

        const char* key = lua_tostring(L, -1);

        // call Lua:
        // if(self.__stack[key]) then
        //     return self.__stack[key]
        // end

        lua_getfield(L, -2, "__stack");
        lua_getfield(L, -1, key);

        if((LuaType)lua_type(L, -1) != LuaType::Nil)
            return 1;

        Lua::pop(L, 2);

        // call Lua:
        // return self.__metatable[key]

        lua_getfield(L, -2, "__metatable");
        lua_getfield(L, -1, key);
        return 1;
    }

    static int32_t LUA_NEWINDEX(lua_State* L)
    {
        // -3 self
        // -2 key
        // -1 value

        const char* key = lua_tostring(L, -2);

        lua_getfield(L, -3, "__stack");
        lua_pushvalue(L, -1 - 1);
        lua_setfield(L, -2, key);

        return 0;
    }



    static T* get(lua_State* L, int32_t idx)
    {
        LuaType type = (LuaType)lua_type(L, idx);
        if(type == LuaType::UserData)
            return (T*)lua_touserdata(L, idx);
        else if(type == LuaType::LightUserData)
            return (T*)lua_touserdata(L, idx);
        else if(type == LuaType::Table)
        {
            lua_getfield(L, idx, "__userdata");
            T* data = (T*)lua_touserdata(L, -1);
            Lua::pop(L);
            return data;
        }

        return nullptr;
    }

    ///////////////////////////////////////////////////////

    static void put(lua_State* L, T* v, std::true_type)
    {
        if(!v)
        {
            Lua::Parser<std::nullptr_t>::put(L, nullptr);
            return;
        }

        int32_t ref = v->getRef();
        if(!ref)
        {
            std::string className = v->getMetatable();
            if(className.empty())
            {
                CLASS_NAME(T);
                className = class_name;
            }


            // local object = {}
            lua_newtable(L);                             // 1

            // object.__userdata = userdata
            lua_pushlightuserdata(L, (void*)v);          // 2
            lua_setfield(L, -2, "__userdata");           // 1

            // object.__stack = {}
            lua_newtable(L);                             // 2
            lua_setfield(L, -2, "__stack");              // 1

            // object.__metatable = className
            lua_getglobal(L, className.c_str());         // 2
            lua_setfield(L, -2, "__metatable");          // 1

            // thats all for 'object'
            // now we need 'metatable' for 'object'

            lua_newtable(L);                             // 2

            lua_pushcfunction(L, LUA_INDEX);             // 3
            lua_setfield(L, -2, "__index");              // 2

            lua_pushcfunction(L, LUA_NEWINDEX);          // 3
            lua_setfield(L, -2, "__newindex");           // 2

            lua_getglobal(L, className.c_str());         // 3
            lua_getfield(L, -1, "tostring");             // 4
            if(!lua_isnil(L, -1))
            {
                lua_setfield(L, -3, "__tostring");           // 3
            	Lua::pop(L);                                 // 2
            }
            else
            	Lua::pop(L, 2);                              // 2

            lua_setmetatable(L, -2);                     // 1

            ref = refShared(L);                          // 0
            v->setRef(ref, L);
        }

        rawgetShared(L, ref);                            // 1
    }

    static void put(lua_State* L, T* v, std::false_type)
    {
        if(!v)
        {
            Lua::Parser<std::nullptr_t>::put(L, nullptr);
            return;
        }

        lua_pushlightuserdata(L, (void*)v);

        CLASS_NAME(T);
        luaL_getmetatable(L, class_name.c_str());
        lua_setmetatable(L, -2);
    }

    static void put(lua_State* L, T* v)
    {
        put(L, v, std::is_base_of<Shared, T>());
    }

    static bool is(lua_State* L, int32_t idx) { return lua_isuserdata(L, idx); }
};

template<typename T>
struct Parser<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
    static T     get(lua_State* L, int32_t idx)  { return (T)lua_tointeger(L, idx); }
    static void  put(lua_State* L, T v)          { lua_pushinteger(L, (int32_t)v); }
    static bool   is(lua_State* L, int32_t idx)  { return lua_isboolean(L, idx); }
};

template <> struct Parser<lua_State*>
{
    static lua_State* get(lua_State* L, int32_t) { return L; }
    static void put(lua_State*, lua_State*) { }
    static bool is(lua_State*, int32_t) { return false; }
};

////////////////      vectors, lists, maps     ////////////////

template <typename V> struct Parser< std::vector<V> >
{
    static std::vector<V> get(lua_State* L, int32_t idx)
    {
        std::vector<V> vector;

        lua_pushnil(L);

        while(lua_next(L, idx - 1))
        {
            lua_pushvalue(L, -2);

            if(Parser<int32_t>::is(L, -1) && Parser<V>::is(L, -2))
                vector.push_back(Parser<V>::get(L, -2));

            Lua::pop(L, 2);
        }

        return vector;
    }

    static void put(lua_State* L, const std::vector<V>& v)
    {
        lua_newtable(L);

        int32_t id = 0;
        for(auto& it : v)
        {
            Lua::Parser<int32_t>::put(L, (++id));
            Lua::Parser<V>::put(L, it);

            lua_settable(L, -3);
        }
    }

    static bool is(lua_State* L, int32_t idx) { return lua_istable(L, idx); }
};

template <typename V> struct Parser< std::list<V> >
{
    static std::list<V> get(lua_State* L, int32_t idx)
    {
        std::list<V> list;

        lua_pushnil(L);

        while(lua_next(L, idx - 1))
        {
            lua_pushvalue(L, -2);

            if(Parser<int32_t>::is(L, -1) && Parser<V>::is(L, -2))
                list.push_back(Parser<V>::get(L, -2));

            Lua::pop(L, 2);
        }

        return list;
    }

    static void put(lua_State* L, const std::list<V>& v)
    {
        lua_newtable(L);

        int32_t id = 0;
        for(auto& it : v)
        {
            Lua::Parser<int32_t>::put(L, (++id));
            Lua::Parser<V>::put(L, it);

            lua_settable(L, -3);
        }
    }

    static bool is(lua_State* L, int32_t idx) { return lua_istable(L, idx); }
};

template <typename K, typename V> struct Parser< std::map<K, V> >
{
    static std::map<K, V> get(lua_State* L, int32_t idx)
    {
        std::map<K, V> map;

        lua_pushnil(L);

        while(lua_next(L, idx - 1))
        {
            lua_pushvalue(L, -2);
            map[Parser<K>::get(L, -1)] = Parser<V>::get(L, -2);
            Lua::pop(L, 2);
        }

        return map;
    }

    static void put(lua_State* L, const std::map<K, V>& v)
    {
        lua_newtable(L);

        for(auto& it : v)
        {
            Lua::Parser<K>::put(L, it.first);
            Lua::Parser<V>::put(L, it.second);

            lua_settable(L, -3);
        }
    }

    static bool is(lua_State* L, int32_t idx) { return lua_istable(L, idx); }
};

////////////////      functions     ////////////////

template <typename Ret, typename... Args>
struct Parser<std::function<Ret(Args...)>>
{
    static std::function<Ret(Args...)> get(lua_State* L, int32_t idx)
    {
        lua_pushvalue(L, idx);
        int function = Lua::ref(L);

        std::function<Ret(Args...)> fn = [L, function](const Args&... args)
                    {
                        Lua::rawget(L, function);
                        Lua::Parser<void>::push(L, args...);

                        if(lua_pcall(L, sizeof...(Args), 1, 0))
                            lua_error(L);

                        Ret ret = Lua::Parser<Ret>::get(L, -1);

                        Lua::pop(L); // remove ret
                        Lua::unref(L, function);
                        return ret;
                    };

        return fn;
    }

    static void put(lua_State* L, const bool v) { } // todo
    static bool is(lua_State* L, int32_t idx)   { return lua_isfunction(L, idx); }
};

template <typename... Args>
struct Parser<std::function<void(Args...)>>
{
    static std::function<void(Args...)> get(lua_State* L, int32_t idx)
    {
        lua_pushvalue(L, idx);
        int function = Lua::ref(L);

        std::function<void(Args...)> fn = [L, function](const Args&... args)
                    {
                        Lua::rawget(L, function);
                        Lua::Parser<void>::push(L, args...);

                        if(lua_pcall(L, sizeof...(Args), 0, 0))
                            lua_error(L);

                        Lua::unref(L, function);
                    };

        return fn;
    }

    static void put(lua_State* L, const bool v) { }  // todo
    static bool is(lua_State* L, int32_t idx)   { return lua_isfunction(L, idx); }
};

template <typename Ret, typename... Args>
struct Parser<std::function<Ret(Args...)>*>
{
    static std::function<Ret(Args...)>* get(lua_State* L, int32_t idx)
    {
        lua_pushvalue(L, idx);
        int function = Lua::ref(L);

        std::function<Ret(Args...)>* fn = new std::function<Ret(Args...)>([L, function](const Args&... args)
                    {
                        Lua::rawget(L, function);
                        Lua::Parser<void>::push(L, args...);

                        if(lua_pcall(L, sizeof...(Args), 1, 0))
                            lua_error(L);

                        Ret ret = Lua::Parser<Ret>::get(L, -1);
                        Lua::pop(L); // remove ret
                        return ret;
                    });

        Lua::addRefFunction(function, fn);
        return fn;
    }

    static void put(lua_State* L, const bool v) { }  // todo
    static bool is(lua_State* L, int32_t idx)   { return lua_isfunction(L, idx); }
};

template <typename... Args>
struct Parser<std::function<void(Args...)>*>
{
    static std::function<void(Args...)>* get(lua_State* L, int32_t idx)
    {
        lua_pushvalue(L, idx);
        int function = Lua::ref(L);

        std::function<void(Args...)>* fn = new std::function<void(Args...)>([L, function](const Args&... args)
                    {
                        Lua::rawget(L, function);
                        Lua::Parser<void>::push(L, args...);

                        if(lua_pcall(L, sizeof...(Args), 0, 0))
                            lua_error(L);
                    });

        Lua::addRefFunction(function, fn);
        return fn;
    }

    static void put(lua_State* L, const bool v) { }  // todo
    static bool is(lua_State* L, int32_t idx)   { return lua_isfunction(L, idx); }
};



template <> struct Parser<LuaType>
{
    static LuaType get(lua_State* L, int32_t idx) { return (LuaType)lua_type(L, idx); }
    static void put(lua_State* L, const LuaType& v)
    {
        switch(v)
        {
            case LuaType::Nil:
                lua_pushnil(L);
                break;
            case LuaType::Boolean:
                lua_pushboolean(L, false);
                break;
            case LuaType::Number:
                lua_pushnumber(L, 0);
                break;
            case LuaType::String:
                lua_pushstring(L, "");
                break;
            case LuaType::Table:
                lua_newtable(L);
                break;
            default:
                break;
        }
    }

    static bool is(lua_State*, int32_t) { return false; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    static void set(lua_State* L, const std::string& name, const T& value)
    {
        std::vector<std::string> explode = Lua::explodeString(name);
        lua_getglobal(L, explode[0].c_str());

        if(explode.size() > 1)
        {
            for(auto it = explode.begin() + 1; it != explode.end() - 1; it++)
                lua_getfield(L, -1, (*it).c_str());

            lua_pushstring(L, explode[explode.size() - 1].c_str());
            Lua::Parser<T>::put(L, value);
            lua_settable(L, -3);
        }
        else
        {
            Lua::Parser<T>::put(L, value);
            lua_setglobal(L, name.c_str());
        }

        Lua::pop(L); // remove global
        ASSERT_TOP(L);
    }

    template <typename T>
    static T get(lua_State* L, const std::string& name)
    {
        std::vector<std::string> explode = Lua::explodeString(name);
        lua_getglobal(L, explode[0].c_str());

        if(explode.size() > 1)
        {
            for(auto it = explode.begin() + 1; it != explode.end(); it++)
                lua_getfield(L, -1, (*it).c_str());
        }

        T ret =  Lua::Parser<T>::get(L, -1);
        Lua::pop(L, explode.size());
        ASSERT_TOP(L);
        return ret;
    }
}
#endif
