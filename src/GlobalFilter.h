#ifndef GLOBALFILTER_H
#define GLOBALFILTER_H

#include <QString>
#include <vector>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace GlobalFilter
{

std::vector<QString> tokenize(const QString& value);
void load(std::vector<QString>& filters, QXmlStreamReader& xml);
void save(const std::vector<QString>& filters, QXmlStreamWriter& xml);

}

#endif // GLOBALFILTER_H
