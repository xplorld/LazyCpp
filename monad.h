//
//  monad.h
//  LazyCpp
//
//  Created by Xplorld on 2016/9/29.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#ifndef monad_h
#define monad_h

#include "base.h"
#include <list>

//a simple & naive monad system
//monads: Id, Maybe, Array
//monads implement: ctor, bind(>>=)
//functions: join, liftM, liftM2

namespace LazyCpp {
    
    //bind :: monad m => m T -> (T -> m U) -> m U
    //if F :: T -> M U
    //bind_result_type :: M U
    //else SFINAE fail
    template<class F, template<class> class M, class T>
    using bind_result_type = typename std::enable_if<
    std::is_same<
        typename result_of<F (const T&)>::type, //type of F T
        M<typename result_of<F (const T&)>::type::type> //F T is an M
    >::value,
    typename result_of<F (const T&)>::type
    >::type;
    
    //join :: Monad m => m (m a) -> m a
    //join m = m >>= id
    template< template<class> class M, class T>
    M<T> join(M<M<T>> mmt) {
        //id :: Monad m => m T -> m T
        auto id = [](const M<T>& mt) {return mt;};
        return mmt.bind(id);
    }
    
    //liftM :: Monad m => (T -> U) -> m T -> m U
    //F :: T -> U
    template< template<class> class M, class T, class F>
    std::function<
    M<typename result_of<F (const T&)>::type>
    (M<T>)>
    liftM(F f) {
        //liftM f m1 = do { x1 <- m1; return (f x1) }
        //listM f m1 = m1 >>= \x. f x
        using U = typename result_of<F (const T&)>::type;
        return [f](const M<T>& mt) {
            auto unit = [f](const T& t) {
                auto u = f(t);
                return M<U> {u};
            };
            return mt.bind(unit);
        };
    }
    
    
    //liftM :: Monad m => (T -> U -> V) -> m T -> m U -> m V
    //F :: T -> U -> V
    template< template<class> class M, class T, class U, class F>
    std::function<
    M<typename result_of<F (const T&, const U&)>::type>
    (M<T>,M<U>)>
    liftM2(F f) {
        //liftM2 f m1 m2 = do { x1 <- m1; x2 <- m2; return (f x1 x2) }
        //liftM2 f m1 m2 = m1 >>= \x1.( m2 >>= \x2. return (f x1 x2) )
        using V = typename result_of<F (const T&, const U&)>::type;
        
        return [f](const M<T>& mt, const M<U>& mu) {
            auto unit1 = [f, mu](const T& t) {
                auto unit2 = [f, t](const U& u) {
                    auto v = f(t, u);
                    return M<V> {v};
                };
                return mu.bind(unit2);
            };
            return mt.bind(unit1);
        };
    }
    
    
    template <class T>
    struct Id {
        using type = T;
        
        const T* value;
        Id(const T& val) : value(new T(val)) { }
        Id(T&& val) : value(new T(val)) { }
        
        Id(const Id& val) : value(new T(*val.value)) { }
        Id(Id&& val) {
            std::swap(value, val.value);
        }
        
        ~Id() {
            delete value;
        }
        
        template <class F>
        using bind_result_type = bind_result_type<F, LazyCpp::Id, T>;
        
        
        //do not name the binding method as operator>>=
        //since its right-to-left associated in C++
        //but should be left-to-right in Haskell
        //things like m >>= f1 >>= f2 is impossible without parenthesis
        //so use ordinary methods instead
        template <class F>
        bind_result_type<F>
        bind(F binded) const {
            return binded(*value);
        }
    };
    
    template <class T>
    struct Maybe {
        using type = T;
        
        const T* value = nullptr;
        Maybe(const T& val) : value(new T(val)) { }
        Maybe(T&& val) : value(new T(val)) { }
        
        Maybe(const Maybe& val) : value(new T(*val.value)) { }
        Maybe(Maybe&& val) {
            std::swap(value, val.value);
        }
        
        
        /* not required by monad */
        Maybe() { }
        
        ~Maybe() {
            delete value;
        }
        
        
        template <class F>
        using bind_result_type = bind_result_type<F, LazyCpp::Maybe, T>;
        
        
        template <class F>
        bind_result_type<F>
        bind(F binded) const {
            if (value == nullptr) return bind_result_type<F>();
            return binded(*value);
        }
    };
    
    template <class T>
    struct Array {
        using type = T;
        
        std::list<T> arr;
        
        //as monad
        Array(const T& val) {
            arr.push_back(val);
        }
        Array(T&& val) {arr.push_back(val);}
        
        //as struct
        Array(const Array& val) : arr(val.arr) { }
        Array(Array&& val) {
            std::swap(arr, val.arr);
        }
        
        //as container
        Array() {}
        
        template< class InputIt >
        Array( InputIt first, InputIt last) : arr(first,last) {}
        //std::initializer_list currently have no move ctor
        //so && is not necessary, have no use
        Array( std::initializer_list<T> init) : arr(init) {}
        
        
        template <class F>
        using bind_result_type = bind_result_type<F, LazyCpp::Array, T>;
        
        // >>= : (M t)→(t→M u)→(M u)
        template <class F>
        bind_result_type<F>
        bind(F binded) const {
            bind_result_type<F> Uarray;
            for(auto& t : arr) {
                //map
                bind_result_type<F> oneArr = binded(t);
                //join
                Uarray.arr.splice(Uarray.arr.end(),oneArr.arr);
            }
            return Uarray;
        }
    };
}


template <template<class> class M, class T>
M<T> mul2(const T& i) {
    return M<T>(2 * i);
}


template <class M>
M mul2int(const int& i) {
    return M(2 * i);
}

template <template<class> class M, class T>
M<T> print(const T& i) {
    std::cout << i << std::endl;
    return M<T>(i);
}


#endif /* monad_h */
