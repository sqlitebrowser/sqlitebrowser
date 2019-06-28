#ifndef ICONCACHE_H
#define ICONCACHE_H

#include <QIcon>

#include <string>
#include <unordered_map>

class IconCache
{
public:
    IconCache() = delete;

    static const QIcon& get(const std::string& name);

private:
    static QIcon null_icon;
    static std::unordered_map<std::string, QIcon> icons;
};

#endif
