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
    std::string getMssg() const { return mssg; }

private:
    std::string mssg;
};

} //namespace grzes
#endif // BIGNUMEXCEPTION_H
