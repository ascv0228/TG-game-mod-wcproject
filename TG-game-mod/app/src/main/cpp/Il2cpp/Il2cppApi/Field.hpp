#pragma once

#include <cstdint>

#include "Types.h"
#include "Array.hpp"
#include "List.hpp"
#include "String.hpp"
#include "../il2cpp-appdata.h"

namespace Il2CppApi {
    template <typename>
    class Array;
    template <typename>
    class List;

    template <typename T>
    class Field {
    protected:
        T *_ptr = nullptr;
    public:
        Field(T &ptr) {
            _ptr = &ptr;
        }

        Field(T &&ptr) {
            _ptr = &ptr;
        }

        template <typename FT>
        Il2CppApi::Field<FT> getField(int32_t offset) const {
            auto field = (FT*)((int8_t*)(*_ptr) + offset);
            return Il2CppApi::Field<FT>(*field);
        }

        Il2CppApi::String getStringField(int32_t offset) const {
            auto field = (Il2CppString**)((int8_t*)(*_ptr) + offset);
            return Il2CppApi::String(*field);
        }

        template <typename AT>
        Il2CppApi::Array<AT> getArrayField(int32_t offset) const {
            auto field = (Il2CppArray**)((int8_t*)(*_ptr) + offset);
            return Il2CppApi::Array<AT>(*field);
        }

        template <typename LT>
        Il2CppApi::List<LT> getListField(int32_t offset) const {
            auto field = (Il2CppApi::Il2CppList**)((int8_t*)(*_ptr) + offset);
            return Il2CppApi::List<LT>(*field);
        }

        T get() const {
            return *_ptr;
        }

        T set(T value) const {
            return *_ptr = value;
        }

        template <typename FT>
        FT* getFieldAddr(int32_t offset) const {
            return (FT*)((int8_t*)(*_ptr) + offset);
        }

        operator intptr_t() const {
            return (intptr_t)(*_ptr);
        }
    };
}
