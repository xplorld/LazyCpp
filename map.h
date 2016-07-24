//
//  map.h
//  LazyCpp
//
//  Created by Xplorld on 2016/7/24.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#ifndef map_h
#define map_h

#include "base.h"

/*
 listT:
 listT::value_type
 listT::iterator
 listT::begin()
 listT::end()
*/
template <typename listT, typename funT>
class MapList {
    listT source;
    funT f;
public:
    using argument_type = typename listT::value_type;
    using value_type = result_type<funT,argument_type>;
    class iterator : public listT::iterator {
        funT f;
    public:
        using value_type = MapList::value_type;
        iterator(funT f, typename listT::iterator iter) :listT::iterator(iter), f(f) {}
        value_type operator*() {
            return f(this->listT::iterator::operator*());
        }
    };
    iterator begin() {
        return iterator(f,source.begin());
    }
    iterator end() {
        return iterator(f,source.end());
    }
    MapList(listT list, funT f) : source(list),f(f) {}
  
    template <typename Container>
    Container toContainer() {
        return Container(begin(),end());
    }
};


template < typename listT, typename funT, typename = result_type<funT,typename listT::value_type>>
auto map(listT args, funT fun) -> MapList<listT, funT> {
    return MapList<listT, funT>(args,fun);
}

#endif /* map_h */
