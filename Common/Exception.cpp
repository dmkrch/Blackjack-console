#include "Exception.hpp"

Exception::Exception(const std::string m) : msg_(m)
{}

Exception::~Exception() throw()
{}

const char* Exception::what() const throw()
{
    return msg_.c_str();
}