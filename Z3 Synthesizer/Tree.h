#ifndef TREE_H
#define TREE_H
#include <vector>
#include <queue>

struct Node
{
public:
    std::vector<Node*> Children;
    Node* PrevPtr;
    int FuncID;
    int DataInt;
    std::vector<int> DataList;


    Node()
    {
        FuncID = 0;
    }
    Node(Node* P) : PrevPtr(P), FuncID(0) {};


};
class Trie
{
private:
    Node* Head;
    std::queue<Node*> NodeQueue;

public:
    Trie(int depth, int maxinputs)
    {
        Head = new Node();
        Node* CurrPtr = Head;
        Node* TempPtr = nullptr;
        int QueueSize = 1;
        NodeQueue.push(Head);
        int NumChildren = maxinputs;

        while (depth != 0)
        {
            depth--;
            QueueSize = NodeQueue.size();
            while (QueueSize != 0)
            {
                QueueSize--;
                NumChildren = maxinputs;
                CurrPtr = NodeQueue.front();
                NodeQueue.pop();
                while (NumChildren != 0)
                {
                    TempPtr = new Node();
                    CurrPtr->Children.push_back(TempPtr);
                    NodeQueue.push(TempPtr);
                    NumChildren--;
                }
            }
        }
    }
    Node* returnhead() { return Head; }

};


void AssignIDs(Trie& T, std::vector<int> V)
{
    std::queue<Node*> MyQueue;
    Node* CurrPtr = nullptr;
    MyQueue.push(T.returnhead());
    int i = 0;
    while (!MyQueue.empty())
    {

        CurrPtr = MyQueue.front();
        MyQueue.pop();
        for (auto a : CurrPtr->Children)
            MyQueue.push(a);
        CurrPtr->FuncID = V[i];
        i++;
    }


}



bool checkifismine(Trie& T, std::vector<int> V)
{
    std::queue<Node*> MyQueue;
    Node* CurrPtr = nullptr;
    MyQueue.push(T.returnhead());
    int i = 0;
    while (!MyQueue.empty())
    {

        CurrPtr = MyQueue.front();
        MyQueue.pop();
        for (auto a : CurrPtr->Children)
            MyQueue.push(a);
        if (CurrPtr->FuncID != V[i])
            return false;
        i++;
    }
    return true;
}

#endif