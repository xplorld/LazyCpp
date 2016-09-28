//
//  main.cpp
//  LazyCpp
//
//  Created by Xplorld on 2016/7/22.
//  Copyright © 2016年 xplorld. All rights reserved.
//

#include <list>
#include <vector>
#include <string>
#include <iostream>
#include "LazyCpp.hpp"

class Shouter {
public:
    int i;
    Shouter(int i) : i(i) {std::cout << "ctor " << i << std::endl;}
    Shouter(Shouter const &other): i(other.i) {std::cout << "copy " << i << std::endl;}
    Shouter(Shouter &&other): i(other.i) {std::cout << "move " << i << std::endl;}
    ~Shouter() {std::cout << "dele " << i << std::endl;}
};
std::ostream &operator<<(std::ostream &os, Shouter const &m) {
    return os << m.i;
}


int add(int i) {
    return i+1;
}
int add(std::vector<int> i) {
    return *i.begin()+1;
}

using namespace LazyCpp;

void monadTest() {
    //>>=
    Array<int> {2,3,4}
    .bind(mul2<Array, int>)
    .bind(mul2int<Array<int>>)
    .bind(print<Array, int>); //8, 12, 16
    
    //maybe
    auto minus_1_positive_or_nil = [](const int& i) {
        if (i < 2) {
            return Maybe<int>();
        } else {
            return Maybe<int>(i-1);
        }
    };
    
    Maybe<int> {2}
    .bind(mul2<Maybe, int>)
    .bind(minus_1_positive_or_nil)
    .bind(print<Maybe, int>); // 3
    
    
    Maybe<int> {1}
    .bind(minus_1_positive_or_nil)
    .bind(print<Maybe, int>); //nil does not call binded function, noting printed
    
    
    join(Array<Array<int>> {{1,2,3},{4,5,6}})
    .bind(print<Array, int>); //1,2,3,4,5,6
    
    join(Maybe<Maybe<int>> {3})
    .bind(print<Maybe, int>); //3
    
    join(Maybe<Maybe<int>>())
    .bind(print<Maybe, int>); //nothing
    
    auto plusHalf = liftM<Array, int>([](const int& i) {return i + 0.5;});
    plusHalf(Array<int> {1,2,3})
    .bind(print<Array, double>); //1.5, 2.5, 3.5
    
    auto add = liftM2<Array, int, int>([](const int&i, const int&j) {return i+j;});
    Array<int> arr {1,2,3};
    Array<int> arr2 {1,2,3};
    add(arr,arr2).bind(print<Array,int>);
    //2,3,4,3,4,5,4,5,6
}


int main(int argc, const char * argv[]) {
    /*
     y combinator
     */
    auto fact([](auto fact){
        return [=](int i) {
            return i ? fact(i-1) * i : 1;
        };
    });
    
    int fact5 = y<int>(fact)(5);
    std::cout << fact5 << std::endl;
    
    /*
     map
     */
    auto int2shouter([](int i){return Shouter(i);});
    //print to show when id is called
    auto id([](auto &i){
        std::cout << "id " << i << std::endl;return i;
    });
    auto inc([](int i){ return i + 1; });
    std::vector<int> source {1,2,3,4,5};

    //x: [Shouter]
    auto x = map(map(map(map(source, id),inc),int2shouter),id);
    //id and inc called when iterator dereferenced first time
    for (auto &i : x) {
        std::cout << "return i = " << i << "<- return i" << std::endl;
    }
    //value are stored, will not be called in consequent dereference
    for (auto i = x.begin(); i != x.end(); i++) {
        std::cout << "return i = " << *i << "<- return i" << std::endl;
    }
    //to container, no longer lazy, everything evaluated
    //todo: move iterator?
    auto list = x.toContainer<std::list<Shouter>>();
    for (auto &i : list) {
        std::cout << "return i = " << i << "<- return i" << std::endl;
    }
    
    /*
     reduce
     */
    auto adder([](int i){
        return [i](int j){
            std::cout << "adding " << i << " + " << j << std::endl;
            return i+j;
        };
    });
    
    //memonized:
    //getted twice, called once.
    auto res = reduce(source, adder, 0);
    std::cout << "res = " << *res << std::endl;
    std::cout << "again, res = " << *res << std::endl;

    /*
     filter
     */
    auto is_odd([](int i) {return bool(i & 0x01);});
    auto odds = filter(map(source,id),is_odd);
    for (auto x : odds) {
        std::cout << x << std::endl;
    }
    for (auto x = odds.begin();
         x != odds.end();
         ++x) {
        std::cout << *x << std::endl;
    }
    
    /*
     monad
     */
    monadTest();
    return 0;
}
