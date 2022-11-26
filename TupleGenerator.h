#ifndef DEAMER_META_FUNCTION_TUPLE_GENERATOR_H
#define DEAMER_META_FUNCTION_TUPLE_GENERATOR_H

#include <type_traits>
#include <variant>
#include <tuple>

// Meta Function for constructing Tuples
// CoreType defines the set of possible Types
//
// TupleGenerator::type counts defines the total desired types of a specific CoreType, in order.
//
// Example:
//  TupleGenerator<int, double, char>
//      ::type<1, 2, 3> == std::tuple<int, double, double, char, char, char>
//
template<typename... CoreType>
struct TupleGenerator
{
    template<typename T>
    struct IsType
    {
        using type = T;
    };
    
    template<int input, int check, typename lhs, typename rhs>
    struct If : IsType<typename rhs::type> {};

    template<int input, typename lhs, typename rhs>
    struct If<input, input, lhs, rhs> : IsType<lhs> {};

    template<int index, typename Type, typename... Rest>
    struct GetIndexType
    {
        using rhs = GetIndexType<index - 1, Rest..., std::monostate>;

        // If index < 0; Then LHS
        // If rest size == 0 Then LHS
        // Else RHS
        using type = typename If<(index < 0) || sizeof...(Rest) == 0, 1, Type, rhs>::type;
    };

    template<int index>
    struct GetIndexType<index, std::monostate>
    {
        using type = int;
    };

    template<int index>
    struct GetIndex
    {
        using type = typename GetIndexType<index - 1, CoreType...>::type;
    };

    template<typename... StoredTs>
    struct TypeCache
    {
        using type = std::tuple<StoredTs...>;
    };

    template<typename newT, typename... typeCache>
    static auto ConstructTypeCache_(std::tuple<typeCache...>)
    {
        return TypeCache<typeCache..., newT>{};
    }

    template<typename newT, typename originalTypeCache>
    using ConstructTypeCache = decltype(
        ConstructTypeCache_<newT>(
                typename originalTypeCache::type{}
            )
        );

    template<typename typeCache, int index, int count, int... counts>
    static auto InternalRepeat()
    {
        if constexpr(count == 0)
        {
            if constexpr (sizeof...(counts) > 0)
            {
                return InternalRepeat<typeCache, index + 1, counts...>();
            }
            else
            {
                return typename typeCache::type{};
            }
        }
        else
        {
            // return InternalRepeat<TypeCache<typename GetIndex<index>::type>, index, count - 1, counts...>();
            return InternalRepeat<ConstructTypeCache<typename GetIndex<index>::type, typeCache>, index, count - 1, counts...>();
        }
    }

    template<int... counts>
    using type = decltype(InternalRepeat<TypeCache<>, 0, counts...>());
};

#endif // DEAMER_META_FUNCTION_TUPLE_GENERATOR_H