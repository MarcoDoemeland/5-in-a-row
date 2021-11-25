// This should contains functions which can be used by any class, object or other function.

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

namespace FIAR{

// Getting input from the user.
// The input is checked using the "fail()" function of std::cin.
// As long as fail() is true, the user is asked to enter a value.
template<typename T>
T getInputFromUser(const std::string& msg);

// Getting controlled input from the user.
// The input is checked using the "fail()" function of std::cin.
// Then it will be compared to the list of values given as parameter.
// As long as fail() is true or the input could not be found in the list,
// the user is asked to enter a value.
template<typename T>
T getValidInputFromUser(const std::string& msg, const std::vector<T>& ref);

}// End namespace FIAR

#endif // UTILITIES_H
