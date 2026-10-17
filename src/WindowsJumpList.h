#ifndef WINDOWSJUMPLIST_H
#define WINDOWSJUMPLIST_H

#include <QStringList>

namespace WindowsJumpList
{

// Replaces the custom category of the taskbar Jump List with the given recent files. The entries use the same format as the
// "General/recentFileList" setting, i.e. read-only files are prefixed with "[ro]". Returns the entries the user has removed
// from the Jump List since the last update, so the caller can drop them from its own list as Windows requires.
QStringList update(const QString& categoryName, const QStringList& recentFiles);

}

#endif
