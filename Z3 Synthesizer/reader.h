#ifndef READER_H
#define READER_H
#include <fstream>
//reads input examples from .txt files
//hardcoded to have at most 2 inputs (i.e. a number and list, or two lists, or two numbers.)
//can have as many examples

enum class IO
{
    I, //0
    L,
    IL,
    LL,
    II
};

struct example
{
    IO Ins;
    IO Out;

    std::vector<int> INPUTINTS;
    std::vector<std::vector<int>> INPUTLISTS;
    int OUTPUTINT;
    std::vector<int> OUTPUTLIST;
    
    void IOCheck()
    {
        if (INPUTINTS.size() == 2)
            Ins = IO::II;
        else if (INPUTINTS.size() == 1 && INPUTLISTS.size() == 1)
            Ins = IO::IL;
        else if (INPUTINTS.size() == 1 && INPUTLISTS.size() == 0)
            Ins = IO::I;
        else if (INPUTINTS.size() == 0 && INPUTLISTS.size() == 2)
            Ins = IO::LL;
        else if (INPUTINTS.size() == 0 && INPUTLISTS.size() == 1)
            Ins = IO::L;

        if (OUTPUTLIST.size() == 0)
            Out = IO::I;
        else
            Out = IO::L;


    }
};


class reader
{
public:

    std::vector<example> examples;

    int numofexamples;
    reader()
    {
        std::vector<int> tempv;
        std::ifstream myfile;
        std::string line;
        myfile.open("examples.txt", std::ios::in);
        int str_i = 0;
        int numtemp = 0;
        int linenum = 0;
        numofexamples = 0;
        if (myfile.is_open())
        {
         
            while (std::getline(myfile, line))
            {
                str_i = 0;
                examples.resize(numofexamples + 1);
                while (str_i < line.length())
                {
                    numtemp = 0;
                    
                    if (line[str_i] == '0')
                    {
                        str_i += 2;
                        while (str_i < line.length() && line[str_i] != ' ')
                        {
                            numtemp = 10 * numtemp + (int(line[str_i] - 48));
                            str_i++;
                        }
                        if (linenum % 2 == 0)
                        {
                            examples[numofexamples].INPUTINTS.push_back(numtemp);
                        }
                        else
                        {
                            examples[numofexamples].OUTPUTINT = numtemp;
                        }
                        numtemp = 0;
                        str_i++;
                    }
                    else if(line[str_i] =='1')
                    {
                        str_i += 2;
                        while (str_i < line.length() && line[str_i] != '*')
                        {                  

                            while (str_i < line.length() && line[str_i] != ' ')
                            {
                                numtemp = 10 * numtemp + (int(line[str_i] - 48));
                                str_i++;
                            }
                            tempv.push_back(numtemp);
                            str_i++;
                            numtemp = 0;
                        }
                        if (linenum % 2 == 0)
                        {
                            examples[numofexamples].INPUTLISTS.push_back(tempv);
                        }
                        else
                        {
                            examples[numofexamples].OUTPUTLIST = tempv;
                        }
                        tempv.clear();
                        str_i+=2;
                    }
                }
                linenum++;
                if (linenum % 2 == 0)
                    numofexamples++;
            }


         

            myfile.close();
            
        }
        else std::cerr << "ERROR OPENING FILE" << std::endl;
        //will read this from file

        for (std::vector<example>::iterator it = examples.begin(); it != examples.end(); it++)
            (*it).IOCheck();

    }
};


#endif