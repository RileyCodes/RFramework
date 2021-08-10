#pragma once
#include "../pch.h"
#include <algorithm>

#include "RLinq.h"
#include <vector>

template<class TQuery>
class OrderBy : public RLinq
{
    TQuery* pQuery;
    TQuery GetQueryFunction() { return *pQuery; }
public:
    OrderBy(TQuery queryFunc){ this->pQuery = &queryFunc; }
    OrderBy& operator-(){ return *this; }

    template <class TElement>
    friend const std::vector<const TElement*> operator<(const  std::vector<TElement>& obj, OrderBy& linq)
    {
        std::vector<const TElement*> result;
        for (auto&& value : obj)
        {
            result.emplace_back(&value);
        }

        std::sort(result.begin(), result.end(), [&linq](const TElement* item1, const TElement* item2)
        {
            auto query = linq.GetQueryFunction();
            auto item1ValueToCompare = query(item1);
            auto item2ValueToCompare = query(item2);
            return item1ValueToCompare  < item2ValueToCompare;
        });
        return result;
    }
};
