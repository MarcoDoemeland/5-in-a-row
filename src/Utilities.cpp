#include "Utilities.h"

namespace FIAR{

// Getting a random value inside of the given limits
int getRandomInt(int min, int max){
    static double s_frac{ 1. / (RAND_MAX + 1.0) };
    return min + static_cast<int>((max - min + 1) * (s_frac * rand()));
}

}// End namespace FIAR
