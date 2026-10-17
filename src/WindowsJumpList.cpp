#include "WindowsJumpList.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <windows.h>
#include <objbase.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <propvarutil.h>
#include <initguid.h>
#include <propkey.h>

namespace
{

// Small RAII helper for COM interface pointers so every early return releases what was acquired
template<typename T>
class ComPtr
{
public:
    ComPtr() = default;
    ~ComPtr() { if(m_ptr) m_ptr->Release(); }
    ComPtr(const ComPtr&) = delete;
    ComPtr& operator=(const ComPtr&) = delete;

    T* operator->() const { return m_ptr; }
    T* get() const { return m_ptr; }
    T** put() { return &m_ptr; }
    void** putVoid() { return reinterpret_cast<void**>(&m_ptr); }

private:
    T* m_ptr = nullptr;
};

std::wstring toWide(const QString& str)
{
    return str.toStdWString();
}

// Builds the command line arguments used for opening a recent file entry
QString argumentsForEntry(const QString& entry)
{
    if(entry.startsWith("[ro]"))
        return QString("-R \"%1\"").arg(QDir::toNativeSeparators(entry.mid(4)));
    else
        return QString("\"%1\"").arg(QDir::toNativeSeparators(entry));
}

bool createLink(const QString& entry, IShellLinkW** result)
{
    QString file = entry;
    const bool readOnly = file.startsWith("[ro]");
    if(readOnly)
        file = file.mid(4);

    ComPtr<IShellLinkW> link;
    if(FAILED(CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkW, link.putVoid())))
        return false;

    const std::wstring exe = toWide(QDir::toNativeSeparators(QCoreApplication::applicationFilePath()));
    const std::wstring workingDir = toWide(QDir::toNativeSeparators(QFileInfo(file).absolutePath()));
    const std::wstring args = toWide(argumentsForEntry(entry));
    QString description = QDir::toNativeSeparators(file);
    if(readOnly)
        description += QCoreApplication::translate("MainWindow", " (read only)");
    const std::wstring desc = toWide(description);

    link->SetPath(exe.c_str());
    link->SetArguments(args.c_str());
    link->SetWorkingDirectory(workingDir.c_str());
    link->SetDescription(desc.c_str());
    link->SetIconLocation(exe.c_str(), 0);

    // The title is what the Jump List shows. Without it, Windows would display the name of the executable.
    ComPtr<IPropertyStore> props;
    if(FAILED(link->QueryInterface(IID_IPropertyStore, props.putVoid())))
        return false;

    QString title = QFileInfo(file).fileName();
    if(readOnly)
        title += QCoreApplication::translate("MainWindow", " (read only)");
    const std::wstring titleW = toWide(title);

    PROPVARIANT value;
    if(FAILED(InitPropVariantFromString(titleW.c_str(), &value)))
        return false;
    const HRESULT hr = props->SetValue(PKEY_Title, value);
    PropVariantClear(&value);
    if(FAILED(hr) || FAILED(props->Commit()))
        return false;

    *result = link.get();
    (*result)->AddRef();
    return true;
}

// Returns the command line arguments of all items the user removed from the Jump List
QStringList removedArguments(IObjectArray* removed)
{
    QStringList result;
    UINT count = 0;
    if(!removed || FAILED(removed->GetCount(&count)))
        return result;

    for(UINT i = 0; i < count; ++i)
    {
        ComPtr<IShellLinkW> link;
        if(FAILED(removed->GetAt(i, IID_IShellLinkW, link.putVoid())))
            continue;

        const int argsSize = MAX_PATH * 2;
        wchar_t args[argsSize] = {};
        if(SUCCEEDED(link->GetArguments(args, argsSize)))
            result.push_back(QString::fromWCharArray(args));
    }
    return result;
}

}

namespace WindowsJumpList
{

QStringList update(const QString& categoryName, const QStringList& recentFiles)
{
    // Qt has already initialised COM for the GUI thread. This only balances the reference count.
    const HRESULT comInit = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    const bool uninitialize = SUCCEEDED(comInit);

    QStringList removedEntries;

    [&]() {
        ComPtr<ICustomDestinationList> destinations;
        if(FAILED(CoCreateInstance(CLSID_DestinationList, nullptr, CLSCTX_INPROC_SERVER, IID_ICustomDestinationList, destinations.putVoid())))
            return;

        UINT maxSlots = 0;
        ComPtr<IObjectArray> removed;
        if(FAILED(destinations->BeginList(&maxSlots, IID_IObjectArray, removed.putVoid())))
            return;

        // Windows refuses a category containing items the user has removed, so those must be skipped
        const QStringList removedArgs = removedArguments(removed.get());

        ComPtr<IObjectCollection> items;
        if(FAILED(CoCreateInstance(CLSID_EnumerableObjectCollection, nullptr, CLSCTX_INPROC_SERVER, IID_IObjectCollection, items.putVoid())))
        {
            destinations->AbortList();
            return;
        }

        UINT added = 0;
        for(const QString& entry : recentFiles)
        {
            if(removedArgs.contains(argumentsForEntry(entry), Qt::CaseInsensitive))
            {
                removedEntries.push_back(entry);
                continue;
            }
            if(added >= maxSlots)
                continue;

            ComPtr<IShellLinkW> link;
            if(createLink(entry, link.put()) && SUCCEEDED(items->AddObject(link.get())))
                added++;
        }

        if(added > 0)
        {
            ComPtr<IObjectArray> array;
            const std::wstring category = toWide(categoryName);
            if(FAILED(items->QueryInterface(IID_IObjectArray, array.putVoid())) ||
               FAILED(destinations->AppendCategory(category.c_str(), array.get())))
            {
                destinations->AbortList();
                return;
            }
        }

        // Committing an empty list clears the Jump List, which is what we want when there are no recent files
        destinations->CommitList();
    }();

    if(uninitialize)
        CoUninitialize();

    return removedEntries;
}

}
