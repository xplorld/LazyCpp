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
#include <iterator>

template <typename listT, typename funT>
class LazyList {
public:
    using argument_type = typename listT::value_type;
    using value_type = typename std::result_of<funT(argument_type)>::type;
    friend class iterator;
private:
    listT source;
    funT f;
    std::list<LazyPointer<value_type>> stored;
    void emplace_back_stored(typename listT::iterator source_iter) {
        stored.emplace_back([f {this->f} ](auto arg_pointer){return f(*arg_pointer);},source_iter);
    }

public:
    class iterator : public std::iterator<std::forward_iterator_tag,value_type> {
        friend LazyList;
        typename std::list<LazyPointer<value_type>>::iterator stored_iter;
        typename listT::iterator source_iter;
        LazyList* thisList;
        
        iterator(typename std::list<LazyPointer<value_type>>::iterator stored_iter,
                 typename listT::iterator source_iter,
                LazyList* thisList):
        stored_iter(stored_iter),
        source_iter(source_iter),
        thisList(thisList) {}
        
    public:
        iterator(const iterator & other) : stored_iter(other.stored_iter), source_iter(other.source_iter), thisList(other.thisList) {}
        
        value_type& operator*() {
            return **stored_iter;
        }
        iterator& operator++() {
            ++source_iter;
            if (source_iter != thisList->source.end() &&
                stored_iter == --thisList->stored.end()) {
                thisList->stored.emplace_back([f {thisList->f} ](auto arg_pointer){return f(*arg_pointer);},source_iter);
                stored_iter = --thisList->stored.end();
            } else {
                ++stored_iter;
            }
            return *this;
        }
        iterator operator++(int) {iterator tmp(*this); ++(*this); return tmp;}

        bool operator==(const iterator & another) { return source_iter == another.source_iter; }
        bool operator!=(const iterator & another) { return !(*this == another); }

    };
    
    iterator begin() {
        if (stored.empty()) {
            emplace_back_stored(source.begin());
        }
        return iterator(stored.begin(), source.begin(), this);
    }
    iterator end() {
        return iterator(stored.end(), source.end(), this);
    }
    
    template <typename Container>
    Container toContainer() {
        return Container(begin(),end());
    }
    
    LazyList(listT list, funT f) : source(list),f(f) {}
};

template < typename listT, typename funT, typename = typename std::result_of<funT(typename listT::value_type)>::type>
auto map(listT args, funT fun) {
    return LazyList<listT, funT>(args,fun);
}

#endif /* map_h */
