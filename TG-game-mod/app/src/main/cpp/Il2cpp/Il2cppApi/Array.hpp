#pragma once

#include <cstdint>

#include "Field.hpp"
#include "List.hpp"
#include "String.hpp"
#include "../il2cpp-appdata.h"

namespace Il2CppApi {
    template <typename T>
    class Field;
    template <typename>
    class List;

    template <typename T>
    class Array {
    protected:
        Il2CppArray *_array;
        size_t _element_size;

    public:
        Array() {
            _array = nullptr;
            _element_size = 0;
        }

        Array(Il2CppArray *&array) {
            _array = array;
            _element_size = sizeof(T);
        }

        Array(Il2CppArray *&&array) {
            _array = array;
            _element_size = sizeof(T);
        }

        Array(Il2CppArray *&array, int32_t element_size) {
            _array = array;
            _element_size = static_cast<size_t>(element_size);
        }

        Array(Il2CppArray *&&array, int32_t element_size) {
            _array = array;
            _element_size = static_cast<size_t>(element_size);
        }

        int32_t length() const {
            if (!_array) return 0;

            return _array->max_length;
        }

        T get(int32_t idx) const {
            return *(T*)((int8_t*)_array->vector + idx * _element_size);
        }

        T* getAddr(int32_t idx) const {
            return (T*)((int8_t*)_array->vector + idx * _element_size);
        }

        Il2CppApi::Field<T> getField(int32_t idx) const {
            auto field = (T*)((int8_t*)_array->vector + idx * _element_size);
            return Il2CppApi::Field<T>(*field);
        }

        Il2CppApi::String getStringField(int32_t idx) const {
            auto field = (Il2CppString**)((int8_t*)_array->vector + idx * _element_size);
            return Il2CppApi::String(*field);
        }

        template <typename AT>
        Il2CppApi::Array<AT> getArrayField(int32_t idx) const {
            auto field = (Il2CppArray**)((int8_t*)_array->vector + idx * _element_size);
            return Il2CppApi::Array<AT>(*field);
        }

        template <typename LT>
        Il2CppApi::Field<LT> getListField(int32_t idx) const {
            auto field = (Il2CppApi::Il2CppList**)((int8_t*)_array->vector + idx * _element_size);
            return Il2CppApi::List<LT>(*field);
        }

        void set(int32_t idx, T value) const {
            *(T*)((int8_t*)_array->vector + idx * _element_size) = value;
        }

        void setData(Il2CppArray *array) {
            _array = array;
        }

        Il2CppArray* getData() const {
            return _array;
        }

        operator intptr_t() const { return (intptr_t)_array; }
    };
}
