#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"
#include <unordered_map>
using namespace z3;


//will print all numbers between range i and j
void all(int i, int j)
{
    assert(i <= j);
   
    context c;
    expr x = c.int_const("x");
    solver s(c);

    s.add(x >= i);
    s.add(x <= j);
    std::cout << s.check() << "\n";

    model m = s.get_model();
    std::cout << m << "\n";
    func_decl v = m[0];
    assert(v.arity() == 0);

    std::string stringval = m.get_const_interp(v).to_string();
    int intval = std::stoi(stringval);

    std::cout << "Solution: x = " << intval << std::endl;
    s.add(x != intval);
    while (s.check() == sat)
    {
        m = s.get_model();
       
        v = m[0];
        assert(v.arity() == 0);

        stringval = m.get_const_interp(v).to_string();
        intval = std::stoi(stringval);

        std::cout << "Solution: x = " << intval << std::endl;
        s.add(x != intval);
    }

}

//permutations of length num
void permut(int num)
{

    
    assert(num >= 2 && num <= 26);
    context c;
    std::vector<expr> vars;
    char NAME[2];
    NAME[0] = 'a';
    NAME[1] = '\0';
    for (int i = 0; i < num; i++)
    {
        vars.push_back(c.int_const(NAME));
        NAME[0]++;
    }
    solver s(c);
    for (int i = 0; i < num; i++)
    {
        s.add(vars[i] >= 0 && vars[i] <= 9);
        for (int j = i + 1; j < num; j++)
            s.add(vars[i] != vars[j]);

    }
   
    expr_vector vals(c);

    while (s.check() == sat)
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
            vals.push_back(vars[i] != intval);
            std::cout << "Solution set: = " << intval << std::endl;

        }
        

        s.add(mk_and(vals));

    }



}


//combinations of length num
void combin(int num)
{

    assert(num >= 2 && num <= 26);
    context c;
    std::vector<expr> vars;
    char NAME[2];
    NAME[0] = 'a';
    NAME[1] = '\0';
    for (int i = 0; i < num; i++)
    {
        vars.push_back(c.int_const(NAME));
        NAME[0]++;
    }

    solver s(c);
    for (int i = 0; i < num; i++)
        s.add(vars[i] >= 0 && vars[i] <= 9);
    std::cout << s.check() << "\n";

    expr_vector vals(c);

    while (s.check() == sat)
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
            vals.push_back(vars[i] != intval);
            std::cout << "Solution set: = " << intval << std::endl;

        }

        //add mk_or
        s.add(mk_and(vals));

    }



}



//will print all trees of given depth, such that each child is even/odd based on if their parent is even/odd. Prints all possible trees.
void tree(int depth)
{

    //User enters depth of tree
    assert(depth > 0 && depth < 10);
    context c;
    std::vector<expr> vars;
    double total = 0;
    //name cstring is used for giving the variables in Z3 different names. Starts naming with "a"
    std::string NAME;


    std::unordered_map<std::string, int> map;


    //calculates total number of elements in tree based on user defined depth. i.e. 3 = 15, 2 = 7, 1= 1, etc
    while (depth != -1)
    {
        total += pow(2, depth); depth--;
    }
    //creates required number of elements PLUS one. Makes accessing correct variable easier later on
    for (int i = 0; i < total; i++)
    {
        NAME = "X"+std::to_string(i);
        vars.push_back(c.int_const(NAME.c_str()));
        map[NAME] = i;
    }
    solver s(c);

    expr_vector vals(c);


  
    for (int i = 0; i < total; i++)
    {
        //first condition is that all variables are between 0 and 9
        s.add(vars[i] >= 0 && vars[i] <= 9);

        if (i != 0) // if they are NOT the first element, then check if they are odd, then children are odd. if they are even, children are even
            s.add(implies(vars[(i-1) / 2] % 2 == 0, vars[i] % 2 == 0) && implies(vars[(i - 1)/2]%2==1,vars[i]%2==1));
    }
   
   
    while (s.check() == sat)
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
            vals.push_back(vars[map[v.name().str()]] != intval);
            std::cout  <<v.name()<<" "<< intval << std::endl;

        }
       
        s.add(mk_or(vals));
    }

}

int main() 
{
    
    system("pause");
    return 0;
}






