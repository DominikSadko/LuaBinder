#ifndef __TUPLER_LIB_H__
#define __TUPLER_LIB_H__

namespace Lua
{
    ////////////////      Pop tuple     ////////////////
    template<int Size>
    struct tupler_pop
    {
        template<typename Tuple>
        tupler_pop(lua_State* L, Tuple& tuple)
        {
            get(L, std::get<Size - 1>(tuple), -(std::tuple_size<Tuple>::value - Size) - 1);
            tupler_pop<Size - 1>(L, tuple);
        }

        private:
            template<typename T>
            static void get(lua_State* L, T& value, int idx)
                { value = Lua::Parser<T>::get(L, idx); }
    };

    template<>
    struct tupler_pop<0>
    {
        template<typename Tuple>
        tupler_pop(lua_State*, Tuple&) { }
    };


    ////////////////      Call tuple     ////////////////
    template <typename Tuple, typename Lambda, bool Done, int32_t Total, int32_t... N>
    struct _tupler_
    {
        static int call(Lambda& lambda, Tuple&& t)
            { return _tupler_<Tuple, Lambda, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(lambda, std::forward<Tuple>(t)); }
    };


    template <typename Tuple, typename Lambda, int32_t Total, int32_t... N>
    struct _tupler_<Tuple, Lambda, true, Total, N...>
    {
        static int call(Lambda& lambda, Tuple&& t)
            { return lambda(std::get<N>(std::forward<Tuple>(t))...); }
    };

    template <typename Tuple, typename Lambda>
    int tupler_call(Lambda& lambda, Tuple&& t)
    {
        typedef typename std::decay<Tuple>::type type;
        return _tupler_<Tuple, Lambda, 0 == std::tuple_size<type>::value, std::tuple_size<type>::value>::call(lambda, std::forward<Tuple>(t));
    }
}


#endif
