#ifndef REMOTEMODEL_H
#define REMOTEMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class RemoteDatabase;

// List of fields stored in the JSON data
enum RemoteModelColumns
{
    RemoteModelColumnName,
    RemoteModelColumnType,
    RemoteModelColumnUrl,
    RemoteModelColumnCommitId,
    RemoteModelColumnSize,
    RemoteModelColumnLastModified,

    RemoteModelColumnCount
};

class RemoteModelItem
{
public:
    RemoteModelItem(RemoteModelItem* parent = nullptr);
    ~RemoteModelItem();

    QVariant value(RemoteModelColumns column) const;
    void setValue(RemoteModelColumns column, QVariant value);

    bool fetchedDirectoryList() const;
    void setFetchedDirectoryList(bool fetched);

    void appendChild(RemoteModelItem* item);
    RemoteModelItem* child(int row) const;
    RemoteModelItem* parent() const;
    int childCount() const;
    int row() const;

    // This function assumes the JSON value it's getting passed is an array ("[{...}, {...}, {...}, ...]"). It returns a list of model items, one
    // per array entry and each with the specified parent set.
    static QList<RemoteModelItem*> loadArray(const QJsonValue& value, RemoteModelItem* parent = nullptr);

private:
    // These are just the fields from the json objects returned by the dbhub.io server
    QVariant m_values[RemoteModelColumnCount];

    // Child items and parent item
    QList<RemoteModelItem*> m_children;
    RemoteModelItem* m_parent;

    // Indicates whether we already tried fetching a directory listing for this item. This serves two purposes:
    // 1) When having an empty directory this allows us to remove the expandable flag for this item.
    // 2) Between sending a network request and getting the reply this flag is already set, avoiding a second or third request being sent in the meantime.
    bool m_fetchedDirectoryList;
};

class RemoteModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit RemoteModel(QObject* parent, RemoteDatabase& remote);
    virtual ~RemoteModel();

    void setNewRootDir(const QString& url, const QString& cert);

    QModelIndex index(int row, int column,const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;

    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    bool hasChildren(const QModelIndex& parent) const;

    bool canFetchMore(const QModelIndex& parent) const;
    void fetchMore(const QModelIndex& parent);

    // This helper function takes a model index and returns the according model item. An invalid model index is used to indicate the
    // root item, so if the index is invalid the root item is returned. This means that if you need to check for actual invalid indices
    // this needs to be done prior to calling this function.
    const RemoteModelItem* modelIndexToItem(const QModelIndex& idx) const;

    // Returns the current client certificate
    const QString& currentClientCertificate() const;

signals:
    // This signal is emitted whenever a directory listing has been received and parsed
    void directoryListingParsed(QModelIndex parent);

private slots:
    // This is called whenever a network reply containing a directory listing arrives. json contains the reply data, userdata
    // contains some custom data passed to the request. In this case we expect this to be the model index of the parent tree item.
    void parseDirectoryListing(const QString& json, const QVariant& userdata);

private:
    // Pointer to the root item. This contains all the actual item data.
    RemoteModelItem* rootItem;

    // Thr header list is a list of column titles. It's a static list that's getting filled in the constructor.
    QStringList headerList;

    // Reference to the remote database object which is stored somewhere in the main window.
    RemoteDatabase& remoteDatabase;

    // This stores the currently used network identity so it can be used for further requests, e.g. for
    // lazy population.
    QString currentRootDirectory;
    QString currentClientCert;
    QString currentUserName;
};

#endif
