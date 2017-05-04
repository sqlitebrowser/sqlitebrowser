// This module implements part of the support for rectangular selections on
// OS/X.  It is a separate file to avoid clashes between OS/X and Scintilla
// data types.
//
// Copyright (c) 2017 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include <qglobal.h>

#if (QT_VERSION >= 0x040200 && QT_VERSION < 0x050000 && defined(Q_OS_MAC)) || (QT_VERSION >= 0x050200 && defined(Q_OS_OSX))

#include <QByteArray>
#include <QLatin1String>
#include <QList>
#include <QString>
#include <QStringList>
#include <QVariant>

#include <QMacPasteboardMime>


static const QLatin1String mimeRectangular("text/x-qscintilla-rectangular");
static const QLatin1String utiRectangularMac("com.scintilla.utf16-plain-text.rectangular");


class RectangularPasteboardMime : public QMacPasteboardMime
{
public:
    RectangularPasteboardMime() : QMacPasteboardMime(MIME_ALL)
    {
    }

    bool canConvert(const QString &mime, QString flav)
    {
        return mime == mimeRectangular && flav == utiRectangularMac;
    }

    QList<QByteArray> convertFromMime(const QString &, QVariant data, QString)
    {
        QList<QByteArray> converted;

        converted.append(data.toByteArray());

        return converted;
    }

    QVariant convertToMime(const QString &, QList<QByteArray> data, QString)
    {
        QByteArray converted;

        foreach (QByteArray i, data)
        {
            converted += i;
        }

        return QVariant(converted);
    }

    QString convertorName()
    {
        return QString("QScintillaRectangular");
    }

    QString flavorFor(const QString &mime)
    {
        if (mime == mimeRectangular)
            return QString(utiRectangularMac);

        return QString();
    }

    QString mimeFor(QString flav)
    {
        if (flav == utiRectangularMac)
            return QString(mimeRectangular);

        return QString();
    }
};


// Initialise the singleton instance.
void initialiseRectangularPasteboardMime()
{
    static RectangularPasteboardMime *instance = 0;

    if (!instance)
    {
        instance = new RectangularPasteboardMime();

        qRegisterDraggedTypes(QStringList(utiRectangularMac));
    }
}


#endif
