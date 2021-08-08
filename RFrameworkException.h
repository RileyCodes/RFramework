#pragma once
#include <exception>
#include <string>


class RFrameworkException : public std::exception
{
    std::string errMessage = "An RFrameworkException occurred: \n";
public:
	
    RFrameworkException(std::string errorMsg);
	
    const char* what() const throw ()
    {
        return errMessage.c_str();
    }
};
