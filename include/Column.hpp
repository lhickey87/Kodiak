#pragma once
#include "Types.h"
#include <concepts>
#include <functional>
#include <stdexcept>
#include <type_traits>

#include <ranges>
#include <algorithm>
#include <vector>
#include <execution>

namespace Kodiak {


    template<typename T>
    class Column;

    template<typename T> struct is_column : std::false_type {};
    template<typename T> struct is_column<Column<T>> : std::true_type {};

    template<typename T>
    concept IsColumn = is_column<std::remove_cvref_t<T>>::value;

    template<typename T>
    struct get_type
    {
        using type = T;
    };

    template<IsColumn T>
    struct get_type<T>
    {
        using type = typename std::remove_cvref_t<T>::value_type;
    };

    template<typename OP, typename LHS, typename RHS>
    concept IsComparator = requires(OP op, const LHS& lhs,const RHS& rhs){
        {op(lhs,rhs)} -> std::same_as<bool>;
    };

    template<typename LHS, typename RHS, typename OP>
    auto binary_op(const LHS& lhs, const RHS& rhs, OP op) {
        constexpr bool L_col = IsColumn<LHS>;
        constexpr bool R_col = IsColumn<RHS>;

        using LType = get_type<LHS>::type;
        using RType = get_type<RHS>::type;

        using Common_T = std::common_type<LType,RType>;

        //if our operand is boolean (std::greater, etc) then type is bool, otherwise it needs to be shared
        // i.e if we have double and int, then common_type
        using Res_T = std::conditional<IsComparator<OP,LHS, RHS>, bool, Common_T>;

        size_t n = L_col ? lhs.size() : rhs.size();

        if constexpr (L_col && R_col) {
            if (lhs.size() != rhs.size())
                throw std::invalid_argument("Size mismatch");
        }

        Column<Res_T> out(n);

        auto idx = std::views::iota(size_t{0}, n);

        std::transform(std::execution::par_unseq,
            idx.begin(), idx.end(),
            out.data(),
            [&](size_t i) {
                auto l = L_col ? lhs[i] : lhs; //resolved at compile time
                auto r = R_col ? rhs[i] : rhs;
                return op(l, r);
            });

        return out;
    }

    template<typename L, typename R>
    auto operator+(const L& l, const R& r) {
        return binary_op(l, r, std::plus<>{});
    }

    template<typename L, typename R>
    auto operator-(const L& l, const R& r) {
        return binary_op(l, r, std::minus<>{});
    }

    template<typename L, typename R>
    auto operator*(const L& l, const R& r) {
        return binary_op(l, r, std::multiplies<>{});
    }

    template<typename L, typename R>
    auto operator/(const L& l, const R& r) {
        return binary_op(l, r, std::divides<>{});
    }

    template<typename L, typename R>
    auto operator>(const L& l, const R& r) {
        return binary_op(l, r, std::greater<>{});
    }

    template<typename L, typename R>
    auto operator<(const L& l, const R& r) {
        return binary_op(l, r, std::less<>{});
    }

    template<typename L, typename R>
    auto operator==(const L& l, const R& r) {
        return binary_op(l, r, std::equal_to<>{});
    }

    template<typename L, typename R>
    auto operator!=(const L& l, const R& r) {
        return binary_op(l, r, std::not_equal_to<>{});
    }

    template<typename T>
    class Column;

    template<>
    class Column<bool> {
        public:
        Column(size_t size) : data_(size), size_(size){}

        uint64_t* data(){
            return data_.data();
        }

        private:
        std::vector<uint64_t> data_;
        size_t size_;
    };

    template<Kodiak::Numerical T>
    class Column<T> {
        public:
        using value_type = T;
        Column(size_t size) : data_(size){}

        Column(Column<T>&& other) noexcept : data_(std::exchange(other.data_,nullptr), std::exchange(other.size,0)){}
        Column(const Column<T>& other) noexcept : data_(other.data_), size(other.size){}

        Column<T> operator=(const Column<T>& rhs) noexcept {
            if (this != &rhs){
                data_ = rhs.data_;
                size = rhs.size;
            }
            return *this;
        }

        Column<T> operator=(Column<T>&& rhs) noexcept {
            if (this != &rhs){
                data_ = std::exchange(rhs.data_,nullptr);
                size = std::exchange(rhs.size,0);
            }
            return *this;
        }

        auto sum();
        auto filter(bool cond);
        auto getColumn();

        size_t getSize() const noexcept {
            return size;
        }

        const T* data() const {
            return data_.data();
        }

        const auto begin() {
            return data_.begin();
        }

        const auto end() {
            return data_.end();
        }

        template<Kodiak::Numerical U>
        void push_back(const U& v){
            data_.push_back(v);
        }

        private:
        std::vector<T> data_;
        size_t size;
    };

    template<Kodiak::Categorical T>
    class Column<T> {
        public:
        using value_type = T;
        auto filter();
        auto getColumn();

        Column<T> operator+(const std::string_view);

        const T* data() {
            return data_.data();
        }

        T* begin(){
            return data_.begin();
        }

        T* end() {
            return data_.end();
        }

        private:
        std::vector<T> data_;
        size_t size_;
    };

};
