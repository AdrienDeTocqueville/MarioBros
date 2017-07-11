#include "constantes.h"
#include <sstream>

std::string toString(int _number)
{
    std::stringstream os;
    os << _number;

    return os.str();
}

std::string toStringF(float _number)
{
    std::stringstream os;
    os << _number;

    return os.str();
}

BLOC getBLOC(char _type)
{
    return static_cast<BLOC>(_type);
}

char getChar(BLOC _type)
{
    return static_cast<char>(_type);
}
