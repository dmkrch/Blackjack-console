#pragma once

#include <string>
#include <exception>

class Exception : public std::exception
{
public:
    Exception(const std::string);

    ~Exception() throw();

    virtual const char* what() const throw();

private:
    std::string msg_;
};