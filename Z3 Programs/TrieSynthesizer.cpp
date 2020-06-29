#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"
#include <unordered_map>
#include <ctime>
#include <queue>
using namespace z3;



struct Node
{
public:
    std::vector<Node*> Children;
    int Data;
    Node()
    {
        Data = -1;
    }
    Node(int n) : Data(n) {};

};
class Trie
{
private:
    Node* Head;
    std::queue<Node*> NodeQueue;

public:
    Trie(int depth)
    {
        Head = new Node();
        Node* CurrPtr = Head;
        Node* TempPtr = nullptr;
        int QueueSize = 1;
        NodeQueue.push(Head);
        int NumChildren = 2;
        //srand(time(0));
       
        while (depth != 0)
        {
            depth--;
            QueueSize = NodeQueue.size();
            while (QueueSize != 0)
            {
                QueueSize--;
                //NumChildren = rand()%5+1; //can have 1 to 5 children
                NumChildren = 2;
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
    void print()
    {
        print_helper(Head);
    }
    void print_helper(Node* CurrPtr)
    {
        std::cout << CurrPtr->Data;
        while (!CurrPtr->Children.empty())
        {
            print_helper(CurrPtr->Children.back());
            CurrPtr->Children.pop_back();
        }
    }
    Node* returnhead() { return Head; }
    void print2DUtil(Node* root, int space)
    {
    
        if (root == NULL)
            return;

        space += 10;
 
        if(root->Children.size()!=0)
         print2DUtil(root->Children[0], space);

        std::cout << std::endl;
        for (int i = 10; i < space; i++)
            std::cout << " ";
        std::cout << root->Data << "\n";
 
        if (root->Children.size() != 0)
         print2DUtil(root->Children[1], space);
    }

    void print2D()
    {
        print2DUtil(Head, 0);
    }


};




void tree(Node* HeadPtr)
{

    assert(HeadPtr);
    context c;
    std::vector<expr> vars;
   
   
    int i = 0;
    std::queue<Node*> NodeQueue;
    Node* CurrPtr;
    std::unordered_map<Node*, int> map;


   
    solver s(c);

    NodeQueue.push(HeadPtr);

    vars.push_back(c.int_const(std::to_string(i).c_str()));
    map[HeadPtr] = 0;

    s.add(vars[i] >= 0 && vars[i] <= 9);
    while(!NodeQueue.empty())
    {
        CurrPtr = NodeQueue.front();
        NodeQueue.pop();
        for (auto a : CurrPtr->Children)
        {
            i++;
            vars.push_back(c.int_const(std::to_string(i).c_str()));
            map[a] = i;
            s.add(implies(vars[map[CurrPtr]] % 2 == 0, vars[i]%2 ==0 ) && implies(vars[map[CurrPtr]] % 2 == 1, vars[i]%2 == 1));
            s.add(vars[i] >= 0 && vars[i] <= 9);
            NodeQueue.push(a);
        }
    }
 

    expr_vector vals(c);

   
    while(s.check() == sat)
    {
        model m = s.get_model();
        std::cout << "New solution:" << "\n";
        while (!vals.empty())
           vals.pop_back();
        for (unsigned i = 0; i < m.size(); i++)
        {
            func_decl v = m[i];
            assert(v.arity() == 0);

            std::string stringval = m.get_const_interp(v).to_string();
            int intval = std::stoi(stringval);
            vals.push_back(vars[std::stoi(v.name().str())] != intval);
            std::cout  <<v.name()<<" "<< intval << std::endl;
        }
       
        s.add(mk_or(vals));
    }

}


int main() 
{
    Trie mytree(1);
    //mytree.print2D();
    tree(mytree.returnhead());

    system("pause");
    return 0;
}

