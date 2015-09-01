#ifndef __SHARED_H__
#define __SHARED_H__

namespace Lua
{
class Shared
{
    public:
        Shared()
        {
            m_ref = 0;
            L = nullptr;
            m_metatable = "";
        }

        virtual ~Shared()
        {
            if(L && m_ref)
                Lua::unrefShared(L, m_ref);

            m_ref = 0;
        }

        bool isRemoved() const { return !m_ref; }
        int32_t getRef() const { return m_ref; }
        void setRef(int32_t ref, lua_State* state)
        {
            m_ref = ref;
            L = state;
        }

        void setMetatable(const std::string& metatable)
        {
            m_metatable = metatable;

            if(L && m_ref)
            {
                rawgetShared(L, m_ref);

                lua_getglobal(L, m_metatable.c_str());
                lua_setfield(L, -2, "__metatable");

                Lua::pop(L);
            }
        }

        std::string getMetatable() const { return m_metatable; }

    private:
        int32_t m_ref;
        lua_State* L;
        std::string m_metatable;
};

}

#endif // __SHARED_H__ //
