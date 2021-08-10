#pragma once
#include "../pch.h"
#include "RLinq.h"
#include <vector>
#include <unordered_map>
template<class QueryFunction>
class Where: public RLinq
{
    QueryFunction* pQuery;
    QueryFunction GetQueryFunction() { return *pQuery; }
public:

    Where(QueryFunction lambda)
    {
        this->pQuery = &lambda;
    }
    Where& operator-()
    {
        return *this;
    }
    template <class TElement>
    friend const std::vector<const TElement*> operator<(const  std::vector<TElement>& obj, Where& linq)
    {
        auto query = linq.GetQueryFunction();
        std::vector<const TElement*> result;
        for (auto&& value : obj)
        {
            if (query(value))
            {
                result.emplace_back(&value);
            }
        }
        return result;
    }

    template <class TKey, class TValue>
    friend const std::vector<std::pair<const TKey*, const TValue*>> operator<(const std::unordered_map<TKey, TValue>& obj, Where& linq)
    {
        auto query = linq.GetQueryFunction();
        std::vector<std::pair<const TKey*, const TValue*>> result;

        for (auto&& keyValuePair : obj)
        {
            if (query(keyValuePair))
            {
                auto res = std::pair(&keyValuePair.first, &keyValuePair.second);
                result.emplace_back(res);
            }
        }

        return result;
    }

};
