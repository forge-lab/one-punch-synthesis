#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"
#include <unordered_map>
#include <queue>
#include <set>
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
       // GivenInputs = 0;
    }
    Node(int n) : Data(n) {};

};
class Trie
{
private:
    Node* Head;
    std::queue<Node*> NodeQueue;

public:
    Trie(int depth,int maxinputs)
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

struct MatchIO
{
public:
    std::vector<int> PNodes;
    std::vector<std::vector<int>> CNodes;
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
    int MaxInputs;
    std::vector<MatchIO> Matches;
   
//public:
    DSL(Type R)
    {
        //Creating all components
        Components.push_back(Component("Empty", Type::EMPTY, std::vector<Type>(), 0)); //Empty component
        Components.push_back(Component("Take", Type::LIST, std::vector<Type>{Type::LIST, Type::INTEGER}, 1)); //Take component
        Components.push_back(Component("Sort", Type::LIST, std::vector<Type>{Type::LIST}, 2)); //Sort component
        Components.push_back(Component("Sum", Type::INTEGER, std::vector<Type>{Type::LIST}, 3)); //Sum component
        Components.push_back(Component("B", Type::LIST, std::vector<Type>(), 4)); //B List component
        Components.push_back(Component("K", Type::INTEGER, std::vector<Type>(), 5)); //K Integer component

        //MaxInputs keeps track of the maximum number of inputs a component can have
        MaxInputs = 0;
        for (auto a : Components)
            MaxInputs = std::max(MaxInputs, int(a.Inputs.size()));

        //If a component has less defined inputs than the max, we add EMPTY inputs
        for (std::vector<Component>::iterator it = Components.begin(); it != Components.end(); it++)
        {
            while(it->Inputs.size()<MaxInputs)
                it->Inputs.push_back(Type::EMPTY);
        }

        //call initializer function. Need more descriptive function name
        initialization();

    }
    void initialization()
    {
        //Create set of all possible inputs
        std::set<std::vector<Type>> AllInputs;
        for (auto a : Components)
            AllInputs.insert(a.Inputs);

        //j variable used later for iteration
        int j = 0;
  

        //We already know how many different cases of inputs we are going to have, so we might as well resize the vector to
        //avoid having to use push which is less efficient
        Matches.resize(AllInputs.size());
       
        //For every single input possibility (i.e. foo(EMPTY,EMPTY), foo(INT, EMPTY), foo(LIST,INT), etc.)
        for (int i = 0; i < AllInputs.size(); i++)
        {

            //Need to make some of this code cleaner
            Matches[i].CNodes.resize(MaxInputs);
            for (auto a : Components) //for every component, check if its INPUT is the same as the one we are looking at
            {
                if (a.Inputs == *std::next(AllInputs.begin(), i))//if it is, add to the vector of Parent Nodes for this particular match
                    Matches[i].PNodes.push_back(a.ID);
            }


            j = 0;//Reset j to zero for the following loop
            for (auto a : *std::next(AllInputs.begin(),i)) //for all INPUT possibilities
            {
                for (auto b : Components) //and for every component
                {
                    if (b.Output == a) //check if the output matches the INPUT we need
                        Matches[i].CNodes[j].push_back(b.ID); //add it to the Children Node J. (i.e. we add ALL functions that can be used
                }// for child 1. For instance, if we require input (LIST, EMPTY), it will first look for all functions 
                //that return LIST and add it to this CNode. Then it will increment J and then look through components again and 
                //add all components that return EMPTY. 
                //NEED paper to explain better
                j++;
            }

        }


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

/*
1) How many children
2) Depth
cap children at 2
choose json parser for C++
*/



void tree(const DSL& MyDSL,Type ResultType)
{
    int MaxInputs = MyDSL.MaxInputs;


    Trie MyTree(3,MaxInputs); //Depth of 3 is hardcoded, number of children is not
    Node* HeadPtr = MyTree.returnhead();
    assert(HeadPtr);

    context c;
    solver s(c);
    std::vector<expr> vars; //all variables used by Z3
    expr_vector validcomponents(c); //vector of predicates

   
   
    int i = 0; //used to keep track of how many variables are created
    int inputs_assigned = 0; //used to keep track of how many inputs have been assigned to particular node so far
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
    validcomponents.resize(0); //CLear;




    expr_vector ParentValMKOR(c);
    expr_vector ChildValMKOR(c);



    int j = 0;


    while(!NodeQueue.empty())
    {
        CurrPtr = NodeQueue.front();
        NodeQueue.pop();

        if (CurrPtr->Children.size() == 0)//LEAF NODE
        {
            
            validcomponents.resize(0);
            for (auto a : MyDSL.Components)
            {
                if (a.Inputs.size() == 0) //no inputs
                {
                    validcomponents.push_back(vars[i] == a.ID); //can only be component that takes in no input
                }
            }
            s.add(mk_or(validcomponents));

        }

        else //NOT LEAF NODE
        {
            
            
            for (auto a : CurrPtr->Children)
            {
                i++;
                vars.push_back(c.int_const(std::to_string(i).c_str())); //create child variable
                map[a] = i; //map pointer a to this variable for future use

                s.add(implies(vars[map[CurrPtr]] == 0, vars[i] == 0)); //for all nodes, if parent is EMPTY, then child is also EMPTY


                NodeQueue.push(a);
            }
            //now, all children for node CurrPtr have been created.

            for (auto a : MyDSL.Matches)
            {
                j = 0;
                ParentValMKOR.resize(0);



                for (std::vector<int>::iterator it = a.PNodes.begin(); it != a.PNodes.end(); it++)
                {
                    ParentValMKOR.push_back(vars[map[CurrPtr]] == *it);



                }
                for (auto b : a.PNodes)
                {
                    ParentValMKOR.push_back(vars[map[CurrPtr]] == b);

                }



                for (auto b : a.CNodes)
                {
                    
                    ChildValMKOR.resize(0);
                    for (auto c : b)
                    {
                        ChildValMKOR.push_back(vars[map[CurrPtr->Children[j]]] == c);
                    }
                    s.add(implies(mk_or(ParentValMKOR), mk_or(ChildValMKOR)));

                    j++;
                }
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
    1) Why is there underline with DSL?
    
    */
    DSL MyDSL(Type::INTEGER);
    //MyDSL.Components[0].Inputs.push_back(Type::EMPTY);
    //std::cout<<MyDSL.MaxInputs;

    MyDSL.print();
    std::cout << std::endl;


    //print matches
    for (auto a: MyDSL.Matches)
    {
        std::cout << " THESE NODES: ";
        for (auto b : a.PNodes)
            std::cout << b<<' ';
        std::cout << " MUST HAVE THESE CHILDREN ";
        for (auto b : a.CNodes)
        {
            std::cout << " NEW CHILD: ";
            for (auto c : b)
                std::cout << c<<' ';
        }
        std::cout << std::endl;
    } 

    std::cout << std::endl;

    tree(MyDSL, Type::INTEGER);
    

    system("pause");
    return 0;
}

