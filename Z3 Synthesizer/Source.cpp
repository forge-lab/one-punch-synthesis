#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"
#include <unordered_map>
#include <queue>
#include <set>
#include "Methods.h"
#include "DSL.h"
#include "Tree.h"
#include "TestingFramework.h"
using namespace z3;


/*

choose json parser for C++
https://github.com/nlohmann/json#examples


[funcname] Haskell

Questions:


3) Is there a scenario where repeating the same function wil give us a new result?


*/

/*
todo:
make testing framework modular
try examples
manually write list of properties that could be used for pruning (min,max,first,last,size,unique)
look at json thing,

*/
/*
1) Create two-phase solver
2) Add multiple testing cases for test framework
3) Fix access??

*/
/*
Bug 1: Minimum will return 0 both if the smallest element is 0, and if the vector is empty. In C++,  NULL evaluates to zero
//in this case, make the current enumerated program as false

Warning 1: For some of the functions, it complains about doing operations with two integers. Something about uisng 4 bytes vs 8 bytes?

Issue: Need to make Methods more modular, expand to include full DSL
*/

/*
use chrono to time for each enum
https://sat-group.github.io/ruben/papers/pldi18.pdf
section 1/2

*/

/*
Add restriction that forces solver to use ALL inputs at least once (B and K in this case)


*/

bool tree(int maxL, const DSL& MyDSL, Type ResultType, int OUTPUT, std::vector<int> INPUTLIST, int INPUTNUM)
{

    std::unordered_map<int, int> RESULTS;
    int MaxInputs = MyDSL.MaxInputs;
    int MaxLines = maxL;

    Trie MyTree(MaxLines, MaxInputs); //Depth of 3 is hardcoded, number of children is not
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

    while (!NodeQueue.empty())
    {




        CurrPtr = NodeQueue.front();
        NodeQueue.pop();

        if (CurrPtr->Children.size() == 0)//LEAF NODE
        {

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
                    ParentValMKOR.push_back(vars[map[CurrPtr]] == b);
                for (auto b : a.CNodes)
                {
                    ChildValMKOR.resize(0);
                    for (auto c : b)
                        ChildValMKOR.push_back(vars[map[CurrPtr->Children[j]]] == c);
                    s.add(implies(mk_or(ParentValMKOR), mk_or(ChildValMKOR)));
                    j++;
                }
            }
        }
    }


    expr_vector vals(c);

    std::vector<int> fIDs;
    int count = 0;
    std::cout << s.check() << std::endl;
    bool solved = false;
 /*
 depth = lines of code;
 */



    //lines of code
    //needs to be modulized
    expr_vector vars_i(c);
    expr_vector haslines(c);
    expr_vector nolines(c);
    
    


    for (int i = 0; i < vars.size(); i++)
    {
        vars_i.push_back(c.int_const(("i" + std::to_string(i)).c_str()));
        haslines.resize(0);
        nolines.resize(0);
        for (auto a : MyDSL.Components)
        {
            if (a.isLine)
            {
                haslines.push_back(vars[i] == a.ID);
            }
            else
                nolines.push_back(vars[i] == a.ID);
        }

     
        

        s.add(implies(mk_or(haslines), vars_i[i] == 1));
        s.add(implies(mk_or(nolines), vars_i[i] == 0));
        

        
        //s.add(implies(vars[i] == 1 || vars[i] == 2 || vars[i] ==3|| vars[i] == 6 || vars[i] == 7 || vars[i] == 8 || vars[i] == 9 || vars[i] == 10 || vars[i] == 11 || vars[i] == 12 , vars_i[i] == 1));
        //s.add(implies(vars[i] == 0 || vars[i] == 4 || vars[i] == 5, vars_i[i] == 0));
    }
    s.add(sum(vars_i) == MaxLines);





    /*
    */
    while (s.check() == sat && !solved)//
    {
        count++;
        model m = s.get_model();

        std::cout << "Trying New solution:" << "\n";
        while (!vals.empty())
            vals.pop_back();
        for (unsigned i = 0; i < m.size(); i++)
        {
            func_decl v = m[i];
            assert(v.arity() == 0);

            // if(v.name().str()[0] == 'i')
              //   std::cout << v.name().str() << std::endl;

            if (v.name().str()[0] != 'i')
            {
                std::string stringval = m.get_const_interp(v).to_string();
                int intval = std::stoi(stringval);
                vals.push_back(vars[std::stoi(v.name().str())] != intval);
                //std::cout << v.name() << " " << intval << std::endl;
                RESULTS[std::stoi(v.name().str())] = intval;
            }


        }
        fIDs.clear();
        for (int k = 0; k < m.size() / 2; k++)
        {
            //std::cout << k << ' ';
            for (auto a : MyDSL.Components)
            {
                if (a.ID == RESULTS[k])
                {
                   // std::cout << a.Name << std::endl;
                    fIDs.push_back(a.ID);
                }
            }

        }
        AssignIDs(MyTree, fIDs);
        solved = testing(MyTree, OUTPUT, INPUTLIST, INPUTNUM);
        if (solved)
        {
            for (int k = 0; k < m.size() / 2; k++)
            {
                std::cout << k << ' ';
                for (auto a : MyDSL.Components)
                    if (a.ID == RESULTS[k])
                    {
                        std::cout << a.Name << std::endl;

                    }

            }
            return true;
        }
        s.add(mk_or(vals));
    }
    std::cout << count << std::endl;
    return false;
}


void run()
{
    DSL MyDSL(Type::INTEGER);
    int OUTPUT = 7;
    std::vector<int> INPUTLIST = { 3,5,4,7,5 };
    int INPUTNUM = 2;
    MyDSL.print();
    std::cout << std::endl;
    bool solved = false;


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

    int iMax = 1;
    while (!solved)
    {
        solved = tree(iMax,MyDSL, Type::INTEGER, OUTPUT, INPUTLIST, INPUTNUM);
        iMax++;
    }
}



int main()
{


    run();



    system("pause");
    return 0;
}
