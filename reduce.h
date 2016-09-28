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

namespace LazyCpp {
    
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

}

#endif /* reduce_h */
