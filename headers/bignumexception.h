#ifndef BIGNUMEXCEPTION_H
#define BIGNUMEXCEPTION_H

#include "headers/grzestoolsexception.h"

namespace grzes
{
class BigNumException : public GrzesToolsException
{
public:
    BigNumException() {}
    BigNumException(std::string mssg_) : GrzesToolsException(mssg_) {}
    BigNumException(const char * mssg_) : GrzesToolsException(mssg_) {}
};

} //namespace grzes
#endif // BIGNUMEXCEPTION_H
