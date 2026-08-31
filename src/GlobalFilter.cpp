#include "GlobalFilter.h"

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

namespace GlobalFilter
{

static std::vector<QString> legacyTokenize(const QString& value)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    const QStringList values = value.trimmed().split(" ", QString::SkipEmptyParts);
#else
    const QStringList values = value.trimmed().split(" ", Qt::SkipEmptyParts);
#endif
    return {values.cbegin(), values.cend()};
}

std::vector<QString> tokenize(const QString& value)
{
    std::vector<QString> filters;
    int position = 0;

    while(position < value.size())
    {
        while(position < value.size() && value.at(position) == ' ')
            ++position;
        if(position == value.size())
            break;

        const int termStart = position;
        if(value.at(termStart) != '/')
        {
            while(position < value.size() && value.at(position) != ' ')
                ++position;
            filters.push_back(value.mid(termStart, position - termStart));
            continue;
        }

        bool foundClosingSlash = false;
        for(position = termStart + 1; position < value.size(); ++position)
        {
            if(value.at(position) != '/')
                continue;

            int backslashes = 0;
            for(int previous = position - 1; previous >= termStart && value.at(previous) == '\\'; --previous)
                ++backslashes;
            const bool unescaped = backslashes % 2 == 0;
            const bool atTermEnd = position + 1 == value.size() || value.at(position + 1) == ' ';
            if(unescaped && !atTermEnd)
                return legacyTokenize(value);
            if(unescaped)
            {
                filters.push_back(value.mid(termStart, position - termStart + 1));
                ++position;
                foundClosingSlash = true;
                break;
            }
        }

        if(!foundClosingSlash)
            return legacyTokenize(value);
    }

    return filters;
}

void load(std::vector<QString>& filters, QXmlStreamReader& xml)
{
    while(xml.readNext() != QXmlStreamReader::EndElement && xml.name() != QT_UNICODE_LITERAL("global_filter"))
    {
        if(xml.name() == QT_UNICODE_LITERAL("filter"))
        {
            filters.push_back(xml.attributes().value("value").toString());
            xml.skipCurrentElement();
        }
    }
}

void save(const std::vector<QString>& filters, QXmlStreamWriter& xml)
{
    xml.writeStartElement("global_filter");
    for(const auto& value : filters)
    {
        xml.writeStartElement("filter");
        xml.writeAttribute("value", value);
        xml.writeEndElement();
    }
    xml.writeEndElement();
}

}
