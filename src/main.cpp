
#include "Application.h"
#include "sqlite.h"
#include <QMessageBox>

static QString message = QString();

void db4sMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    // Emulate the default Qt treatment. This might not work in some OS, like Windows.
    fprintf(stderr, "%s\n", localMsg.constData());

    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    localMsg = QString("%1 (%2, %3:%4)\n").arg(msg, function, file).arg(context.line).toLocal8Bit();

    // Log using the SQLite log mechanism, so it gets the same treatment than messages
    // reported by SQLite itself. This will allow these messages to be seen in our Log window.
    // Error code will be the type
    sqlite3_log(static_cast<int>(type), localMsg.constData());
}

void boxMessageOutput(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    message += msg + "\n";
}

int main( int argc, char ** argv )
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#endif
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#ifdef Q_OS_WIN
    // In Windows, there is no output to terminal for a graphical application, so we install
    // the output to message box, until the main window is shown or the application exits.
    qInstallMessageHandler(boxMessageOutput);
#endif

    // Create application object. All the initialisation stuff happens in there
    Application a(argc, argv);

    // If there has been invocations to the message handler, show it to user in a message box.
    if(!message.isEmpty()) {
        QMessageBox messageBox;
        messageBox.setTextFormat(Qt::RichText);
        messageBox.setText("<pre>" + message.toHtmlEscaped() + "</pre>");
        messageBox.exec();
    }

    // Quit application now if user doesn't want to see the UI
    if(!a.showMainWindow())
        return 0;

    qInstallMessageHandler(db4sMessageOutput);

    // Run application
    return a.exec();
}
