//
//  lazycppTypeTraits.h
//  LazyCpp
//
//  Created by Xplorld on 2016/7/31.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#ifndef lazycppTypeTraits_h
#define lazycppTypeTraits_h

namespace LazyCpp {
    
    template <typename T>
    struct result_of;
    
    template <class Fp, class ...Args>
    struct result_of<Fp(Args...)>
    {
        using type = typename std::result_of<Fp(Args&...)>::type;
    };

}

#endif /* lazycppTypeTraits_h */
