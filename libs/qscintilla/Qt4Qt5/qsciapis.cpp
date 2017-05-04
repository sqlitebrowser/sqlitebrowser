// This module implements the QsciAPIs class.
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


#include <stdlib.h>

#include "Qsci/qsciapis.h"

#include <QApplication>
#include <QDataStream>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QLibraryInfo>
#include <QMap>
#include <QTextStream>
#include <QThread>

#include "Qsci/qscilexer.h"



// The version number of the prepared API information format.
const unsigned char PreparedDataFormatVersion = 0;


// This class contains prepared API information.
struct QsciAPIsPrepared
{
    // The word dictionary is a map of individual words and a list of positions
    // each occurs in the sorted list of APIs.  A position is a tuple of the
    // index into the list of APIs and the index into the particular API.
    QMap<QString, QsciAPIs::WordIndexList> wdict;

    // The case dictionary maps the case insensitive words to the form in which
    // they are to be used.  It is only used if the language is case
    // insensitive.
    QMap<QString, QString> cdict;

    // The raw API information.
    QStringList raw_apis;

    QStringList apiWords(int api_idx, const QStringList &wseps,
            bool strip_image) const;
    static QString apiBaseName(const QString &api);
};


// Return a particular API entry as a list of words.
QStringList QsciAPIsPrepared::apiWords(int api_idx, const QStringList &wseps,
        bool strip_image) const
{
    QString base = apiBaseName(raw_apis[api_idx]);

    // Remove any embedded image reference if necessary.
    if (strip_image)
    {
        int tail = base.indexOf('?');

        if (tail >= 0)
            base.truncate(tail);
    }

    if (wseps.isEmpty())
        return QStringList(base);

    return base.split(wseps.first());
}


// Return the name of an API function, ie. without the arguments.
QString QsciAPIsPrepared::apiBaseName(const QString &api)
{
    QString base = api;
    int tail = base.indexOf('(');

    if (tail >= 0)
        base.truncate(tail);

    return base.simplified();
}


// The user event type that signals that the worker thread has started.
const QEvent::Type WorkerStarted = static_cast<QEvent::Type>(QEvent::User + 1012);


// The user event type that signals that the worker thread has finished.
const QEvent::Type WorkerFinished = static_cast<QEvent::Type>(QEvent::User + 1013);


// The user event type that signals that the worker thread has aborted.
const QEvent::Type WorkerAborted = static_cast<QEvent::Type>(QEvent::User + 1014);


// This class is the worker thread that post-processes the API set.
class QsciAPIsWorker : public QThread
{
public:
    QsciAPIsWorker(QsciAPIs *apis);
    virtual ~QsciAPIsWorker();

    virtual void run();

    QsciAPIsPrepared *prepared;

private:
    QsciAPIs *proxy;
    bool abort;
};


// The worker thread ctor.
QsciAPIsWorker::QsciAPIsWorker(QsciAPIs *apis)
    : proxy(apis), prepared(0), abort(false)
{
}


// The worker thread dtor.
QsciAPIsWorker::~QsciAPIsWorker()
{
    // Tell the thread to stop.  There is no need to bother with a mutex.
    abort = true;

    // Wait for it to do so and hit it if it doesn't.
    if (!wait(500))
        terminate();

    if (prepared)
        delete prepared;
}


// The worker thread entry point.
void QsciAPIsWorker::run()
{
    // Sanity check.
    if (!prepared)
        return;

    // Tell the main thread we have started.
    QApplication::postEvent(proxy, new QEvent(WorkerStarted));

    // Sort the full list.
    prepared->raw_apis.sort();

    QStringList wseps = proxy->lexer()->autoCompletionWordSeparators();
    bool cs = proxy->lexer()->caseSensitive();

    // Split each entry into separate words but ignoring any arguments.
    for (int a = 0; a < prepared->raw_apis.count(); ++a)
    {
        // Check to see if we should stop.
        if (abort)
            break;

        QStringList words = prepared->apiWords(a, wseps, true);

        for (int w = 0; w < words.count(); ++w)
        {
            const QString &word = words[w];

            // Add the word's position to any existing list for this word.
            QsciAPIs::WordIndexList wil = prepared->wdict[word];

            // If the language is case insensitive and we haven't seen this
            // word before then save it in the case dictionary.
            if (!cs && wil.count() == 0)
                prepared->cdict[word.toUpper()] = word;

            wil.append(QsciAPIs::WordIndex(a, w));
            prepared->wdict[word] = wil;
        }
    }

    // Tell the main thread we have finished.
    QApplication::postEvent(proxy, new QEvent(abort ? WorkerAborted : WorkerFinished));
}


// The ctor.
QsciAPIs::QsciAPIs(QsciLexer *lexer)
    : QsciAbstractAPIs(lexer), worker(0), origin_len(0)
{
    prep = new QsciAPIsPrepared;
}


// The dtor.
QsciAPIs::~QsciAPIs()
{
    deleteWorker();
    delete prep;
}


// Delete the worker thread if there is one.
void QsciAPIs::deleteWorker()
{
    if (worker)
    {
        delete worker;
        worker = 0;
    }
}


//! Handle termination events from the worker thread.
bool QsciAPIs::event(QEvent *e)
{
    switch (e->type())
    {
    case WorkerStarted:
        emit apiPreparationStarted();
        return true;

    case WorkerAborted:
        deleteWorker();
        emit apiPreparationCancelled();
        return true;

    case WorkerFinished:
        delete prep;
        old_context.clear();

        prep = worker->prepared;
        worker->prepared = 0;
        deleteWorker();

        // Allow the raw API information to be modified.
        apis = prep->raw_apis;

        emit apiPreparationFinished();

        return true;
    }

    return QObject::event(e);
}


// Clear the current raw API entries.
void QsciAPIs::clear()
{
    apis.clear();
}


// Clear out all API information.
bool QsciAPIs::load(const QString &filename)
{
    QFile f(filename);

    if (!f.open(QIODevice::ReadOnly))
        return false;

    QTextStream ts(&f);

    for (;;)
    {
        QString line = ts.readLine();

        if (line.isEmpty())
            break;

        apis.append(line);
    }

    return true;
}


// Add a single API entry.
void QsciAPIs::add(const QString &entry)
{
    apis.append(entry);
}


// Remove a single API entry.
void QsciAPIs::remove(const QString &entry)
{
    int idx = apis.indexOf(entry);

    if (idx >= 0)
        apis.removeAt(idx);
}


// Position the "origin" cursor into the API entries according to the user
// supplied context.
QStringList QsciAPIs::positionOrigin(const QStringList &context, QString &path)
{
    // Get the list of words and see if the context is the same as last time we
    // were called.
    QStringList new_context;
    bool same_context = (old_context.count() > 0 && old_context.count() < context.count());

    for (int i = 0; i < context.count(); ++i)
    {
        QString word = context[i];

        if (!lexer()->caseSensitive())
            word = word.toUpper();

        if (i < old_context.count() && old_context[i] != word)
            same_context = false;

        new_context << word;
    }

    // If the context has changed then reset the origin.
    if (!same_context)
        origin_len = 0;

    // If we have a current origin (ie. the user made a specific selection in
    // the current context) then adjust the origin to include the last complete
    // word as the user may have entered more parts of the name without using
    // auto-completion.
    if (origin_len > 0)
    {
        const QString wsep = lexer()->autoCompletionWordSeparators().first();

        int start_new = old_context.count();
        int end_new = new_context.count() - 1;

        if (start_new == end_new)
        {
            path = old_context.join(wsep);
            origin_len = path.length();
        }
        else
        {
            QString fixed = *origin;
            fixed.truncate(origin_len);

            path = fixed;

            while (start_new < end_new)
            {
                // Add this word to the current path.
                path.append(wsep);
                path.append(new_context[start_new]);
                origin_len = path.length();

                // Skip entries in the current origin that don't match the
                // path.
                while (origin != prep->raw_apis.end())
                {
                    // See if the current origin has come to an end.
                    if (!originStartsWith(fixed, wsep))
                        origin = prep->raw_apis.end();
                    else if (originStartsWith(path, wsep))
                        break;
                    else
                        ++origin;
                }

                if (origin == prep->raw_apis.end())
                    break;

                ++start_new;
            }
        }

        // Terminate the path.
        path.append(wsep);

        // If the new text wasn't recognised then reset the origin.
        if (origin == prep->raw_apis.end())
            origin_len = 0;
    }

    if (origin_len == 0)
        path.truncate(0);

    // Save the "committed" context for next time.
    old_context = new_context;
    old_context.removeLast();

    return new_context;
}


// Return true if the origin starts with the given path.
bool QsciAPIs::originStartsWith(const QString &path, const QString &wsep)
{
    const QString &orig = *origin;

    if (!orig.startsWith(path))
        return false;

    // Check that the path corresponds to the end of a word, ie. that what
    // follows in the origin is either a word separator or a (.
    QString tail = orig.mid(path.length());

    return (!tail.isEmpty() && (tail.startsWith(wsep) || tail.at(0) == '('));
}


// Add auto-completion words to an existing list.
void QsciAPIs::updateAutoCompletionList(const QStringList &context,
        QStringList &list)
{
    QString path;
    QStringList new_context = positionOrigin(context, path);

    if (origin_len > 0)
    {
        const QString wsep = lexer()->autoCompletionWordSeparators().first();
        QStringList::const_iterator it = origin;

        unambiguous_context = path;

        while (it != prep->raw_apis.end())
        {
            QString base = QsciAPIsPrepared::apiBaseName(*it);

            if (!base.startsWith(path))
                break;

            // Make sure we have something after the path.
            if (base != path)
            {
                // Get the word we are interested in (ie. the one after the
                // current origin in path).
                QString w = base.mid(origin_len + wsep.length()).split(wsep).first();

                // Append the space, we know the origin is unambiguous.
                w.append(' ');

                if (!list.contains(w))
                    list << w;
            }

            ++it;
        }
    }
    else
    {
        // At the moment we assume we will add words from multiple contexts so
        // mark the unambiguous context as unknown.
        unambiguous_context = QString();

        bool unambig = true;
        QStringList with_context;

        if (new_context.last().isEmpty())
            lastCompleteWord(new_context[new_context.count() - 2], with_context, unambig);
        else
            lastPartialWord(new_context.last(), with_context, unambig);

        for (int i = 0; i < with_context.count(); ++i)
        {
            // Remove any unambigious context (allowing for a possible image
            // identifier).
            QString noc = with_context[i];

            if (unambig)
            {
                int op = noc.indexOf(QLatin1String(" ("));

                if (op >= 0)
                {
                    int cl = noc.indexOf(QLatin1String(")"));

                    if (cl > op)
                        noc.remove(op, cl - op + 1);
                    else
                        noc.truncate(op);
                }
            }

            list << noc;
        }
    }
}


// Get the index list for a particular word if there is one.
const QsciAPIs::WordIndexList *QsciAPIs::wordIndexOf(const QString &word) const
{
    QString csword;

    // Indirect through the case dictionary if the language isn't case
    // sensitive.
    if (lexer()->caseSensitive())
        csword = word;
    else
    {
        csword = prep->cdict[word];

        if (csword.isEmpty())
            return 0;
    }

    // Get the possible API entries if any.
    const WordIndexList *wl = &prep->wdict[csword];

    if (wl->isEmpty())
        return 0;

    return wl;
}


// Add auto-completion words based on the last complete word entered.
void QsciAPIs::lastCompleteWord(const QString &word, QStringList &with_context, bool &unambig)
{
    // Get the possible API entries if any.
    const WordIndexList *wl = wordIndexOf(word);

    if (wl)
        addAPIEntries(*wl, true, with_context, unambig);
}


// Add auto-completion words based on the last partial word entered.
void QsciAPIs::lastPartialWord(const QString &word, QStringList &with_context, bool &unambig)
{
    if (lexer()->caseSensitive())
    {
        QMap<QString, WordIndexList>::const_iterator it = prep->wdict.lowerBound(word);

        while (it != prep->wdict.end())
        {
            if (!it.key().startsWith(word))
                break;

            addAPIEntries(it.value(), false, with_context, unambig);

            ++it;
        }
    }
    else
    {
        QMap<QString, QString>::const_iterator it = prep->cdict.lowerBound(word);

        while (it != prep->cdict.end())
        {
            if (!it.key().startsWith(word))
                break;

            addAPIEntries(prep->wdict[it.value()], false, with_context, unambig);

            ++it;
        }
    }
}


// Handle the selection of an entry in the auto-completion list.
void QsciAPIs::autoCompletionSelected(const QString &selection)
{
    // If the selection is an API (ie. it has a space separating the selected
    // word and the optional origin) then remember the origin.
    QStringList lst = selection.split(' ');

    if (lst.count() != 2)
    {
        origin_len = 0;
        return;
    }

    const QString &path = lst[1];
    QString owords;

    if (path.isEmpty())
        owords = unambiguous_context;
    else
    {
        // Check the parenthesis.
        if (!path.startsWith("(") || !path.endsWith(")"))
        {
            origin_len = 0;
            return;
        }

        // Remove the parenthesis.
        owords = path.mid(1, path.length() - 2);
    }

    origin = qLowerBound(prep->raw_apis, owords);
    /*
     * There is a bug somewhere, either in qLowerBound() or QList (or in GCC as
     * it seems to be Linux specific and the Qt code is the same on all
     * platforms) that the following line seems to fix.  Note that it is
     * actually the call to detach() within begin() that is the important bit.
     */
    prep->raw_apis.begin();
    origin_len = owords.length();
}


// Add auto-completion words for a particular word (defined by where it appears
// in the APIs) and depending on whether the word was complete (when it's
// actually the next word in the API entry that is of interest) or not.
void QsciAPIs::addAPIEntries(const WordIndexList &wl, bool complete,
        QStringList &with_context, bool &unambig)
{
    QStringList wseps = lexer()->autoCompletionWordSeparators();

    for (int w = 0; w < wl.count(); ++w)
    {
        const WordIndex &wi = wl[w];

        QStringList api_words = prep->apiWords(wi.first, wseps, false);

        int idx = wi.second;

        if (complete)
        {
            // Skip if this is the last word.
            if (++idx >= api_words.count())
                continue;
        }

        QString api_word, org;

        if (idx == 0)
        {
            api_word = api_words[0] + ' ';
            org = QString::fromLatin1("");
        }
        else
        {
            QStringList orgl = api_words.mid(0, idx);
            org = orgl.join(wseps.first());

            // Add the context (allowing for a possible image identifier).
            QString w = api_words[idx];
            QString type;
            int type_idx = w.indexOf(QLatin1String("?"));

            if (type_idx >= 0)
            {
                type = w.mid(type_idx);
                w.truncate(type_idx);
            }

            api_word = QString("%1 (%2)%3").arg(w).arg(org).arg(type);
        }

        // If the origin is different to the context then the context is
        // ambiguous.
        if (unambig)
        {
            if (unambiguous_context.isNull())
            {
                unambiguous_context = org;
            }
            else if (unambiguous_context != org)
            {
                unambiguous_context.truncate(0);
                unambig = false;
            }
        }

        if (!with_context.contains(api_word))
            with_context.append(api_word);
    }
}


// Return the call tip for a function.
QStringList QsciAPIs::callTips(const QStringList &context, int commas,
        QsciScintilla::CallTipsStyle style, QList<int> &shifts)
{
    QString path;
    QStringList new_context = positionOrigin(context, path);
    QStringList wseps = lexer()->autoCompletionWordSeparators();
    QStringList cts;

    if (origin_len > 0)
    {
        // The path should have a trailing word separator.
        const QString &wsep = wseps.first();
        path.chop(wsep.length());

        QStringList::const_iterator it = origin;
        QString prev;

        // Work out the length of the context.
        QStringList strip = path.split(wsep);
        strip.removeLast();
        int ctstart = strip.join(wsep).length();

        if (ctstart)
            ctstart += wsep.length();

        int shift;

        if (style == QsciScintilla::CallTipsContext)
        {
            shift = ctstart;
            ctstart = 0;
        }
        else
            shift = 0;

        // Make sure we only look at the functions we are interested in.
        path.append('(');

        while (it != prep->raw_apis.end() && (*it).startsWith(path))
        {
            QString w = (*it).mid(ctstart);

            if (w != prev && enoughCommas(w, commas))
            {
                shifts << shift;
                cts << w;
                prev = w;
            }

            ++it;
        }
    }
    else
    {
        const QString &fname = new_context[new_context.count() - 2];

        // Find everywhere the function name appears in the APIs.
        const WordIndexList *wil = wordIndexOf(fname);

        if (wil)
            for (int i = 0; i < wil->count(); ++i)
            {
                const WordIndex &wi = (*wil)[i];
                QStringList awords = prep->apiWords(wi.first, wseps, true);

                // Check the word is the function name and not part of any
                // context.
                if (wi.second != awords.count() - 1)
                    continue;

                const QString &api = prep->raw_apis[wi.first];

                int tail = api.indexOf('(');

                if (tail < 0)
                    continue;

                if (!enoughCommas(api, commas))
                    continue;

                if (style == QsciScintilla::CallTipsNoContext)
                {
                    shifts << 0;
                    cts << (fname + api.mid(tail));
                }
                else
                {
                    shifts << tail - fname.length();

                    // Remove any image type.
                    int im_type = api.indexOf('?');

                    if (im_type <= 0)
                        cts << api;
                    else
                        cts << (api.left(im_type - 1) + api.mid(tail));
                }
            }
    }

    return cts;
}


// Return true if a string has enough commas in the argument list.
bool QsciAPIs::enoughCommas(const QString &s, int commas)
{
    int end = s.indexOf(')');

    if (end < 0)
        return false;

    QString w = s.left(end);

    return (w.count(',') >= commas);
}


// Ensure the list is ready.
void QsciAPIs::prepare()
{
    // Handle the trivial case.
    if (worker)
        return;

    QsciAPIsPrepared *new_apis = new QsciAPIsPrepared;
    new_apis->raw_apis = apis;

    worker = new QsciAPIsWorker(this);
    worker->prepared = new_apis;
    worker->start();
}


// Cancel any current preparation.
void QsciAPIs::cancelPreparation()
{
    deleteWorker();
}


// Check that a prepared API file exists.
bool QsciAPIs::isPrepared(const QString &filename) const
{
    QString pname = prepName(filename);

    if (pname.isEmpty())
        return false;

    QFileInfo fi(pname);

    return fi.exists();
}


// Load the prepared API information.
bool QsciAPIs::loadPrepared(const QString &filename)
{
    QString pname = prepName(filename);

    if (pname.isEmpty())
        return false;

    // Read the prepared data and decompress it.
    QFile pf(pname);

    if (!pf.open(QIODevice::ReadOnly))
        return false;

    QByteArray cpdata = pf.readAll();

    pf.close();

    if (cpdata.count() == 0)
        return false;

    QByteArray pdata = qUncompress(cpdata);

    // Extract the data.
    QDataStream pds(pdata);

    unsigned char vers;
    pds >> vers;

    if (vers > PreparedDataFormatVersion)
        return false;

    char *lex_name;
    pds >> lex_name;

    if (qstrcmp(lex_name, lexer()->lexer()) != 0)
    {
        delete[] lex_name;
        return false;
    }

    delete[] lex_name;

    prep->wdict.clear();
    pds >> prep->wdict;

    if (!lexer()->caseSensitive())
    {
        // Build up the case dictionary.
        prep->cdict.clear();

        QMap<QString, WordIndexList>::const_iterator it = prep->wdict.begin();

        while (it != prep->wdict.end())
        {
            prep->cdict[it.key().toUpper()] = it.key();
            ++it;
        }
    }

    prep->raw_apis.clear();
    pds >> prep->raw_apis;

    // Allow the raw API information to be modified.
    apis = prep->raw_apis;

    return true;
}


// Save the prepared API information.
bool QsciAPIs::savePrepared(const QString &filename) const
{
    QString pname = prepName(filename, true);

    if (pname.isEmpty())
        return false;

    // Write the prepared data to a memory buffer.
    QByteArray pdata;
    QDataStream pds(&pdata, QIODevice::WriteOnly);

    // Use a serialisation format supported by Qt v3.0 and later.
    pds.setVersion(QDataStream::Qt_3_0);
    pds << PreparedDataFormatVersion;
    pds << lexer()->lexer();
    pds << prep->wdict;
    pds << prep->raw_apis;

    // Compress the data and write it.
    QFile pf(pname);

    if (!pf.open(QIODevice::WriteOnly|QIODevice::Truncate))
        return false;

    if (pf.write(qCompress(pdata)) < 0)
    {
        pf.close();
        return false;
    }

    pf.close();
    return true;
}


// Return the name of the default prepared API file.
QString QsciAPIs::defaultPreparedName() const
{
    return prepName(QString());
}


// Return the name of a prepared API file.
QString QsciAPIs::prepName(const QString &filename, bool mkpath) const
{
    // Handle the tivial case.
    if (!filename.isEmpty())
        return filename;

    QString pdname;
    char *qsci = getenv("QSCIDIR");

    if (qsci)
        pdname = qsci;
    else
    {
        static const char *qsci_dir = ".qsci";

        QDir pd = QDir::home();

        if (mkpath && !pd.exists(qsci_dir) && !pd.mkdir(qsci_dir))
            return QString();

        pdname = pd.filePath(qsci_dir);
    }

    return QString("%1/%2.pap").arg(pdname).arg(lexer()->lexer());
}


// Return installed API files.
QStringList QsciAPIs::installedAPIFiles() const
{
    QString qtdir = QLibraryInfo::location(QLibraryInfo::DataPath);

    QDir apidir = QDir(QString("%1/qsci/api/%2").arg(qtdir).arg(lexer()->lexer()));
    QStringList filenames;

    QStringList filters;
    filters << "*.api";

    QFileInfoList flist = apidir.entryInfoList(filters, QDir::Files, QDir::IgnoreCase);

    foreach (QFileInfo fi, flist)
        filenames << fi.absoluteFilePath();

    return filenames;
}
