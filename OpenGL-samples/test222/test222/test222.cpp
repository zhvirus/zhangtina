// test222.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <stdio.h>

class A
{
public:
    A(){ std::cout << "create A\n"; }
    ~A() { std::cout << "destroy A\n"; }
};

class B : public A
{
public:
    B(){ std::cout << "create B\n"; }
    virtual ~B() { std::cout << "destroy B\n"; }

    void print(){ std::cout << "B\n"; }

};



class C : public B
{
public:
    inline C(){ std::cout << "create C\n"; }
    inline ~C() { std::cout << "destroy C\n"; }


    virtual void a(){ std::cout << "c~~~\n"; }

    void print()const { std::cout << "C\n"; }

};


class Apple
{
public:
    Apple(){}
    void f()const{
        Apple* ttt = const_cast<Apple*>(this);
        ttt->i = 6;
    }
protected:
    int i;
};




int _tmain(int argc, _TCHAR* argv[])
{
    Apple* a = new Apple();

    printf("0x%p\n", a);

	return 0;
}

