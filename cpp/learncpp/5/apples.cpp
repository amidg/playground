#include <iostream>
#include <string>

/*
 * Write the function getQuantityPhrase() that does this:
 * < 0 = “negative”
 *  0 = “no”
 *  1 = “a single”
 *  2 = “a couple of”
 *  3 = “a few”
 *  > 3 = “many”
 */
const std::string getQuantityPhrase(const int& n) {
    std::string phrase = "";
    if (n < 0)
        phrase = "negative";
    else if (n == 0)
        phrase = "no";
    else if (n == 1)
        phrase = "a single";
    else if (n == 2)
        phrase = "a couple of";
    else if (n == 3)
        phrase = "a few";
    else if (n > 3)
        phrase = "many";
    return phrase;
}


/* 
 * Write the function getApplesPluralized() here that add "s" to the end
 */
std::string getApplesPluralized(int n) {
    std::string apples = "apple";
    switch (n) {
        case 1:
            break;
        default:
            apples = "apples";
            break;
    }
    return apples;
}


int main()
{
    constexpr int maryApples { 3 };
    std::cout << "Mary has " << getQuantityPhrase(maryApples) << ' ' << getApplesPluralized(maryApples) << ".\n";

    std::cout << "How many apples do you have? ";
    int numApples{};
    std::cin >> numApples;

    std::cout << "You have " << getQuantityPhrase(numApples) << ' ' << getApplesPluralized(numApples) << ".\n";

    return 0;
}
