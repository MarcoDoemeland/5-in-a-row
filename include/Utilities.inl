#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

namespace FIAR{

// Getting an int code given from the user.
template<typename T>
T getInputFromUser(const std::string& msg){
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    T helper;
    std::cout << msg;
    std::cin >> helper;
    while(std::cin.fail()){
        std::cout << "Invalid input.\n" << msg;
        // Clearing the error
        std::cin.clear();
        std::cin >> helper;
    }
    return helper;
}

// Getting controlled input from the user.
template<typename T>
T getValidInputFromUser(const std::string& msg, const std::vector<T>& ref){
    // If the reference vector contains nothing, calling "getCodeFromUser()"
    if(ref.size() <= 0) return getInputFromUser<T>(msg);
    // Else processing normally:
    T helper;
    std::cout << msg;
    std::cin >> helper;
    while(std::cin.fail() || std::find(ref.begin(), ref.end(), helper) == ref.end()){
        std::cout << "Invalid input.\n" << msg;
        // Clearing the error
        std::cin.clear();
        std::cin >> helper;
    }
    return helper;
}

}// End namespace FIAR
