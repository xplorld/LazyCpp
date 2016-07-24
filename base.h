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

template <typename funT,typename arg>
using result_type = decltype(std::declval<funT>()(std::declval<arg>()));

#endif /* base_h */
