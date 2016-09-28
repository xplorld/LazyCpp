//
//  filter.h
//  LazyCpp
//
//  Created by Xplorld on 2016/7/24.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#ifndef filter_h
#define filter_h

namespace LazyCpp {
    template <typename listT, typename funT
    /*,typename = std::enable_if<std::is_same<
     bool,
     result_type<funT,typename listT::value_type>
     >::value>*/
    >
    class FilterList {
    public:
        using value_type = typename listT::value_type;
    private:
        listT source;
        funT f;
        typename listT::iterator next;
        friend class iterator;
        std::list<value_type> stored;
        bool completed() {
            return next == source.end();
        }
    public:
        class iterator : public std::iterator<std::forward_iterator_tag,value_type> {
            friend FilterList;
            FilterList* thisList;
            typename std::list<value_type>::iterator stored_iter;
            
            iterator(FilterList* thisList,
                     typename std::list<value_type>::iterator stored_iter):
            thisList(thisList),
            stored_iter(stored_iter) {}
            
        public:
            value_type& operator*() {
                return *(stored_iter);
            }
            iterator& operator++() {
                //stored_iter < --end()
                if (stored_iter != --thisList->stored.end()) {
                    ++stored_iter;
                    return *this;
                }
                //if stored_iter > end()
                //undefined
                
                //now stored_iter == --end() or == end()
                while (1) {
                    if (thisList->next == thisList->source.end()) {
                        //let this be end()
                        ++stored_iter;
                        return *this;
                    }
                    
                    value_type& val = *thisList->next;
                    ++thisList->next;
                    if (thisList->f(val)) {
                        //good
                        thisList->stored.push_back(val);
                        ++stored_iter;
                        return *this;
                    }
                }
                //placeholder, never here
                return *this;
            }
            iterator operator++(int) {iterator tmp(*this); ++(*this); return tmp;}
            
            bool operator==(const iterator & another) { return thisList->completed() && stored_iter == another.stored_iter ;}
            bool operator!=(const iterator & another) { return !(*this == another); }
            
        };
        iterator begin() {
            return iterator(this,stored.begin());
        }
        iterator end() {
            return iterator(this,stored.end());
        }
        FilterList(listT list, funT f) : source(list),f(f),next(source.begin()) {}
    };
    
    
    
    template < typename listT, typename funT>
    FilterList<listT, funT> filter(listT args, funT fun) {
        return FilterList<listT, funT>(args,fun);
    }
  

}

#endif /* filter_h */
