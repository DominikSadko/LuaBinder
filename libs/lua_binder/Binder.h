#ifndef __BINDER_LIB_H__
#define __BINDER_LIB_H__

template<typename T> struct remove_const_ref { typedef typename std::remove_const<typename std::remove_reference<T>::type>::type type; };

template <typename Ret, typename... Args>
struct Binder_Template
{
    public:
        Binder_Template() { m_name = ""; }
        virtual ~Binder_Template() { };

        virtual void init(lua_State* L, const std::string& name)
        {
            m_name = name;
            Lua::set(L, name, this);
            ASSERT_TOP(L);
        }

        virtual void remove(lua_State* L)
        {
            Lua::set(L, m_name, nullptr);
            delete this;
            ASSERT_TOP(L);
        }

        virtual int32_t lua_invoke(lua_State* L)
        {
            typedef typename std::tuple<typename remove_const_ref<Args>::type...> Tuple;
            Tuple tuple;
            Lua::tupler_pop<sizeof...(Args)>(L, tuple);
            Lua::pop(L, sizeof...(Args));

            int32_t ret = Lua::tupler_call(m_lambda, tuple);

            if(!Lua::m_weakFunctions.empty())
            {
                for(auto it = Lua::m_weakFunctions.begin(); it != Lua::m_weakFunctions.end(); it++)
                    Lua::unref(L, *it);

                Lua::m_weakFunctions.clear();
            }

            return ret;
        }

        static int32_t lua_callback(lua_State* L)
        {
            auto binder = (Binder_Template<Ret, Args...>*)lua_touserdata(L, lua_upvalueindex(1));
            return binder->lua_invoke(L);
        }

    protected:
        std::function<int(Args...)> m_lambda;
        std::string m_name;
};


template <class FC, typename Ret, typename... Args>
class Binder { };

////////////////      Static functions     ////////////////

template <typename Ret, typename... Args>
struct Binder<void, Ret, Args...> : public Binder_Template<Ret, Args...>
{
    // function(args...)
    Binder(lua_State* L, Ret(*function)(Args... args), const std::string& name)
    {
        Binder_Template<Ret, Args...>::init(L, name);
        Binder_Template<Ret, Args...>::m_lambda = std::function<int(Args...)>([L, function, this]
                (Args... args)
                {
                    Lua::Parser<Ret>::put(L, function(args...));
                    return 1;
                });
    };

    // std::function(args...)
    Binder(lua_State* L, std::function<Ret(Args...)> function, const std::string& name)
    {
        Binder_Template<Ret, Args...>::init(L, name);
        Binder_Template<Ret, Args...>::m_lambda = std::function<int(Args...)>([L, function, this]
                (Args... args)
                {
                    Lua::Parser<Ret>::put(L, function(args...));
                    return 1;
                });
    };

    ~Binder() { };
};

template <typename... Args>
struct Binder<void, void, Args...> : public Binder_Template<void, Args...>
{
    // function(args...)
    Binder(lua_State* L, void(*function)(Args... args), const std::string& name)
    {
        Binder_Template<void, Args...>::init(L, name);
        Binder_Template<void, Args...>::m_lambda = std::function<int(Args...)>([L, function, this]
                (Args... args)
                {
                    function(args...);
                    return 0;
                });
    };

    // std::function(args...)
    Binder(lua_State* L, std::function<void(Args...)> function, const std::string& name)
    {
        Binder_Template<void, Args...>::init(L, name);
        Binder_Template<void, Args...>::m_lambda = std::function<int(Args...)>([L, function, this]
                (Args... args)
                {
                    function(args...);
                    return 0;
                });
    };

    ~Binder() { };
};





////////////////      class instance functions     ////////////////
template <class FC, typename Ret, typename... Args>
struct Binder<FC*, Ret, Args...> : public Binder_Template<Ret, Args...>
{
    FC* m_instance;

    // Class::Function(args...) const
    Binder(lua_State* L, Ret (FC::*function)(Args... args), FC* instance, const std::string& name)
    {
        auto mf = std::mem_fn(function);
        m_instance = instance;

        Binder_Template<Ret, Args...>::init(L, name);
        Binder_Template<Ret, Args...>::m_lambda = std::function<int(Args...)>([L, mf, this]
                (Args... args)
                {
                    if(m_instance)
                        Lua::Parser<Ret>::put(L, mf(m_instance, args...));
                    else
                    {
                        FC* instance = Lua::Parser<FC*>::get(L, -1);
                        if(!instance)
                        	LOGE("[0x01] instance not found.");
                        else
                        	Lua::Parser<Ret>::put(L, mf(instance, args...));
                    }

                    return 1;
                });
    };

    // Class::Function(args...) const
    Binder(lua_State* L, Ret (FC::*function)(Args... args) const, FC* instance, const std::string& name)
    {
        auto mf = std::mem_fn(function);
        m_instance = instance;

        Binder_Template<Ret, Args...>::init(L, name);
        Binder_Template<Ret, Args...>::m_lambda = std::function<int(Args...)>([L, mf, this]
                (Args... args)
                {
                    if(m_instance)
                        Lua::Parser<Ret>::put(L, mf(m_instance, args...));
                    else
                    {
                        FC* instance = Lua::Parser<FC*>::get(L, -1);
                        if(!instance)
                        	LOGE("[0x02] instance not found.");
                        else
                        	Lua::Parser<Ret>::put(L, mf(instance, args...));
                    }

                    return 1;
                });
    };

    void setInstance(FC* instance) { m_instance = instance; }

    ~Binder() { };
};

template <class FC, typename... Args>
struct Binder<FC*, void, Args...> : public Binder_Template<void, Args...>
{
    FC* m_instance;

    // Class::Function(args...)
    Binder(lua_State* L, void (FC::*function)(Args... args), FC* instance, const std::string& name)
    {
        auto mf = std::mem_fn(function);
        m_instance = instance;

        Binder_Template<void, Args...>::init(L, name);
        Binder_Template<void, Args...>::m_lambda = std::function<int(Args...)>([L, mf, this]
                (Args... args)
                {
                    if(m_instance)
                        mf(m_instance, args...);
                    else
                    {
                        FC* instance = Lua::Parser<FC*>::get(L, -1);
                        if(!instance)
                        	LOGE("[0x03] instance not found.");
                        else
                        	mf(instance, args...);
                    }

                    return 0;
                });
    };

    // Class::Function(args...) const
    Binder(lua_State* L, void (FC::*function)(Args... args) const, FC* instance, const std::string& name)
    {
        auto mf = std::mem_fn(function);
        m_instance = instance;

        Binder_Template<void, Args...>::init(L, name);
        Binder_Template<void, Args...>::m_lambda = std::function<int(Args...)>([L, mf, this]
                (Args... args)
                {
                    if(m_instance)
                        mf(m_instance, args...);
                    else
                    {
                        FC* instance = Lua::Parser<FC*>::get(L, -1);
                        if(!instance)
                        	LOGE("[0x04] instance not found.");
                        else
                        	mf(instance, args...);
                    }

                    return 0;
                });
    };

    void setInstance(FC* instance) { m_instance = instance; }

    ~Binder() { };
};

#endif
