#pragma once
#include "../pch.h"
#include "RLinq.h"
#include <vector>

template<class TQuery>
class Remove : public RLinq
{
    TQuery* pQuery;
    TQuery GetQueryFunction() { return *pQuery; }
public:

    Remove(TQuery lambda)
    {
        this->pQuery = &lambda;
    }
    Remove& operator-()
    {
        return *this;
    }
    template <class TElement>
    friend const bool operator<(std::vector<TElement>& obj, Remove& linq)
    {
        bool isAnythingRemoved = false;
        auto query = linq.GetQueryFunction();
        int i = 0;
        for (auto&& value : obj)
        {
            if (query(value))
            {
                obj.erase(obj.begin() + i);
                isAnythingRemoved = true;
            }
            i++;
        }
        return isAnythingRemoved;
    }
};
