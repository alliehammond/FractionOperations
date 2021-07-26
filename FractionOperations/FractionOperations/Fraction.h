#pragma once
#include <string>
#include <vector>

//Fraction class automatically keeps fractions reduced
//Error bits propogate through operations on other fractions
//If division by zero bit is set, operations will return initial lhs Fraction
class Fraction
{
public:
    Fraction(int numerator_, int denominator_);

    void SetFraction(int numerator_, int denominator_);
    void SetNumerator(int numerator_);
    void SetDenominator(int denominator_);
    int GetNumerator() const { return numerator; }
    int GetDenominator() const { return denominator; }

    //Returns true on overflow/divide by zero
    bool CheckOverflow() const { return bitFields[0]; }
    bool CheckDivByZero() const { return bitFields[1]; }

    void ResetOverflowBit() { bitFields[0] = false; }
    void ResetDivByZeroBit() { bitFields[1] = false; }
    void SetOverflowBit() { bitFields[0] = true; }
    void SetDivByZeroBit() { bitFields[1] = true; }

    Fraction& operator+=(const Fraction &rhs);
    Fraction &operator-=(const Fraction &rhs);
    Fraction &operator*=(const Fraction &rhs);
    Fraction &operator/=(const Fraction &rhs);

    Fraction operator+(const Fraction &rhs);
    Fraction operator-(const Fraction &rhs);
    Fraction operator*(const Fraction &rhs);
    Fraction operator/(const Fraction &rhs);

    //Format: 3_1/2 represents 3 and 1/2
    std::string const Print();

private:
    //Reduces fraction, removes negative symbols if there are two and sets numerator to negative instead of denominator if needed
    void Reduce();

    int numerator, denominator;
    //Bitfields storing errors, true if error occured
    //bitFields[0] - Overflow bit [1] - Divide by zero bit
    std::vector<bool> bitFields;
};
