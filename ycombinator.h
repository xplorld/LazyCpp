//
//  ycombinator.h
//  LazyCpp
//
//  Created by Xplorld on 2016/7/24.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#ifndef ycombinator_h
#define ycombinator_h

#include "base.h"

//f: Type t => (t -> t) -> (t -> t)
//y:Type t => ((t -> t) -> (t -> t)) -> t -> t
template <typename Type>
auto y (std::function<std::function<Type(Type)>(std::function<Type(Type)>)> f) -> std::function<Type(Type)>
{
    return f([=](Type x){
        return y<Type>(f)(x);
    });
}

auto pedantic_y = ([] (auto f) { return
    ([] (auto x) { return
        x
        (x); }
     ([=] (auto y) -> std:: function <int (int)> { return
        f ([=] (auto a) { return
            (y
             (y))
            (a) ;});}));});

#endif /* ycombinator_h */
