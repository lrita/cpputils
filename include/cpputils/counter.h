// MIT License
//
// Copyright (c) 2022 Martin Leitner-Ankerl
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once
#include <fmt/core.h> // for format_context, format_parse_context, format_to

#include <cstddef>     // for size_t
#include <cstring>     // for memcmp
#include <functional>  // for hash
#include <iosfwd>      // for ostream
#include <string>      // for allocator, string
#include <string_view> // for hash, string_view
#include <type_traits>

namespace cpputils {
class counter {
public:
    struct data_t {
        size_t m_ctor{};
        size_t m_default_ctor{};
        size_t m_copy_ctor{};
        size_t m_dtor{};
        size_t m_assign{};
        size_t m_swaps{};
        size_t m_get{};
        size_t m_const_get{};
        size_t m_hash{};
        size_t m_equals{};
        size_t m_less{};
        size_t m_move_ctor{};
        size_t m_move_assign{};

        friend auto operator==(data_t const& a, data_t const& b) -> bool {
            static_assert(std::has_unique_object_representations_v<data_t>);
            return 0 == std::memcmp(&a, &b, sizeof(data_t));
        }

        friend auto operator!=(data_t const& a, data_t const& b) -> bool {
            return !(a == b);
        }
    };

    counter(counter const&) = delete;
    counter(counter&&) = delete;
    auto operator=(counter const&) -> counter& = delete;
    auto operator=(counter&&) -> counter&& = delete;

    // Obj for only swaps & equals. Used for optimizing.
    // Can't use static counters here because I want to do it in parallel.
    class obj {
    public:
        // required for operator[]
        obj();
        obj(const size_t& data, counter& counts);
        obj(const obj& o);
        obj(obj&& o) noexcept;
        ~obj();

        auto operator==(const obj& o) const -> bool;
        auto operator<(const obj& o) const -> bool;
        auto operator=(const obj& o) -> obj&;
        auto operator=(obj&& o) noexcept -> obj&;

        [[nodiscard]] auto get() const -> size_t const&;
        auto get() -> size_t&;

        void swap(obj& other);
        [[nodiscard]] auto get_for_hash() const -> size_t;

    private:
        size_t m_data;
        counter* m_counts;
    };

    counter();
    ~counter();

    void check_all_done() const;

    [[nodiscard]] auto ctor() const -> size_t {
        return m_data.m_ctor;
    }

    [[nodiscard]] auto default_ctor() const -> size_t {
        return m_data.m_default_ctor;
    }

    [[nodiscard]] auto copy_ctor() const -> size_t {
        return m_data.m_copy_ctor;
    }

    [[nodiscard]] auto dtor() const -> size_t {
        return m_data.m_dtor;
    }

    [[nodiscard]] auto equals() const -> size_t {
        return m_data.m_equals;
    }

    [[nodiscard]] auto less() const -> size_t {
        return m_data.m_less;
    }

    [[nodiscard]] auto assign() const -> size_t {
        return m_data.m_assign;
    }

    [[nodiscard]] auto swaps() const -> size_t {
        return m_data.m_swaps;
    }

    [[nodiscard]] auto get() const -> size_t {
        return m_data.m_get;
    }

    [[nodiscard]] auto const_get() const -> size_t {
        return m_data.m_const_get;
    }

    [[nodiscard]] auto hash() const -> size_t {
        return m_data.m_hash;
    }

    [[nodiscard]] auto move_ctor() const -> size_t {
        return m_data.m_move_ctor;
    }

    [[nodiscard]] auto move_assign() const -> size_t {
        return m_data.m_move_assign;
    }

    [[nodiscard]] auto data() const -> data_t const& {
        return m_data;
    }

    friend auto operator<<(std::ostream& os, counter const& c) -> std::ostream&;

    void operator()(std::string_view title);

    [[nodiscard]] auto total() const -> size_t;

    static size_t static_default_ctor; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    static size_t static_dtor;         // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

private:
    data_t m_data{};

    std::string m_records =
        "\n     ctor  defctor  cpyctor     dtor   assign    swaps      get  cnstget     hash   equals     less   ctormv assignmv|   total |\n";
};
} // namespace cpputils

// Throws an exception, this overload should never be taken!
inline auto operator new(size_t s, cpputils::counter::obj* ptr) -> void*;

namespace std {

template <>
struct hash<cpputils::counter::obj> {
    [[nodiscard]] auto operator()(const cpputils::counter::obj& c) const noexcept -> size_t {
        return hash<size_t>{}(c.get_for_hash());
    }
};

} // namespace std

template <>
struct fmt::formatter<cpputils::counter::obj> {
    static constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }
    static auto format(cpputils::counter::obj const& o, fmt::format_context& ctx) -> decltype(ctx.out()) {
        return fmt::format_to(ctx.out(), "{}", o.get());
    }
};
