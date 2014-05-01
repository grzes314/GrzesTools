#ifndef BIGNUMEXCEPTION_H
#define BIGNUMEXCEPTION_H

#include <string>

namespace grzes
{
class BigNumException
{
public:
    BigNumException() {}
    BigNumException(std::string mssg_) : mssg(mssg_) {}
    BigNumException(const char * mssg_) : mssg(mssg_) {}
    std::string getMssg() const;

private:
    std::string mssg;
};

std::string BigNumException::getMssg() const
{
    return mssg;
}

} //namespace grzes
#endif // BIGNUMEXCEPTION_H
