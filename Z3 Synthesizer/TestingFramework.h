#ifndef TESTINGFRAMEWORK_H
#define TESTINGFRAMEWORK_H
#include "Tree.h"
#include "Methods.h"
#include <iostream>
#include <stack>
#include "DSL.h"

bool testing(Trie& T,int OUTPUT,std::vector<int> INPUTLIST,int INPUTNUM)
{
    Methods MyMethods;
    std::queue<Node*> MyQueue;
    std::stack<Node*> MyStack;
    Node* CurrPtr = nullptr;
    MyQueue.push(T.returnhead());
    int i = 0;
    while (!MyQueue.empty())
    {

        CurrPtr = MyQueue.front();
        MyQueue.pop();
        for (auto a : CurrPtr->Children)
            MyQueue.push(a);
        MyStack.push(CurrPtr);
    }

    while (!MyStack.empty())
    {


        CurrPtr = MyStack.top();
        MyStack.pop();
        switch (CurrPtr->FuncID)
        {
        case 0:
            CurrPtr->DataInt = 0;
            break;
        case 1:
            CurrPtr->DataList = MyMethods.UseFunctionL_LI(1, CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataInt);
            break;
        case 2:
            CurrPtr->DataList = MyMethods.UseFunctionL_LI(2, CurrPtr->Children[0]->DataList, 0);
            break;
        case 3:
            CurrPtr->DataInt = MyMethods.UseFunctionI_L(3, CurrPtr->Children[0]->DataList);
            break;
        case 4:
            CurrPtr->DataList = INPUTLIST;
            break;
        case 5:
            CurrPtr->DataInt = INPUTNUM;

        }


        i++;
    }



    if (CurrPtr->DataInt == OUTPUT)
    {
        std::cout << "IT WORKSSSSSSSSSSSSS heres the solution" << std::endl;
        return true;
    }
    return false;


    
}  
 


#endif
