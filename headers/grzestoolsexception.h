#ifndef GRZESTOOLSEXCEPTION_H
#define GRZESTOOLSEXCEPTION_H

#include <string>

namespace grzes
{
class GrzesToolsException
{
public:
    GrzesToolsException() {}
    GrzesToolsException(std::string mssg_) : mssg(mssg_) {}
    GrzesToolsException(const char * mssg_) : mssg(mssg_) {}
    virtual ~GrzesToolsException() {}
    virtual std::string getMssg() const { return mssg; }

private:
    std::string mssg;
};

} //namespace grzes
#endif // GRZESTOOLSEXCEPTION_H
