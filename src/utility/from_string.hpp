#include <sstream>
#include <string>

namespace arrow {
    template <typename T>
    T from_string(std::string const & str)
    {
        T result;
        std::istringstream(str) >> result;
        return result;
    }
}