#pragma once
#include <map>
#include <vector>

template <typename T, typename T2>
void insertToMapVector(std::map<T, std::vector<T2>>& map, const T& key, const T2& value)
{
    if (map.count(key))
    {
        map[key].push_back(value);
    }
    else
    {
        map[key] = vector<T2>(0);
        map[key].push_back(value);
    }
}