#ifndef BIGNUMPARSEEXCEPTION_H
#define BIGNUMPARSEEXCEPTION_H

#include "headers/bignumexception.h"

namespace grzes
{
class BigNumParseException : public BigNumException
{
public:
    BigNumParseException() {}
    BigNumParseException(std::string mssg_) : BigNumException(mssg_) {}
    BigNumParseException(const char * mssg_) : BigNumException(mssg_) {}
};

} //namespace grzes

#endif // BIGNUMPARSEEXCEPTION_H
