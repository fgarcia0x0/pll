#pragma once
#include <token.hpp>
#include <token_stream.h>

[[nodiscard]]
static inline auto conn_extract_props(pll::token token, 
                                      const pll::token_stream& stream) noexcept
{
    return stream.get_connective_props(token);
}

[[nodiscard]]
static inline std::optional<pll::connective_properties> 
conn_extract_props(pll::token target, const pll::connective_prop_map& conn_map) noexcept
{
    const auto search = conn_map.find(target);
    if (search != conn_map.cend())
        return search->second;
    else
        return std::nullopt;
}

[[nodiscard]]
static bool check_operation_mode(pll::token token, 
                                 const pll::token_stream& stream,
                                 pll::operation_mode target_mode) noexcept
{
    auto props = conn_extract_props(token, stream);
    if (!props)
        return false;

    return props->mode == target_mode;
}

[[nodiscard]]
static bool check_associativity(pll::token token, 
                                 const pll::token_stream& stream,
                                 pll::associativity assoc) noexcept
{
    auto props = conn_extract_props(token, stream);
    if (!props)
        return false;

    return props->assoc == assoc;
}

[[nodiscard]]
static bool is_binary_op(pll::token token, 
                         const pll::token_stream& stream) noexcept
{
    return check_operation_mode(token, stream, pll::operation_mode::binary);
}

[[nodiscard]]
static bool is_unary_op(pll::token token, 
                        const pll::token_stream& stream) noexcept
{
     return check_operation_mode(token, stream, pll::operation_mode::unary);
}

[[nodiscard]]
static bool is_left_assoc(pll::token token, 
                          const pll::token_stream& stream) noexcept
{
     return check_associativity(token, stream, pll::associativity::left);
}

[[nodiscard]]
static bool is_right_assoc(pll::token token, 
                          const pll::token_stream& stream) noexcept
{
     return check_associativity(token, stream, pll::associativity::right);
}

static bool is_connective_type(pll::token target,
                               const pll::connective_prop_map& conn_map,
                               pll::connective_type conn_type)
{
    auto conn_prop = conn_extract_props(target, conn_map);
    if (conn_prop)
        return conn_prop->type == conn_type;
    else
        return false;
}

static pll::token extract_token(pll::connective_type type,
                                const pll::connective_prop_map& conn_map)
{
    auto iter = std::find_if(conn_map.cbegin(), conn_map.cend(), [type](const auto& elem)
    {
        return elem.second.type == type;
    });

    return iter->first;
}