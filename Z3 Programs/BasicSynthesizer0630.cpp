#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"
#include <unordered_map>
#include <ctime>
#include <queue>
using namespace z3;



enum class Type
{
    EMPTY, //0
    LIST, //1
    INTEGER, //2
};


struct Node
{
public:
    std::vector<Node*> Children;
    int Data;
    
    
    Node()
    {
        Data = -1;
        GivenInputs = 0;
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



struct Component
{
public:
    std::string Name;
    Type Output;
    std::vector<Type> Inputs;
    int ID;
    Component(std::string N, Type O, std::vector<Type> I,int J) :Name(N), Output(O), Inputs(I), ID(J) {};
};

class DSL
{
public:
    std::vector<Component> Components;
    
    int NumTypes;
//public:
    DSL(Type R)
    {
        Components.push_back(Component("Empty", Type::EMPTY, std::vector<Type>(), 0)); //Empty component
        Components.push_back(Component("Take", Type::LIST, std::vector<Type>{Type::LIST, Type::INTEGER}, 1)); //Take component
        Components.push_back(Component("Sort", Type::LIST, std::vector<Type>{Type::LIST}, 2)); //Sort component
        Components.push_back(Component("Sum", Type::INTEGER, std::vector<Type>{Type::LIST}, 3)); //Sum component
        Components.push_back(Component("B", Type::LIST, std::vector<Type>(), 4)); //B List component
        Components.push_back(Component("K", Type::INTEGER, std::vector<Type>(), 5)); //K Integer component
        NumTypes = 3;
    }
    void print()
    {
        for (auto a : Components)
        {
            std::cout << a.Name << " with output type: " << int(a.Output) << " and ID " << a.ID << " and following inputs: ";
            for (auto b : a.Inputs)
                std::cout << int(b) << ' ';
            std::cout << std::endl;
        }
    }

};


void tree(const DSL& MyDSL,Type ResultType)
{
    Trie MyTree(3); //Depth of 3 is hardcoded
    Node* HeadPtr = MyTree.returnhead();
    assert(HeadPtr);

    context c;
    solver s(c);
    std::vector<expr> vars; //all variables used by Z3
    expr_vector validcomponents(c); //vector of predicates
   
   
    int i = 0; //used to keep track of how many variables are created

    std::queue<Node*> NodeQueue; //used to iterate through trie
    Node* CurrPtr; 

    std::unordered_map<Node*, int> map; // used to keep track of which node corresponds to which variable in vector vars


   

    NodeQueue.push(HeadPtr); //start with headptr

    vars.push_back(c.int_const(std::to_string(i).c_str()));
    map[HeadPtr] = 0;

    s.add(vars[i] >= 0 && vars[i] <= int(MyDSL.Components.size())); // All nodes must have valid ID, i.e. 0 to 5


    //FIRST NODE must return the type specified in the DSL. It checkes which components to see which IDs are valid in this case
    for (auto a : MyDSL.Components)
    {
        if (a.Output == ResultType)
        {
            validcomponents.push_back(vars[i] == a.ID);
        }
    }
    
    s.add(mk_or(validcomponents)); //adds rule for first node
    validcomponents.resize(0);











    while(!NodeQueue.empty())
    {
        CurrPtr = NodeQueue.front();
        NodeQueue.pop();

        if (CurrPtr->Children.size() == 0)//LEAF NODE
        {
            /*
            validcomponents.resize(0);
            for (auto a : MyDSL.Components)
            {
                if (a.Inputs.size() == 0) //no inputs
                {
                    validcomponents.push_back(vars[i] == a.ID); //can only be component that takes in no input
                }
            }
            s.add(mk_or(validcomponents));*/

        }
        else //NOT LEAF NODE
        {
          
            for (auto a : CurrPtr->Children)
            {
                i++;
                vars.push_back(c.int_const(std::to_string(i).c_str())); //create child variable
                map[a] = i; //map pointer a to this variable for future use
                s.add(implies(vars[map[CurrPtr]] == 0, vars[i] == 0));
                s.add(implies(vars[map[CurrPtr]] == 1 || vars[map[CurrPtr]] == 2 || vars[map[CurrPtr]] == 4, vars[i] == 0))
                //if(a->GivenInputs )
                //s.add(implies(vars[map[CurrPtr]] % 2 == 0, vars[i] % 2 == 0) && implies(vars[map[CurrPtr]] % 2 == 1, vars[i] % 2 == 1));
                //s.add(vars[i] >= 0 && vars[i] <= 9);



                NodeQueue.push(a);
            }
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
    /*
    Hardcoded: 
    1) Tree depth
    
    */

    /*
    Questions:
    1) How can we 
    
    */
    DSL MyDSL(Type::INTEGER);
    MyDSL.print();
    Trie mytree(3);


    system("pause");
    return 0;
}

