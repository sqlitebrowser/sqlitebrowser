#include "MainWindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include <QDebug>

#if 0 and defined(Q_WS_MAC)
#include <Carbon/Carbon.h>
static OSErr checkAppleEventForMissingParams(const AppleEvent& theAppleEvent)
{
    DescType returnedType;
    Size actualSize;
    OSErr err;

    switch (err = AEGetAttributePtr(&theAppleEvent,
                                    keyMissedKeywordAttr, typeWildCard, &returnedType, nil, 0,
                                    &actualSize)) {
    case errAEDescNotFound:  // If we couldnt find the error attribute
        return noErr;          //    everything is ok, return noErr
    case noErr:                      // We found an error attribute, so
        return errAEEventNotHandled;   // tell the client we ignored the event
    default:
        return err;                   // Something else happened, return it
    }
}

static pascal OSErr odocHandler(const AppleEvent* inEvent, AppleEvent*
                                /*reply*/, SInt32 refCon)
{
    AEDescList documentList;
    OSErr err = AEGetParamDesc(inEvent, keyDirectObject, typeAEList,
                               &documentList);
    if (err == noErr) {
        err = checkAppleEventForMissingParams(*inEvent);

        if (err == noErr) {
            long documentCount;
            err = AECountItems(&documentList, &documentCount);

            for (long documentIndex = 1; err == noErr && documentIndex <=
                 documentCount; documentIndex++) {
                // What kind of document is it?
                DescType returnedType;
                Size actualSize;
                err = AESizeOfNthItem(&documentList, documentIndex, &returnedType,
                                      &actualSize);
                if (err == noErr) {

                    // It is just a normal document file
                    AEKeyword keyword;
                    FSRef ref;
                    err = AEGetNthPtr(&documentList, documentIndex, typeFSRef,
                                      &keyword, &returnedType, (Ptr)&ref, sizeof(FSRef), &actualSize);

                    if (err == noErr) {
                        char buf[1024];
                        err = FSRefMakePath(&ref, reinterpret_cast<UInt8*>(buf), 1024);
                        if ( err == noErr )
                            reinterpret_cast<MainWindow*>(refCon)->fileOpen(QString::fromUtf8(buf));
                    }
                }
            }
        }
        AEDisposeDesc(&documentList);
    }
    return err;
}

#endif // Q_WS_MAC



int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    a.setOrganizationName("sqlitebrowser");
    a.setApplicationName("SQLite Database Browser");

    // Set character encoding to UTF8
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    // Enable translation
    QTranslator translator;
    translator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&translator);
    QTranslator apptranslator;
    apptranslator.load("translations/tr_" + QLocale::system().name());
    a.installTranslator(&apptranslator);

    // Parse command line
    QString fileToOpen;
    QStringList sqlToExecute;
    bool quitApplication = false;
    for(int i=1;i<a.arguments().size();i++)
    {
        // Check next command line argument
        if(a.arguments().at(i) == "-h" || a.arguments().at(i) == "--help")
        {
            // Help
            qWarning() << qPrintable(QObject::tr("Usage: %1 [options] [db]\n").arg(argv[0]));
            qWarning() << qPrintable(QObject::tr("Possible command line arguments:"));
            qWarning() << qPrintable(QObject::tr("  -h, --help\t\tShow command line options"));
            qWarning() << qPrintable(QObject::tr("  -s, --sql  [file]\tExecute this SQL file after opening the DB"));
            qWarning() << qPrintable(QObject::tr("  -q, --quit\t\tExit application after running scripts"));
            qWarning() << qPrintable(QObject::tr("  [file]\t\tOpen this SQLite database"));
            return 0;
        } else if(a.arguments().at(i) == "-s" || a.arguments().at(i) == "--sql") {
            // Run SQL file: If file exists add it to list of scripts to execute
            if(++i >= a.arguments().size())
                qWarning() << qPrintable(QObject::tr("The -s/--sql option requires an argument"));
            else if(!QFile::exists(a.arguments().at(i)))
                qWarning() << qPrintable(QObject::tr("The file %1 does not exist").arg(a.arguments().at(i)));
            else
                sqlToExecute.append(a.arguments().at(i));
        } else if(a.arguments().at(i) == "-q" || a.arguments().at(i) == "--quit") {
            quitApplication = true;
        } else {
            // Other: Check if it's a valid file name
            if(QFile::exists(a.arguments().at(i)))
                fileToOpen = a.arguments().at(i);
            else
                qWarning() << qPrintable(QObject::tr("Invalid option/non-existant file: %1").arg(a.arguments().at(i)));
        }
    }

    MainWindow w;
#if defined(Q_WS_MAC)
//    AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments,
//                          NewAEEventHandlerUPP(odocHandler),reinterpret_cast<long>(&w),false);
#endif // Q_WS_MAC

    // Show main window
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));

    // Open database if one was specified
    if(fileToOpen.size())
    {
        if(w.fileOpen(fileToOpen))
        {
            // If database could be opened run the SQL scripts
            foreach(const QString& f, sqlToExecute)
            {
                QFile file(f);
                if(file.open(QIODevice::ReadOnly))
                {
                    w.getDb()->executeMultiSQL(file.readAll(), false, true);
                    file.close();
                }
            }
            if(!sqlToExecute.isEmpty())
                w.browseRefresh();
        }
    }

    // Quit application now if user doesn't want to see the UI
    if(quitApplication)
        return 0;

    // Run application
    return a.exec();
}
