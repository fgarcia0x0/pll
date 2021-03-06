#pragma once

#include <string_view>
#include <optional>
#include <vector>
#include <unordered_map>
#include <cassert>
#include <token.hpp>

namespace pll
{
    /**
     * @brief 
     * 
     */
    class token_stream
    {
    public:
        using connective_props_map_type = std::unordered_map<token, connective_properties>;

        constexpr token_stream() = default;
        constexpr token_stream(const token_stream&) = delete;
        constexpr auto& operator=(const token_stream&) = delete;
        constexpr token_stream(token_stream&&) = default;

        constexpr token_stream(std::string_view input, 
                               std::string_view alphabet,
                               std::string_view connectives,
                               const connective_props_map_type& connective_props_map)
            : m_data_ptr{ input.begin() },
              m_alphabet{ alphabet },
              m_connectives{ connectives },
              m_connective_props_map_ptr{ &connective_props_map }
        {
            assert(m_connective_props_map_ptr);
        }
        
        // EndOfStream
        constexpr bool eos() noexcept
        {
            return *m_data_ptr == '\0';
        }

        [[nodiscard]] 
        std::optional<token> current_token() noexcept;

        [[maybe_unused]]
        std::optional<token> next_token() noexcept;
        
        template <typename OutputIter>
        void write(OutputIter output)
        {
            auto* save = m_data_ptr;

            while (!eos())
                *output++ = next_token();

            m_data_ptr = save;
        }

        std::optional<connective_properties> get_connective_props(token target) const noexcept
        {
            const auto search = m_connective_props_map_ptr->find(target);
            if (search != m_connective_props_map_ptr->cend())
                return search->second;
            else
                return std::nullopt; 
        }

        std::optional<token> operator++(int) = delete;

        [[maybe_unused]]
        std::optional<token> operator++()
        {
            return next_token();
        }

        std::optional<token> operator*()
        {
            return current_token();
        }

    private:
        mutable std::string_view::iterator m_data_ptr{};
        std::string_view m_alphabet{};
        std::string_view m_connectives{};
        const connective_props_map_type* m_connective_props_map_ptr{};
    };
}