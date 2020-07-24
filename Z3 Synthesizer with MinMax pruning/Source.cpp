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
before prune //after prune
p0: 596 // 1214
p1: 3220 // 2524
p2: 64 // 94
p3: 5265 // 2092
p4: n/a
p5: 117 // 143
p6: 665 // 665 (diff but valid sol) 
p7: 234 // 215
p8: 347  // 127 (diff but valid sol)



*/


/*
real pruning values:
p0: 70 //596
p1: 2707 //3220
p2: 57 //  64
p3: 727 // 5265
p4: n/a
p5: 141 // 117
p6: 433 // 665
p7: 192 //234
p8: 149 // 347
total: 4471 // 10507 ~57.44% improvement
27 seconds vs 60

searchbase:
p0: 562 // 2165
p1: 4440 // 10152
p2: 6684 // 10152
p3: 3196 //8145
p4: n/a
p5: 7667 // 30728
p6: 3368 // 10152
p7: 4440 // 10152
p8: 3196 // 8145

total: 33553 // 89791 ~62.63% improvement

*/



/*

choose json parser for C++
https://github.com/nlohmann/json#examples


[funcname] Haskell

Questions:


3) Is there a scenario where repeating the same function wil give us a new result?


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

Behavior CheckBehavior(bool formax, reader textinfo)
{
    std::vector<Behavior> results;
    for (auto a : textinfo.examples)
    {

        if (a.Ins == IO::I || a.Ins == IO::II)
            return Behavior::NOTDEFINED;
        else if (a.Ins == IO::L || a.Ins == IO::LI)
        {
            int smallest_element = a.INPUTLISTS[0][0]; //let, first element is the smallest one
            int largest_element = a.INPUTLISTS[0][0]; //also let, first element is the biggest one
            for (int i = 1; i < a.INPUTLISTS[0].size(); i++)  //start iterating from the second element
            {
                if (a.INPUTLISTS[0][i] < smallest_element)
                {
                    smallest_element = a.INPUTLISTS[0][i];
                }
                if (a.INPUTLISTS[0][i] > largest_element)
                {
                    largest_element = a.INPUTLISTS[0][i];
                }
            }

            int output_max;
            int output_min;
            if (a.Out == IO::I)
            {
                output_min = a.OUTPUTINT;
                output_max = a.OUTPUTINT;
            }
            else
            {
                output_max = a.OUTPUTLIST[0];
                output_min = a.OUTPUTLIST[0];
                for (int i = 1; i < a.OUTPUTLIST.size(); i++)  //start iterating from the second element
                {
                    if (a.OUTPUTLIST[i] < output_min)
                    {
                        output_min = a.OUTPUTLIST[i];
                    }
                    if (a.OUTPUTLIST[i] > output_max)
                    {
                        output_max = a.OUTPUTLIST[i];
                    }
                }

            }

            if (formax)
            {
                if (output_max > largest_element)
                    results.push_back(Behavior::MAXPLUS);
                else if (output_max < largest_element)
                    results.push_back(Behavior::MAXMINUS);
                else
                    return Behavior::NOTDEFINED;
            }
            else
            {
                if (output_min > smallest_element)
                    results.push_back(Behavior::MINPLUS);
                else if (output_min < smallest_element)
                    results.push_back(Behavior::MAXMINUS);
                else
                    return Behavior::NOTDEFINED;
            }
        }
        else
        {
            int smallest_element0 = a.INPUTLISTS[0][0]; //let, first element is the smallest one
            int largest_element0 = a.INPUTLISTS[0][0]; //also let, first element is the biggest one
            for (int i = 1; i < a.INPUTLISTS[0].size(); i++)  //start iterating from the second element
            {
                if (a.INPUTLISTS[0][i] < smallest_element0)
                {
                    smallest_element0 = a.INPUTLISTS[0][i];
                }
                if (a.INPUTLISTS[0][i] > largest_element0)
                {
                    largest_element0 = a.INPUTLISTS[0][i];
                }
            }
            int smallest_element1 = a.INPUTLISTS[1][0]; //let, first element is the smallest one
            int largest_element1 = a.INPUTLISTS[1][0]; //also let, first element is the biggest one
            for (int i = 1; i < a.INPUTLISTS[1].size(); i++)  //start iterating from the second element
            {
                if (a.INPUTLISTS[1][i] < smallest_element1)
                {
                    smallest_element1 = a.INPUTLISTS[1][i];
                }
                if (a.INPUTLISTS[1][i] > largest_element1)
                {
                    largest_element1 = a.INPUTLISTS[1][i];
                }
            }
            int input_max = std::max(largest_element0, largest_element1);
            int input_min = std::max(smallest_element0, smallest_element1);

            int output_max;
            int output_min;
            if (a.Out == IO::I)
            {
                output_min = a.OUTPUTINT;
                output_max = a.OUTPUTINT;
            }
            else
            {
                output_max = a.OUTPUTLIST[0];
                output_min = a.OUTPUTLIST[0];
                for (int i = 1; i < a.OUTPUTLIST.size(); i++)  //start iterating from the second element
                {
                    if (a.OUTPUTLIST[i] < output_min)
                    {
                        output_min = a.OUTPUTLIST[i];
                    }
                    if (a.OUTPUTLIST[i] > output_max)
                    {
                        output_max = a.OUTPUTLIST[i];
                    }
                }

            }

            if (formax)
            {
                if (output_max > input_max)
                    results.push_back(Behavior::MAXPLUS);
                else if (output_max < input_max)
                    results.push_back(Behavior::MAXMINUS);
                else
                    return Behavior::NOTDEFINED;
            }
            else
            {
                if (output_min > input_min)
                    results.push_back(Behavior::MINPLUS);
                else if (output_min < input_min)
                    results.push_back(Behavior::MAXMINUS);
                else
                    return Behavior::NOTDEFINED;
            }
        }
    }

    
    for (auto a : results)
    {
        if (a != results[0])
            return Behavior::NOTDEFINED;
    }
    return results[0];
}


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


    //All variables must be equal to a value corresponding to a component ID (component IDs MUST be in-order)
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////ADD VALID IDs TO ENSURE ONLY THOSE CAN BE GENERATED BY ENUMERATOR/////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<int> AllFuncIDs;
    expr_vector ValidIDsForVars(c);
    for (auto a : MyDSL.Components)
        ValidIDsForVars.push_back(vars[i] == a.ID);
    s.add(mk_or(ValidIDsForVars));
    
    //s.add(vars[i] >= 0 && vars[i] <= int(MyDSL.Components.size())); // old version using length of components
    //note, we only actually need to add these rules to the head node. The rest will obey the implication rules

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////ADD VALID IDs TO ENSURE ONLY THOSE CAN BE GENERATED BY ENUMERATOR/////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    //std::cout <<"new"<< s.check() << std::endl;
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

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////USING ALL GIVEN INPUTS//////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<int> IDsofInputs;
    expr_vector UsesInputs(c);
    for (auto a : MyDSL.Components)
    {
        if (a.Name == "K" || a.Name == "K1" || a.Name == "K2" || a.Name == "B" || a.Name == "B1" || a.Name == "B2")
            IDsofInputs.push_back(a.ID);
    }
    for (auto a : IDsofInputs)
    {
        UsesInputs.resize(0);
        for (int k = 0; k <= i; k++)
        {

            UsesInputs.push_back(vars[k] == a);
        }

        s.add(mk_or(UsesInputs));
    }



    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////MAX PRUNING///////////////////////////////////////////////////////////////////////
   


    
    Behavior ValChangeMax = CheckBehavior(true, textinfo);
    Behavior ValChangeMin = CheckBehavior(false,textinfo);

    //checks if we can apply max/min pruning
    
    

    expr_vector pruningrule(c);
    if (ValChangeMax != Behavior::NOTDEFINED || ValChangeMin != Behavior::NOTDEFINED)
    {
        pruningrule.resize(0);
        for (auto a : MyDSL.Components)
        {

            if ((a.ValChangeMAX == ValChangeMax && a.ValChangeMAX != Behavior::NOTDEFINED) || (a.ValChangeMIN == ValChangeMin && a.ValChangeMIN != Behavior::NOTDEFINED))
            {
                
                for (int k = 0; k <= i; k++)
                {
                    pruningrule.push_back(vars[k] == a.ID);
                }
                
            }
            
        }
        //std::cout << mk_or(pruningrule) << std::endl;

        s.add(mk_or(pruningrule));
    }
 

   //std::cout << mk_or(pruningrule) << std::endl;
   




    std::cout << s.check() << std::endl;
    //std::cout<<s<<std::endl; //prints model

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
              //  std::cout << v.name().str() << std::endl;

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
           // std::cout << k << ' ';
            for (auto a : MyDSL.Components)
            {
                if (a.ID == RESULTS[k])
                {
                   // std::cout << a.Name << std::endl;
                    fIDs.push_back(a.ID);
                }
            }

        }
       // std::cout << std::endl;
        AssignIDs(MyTree, fIDs);
        //if (checkifismine(MyTree, { 11,30,0,27,27,0,0,5,14,4,14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }))
        {
            for (auto a : textinfo.examples)
            {


                solved = testing(MyTree, a);
                

                if (!solved)
                    break;
            }
        }


        if (solved)
        {
            std::cout << "SOLVED" << std::endl;
            for (int k = 0; k < m.size() / 2; k++)
            {
                std::cout << k << ' ';
                for (auto a : MyDSL.Components)
                    if (a.ID == RESULTS[k])
                    {
                        std::cout << a.Name << std::endl;
                    }

            }
            std::cout << "Enumerated this many before finding (for current level): " << count << std::endl;
            return true;

        }
        
        s.add(mk_or(vals));
       // mytimer.print_elapsed();
    }
    std::cout << count << std::endl;
    return false;
}

/*


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
    //std::cout << std::endl;
    int iMax = 1;

 



   while (!solved && iMax <4)
   {
       solved = tree(iMax, MyDSL, textinfo);
       iMax++;
   }

   
}

/*
create stats on powerpoint for number enumerated/time taken for each program (except 4)
*/

int main()
{
   for(int i =0;i<=8;i++)
       run("program" + std::to_string(i) + ".txt");


   /*
   07/19 tests 10,507 programs per minute, ~175 per second
   
   */

   system("pause");
    return 0;

}
