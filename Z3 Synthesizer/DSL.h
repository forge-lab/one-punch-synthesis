#ifndef DSL_H
#define DSL_H
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <set>
#include "reader.h"


enum class Type
{
    EMPTY, //0
    LIST, //1
    INTEGER, //2
    LAMBDA_INT, //3, for Map, a type of function that returns ints
    LAMBDA_BOOL //4, for Filter and Count, a type of function that returns bools
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
    bool isLine;
    Component(std::string N, Type O, std::vector<Type> I, int J,bool B = true) :Name(N), Output(O), Inputs(I), ID(J), isLine(B) {};
};

class DSL
{
public:
    std::vector<Component> Components;
    int MaxInputs;
    std::vector<MatchIO> Matches;

    //public:
    DSL(reader textinfo)
    {
        //Creating all components
        Components.push_back(Component("Empty", Type::EMPTY, std::vector<Type>(), 0,false)); //Empty component
       Components.push_back(Component("Take", Type::LIST, std::vector<Type>{Type::LIST, Type::INTEGER}, 1)); //Take component
       Components.push_back(Component("Sort", Type::LIST, std::vector<Type>{Type::LIST}, 2)); //Sort component
       Components.push_back(Component("Sum", Type::INTEGER, std::vector<Type>{Type::LIST}, 3)); 
        switch (textinfo.examples[0].Ins)
        {
        case IO::I:
            Components.push_back(Component("K", Type::INTEGER, std::vector<Type>(), 4, false));
            break;
        case IO::L:
            Components.push_back(Component("B", Type::LIST, std::vector<Type>(), 4, false));
            break;
        case IO::LI:
            Components.push_back(Component("B", Type::LIST, std::vector<Type>(), 4, false)); //B List component
            Components.push_back(Component("K", Type::INTEGER, std::vector<Type>(), 5, false)); //K Integer component
            break;
        case IO::II:
            Components.push_back(Component("K1", Type::INTEGER, std::vector<Type>(), 4, false)); //k1 int component
            Components.push_back(Component("K2", Type::INTEGER, std::vector<Type>(), 5, false)); //K2 Integer component
            break;
        case IO::LL:
            Components.push_back(Component("B1", Type::LIST, std::vector<Type>(), 4, false)); //B List component
            Components.push_back(Component("B2", Type::LIST, std::vector<Type>(), 5, false)); //K Integer component
            break;
        }

        
       Components.push_back(Component("Reverse", Type::LIST, std::vector<Type>{Type::LIST}, 6));
        Components.push_back(Component("Head", Type::INTEGER, std::vector<Type>{Type::LIST}, 7));
       Components.push_back(Component("Last", Type::INTEGER, std::vector<Type>{Type::LIST}, 8));
      Components.push_back(Component("Drop", Type::LIST, std::vector<Type>{Type::LIST,Type::INTEGER}, 9));
        Components.push_back(Component("Access", Type::INTEGER, std::vector<Type>{Type::LIST,Type::INTEGER}, 10));
        Components.push_back(Component("Minimum", Type::INTEGER, std::vector<Type>{Type::LIST}, 11));
        Components.push_back(Component("Maximum", Type::INTEGER, std::vector<Type>{Type::LIST}, 12)); 
        
        
        //Functions for HOF Map
     
        
        Components.push_back(Component("AddOneLambda", Type::LAMBDA_INT, std::vector<Type>{}, 13,false));
        Components.push_back(Component("SubOneLambda", Type::LAMBDA_INT, std::vector<Type>{}, 14,false));
        Components.push_back(Component("Mult2Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 15,false));
        Components.push_back(Component("Div2Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 16,false));
        Components.push_back(Component("Mult3Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 17,false));
        Components.push_back(Component("Div3Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 18,false));
        Components.push_back(Component("Mult4Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 19,false));
        Components.push_back(Component("Div4Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 20,false));
        Components.push_back(Component("Pow2Lambda", Type::LAMBDA_INT, std::vector<Type>{}, 21,false));
        Components.push_back(Component("NegateLambda", Type::LAMBDA_INT, std::vector<Type>{}, 22,false));
        //Functions for HOF Filter and Count

        Components.push_back(Component("GZLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 23,false));
        Components.push_back(Component("LZLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 24,false));
        Components.push_back(Component("IsEvenLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 25,false));
        Components.push_back(Component("IsOddLambda", Type::LAMBDA_BOOL, std::vector<Type>{}, 26,false));
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
        
        //MaxInputs keeps track of the maximum number of inputs a component can have



        MaxInputs = 0;
        for (auto a : Components)
            MaxInputs = std::max(MaxInputs, int(a.Inputs.size()));

        //If a component has less defined inputs than the max, we add EMPTY inputs
        for (std::vector<Component>::iterator it = Components.begin(); it != Components.end(); it++)
        {
            while (it->Inputs.size() < MaxInputs)
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
            for (auto a : *std::next(AllInputs.begin(), i)) //for all INPUT possibilities
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


#endif