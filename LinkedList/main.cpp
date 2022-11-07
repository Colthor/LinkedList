// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "LinkedList.h"
#include <iostream>
#include <cassert>

// The stack overflow tests are very slow, turn them on/off here.
const bool CHECK_STACK_OVERFLOW{ false };

void testPushPopFront()
{
    LinkedList<int> l;
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);
    assert(3 == l.pop_front());
    assert(2 == l.pop_front());
    assert(1 == l.pop_front());
    assert(0 == l.size());
}
void testPushPopBack()
{
    LinkedList<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    assert(3 == l.pop_back());
    assert(2 == l.pop_back());
    assert(1 == l.pop_back());
    assert(0 == l.size());
}

void testPushPopBoth()
{
    LinkedList<int> l;
    l.push_back(1);
    assert(1 == l.pop_front());
    assert(0 == l.size());

    l.push_front(3);
    assert(3 == l.pop_back());
    assert(0 == l.size());

    l.push_back(1);
    l.push_back(2);
    assert(1 == l.pop_front());
    assert(2 == l.pop_front());
    assert(0 == l.size());

    l.push_front(1);
    l.push_front(2);
    assert(1 == l.pop_back());
    assert(2 == l.pop_back());
    assert(0 == l.size());

}

void testListEquality()
{
    LinkedList<int> L1, L2;

    assert(L1 == L2);

    L1.push_back(1);
    assert(L1 != L2);

    L2.push_back(1);
    assert(L1 == L2);

    L1.push_back(2);
    assert(L1 != L2);

    L2.push_back(2);
    assert(L1 == L2);

}

void testListCopy(LinkedList<int> l)
{
    assert(5 == l.pop_back());
    assert(3 == l.pop_back());
    assert(1 == l.pop_back());
    assert(1 == l.pop_back());
    assert(0 == l.size());
}

void testListCopyAssignment()
{
    LinkedList<int> L1, L2;

    assert(&L1 != &L2);
    assert(L1 == L2);

    L1.push_back(1);
    L1.push_back(1);
    L1.push_back(3);
    L1.push_back(5);

    L2 = L1;
    assert(&L1 != &L2);
    assert(L1 == L2);
    testListCopy(L1);
    assert(L1 == L2);
    L2 = L2;
    assert(L1 == L2);
    L2 = L1;
    L1.push_back(1);
    L1.push_back(1);
    L1.push_back(3);
    L1.push_back(5);
    assert(L1 != L2);


    if (CHECK_STACK_OVERFLOW)
    {
        //check stack overflow on assignment
        for (auto i{ 0 }; i < 10000000; ++i)
        {
            L2.push_back(i);
        }
        L2 = L1;
        assert(L1 == L2);
    }

}

void testDropStackOverflow()
{
    if (CHECK_STACK_OVERFLOW)
    {
        LinkedList<int> l;
        for (auto i{ 0 }; i < 10000000; ++i)
        {
            l.push_back(i);
        }
    }
}

void testListMove()
{
    LinkedList<int> L1, L2, L3;

    L1.push_back(1);
    L1.push_back(1);
    L1.push_back(3);
    L1.push_back(5);

    L2 = L1;
    assert(L1 == L2);
    L3 = std::move(L2);
    assert(L1 == L3);
    assert(0 == L2.size());

}

void testIterator()
{
    LinkedList<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    
    auto it = l.begin();
    assert(1 == *it);
    ++it;
    assert(2 == *it);
    ++it;
    assert(3 == *it);
    ++it;
    assert(l.end() == it);

}

void testRangeFor()
{
    LinkedList<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    int t = 1;
    for (auto& i : l)
    {
        assert(i == t);
        ++t;
    }

}

void testInitialiserList()
{
    LinkedList<int> L1{1,2,3,4}, L2;

    L2.push_back(1);
    L2.push_back(2);
    L2.push_back(3);
    L2.push_back(4);
    assert(L1 == L2);
}

void testReverseIterator()
{
    LinkedList<int> L1{ 1,2,3,4 };
    int i = 4;

    for (auto it = L1.rbegin(); it != L1.rend(); --it)
    {
        assert(i == *it);
        --i;
    }

}

void testMutableIterator()
{
    LinkedList<int> l{ 1,2,3,4 };
    for (auto& i : l)
    {
        i *= 2;
    }

    int t = 1;
    for (auto& i : l)
    {
        assert(i == 2*t);
        ++t;
    }
}

int main()
{
    testPushPopFront();
    testPushPopBack();
    testPushPopBoth();
    testListEquality();
    testListCopyAssignment();
    testDropStackOverflow();
    testListMove();
    testIterator();
    testRangeFor();
    testInitialiserList();
    testReverseIterator();
    testMutableIterator();

    std::cout << "All tests done." << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
