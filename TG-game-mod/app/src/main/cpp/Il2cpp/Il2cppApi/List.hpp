#pragma once

#include "Types.h"
#include "Array.hpp"

namespace Il2CppApi {
    template <typename>
    class Array;

    template <typename T>
    class List {
    private:
        Il2CppList *_list;
        Array<T> _data;
    public:
        List<T>(Il2CppList *&list) {
            _list = list;
            _data = Array<T>(list ? list->data : NULL);
        }

        List<T>(Il2CppList *&&list) {
            _list = list;
            _data = Array<T>(list ? list->data : NULL);
        }

        List(Il2CppList *&list, int32_t element_size) {
            _list = list;
            _data = Array<T>(list ? list->data : NULL, element_size);
        }

        List(Il2CppList *&&list, int32_t element_size) {
            _list = list;
            _data = Array<T>(list ? list->data : NULL, element_size);
        }

        Array<T> data() {
            _data.setData(_list ? _list->data : NULL);
            return _data;
        }

        int32_t size() const {
            if (!_list) return 0;

            return _list->length;
        }

        operator intptr_t() const { return (intptr_t)_list; }
    };
}
