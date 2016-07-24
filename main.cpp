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
    //print to show when id is called
    auto id([](int i){
        std::cout << "id " << i << std::endl;return i;
    });
    auto inc([](int i){ return i + 1; });
    std::vector<int> source {1,2,3,4,5};

    //x: [int]
    auto x = map(map(source, id),inc);
    for (auto i : x) {
        std::cout << "return i = " << i << "<- return i" << std::endl;
    }
    
    //not memonized yet;
    //multiple trails getting value from mapp'd containers will call the function multiple times.
    //to be implemented
    auto list = x.toContainer<std::list<int>>();
    for (auto i : list) {
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
    
    return 0;
}
