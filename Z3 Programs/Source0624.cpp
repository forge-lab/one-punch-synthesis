#include<vector>
#include"C:\Users\user1\Desktop\z3-4.8.8-x64-win\include\z3++.h"

using namespace z3;


//works
void all(int i, int j)//i is low range, j is high. All numbers i to j
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

//works
void permut()
{

    int num = 5;
    assert(num >= 2 && num <= 26);
    context c;
    std::vector<expr> vars;
    std::vector<int> vals(num);
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
    std::cout << s.check() << "\n";


    while (s.check() == sat)
    {
        model m = s.get_model();
         std::cout << "New solution:" << "\n";
        for (unsigned i = 0; i < m.size(); i++)
        {
            func_decl v = m[i];
            assert(v.arity() == 0);

            std::string stringval = m.get_const_interp(v).to_string();
            int intval = std::stoi(stringval);
            vals[i] = intval;
            std::cout << "Solution set: = " << intval << std::endl;

        }

        s.add(vars[0] != vals[0] || vars[1] != vals[1] || vars[2] != vals[2] || vars[3] != vals[3] || vars[4] != vals[4]);

    }



}


//works
void combin()
{

    int num = 5;
    assert(num >= 2 && num <= 26);
    context c;
    std::vector<expr> vars;
    std::vector<int> vals(num);
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


    while (s.check() == sat)
    {
        model m = s.get_model();
        std::cout << "New solution:" << "\n";
        for (unsigned i = 0; i < m.size(); i++)
        {
            func_decl v = m[i];
            assert(v.arity() == 0);

            std::string stringval = m.get_const_interp(v).to_string();
            int intval = std::stoi(stringval);
            vals[i] = intval;
            std::cout << "Solution set: = " << intval << std::endl;

        }

        s.add(vars[0] != vals[0] || vars[1] != vals[1] || vars[2] != vals[2] || vars[3] != vals[3] || vars[4] != vals[4]);

    }



}


/*

                                  X1
               X2                                 X3
     X4                 X5                 X6              X7
x8        x9         x10   x11          x12   x13       x14   x15
all possibilities


for X1, no restriction (1-15)
for X2, if X1 odd then X2 must be odd. Else, even
for X3, if X1 odd then X3 must be odd. Else, even
for X4, if X2 odd then X4 must be odd. Else, even

*/


//only displays one tree.
void tree(int depth)
{
    //User enters depth of tree
    assert(depth > 0 && depth < 10);
    context c;
    std::vector<expr> vars;
    double total = 0;
    //name cstring is used for giving the variables in Z3 different names. Starts naming with "a"
    char NAME[2];
    NAME[0] = 'a';
    NAME[1] = '\0';
    //calculates total number of elements in tree based on user defined depth. i.e. 3 = 15, 2 = 7, 1= 1, etc
    while (depth != -1)
    {
        total += pow(2, depth); depth--;
    }
    //creates required number of elements PLUS one. Makes accessing correct variable easier later on
    for (int i = 0; i <= total; i++)
    {
        vars.push_back(c.int_const(NAME));
        NAME[0]++;
    }
    solver s(c);
    for (int i = 1; i <= total; i++)
    {
        //first condition is that all variables are between 0 and 9
        s.add(vars[i] >= 0 && vars[i] <= 9);
        
        if (i != 1) // if they are NOT the first element, then check if they are odd, then children are odd. if they are even, children are even
            if (vars[i / 2] == 1 || vars[i / 2] == 3 || vars[i / 2] == 5 || vars[i / 2] == 7 || vars[i / 2] == 9)
                s.add(vars[i] == 1 || vars[i] == 3 || vars[i] == 5 || vars[i] == 7 || vars[i] == 9);
            else if (vars[i / 2] == 0 || vars[i / 2] == 2 || vars[i / 2] == 4 || vars[i / 2] == 6 || vars[i / 2] == 8)
                s.add(vars[i] == 0 || vars[i] == 2 || vars[i] == 4 || vars[i] == 6 || vars[i] == 8);
    }
    //because the first element is irrelevant in the vector, we just have it be equal to 1. Not technically necessary.
    s.add(vars[0] == 1);


    if (s.check() == sat)
    {
        model m = s.get_model();
        std::cout << "New solution:" << "\n";

        for (unsigned i = 0; i < m.size(); i++)
        {
            func_decl v = m[i];
            assert(v.arity() == 0);
            std::cout << m.get_const_interp(v) << std::endl;

        }
    }

}

int main() 
{
    tree(3);
   
    system("pause");
    return 0;
}






