#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"
#include <unordered_map>
#include <queue>
#include <set>
#include "Methods.h"
using namespace z3;



enum class Type
{
    EMPTY, //0
    LIST, //1
    INTEGER, //2
    LAMBDA_INT, //3, for Map, a type of function that returns ints
    LAMBDA_BOOL //4, for Filter and Count, a type of function that returns bools
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
        

        //takes about 10 minutes to generate 21741 programs, not using the ones numbered 6-12
        /*
        Components.push_back(Component("Reverse", Type::LIST, std::vector<Type>{Type::LIST}, 6));
        Components.push_back(Component("Head", Type::INTEGER, std::vector<Type>{Type::LIST}, 7));
        Components.push_back(Component("Last", Type::INTEGER, std::vector<Type>{Type::LIST}, 8));
        Components.push_back(Component("Drop", Type::LIST, std::vector<Type>{Type::LIST,Type::INTEGER}, 9));
        Components.push_back(Component("Access", Type::INTEGER, std::vector<Type>{Type::LIST,Type::INTEGER}, 10));
        Components.push_back(Component("Minimum", Type::INTEGER, std::vector<Type>{Type::LIST}, 11));
        Components.push_back(Component("Maximum", Type::INTEGER, std::vector<Type>{Type::LIST}, 12));
        */

        //Functions for HOF Map
        /*
        Components.push_back(Component("AddOneLambda", Type::LAMBDA_INT, std::vector<Type>{}, 13));
        Components.push_back(Component("SubOneLambda", Type::LAMBDA_INT, std::vector<Type>{}, 14));
        Components.push_back(Component("Mult2Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 15));
        Components.push_back(Component("Div2Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 16));
        Components.push_back(Component("Mult3Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 17));
        Components.push_back(Component("Div3Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 18));
        Components.push_back(Component("Mult4Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 19));
        Components.push_back(Component("Div4Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 20));
        Components.push_back(Component("Pow2Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 21));
        Components.push_back(Component("NegateLambda", Type::LAMBDA_INT, std::vector<Type>{}, 22));
        //Functions for HOF Filter and Count
        Components.push_back(Component("GZLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 23));
        Components.push_back(Component("LZLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 24));
        Components.push_back(Component("IsEvenLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 25));
        Components.push_back(Component("IsOddLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 26));
        //HOF
        Components.push_back(Component("Map", Type::LIST, std::vector<Type>{Type::LIST,Type::LAMBDA_INT}, 27));
        Components.push_back(Component("Filter", Type::LIST, std::vector<Type>{Type::LIST,Type::LAMBDA_BOOL}, 28));
        Components.push_back(Component("Count", Type::INTEGER, std::vector<Type>{Type::LIST,Type::LAMBDA_BOOL}, 29));

        Components.push_back(Component("ZipWithSum", Type::LIST, std::vector<Type>{Type::LIST, Type::LIST}, 30));
        Components.push_back(Component("ZipWithDif", Type::LIST, std::vector<Type>{Type::LIST, Type::LIST}, 31));
        Components.push_back(Component("ZipWithMult", Type::LIST, std::vector<Type>{Type::LIST, Type::LIST}, 32));
        Components.push_back(Component("ZipWithMin", Type::LIST, std::vector<Type>{Type::LIST, Type::LIST}, 33));
        Components.push_back(Component("ZipWithMax", Type::LIST, std::vector<Type>{Type::LIST, Type::LIST}, 34));

        Components.push_back(Component("ScanL1Sum", Type::LIST, std::vector<Type>{Type::LIST}, 35));
        Components.push_back(Component("ScanL1Dif", Type::LIST, std::vector<Type>{Type::LIST}, 36));
        Components.push_back(Component("ScanL1Mult", Type::LIST, std::vector<Type>{Type::LIST}, 37));
        Components.push_back(Component("ScanL1Min", Type::LIST, std::vector<Type>{Type::LIST}, 38));
        Components.push_back(Component("ScanL1Max", Type::LIST, std::vector<Type>{Type::LIST}, 39));
        */
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

choose json parser for C++
https://github.com/nlohmann/json#examples

[funcname] Haskell

Questions:
1) Should we minimize the number of rules we give Z3? For example, it would be beneficial for it to not repeat functions such as sort. 
Adding a new rule that for every node, if it is sort, then its child cannot be sort, would make Z3 have to do a lot of checks, than if it just
made a few extra trees. What would be better option?

2) Making generic placeholder for similar functions. For example, for all functions that take a list and return a list, lets 
make one single placeholder for them during enumerator. Then during the program testing framework, test all the functions that correspond
to that placeholder. We can have the DSL constructor handle this so the user can enter whatever functions they want
Going from 6 components to 13, the number of trees increased from 11 to 5347. This number can be significantly
reduced by having placeholder functions for similar components


3) Is there a scenario where repeating the same function wil give us a new result?

*/



void tree(const DSL& MyDSL,Type ResultType)
{

    std::unordered_map<int, int> RESULTS;
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
    bool has_allempty = true;

    while(!NodeQueue.empty())
    {
        CurrPtr = NodeQueue.front();
        NodeQueue.pop();

        if (CurrPtr->Children.size() == 0)//LEAF NODE
        {
            //fix
            validcomponents.resize(0);
            for (auto a : MyDSL.Components)
            {
                has_allempty = true;
                for (auto b : a.Inputs)
                {
                    if (b != Type::EMPTY)
                        has_allempty = false;
                }
                if (has_allempty) //no inputs
                {
                    validcomponents.push_back(vars[map[CurrPtr]] == a.ID); //can only be component that takes in no input
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

    int count = 0;
        std::cout << s.check() << std::endl;
    while(s.check() == sat)
    {
        count++;
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
            //std::cout  <<v.name()<<" "<< intval << std::endl;
            RESULTS[std::stoi(v.name().str())] = intval;


        }
        for (int k = 0; k < m.size(); k++)
        {
            std::cout << k << ' ';
            for (auto a : MyDSL.Components)
            {
                if (a.ID == RESULTS[k])
                    std::cout << a.Name << std::endl;
            }
        }
       
        s.add(mk_or(vals));
    }
    std::cout << count << std::endl;
}


void run()
{
    DSL MyDSL(Type::INTEGER);
    //MyDSL.Components[0].Inputs.push_back(Type::EMPTY);
    //std::cout<<MyDSL.MaxInputs;

    MyDSL.print();
    std::cout << std::endl;


    //print matches
    for (auto a : MyDSL.Matches)
    {
        std::cout << " THESE NODES: ";
        for (auto b : a.PNodes)
            std::cout << b << ' ';
        std::cout << " MUST HAVE THESE CHILDREN ";
        for (auto b : a.CNodes)
        {
            std::cout << " NEW CHILD: ";
            for (auto c : b)
                std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    tree(MyDSL, Type::INTEGER);
}

/*
Bug 1: Minimum will return 0 both if the smallest element is 0, and if the vector is empty. In C++,  NULL evaluates to zero

Warning 1: For some of the functions, it complains about doing operations with two integers. Something about uisng 4 bytes vs 8 bytes?
*/


int main() 
{


    run();
   Methods testing;
   std::vector<int> myfac{1,2,3,4};
   std::vector<int> myfac2{ 4,4,4,4 };
    std::vector<int> x= testing.ScanL1Sum(myfac);
   
    
    
    system("pause");
    return 0;
}

