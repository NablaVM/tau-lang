#ifndef APP_TEST_HELPERS
#define APP_TEST_HELPERS

#include <random>

namespace TEST
{
    template<class T>
    static int64_t getRandom(T low, T high)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(low, high);
        return dis(gen);
    }
}

#endif 