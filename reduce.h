//
//  reduce.h
//  LazyCpp
//
//  Created by Xplorld on 2016/7/24.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#ifndef reduce_h
#define reduce_h

#include "base.h"

template <typename value_type>
class LazyPointer {
private:
    value_type* pointer = nullptr;
    std::function<value_type(void)> f;
public:
    template <typename funT, typename arg, typename = std::enable_if<std::is_same<result_type<funT,arg>, value_type>::value>>
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


template <typename listT, typename funT>
auto reduce(listT source,funT fun, typename listT::value_type initial) -> LazyPointer<typename listT::value_type> {
    return LazyPointer<typename listT::value_type>([=,source {std::move(source)}]() {
        typename listT::value_type accu = initial;
        for (auto iter = source.begin();iter != source.end();++iter) {
            accu = fun(accu)(*iter);
        }
        return accu;
    });
}

#endif /* reduce_h */
