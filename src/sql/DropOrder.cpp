#include "DropOrder.h"

#include <algorithm>
#include <cctype>

namespace sqlb {

std::vector<DropItem> orderForDrop(const std::vector<DropItem>& items)
{
    // Objects which depend on others must be dropped first
    static const char* const type_order[] = {"trigger", "index", "view", "table"};

    std::vector<DropItem> result;
    for(const char* type : type_order)
    {
        for(const DropItem& item : items)
        {
            if(item.type == type && std::find(result.begin(), result.end(), item) == result.end())
                result.push_back(item);
        }
    }
    return result;
}

std::string dropStatement(const DropItem& item)
{
    std::string type = item.type;
    std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return "DROP " + type + " " + item.object.toString() + ";";
}

}
