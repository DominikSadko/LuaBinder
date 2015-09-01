#ifndef __OBJECT_LIB_H__
#define __OBJECT_LIB_H__

namespace Lua
{
class Object
{
    public:
        Object() {}

        // by global
        Object(lua_State* newState, const std::string& name)
        {
            L = newState;
            lua_getglobal(L, name.c_str());
            m_ref = Lua::ref(L);
        }

        Object(lua_State* newState, const char* name)
        {
            L = newState;
            lua_getglobal(L, name);
            m_ref = Lua::ref(L);
        }

        // by childs
        template <typename T>
        Object(Object& object, T field)
        {
            L = object.L;

            Lua::rawget(L, object.m_ref); // pop parent
            if(!lua_istable(L, -1))
            {
                Lua::pop(L);
                return;
            }

            Lua::Parser<T>::put(L, field);
            Lua::getTable(L, -2);

            m_ref = Lua::ref(L);
            Lua::pop(L); // remove rawget
        }

        // by index
        Object(lua_State* newState, int32_t idx)
        {
            L = newState;

            lua_pushvalue(L, idx);
            m_ref = Lua::ref(L);
        }

        // by LuaType - nil
        Object(lua_State* newState)
        {
            L = newState;
            Lua::Parser<std::nullptr_t>::put(L, nullptr);
            m_ref = Lua::ref(L);
        }

        // by LuaType
        Object(lua_State* newState, LuaType type)
        {
            L = newState;
            Lua::Parser<LuaType>::put(L, type);
            m_ref = Lua::ref(L);
        }

        ~Object()  { Lua::unref(L, m_ref); }

        /////////////////////////////////////////////////////////////////

        lua_State* state() const { return L; }
        int32_t getRef() const { return m_ref; }
        LuaType getType()
        {
            Lua::rawget(L, m_ref);
            LuaType t = (LuaType)lua_type(L, -1);
            Lua::pop(L); // remove rawget

            return t;
        }

        bool isFunction() { return getType() == LuaType::Function; }

        /////////////////////////////////////////////////////////////////

        operator bool()  const { return (m_ref != -1); }
        bool operator!() const { return (m_ref == -1); }

        template <typename T>
        Object& operator=(Object& object) { return object; }

        template <typename T>
        Object operator[](T field) { return Object(*this, field); }


        template <typename T>
        bool operator==(T value) { return (get<T>() == value); }

        bool operator==(const char* value) { return strcmp(get<const char*>(), value) == 0; }
        bool operator==(char* value) { return strcmp(get<char*>(), value) == 0; }

        /////////////////////////////////////////////////////////////////

        template <typename... Args>
        bool safeCall(const Args&... args)
        {
            Lua::rawget(L, m_ref);

            if(!lua_isfunction(L, -1))
            {
            	LOGE("is not function");
                return false;
            }

            Lua::Parser<void>::push(L, args...);
            if(lua_pcall(L, sizeof...(Args), 0, 0))
            {
                lua_error(L);
                return false;
            }

            return true;
        }

        template <typename... Args>
        void call(const Args&... args) { safeCall(args...); }

        template <typename Ret, typename... Args>
        Ret call(const Args&... args)
        {
            Lua::rawget(L, m_ref);

            if(!lua_isfunction(L, -1))
            	LOGE("is not function");

            Lua::Parser<void>::push(L, args...);
            if(lua_pcall(L, sizeof...(Args), 1, 0))
                lua_error(L);

            Ret ret = Lua::Parser<Ret>::get(L, -1);
            Lua::pop(L); // remove ret
            return ret;
        }

        /////////////////////////////////////////////////////////////////

        template <typename V>
        void set(V v)
        {
            Lua::unref(L, m_ref);
            Lua::Parser<V>::put(L, v);
            m_ref = Lua::ref(L);
        }

        template <typename K, typename V>
        void set(K k, V v)
        {
            Lua::rawget(L, m_ref);

            Lua::Parser<K>::put(L, k);
            Lua::Parser<V>::put(L, v);

            Lua::setTable(L, -3);
            Lua::pop(L); // remove rawget
        }


        template <typename T>
        T get()
        {
            Lua::rawget(L, m_ref); // pop parent
            T ret = Lua::Parser<T>::get(L, -1);
            Lua::pop(L); // remove rawget
            return ret;
        }

    private:
        lua_State* L;
        int m_ref;
};
}

#endif
