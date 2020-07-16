#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"
#include <unordered_map>
#include <queue>
#include <set>
#include "Methods.h"
#include "DSL.h"
#include "Tree.h"
#include "TestingFramework.h"
#include "reader.h"
using namespace z3;

#include "timer.h"




/*

choose json parser for C++
https://github.com/nlohmann/json#examples


[funcname] Haskell

Questions:


3) Is there a scenario where repeating the same function wil give us a new result?


*/

/*
create class: 
solutions
will have vector of ints, vector of vectors
will have string name inputvals with either E, I, L, IL,LL,II (empty, intege,list, integerlist,listlist,intint)
will have sinlge int and single vect as output
will have string name outputvals with either E, I, L (empty, integer, list)
switch(inpuvals). switch will add appropriate components to DSL, and pass the output val to solver to set rule for first node

File template:
(INT) (Valueofint) (LIST) (values) (endsentinel) (LIST) (values) *
(LIST) (valuesoflist) *
(INT) (Valueofint) (LIST) (values) (endsentinel) (LIST) (values) *
(LIST) (valuesoflist) *
(INT) (Valueofint) (LIST) (values) (endsentinel) (LIST) (values) *
(LIST) (valuesoflist) *
(INT) (Valueofint) (LIST) (values) (endsentinel) (LIST) (values) *
(LIST) (valuesoflist) *

in testingframework, will take in this class, and for inputvals, will have appropriate template


*/
/*
1) create system to read examples
2) Add multiple testing cases for test framework
3) Fix access
4) Make list of possible pruning specs
5) add pruning (max,min, etc)


*/

/*
performance
Speed: depth 4, 0.0080 ~0.0095, the more it runs the more it leans toward the latter. occasionally spikes to ~0.0150 - 0.0200
18min, spikes increasing in regularity. 0.82ish still common tho.
19m 71mb
23min, spikes of 0.25 occuring. 83mb, 0.0095-0.0110 more common. half are still 0.008ish
2.7min

*/
/*
Bug 1: Minimum will return 0 both if the smallest element is 0, and if the vector is empty. In C++,  NULL evaluates to zero
//in this case, make the current enumerated program as false

Warning 1: For some of the functions, it complains about doing operations with two integers. Something about uisng 4 bytes vs 8 bytes?

Issue: Need to make Methods more modular, expand to include full DSL
*/

/*

https://sat-group.github.io/ruben/papers/pldi18.pdf
section 1/2

*/

/*
Add restriction that forces solver to use ALL inputs at least once (B and K in this case)


*/


bool tree(int maxL, const DSL& MyDSL, reader textinfo)
{

    std::unordered_map<int, int> RESULTS;
    int MaxInputs = MyDSL.MaxInputs;
    int MaxLines = maxL;

    Trie MyTree(MaxLines, MaxInputs); 
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

    Type CONVERTED;
    if (textinfo.examples[0].Out == IO::I)
        CONVERTED = Type::INTEGER;
    else
        CONVERTED = Type::LIST;
    //FIRST NODE must return the type specified in the DSL. It checkes which components to see which IDs are valid in this case
    for (auto a : MyDSL.Components)
    {
        if (a.Output == CONVERTED )
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
        
    }
    s.add(sum(vars_i) == MaxLines);





   // timer mytimer;
    while (s.check() == sat )//&& !solved
    {
        count++;
        model m = s.get_model();
       // mytimer.begin();
        //std::cout << "Trying New solution with depth: "<<MaxLines << "\n";
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
            std::cout << k << ' ';
            for (auto a : MyDSL.Components)
            {
                if (a.ID == RESULTS[k])
                {
                    std::cout << a.Name << std::endl;
                    fIDs.push_back(a.ID);
                }
            }

        }
        std::cout << std::endl;
        AssignIDs(MyTree, fIDs);

        /*
        access
        b
        k
        */

        for (auto a : textinfo.examples)
        {
            solved = testing(MyTree, a);


            if (!solved)
                break;
        }


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
       // mytimer.print_elapsed();
    }
    std::cout << count << std::endl;
    return false;
}

/*
DSL constructor - rremove type thing
initializer - remove B and K
editor - add appropriate ones
make sure to rebuild DSL for every example
*/
void run(std::string filename)
{

  


    reader textinfo;
    if(!textinfo.open(filename))
        return;
    DSL MyDSL(textinfo);
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

 

    iMax = 1;


   while (!solved && iMax < 5)
   {
       solved = tree(iMax, MyDSL, textinfo);
       iMax++;
   }

   
}

/*
Program5 is unsolvable for current system, unknown issue

*/


int main()
{
   for(int i =0;i<5;i++)
       run("program" + std::to_string(i) + ".txt");



    system("pause");
    return 0;
}
