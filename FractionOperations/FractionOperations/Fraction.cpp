#include "Fraction.h"
#include <cstdlib>
#include <numeric>

Fraction::Fraction(int numerator_, int denominator_) : numerator(numerator_), denominator(denominator_), bitFields(2)
{
    //Initialize error bitfields
    bitFields[0] = false;
    bitFields[1] = false;

    if(denominator == 0)bitFields[1] = true;

    Reduce();
}

void Fraction::SetFraction(int numerator_, int denominator_)
{
    numerator = numerator_;
    denominator = denominator_;
    if(denominator == 0)bitFields[1] = true;
    Reduce();
}

void Fraction::SetNumerator(int numerator_)
{
    numerator = numerator_;
    Reduce();
}

void Fraction::SetDenominator(int denominator_)
{
    denominator = denominator_;
    if(denominator == 0)bitFields[1] = true;
    Reduce();
}

//Format: 3_1/2 represents 3 and 1/2
std::string const Fraction::Print()
{
    int absNumerator = std::abs(numerator);
    if(absNumerator == 0)return "0";

    std::string ret = "";
    if(numerator < 0)ret += "-";

    int wholeNumber = 0;
    if(denominator != 0)wholeNumber = absNumerator / denominator;
    if(wholeNumber > 0)ret += std::to_string(wholeNumber);
    
    //Don't print fractional part if there is no leftover fraction
    if(absNumerator % denominator != 0)
    {
        if(wholeNumber > 0)ret += "_";
        ret += std::to_string(absNumerator - wholeNumber * denominator);
        ret += "/";
        ret += std::to_string(denominator);
    }

    return ret;
}

void Fraction::Reduce()
{
    //First ensure there is only a maximum of one negative sign and it is on numerator (if it exists)
    if(numerator < 0 && denominator < 0)
    {
        numerator *= -1;
        denominator *= -1;
    }
    else
    if(denominator < 0)
    {
        numerator *= -1;
        denominator *= -1;
    }
    //If numerator is 0, set denominator to 2 to keep fractions small
    if(numerator == 0)denominator = 2;

    //Reduce fraction to simplest terms
    int absNumerator = std::abs(numerator);
    int gcd = std::gcd(absNumerator, denominator);
    
    //Avoid potential divide by zero (as well as skipping unneeded divisions)
    if(gcd > 1)
    {
        numerator /= gcd;
        denominator /= gcd;
    }
}

//Returns true if the two numbers would overflow if added
bool IsAdditionOverflow(int a, int b)
{
    int result = a + b;
    if(a > 0 && b > 0 && result <= 0)return true;
    if(a < 0 && b < 0 && result >= 0)return true;
    return false;
}

//Returns true if the two numbers would overflow if multiplied
bool IsMultiplyOverflow(int a, int b)
{
    if(a == 0 || b == 0)return false;

    int result = a * b;
    if(a == result / b)return false;

    return true;
}

Fraction &Fraction::operator+=(const Fraction &rhs)
{
    //Propogate bitfields errors if needed
    if(rhs.CheckOverflow())bitFields[0] = true;
    if(rhs.CheckDivByZero() || CheckDivByZero())
    {
        bitFields[1] = true;
        return *this;
    }

    //Find lowest common multiple (resulting denominator)
    int LCM = std::gcd(denominator, rhs.denominator);
    LCM = (denominator * rhs.denominator) / LCM;
    //std::gcd has undefined behavior (from what I can tell) if the resulting integer would overflow, so assume anything under/equal to 0 is overflow
    if(LCM <= 0)bitFields[0] = true;

    //Resulting numerators after setting demoninators to be the same
    int firstResNumerator = numerator * (LCM / denominator);
    int secondResNumerator = rhs.numerator * (LCM / rhs.denominator);

    //Check for overflow
    if(IsAdditionOverflow(firstResNumerator, secondResNumerator))bitFields[0] = true;

    numerator = firstResNumerator + secondResNumerator;
    denominator = LCM;

    Reduce();
    return *this;
}

Fraction &Fraction::operator-=(const Fraction &rhs)
{
    Fraction negatedRhs(-1 * rhs.numerator, rhs.denominator);
    (*this) += negatedRhs;
    return *this;
}

Fraction &Fraction::operator*=(const Fraction &rhs)
{
    //Propogate bitfields errors if needed
    if(rhs.CheckOverflow())bitFields[0] = true;
    if(rhs.CheckDivByZero() || CheckDivByZero() || rhs.denominator == 0)
    {
        bitFields[1] = true;
        return *this;
    }

    //Check for overflow
    if(IsMultiplyOverflow(numerator, rhs.numerator))bitFields[0] = true;
    if(IsMultiplyOverflow(denominator, rhs.denominator))bitFields[0] = true;

    numerator *= rhs.numerator;
    denominator *= rhs.denominator;

    Reduce();
    return *this;
}

Fraction &Fraction::operator/=(const Fraction &rhs)
{
    Fraction invertedRhs(rhs.denominator, rhs.numerator);
    (*this) *= invertedRhs;
    return *this;
}

Fraction Fraction::operator+(const Fraction &rhs)
{
    Fraction ret = *this;
    ret += rhs;
    return ret;
}

Fraction Fraction::operator-(const Fraction &rhs)
{
    Fraction ret = *this;
    ret -= rhs;
    return ret;
}

Fraction Fraction::operator*(const Fraction &rhs)
{
    Fraction ret = *this;
    ret *= rhs;
    return ret;
}

Fraction Fraction::operator/(const Fraction &rhs)
{
    Fraction ret = *this;
    ret /= rhs;
    return ret;
}
