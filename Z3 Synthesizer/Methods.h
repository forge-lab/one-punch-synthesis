#ifndef METHODS_H
#define METHODS_H

#include <vector>
#include <algorithm>
#include <numeric>
class Methods
{
private:

public:
	/*
	Create separate class that takes in the DSL. Add new value to components, function pointer. Methods class should go
	through all components in DSL and for each component, add that function pointer (and associated ID) to the correct
	vector that cointains the functions of that particular type, I_LI, or I_L, L_L, etc
	
	*/
	std::vector<int> UseFunctionL_LI(int ID, std::vector<int> IN1,int IN2)
	{
		switch (ID)
		{
		case 1:
			return Take(IN1, IN2);
			break;
		case 9:
			return Drop(IN1, IN2);
		}
	}
	int UseFunctionI_LI(int ID, std::vector<int> IN1,int IN2)
	{
		switch (ID)
		{
		case 10:
			return Access(IN1, IN2);
		}
	}
	int UseFunctionI_L(int ID, std::vector<int> IN1)
	{
		switch (ID)
		{
		case 3:
			return Sum(IN1);
			break;
		case 7:
			return Head(IN1);
			break;
		case 8:
			return Last(IN1);
			break;
		case 11:
			return Minimum(IN1);
			break;
		case 12:
			return Maximum(IN1);
			break;

		}
	}
	std::vector<int> UseFunctionL_L(int ID, std::vector<int> IN1)
	{
		switch (ID)
		{
		case 2:
			return Sort(IN1);
			break;
		case 6:
			return Reverse(IN1);
			break;
			
		}
	}





	std::vector<int> Take(std::vector<int> in1, int in2)
	{
		if (in2 < in1.size())
			in1.resize(in2);
		return in1;
	}
	std::vector<int> Drop(std::vector<int> in1, int in2)
	{
		if (in2 > in1.size())
			in2 = in1.size();
		std::vector<decltype(in1)::value_type>(in1.begin() + in2, in1.end()).swap(in1);
		return in1;
	}
	int Access(std::vector<int> in1, int in2) //problem with access???
	{
		if (in2 > in1.size() || in2 <= 0)
			return NULL;
		return in1[in2 - 1]; 
	}
	int Head(std::vector<int> in1)
	{
		return in1.size() ? in1[0] : 0;
	}
	int Last(std::vector<int> in1)
	{
		return in1.size() ? in1[in1.size() - 1] : 0;
	}
	int Minimum(std::vector<int> in1)
	{
		if (!in1.size())
			return NULL;
		return *std::min_element(in1.begin(), in1.end());
	}
	int Maximum(std::vector<int> in1)
	{
		if (!in1.size())
			return NULL;
		return *std::max_element(in1.begin(), in1.end());
	}
	std::vector<int> Reverse(std::vector<int> in1)
	{
		std::reverse(in1.begin(), in1.end());
		return in1;
	}
	std::vector<int> Sort(std::vector<int> in1)
	{
		std::sort(in1.begin(), in1.end());
		return in1;
	}
	int Sum(std::vector<int> in1)
	{
		return std::accumulate(in1.begin(), in1.end(), 0);
	}

	//HOF
	std::vector<int> MapAdd(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it!=in1.end(); it++)
			(*it) += 1;
		return in1;
	}
	std::vector<int> MapSub(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) -= 1;
		return in1;
	}
	std::vector<int> MapMult2(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) *= 2;
		return in1;
	}
	std::vector<int> MapDiv2(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) /= 2;
		return in1;
	}
	std::vector<int> MapNeg(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) *= -1;
		return in1;
	}
	std::vector<int> MapPow(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) = pow(*it,2);
		return in1;
	}
	std::vector<int> MapMult3(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) *= 3;
		return in1;
	}
	std::vector<int> MapDiv3(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) /= 3;
		return in1;
	}
	std::vector<int> MapMult4(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) *= 4;
		return in1;
	}
	std::vector<int> MapDiv4(std::vector<int> in1)
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
			(*it) /= 4;
		return in1;
	}

	std::vector<int> FilterEven(std::vector<int> in1) //Keeps evens
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end();)
		{
			if (*it & 1)
				it = in1.erase(it);
			else
				it++;
		}
		return in1;
	}
	std::vector<int> FilterOdd(std::vector<int> in1) //Keeps odds
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end();)
		{
			if (*it + 1 & 1)
				it = in1.erase(it);
			else
				it++;
		}
		return in1;
	}
	std::vector<int> FilterGZ(std::vector<int> in1) //keeps greater than zero
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end();)
		{
			if (*it<=0)
				it = in1.erase(it);
			else
				it++;
		}
		return in1;
	}
	std::vector<int> FilterLZ(std::vector<int> in1) //keeps greater than zero
	{
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end();)
		{
			if (*it >= 0)
				it = in1.erase(it);
			else
				it++;
		}
		return in1;
	}

	//COUNT


	int CountEven(std::vector<int> in1)
	{
		int i = 0;
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end();it++)
		{
			if (*it%2==0)
				i++;
		}
		return i;
	}
	int CountOdd(std::vector<int> in1) 
	{
		int i = 0;
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
		{
			if (*it % 2 == 1)
				i++;
		}
		return i;
	}
	int CountGZ(std::vector<int> in1) 
	{
		int i = 0;
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
		{
			if (*it > 0)
				i++;
		}
		return i;
	}
	int CountLZ(std::vector<int> in1) 
	{
		int i = 0;
		for (std::vector<int>::iterator it = in1.begin(); it != in1.end(); it++)
		{
			if (*it < 0)
				i++;
		}
		return i;
	}

	//ZIPWITH
	//
	//
	std::vector<int> ZipWithSum(std::vector<int>in1, std::vector<int>in2)
	{
		std::vector<int> returnv(std::min(in1.size(), in2.size()));

		for (int i=0;i<returnv.size();i++)
			returnv[i] = in1[i] + in2[i];
		return returnv;
	}
	std::vector<int> ZipWithDif(std::vector<int>in1, std::vector<int>in2)
	{
		std::vector<int> returnv(std::min(in1.size(), in2.size()));

		for (int i = 0; i < returnv.size(); i++)
			returnv[i] = in1[i] - in2[i];
		return returnv;
	}
	std::vector<int> ZipWithMult(std::vector<int>in1, std::vector<int>in2)
	{
		std::vector<int> returnv(std::min(in1.size(), in2.size()));

		for (int i = 0; i < returnv.size(); i++)
			returnv[i] = in1[i] * in2[i];
		return returnv;
	}
	std::vector<int> ZipWithMax(std::vector<int>in1, std::vector<int>in2)
	{
		std::vector<int> returnv(std::min(in1.size(), in2.size()));

		for (int i = 0; i < returnv.size(); i++)
			returnv[i] = std::max(in1[i], in2[i]);
		return returnv;
	}
	std::vector<int> ZipWithMin(std::vector<int>in1, std::vector<int>in2)
	{
		std::vector<int> returnv(std::min(in1.size(), in2.size()));

		for (int i = 0; i < returnv.size(); i++)
			returnv[i] = std::min(in1[i], in2[i]);
		return returnv;
	}

	//SCANL1

	std::vector<int> ScanL1Sum(std::vector<int>in1)
	{
		std::vector<int> returnv(in1.size());
		returnv[0] = in1[0];

		for (int i = 1; i < returnv.size(); i++)
		{
			returnv[i] = in1[i] + returnv[i - 1];
		}
		return returnv;
	}
	std::vector<int> ScanL1Dif(std::vector<int>in1)
	{
		std::vector<int> returnv(in1.size());
		returnv[0] = in1[0];

		for (int i = 1; i < returnv.size(); i++)
		{
			returnv[i] = in1[i] - returnv[i - 1];
		}
		return returnv;
	}
	std::vector<int> ScanL1Mult(std::vector<int>in1)
	{
		std::vector<int> returnv(in1.size());
		returnv[0] = in1[0];

		for (int i = 1; i < returnv.size(); i++)
		{
			returnv[i] = in1[i] * returnv[i - 1];
		}
		return returnv;
	}
	std::vector<int> ScanL1Max(std::vector<int>in1)
	{
		std::vector<int> returnv(in1.size());
		returnv[0] = in1[0];

		for (int i = 1; i < returnv.size(); i++)
		{
			returnv[i] = std::max(in1[i], returnv[i - 1]);
		}
		return returnv;
	}
	std::vector<int> ScanL1Min(std::vector<int>in1)
	{
		std::vector<int> returnv(in1.size());
		returnv[0] = in1[0];

		for (int i = 1; i < returnv.size(); i++)
		{
			returnv[i] = std::min(in1[i], returnv[i - 1]);
		}
		return returnv;
	}
};

#endif