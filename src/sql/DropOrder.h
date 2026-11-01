#ifndef DROPORDER_H
#define DROPORDER_H

#include "ObjectIdentifier.h"

#include <string>
#include <vector>

namespace sqlb {

// A database object which is going to be dropped. The type is one of "table", "view", "index" and "trigger".
struct DropItem
{
    std::string type;
    ObjectIdentifier object;

    bool operator==(const DropItem& rhs) const
    {
        return type == rhs.type && object == rhs.object;
    }
};

// Return the items in an order in which they can safely be dropped one after the other: triggers first, then indexes,
// then views and finally tables. This way no object is dropped before the objects which depend on it, and dropping a
// table never makes the later DROP of one of its own indexes or triggers fail. Duplicates and items of unknown type are
// removed. The relative order of items of the same type is preserved.
std::vector<DropItem> orderForDrop(const std::vector<DropItem>& items);

// Build the statement which drops a single item
std::string dropStatement(const DropItem& item);

}

#endif
