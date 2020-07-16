#ifndef TESTINGFRAMEWORK_H
#define TESTINGFRAMEWORK_H
#include "Tree.h"
#include "Methods.h"
#include <iostream>
#include <stack>
#include "reader.h"
#include "DSL.h"


bool testing(Trie& T,example example1)
{

    //int StartingDepth=
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
        case 1 || 9:
            CurrPtr->DataList = MyMethods.UseFunctionL_LI(CurrPtr->FuncID, CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataInt);
            break;
        case 3:
        case 7:
        case 8:
        case 11:
        case 12:
            CurrPtr->DataInt = MyMethods.UseFunctionI_L(CurrPtr->FuncID, CurrPtr->Children[0]->DataList);
            break;
        case 4:
            switch (example1.Ins)
            {
            case IO::L:
            case IO::LL:
                CurrPtr->DataList = example1.INPUTLISTS[0];
                break;
            case IO::I:
            case IO::II:
            case IO::IL:
                CurrPtr->DataInt = example1.INPUTINTS[0];
            }
            break;
        case 5:
            switch (example1.Ins)
            {
            case IO::II:
                CurrPtr->DataInt = example1.INPUTINTS[1];
                break;
            case IO::IL:
            case IO::LL:
                CurrPtr->DataList = example1.INPUTLISTS[1];
            }
            break;
        case 10:
            CurrPtr->DataInt = MyMethods.UseFunctionI_LI(CurrPtr->FuncID, CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataInt);
            break;
        case 2:
            CurrPtr->DataList = MyMethods.UseFunctionL_L(CurrPtr->FuncID, CurrPtr->Children[0]->DataList);
            break;
        case 27:
            switch (CurrPtr->Children[1]->FuncID)
            {
            case 13:
                CurrPtr->DataList = MyMethods.MapAdd(CurrPtr->Children[0]->DataList);
                break;
            case 14:
                CurrPtr->DataList = MyMethods.MapSub(CurrPtr->Children[0]->DataList);
                break;
            case 15:
                CurrPtr->DataList = MyMethods.MapMult2(CurrPtr->Children[0]->DataList);
                break;
            case 16:
                CurrPtr->DataList = MyMethods.MapDiv2(CurrPtr->Children[0]->DataList);
                break;
            case 17:
                CurrPtr->DataList = MyMethods.MapMult3(CurrPtr->Children[0]->DataList);
                break;
            case 18:
                CurrPtr->DataList = MyMethods.MapDiv3(CurrPtr->Children[0]->DataList);
                break;
            case 19:
                CurrPtr->DataList = MyMethods.MapMult4(CurrPtr->Children[0]->DataList);
                break;
            case 20:
                CurrPtr->DataList = MyMethods.MapDiv4(CurrPtr->Children[0]->DataList);
                break;
            case 21:
                CurrPtr->DataList = MyMethods.MapPow(CurrPtr->Children[0]->DataList);
                break;
            case 22:
                CurrPtr->DataList = MyMethods.MapNeg(CurrPtr->Children[0]->DataList);
                break;
           }
            break;
        case 28:
            switch (CurrPtr->Children[1]->FuncID)
            {
            case 23:
                CurrPtr->DataList = MyMethods.FilterGZ(CurrPtr->Children[0]->DataList);
                break;
            case 24:
                CurrPtr->DataList = MyMethods.FilterLZ(CurrPtr->Children[0]->DataList);
                break;
            case 25:
                CurrPtr->DataList = MyMethods.FilterEven(CurrPtr->Children[0]->DataList);
                break;
            case 26:
                CurrPtr->DataList = MyMethods.FilterOdd(CurrPtr->Children[0]->DataList);
                break;
            }
            break;
        case 29:
            switch (CurrPtr->Children[1]->FuncID)
            {
            case 23:
                CurrPtr->DataInt = MyMethods.CountGZ(CurrPtr->Children[0]->DataList);
                break;
            case 24:
                CurrPtr->DataInt = MyMethods.CountLZ(CurrPtr->Children[0]->DataList);
                break;
            case 25:
                CurrPtr->DataInt = MyMethods.CountEven(CurrPtr->Children[0]->DataList);
                break;
            case 26:
                CurrPtr->DataInt = MyMethods.CountOdd(CurrPtr->Children[0]->DataList);
                break;
            }
            break;
        case 30:
            CurrPtr->DataList = MyMethods.ZipWithSum(CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataList);
            break;
        case 31:
            CurrPtr->DataList = MyMethods.ZipWithDif(CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataList);
            break;
        case 32:
            CurrPtr->DataList = MyMethods.ZipWithMult(CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataList);
            break;
        case 33:
            CurrPtr->DataList = MyMethods.ZipWithMin(CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataList);
            break;
        case 34:
            CurrPtr->DataList = MyMethods.ZipWithMax(CurrPtr->Children[0]->DataList, CurrPtr->Children[1]->DataList);
            break;
        case 35:
            CurrPtr->DataList = MyMethods.ScanL1Sum(CurrPtr->Children[0]->DataList);
            break;
        case 36:
            CurrPtr->DataList = MyMethods.ScanL1Dif(CurrPtr->Children[0]->DataList);
            break;
        case 37:
            CurrPtr->DataList = MyMethods.ScanL1Mult(CurrPtr->Children[0]->DataList);
            break;
        case 38:
            CurrPtr->DataList = MyMethods.ScanL1Min(CurrPtr->Children[0]->DataList);
            break;
        case 39:
            CurrPtr->DataList = MyMethods.ScanL1Max(CurrPtr->Children[0]->DataList);
            break;
        }

        i++;
    }


    switch (example1.Out)
    {
    case IO::I:
        if (CurrPtr->DataInt == example1.OUTPUTINT)
        {
            std::cout << "Passed test" << std::endl;
            return true;
        }
        else
            return false;
    case IO::L:
        if (CurrPtr->DataList == example1.OUTPUTLIST)
        {
            std::cout << "Passed test" << std::endl;
            return true;
        }
        else
            return false;
    }


    
}  
 


#endif
