#include "pch.h"
#include "RFrameworkException.h"

RFrameworkException::RFrameworkException(std::string errorMsg)
{
	this->errMessage += errorMsg;
}
