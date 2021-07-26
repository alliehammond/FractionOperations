//Note the tester works on windows only and doesn't do error checking (executable doesn't exist etc) since it's a tester program
#include <iostream>
#include <string>
#include <fstream>

void Test(std::string input, std::string expectedOutput)
{
    std::string command = "FractionOperations.exe " + input + " > output.txt";
    system(command.c_str());
    std::ifstream outFile("output.txt");
    std::string line, output;
    while(std::getline(outFile, line))
        output.append(line);

    if(output == expectedOutput)
    {
        std::cout << "Success: " + input + " -> " + expectedOutput;
    }
    else
    {
        std::cout << "Failed!: " + input + " -> " + output + " | Expected: " + expectedOutput;
    }
    std::cout << std::endl;
}

int main()
{
    //Regular tests
    Test("1/2 * 3_3/4", "1_7/8");
    Test("2_3/8 + 9/8", "3_1/2");
    Test("1_11/8 + 9/8", "3_1/2");
    Test("2 * 3/4", "1_1/2");
    Test("1/2 * 0/4", "0");
    Test("2_3/8 + 0/8", "2_3/8");
    Test("2_3/8 + 0_1/8", "2_1/2");
    Test("2_3/8 - 9/24", "2");
    Test("2_3/8 - 9/25", "2_3/200");
    Test("2_3/8 + 9/25", "2_147/200");
    Test("1/2 / 3_3/4", "2/15");
    Test("3_3/4 / 1/2", "7_1/2");
    //With negative numbers (note negative symbol can only be in front of each operand)
    Test("-1/2 * 3_3/4", "-1_7/8");
    Test("2_3/8 + -9/25", "2_3/200");
    Test("2_3/8 - -9/25", "2_147/200");
    Test("-1/2 / 3_3/4", "-2/15");
    Test("-3_3/4 / -1/2", "7_1/2");

    //Different amounts/placements of spaces
    Test("2_3/8 +   9/8", "3_1/2");
    Test("2_3/8   + 9/8", "3_1/2");
    Test("2_3/8   +   9/8", "3_1/2");
    Test("2_3 /8 + 9/8", "Invalid input!");
    Test("2 _3/8 * 9/8", "Invalid input!");

    //Invalid syntax
    Test("1/-2 * 3_3/4", "Invalid input!");
    Test("1/2 * 3_-3/4", "Invalid input!");
    Test("2.1_1/2 * 3/4", "Invalid input!");
    Test("2_ * 9/8", "Invalid input!");
    Test("2_3/8", "Invalid input!");
    Test("aa2_3/8 + 9/8", "Invalid input!");
    Test("2_3/8 aa - 9/8", "Invalid input!");
    Test("2_3/8 / 9/8aa", "Invalid input!");
    Test("2_3/8 +", "Invalid input!");
    Test("", "Invalid input!");

    //Overflows
    Test("999999999999999999999999999999999999999/2 * 3_3/4", "Overflow!");
    Test("-1/50000 * 1/50000", "Overflow!");
    Test("50000 * 50000/1", "Overflow!");
    Test("1/50000 / 50000_3/4", "Overflow!");
    Test("1073741900_3/1 + 1073741900_39/1", "Overflow!");
    Test("-10737419003_3/1 - 1073741900_3/1", "Overflow!");

    //Division by zero
    Test("1/0 * 3_3/4", "Division by zero!");
    Test("0/0 * 3_3/4", "Division by zero!");
    Test("1/2 * 3_3/0", "Division by zero!");
    Test("1/2 / 0/2", "Division by zero!");

    return 0;
}
