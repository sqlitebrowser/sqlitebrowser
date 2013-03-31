#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>

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

    MainWindow w;
#if defined(Q_WS_MAC)
//    AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments,
//                          NewAEEventHandlerUPP(odocHandler),reinterpret_cast<long>(&w),false);
#endif // Q_WS_MAC
    w.show();
    if (argc>1) {
        //first and only argument we accept is the name of the database to open
        w.fileOpen(QString(argv[1]));
    }
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
