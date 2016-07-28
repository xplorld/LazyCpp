//
//  base.h
//  LazyCpp
//
//  Created by Xplorld on 2016/7/24.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#ifndef base_h
#define base_h

#include <functional>
#include <algorithm>
#include <type_traits>

template <typename value_type>
class LazyPointer {
private:
    value_type* pointer = nullptr;
    std::function<value_type(void)> f;
public:
    template <typename funT, typename arg, typename = std::enable_if<std::is_same<typename std::result_of<funT(arg)>::type, value_type>::value>>
    LazyPointer(funT fun, arg a) {
        f = std::bind(fun, a);
    }
    
    LazyPointer(std::function<value_type(void)> f) : f(f) {}
    
    value_type& operator*() {
        if (!pointer) {
            pointer = new value_type(f()); //move
        }
        return *pointer;
    }
    ~LazyPointer() {
        delete pointer;
    }
};


#endif /* base_h */
