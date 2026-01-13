#pragma once
#include "Column.hpp"
#include <unordered_map>


namespace Kodiak {

    //this will be our heterogenous vector, we will have a map of const DataVector to a std::vector of type T
    class DataVector {
        using ClearFunction = void(*)(DataVector&);
        using CopyFunction = void(*)(const DataVector&, DataVector&);
        using MoveFunction = void(*)(DataVector&, DataVector&);

        public:

        DataVector() = default;
        //given we don't actually dynamically manage objects this may not be the best idea
        DataVector(const DataVector& other)
        {
            *this = other;
        }

        DataVector(DataVector&& other)
        {
            *this = std::move(other);
        }

        DataVector& operator=(const DataVector& other)
        {
            if (this != &other){
                copyFunctions_ = other.copyFunctions_;
                clearFunctions_ = other.clearFunctions_;
                moveFunctions_ = other.moveFunctions_;

                for (auto&& copyFunc : copyFunctions_){
                    copyFunc(other,*this);
                }
            }
            return *(this);
        }

        DataVector& operator=(DataVector&& other)
        {
            if (this != &other){
                clearFunctions_ = std::move(other.clearFunctions_);
                copyFunctions_ = std::move(other.copyFunctions_);
                moveFunctions_ = std::move(other.moveFunctions_);

                for (auto&& moveFunction : moveFunctions_){
                    moveFunction(other,*this);
                }
            }
        }

        ~DataVector() { clear();}

        template<typename T>
        auto push_back(const T& v){
            get_column<T>().push_back(v);
        }

        template<typename T>
        auto reserve(size_t size){
            get_column<T>().reserve(size);
        }

        template<typename T>
        auto size(){
            get_column<T>().getSize();
        }

        template<typename T, typename... Args>
        auto emplace_back(Args&&... args){
            get_column<T>().push_back(std::forward<Args>(args)...);
        }

        void clear()
        {
            for (auto&& clearFunc : clearFunctions_)
            {
                clearFunc(*this);
            }
        }

        template <typename T>
        [[nodiscard]] Column<T>& get_column(){
            auto iter = vectors_<T>.find(this);
            if (iter == vectors_<T>.end())
            {
                clearFunctions_.push_back([](DataVector& instance){ vectors_<T>.erase(instance);});

                copyFunctions_.push_back([](const DataVector& from, DataVector& to){ vectors_<T>[&to] = vectors_<T>[&from];});

                moveFunctions_.push_back([](DataVector& from, DataVector& to){vectors_<T>[&to] = std::move(vectors_<T>[&from]);});

            }
            return iter->second;
        }

        template <typename T>
        [[nodiscard]] const Column<T>& get_column() const {
            return vectors_<T>[this];
        }

        private:
        std::vector<ClearFunction> clearFunctions_;
        std::vector<CopyFunction> copyFunctions_;
        std::vector<MoveFunction> moveFunctions_;

        template<typename T>
        inline static std::unordered_map<const DataVector*, Column<T>> vectors_{};

    };
};
