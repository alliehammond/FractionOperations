#include <iostream>
#include <string>
#include "Fraction.h"

enum class Operator
{
    MULT,
    DIV,
    ADD,
    SUBTR,
    OPERATORCOUNT
};

//Returns the first integer input from string, returns -1 if invalid input or -2 if overflow input
//ret.first == first integer, ret.second == position of next character in string after integer
//next character position invalid on error
std::pair<int, unsigned int> GetFirstPositiveIntegerFromString(std::string input)
{
    //Read an integer from the string and make substr until end of string, 11 characters is reached, or a non digit is reached
    //This is necessary to reverse the integer part of the string for calculating the number
    std::string revString = "";
    for(unsigned i = 0; i < input.size() && i < 12; ++i)
    {
        if(isdigit(input[i]))revString += input[i];
        else
            break;
    }

    //Assign revstring to input and reverse it for correct calculation of output (specifically the pow part)
    std::reverse(revString.begin(), revString.end());

    if(revString.size() == 0)return std::pair<int, unsigned int>(-1, 0);

    int64_t output = 0;
    unsigned i = 0;
    //Loop through string, taking input until a non digit character while checking for int32 overflow
    while(i < revString.size())
    {
        if(!isdigit(revString[i]))break;
        
        int curNum = revString[i] - '0';
        output += static_cast<int64_t>(pow(10, i)) * curNum;

        if(output > INT32_MAX)return std::pair<int, unsigned int>(-2, i + 1);

        ++i;
    }

    return std::pair<int, unsigned int>(static_cast<int>(output), i);
}

//Helper function for GetFractionInput(..)
Fraction GetErrorFractionReturn(int num)
{
    if(num == -1)return Fraction(1, 1);
    if(num == -2)
    {
        Fraction ret(1, 1);
        ret.SetOverflowBit();
        return ret;
    }
    return Fraction(1, 1);
}

//Boolean return value is true on success, false on failure
std::pair<Fraction, bool> GetFractionInput(std::string input)
{
    //Input algorithm works by taking substrings of input after each operation and doing input operations on those substrings
    int negativeBool = 1;
    int wholeNumber = 0, numerator = 1, denominator = 1;
    //Check for negative sign
    if(input.size() > 1 && input[0] == '-')
    {
        negativeBool = -1;
        input = input.substr(1);
    }
    
    //Get first number
    std::pair<int, unsigned> num = GetFirstPositiveIntegerFromString(input);
    //Return errors if they exist
    if(num.first < 0)return std::pair<Fraction, bool>(GetErrorFractionReturn(num.first), false);
    input = input.substr(num.second);

    //Check special case where input is only a whole number
    if(input.size() == 0)
    {
        return std::pair<Fraction, bool>(Fraction(num.first * negativeBool, 1), true);
    }

    //Check if the number just got is the numerator or a whole number
    if(input[0] != '_')
    {
        numerator = num.first;
    }
    else
    {
        wholeNumber = num.first;
        input = input.substr(1);
        
        //Get the numerator of fraction
        num = GetFirstPositiveIntegerFromString(input);
        //Return errors if they exist
        if(num.first < 0)return std::pair<Fraction, bool>(GetErrorFractionReturn(num.first), false);
        input = input.substr(num.second);
        numerator = num.first;
    }

    
    //Return error if the / sign isn't after the numerator
    if(input.size() == 0 || input[0] != '/')
        return std::pair<Fraction, bool>(Fraction(1, 1), false);
    input = input.substr(1);

    //Get the third number(denominator of fraction)
    num = GetFirstPositiveIntegerFromString(input);
    //Return errors if they exist
    if(num.first < 0)return std::pair<Fraction, bool>(GetErrorFractionReturn(num.first), false);
    
    //Make sure there aren't any characters after the end of the input
    if(num.second != input.size())
        return std::pair<Fraction, bool>(Fraction(1, 1), false);
    denominator = num.first;

    return std::pair<Fraction, bool>(Fraction((numerator + (wholeNumber * denominator)) * negativeBool, denominator), true);
}

//Boolean return value is true on success, false on failure
std::pair<Operator, bool> GetOperatorInput(std::string input)
{
    if(input.size() != 1)
        return std::pair<Operator, bool>(Operator::OPERATORCOUNT, false);

    switch(input[0])
    {
    case '-':
    {
        return std::pair<Operator, bool>(Operator::SUBTR, true);
    }
    case '+':
    {
        return std::pair<Operator, bool>(Operator::ADD, true);
    }
    case '*':
    {
        return std::pair<Operator, bool>(Operator::MULT, true);
    }
    case '/':
    {
        return std::pair<Operator, bool>(Operator::DIV, true);
    }
    }

    return std::pair<Operator, bool>(Operator::OPERATORCOUNT, false);
}

int main(int argc, char **argv)
{
    //Program requires 4 arguments - program name, first operand, operator, second operand
    if(argc != 4)
    {
        std::cout << "Invalid input!";
        return -1;
    }

    std::pair<Fraction, bool> operand1 = GetFractionInput(argv[1]);
    std::pair<Operator, bool> operator_ = GetOperatorInput(argv[2]);
    std::pair<Fraction, bool> operand2 = GetFractionInput(argv[3]);
    
    if(operand1.first.CheckDivByZero() || operand2.first.CheckDivByZero())
    {
        std::cout << "Division by zero!";
        return -1;
    }
    if(operand1.first.CheckOverflow() || operand2.first.CheckOverflow())
    {
        std::cout << "Overflow!";
        return -1;
    }
    if(operand1.second == false || operand2.second == false || operator_.second == false)
    {
        std::cout << "Invalid input!";
        return -1;
    }
    
    Fraction result = operand1.first;
    
    //Do the appropriate operation on the fraction
    switch(operator_.first)
    {
        case Operator::ADD:
        {
            result += operand2.first;
            break;
        }
        case Operator::SUBTR:
        {
            result -= operand2.first;
            break;
        }
        case Operator::MULT:
        {
            result *= operand2.first;
            break;
        }
        case Operator::DIV:
        {
            result /= operand2.first;
            break;
        }
        default:
        {
            std::cout << "GetOperatorInput() returns invalid value";
            return -2;
        }
    }

    if(result.CheckDivByZero())
    {
        std::cout << "Division by zero!";
        return -1;
    }
    if(result.CheckOverflow())
    {
        std::cout << "Overflow!";
        return -1;
    }

    std::cout << result.Print();

    return 0;
}
