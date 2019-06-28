#include "IconCache.h"

QIcon IconCache::null_icon;
std::unordered_map<std::string, QIcon> IconCache::icons;

const QIcon& IconCache::get(const std::string& name)
{
    // Check if we already have an icon object with that name in the cache. If so, just return that
    auto it = icons.find(name);
    if(it != icons.end())
        return it->second;

    // If now, try to load an icon with that name and insert it into the cache
    QIcon icon(":/icons/" + QString::fromStdString(name));
    auto ins = icons.insert({name, icon});

    // If the insertion was successful, return the inserted icon object. If it was not, return a null icon.
    if(ins.second)
        return ins.first->second;
    else
        return null_icon;
}
