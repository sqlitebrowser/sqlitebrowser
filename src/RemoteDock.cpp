#include <QClipboard>
#include <QDesktopServices>
#include <QFileInfo>
#include <QInputDialog>
#include <QMessageBox>
#include <QUrl>
#include <QUrlQuery>

#include "RemoteDock.h"
#include "ui_RemoteDock.h"
#include "Settings.h"
#include "RemoteCommitsModel.h"
#include "RemoteDatabase.h"
#include "RemoteLocalFilesModel.h"
#include "RemoteModel.h"
#include "MainWindow.h"
#include "RemotePushDialog.h"
#include "PreferencesDialog.h"

RemoteDock::RemoteDock(MainWindow* parent)
    : QDialog(parent),
      ui(new Ui::RemoteDock),
      mainWindow(parent),
      remoteDatabase(parent->getRemote()),
      remoteModel(new RemoteModel(this, parent->getRemote())),
      remoteLocalFilesModel(new RemoteLocalFilesModel(this, parent->getRemote())),
      remoteCommitsModel(new RemoteCommitsModel(this))
{
    ui->setupUi(this);

    // Set models
    ui->treeRemote->setModel(remoteModel);
    ui->treeLocal->setModel(remoteLocalFilesModel);
    ui->treeDatabaseCommits->setModel(remoteCommitsModel);

    // When a database has been downloaded and must be opened, notify users of this class
    connect(&remoteDatabase, &RemoteDatabase::openFile, this, &RemoteDock::openFile);

    // Reload the directory tree and the list of local checkouts when a database upload has finished
    connect(&remoteDatabase, &RemoteDatabase::uploadFinished, remoteModel, &RemoteModel::refresh);
    connect(&remoteDatabase, &RemoteDatabase::uploadFinished, this, &RemoteDock::refreshLocalFileList);
    connect(&remoteDatabase, &RemoteDatabase::uploadFinished, [this]() {
        if(!currently_opened_file_info.file.empty())
            refreshMetadata(currently_opened_file_info.user_name(), QString::fromStdString(currently_opened_file_info.name));
    });
    connect(&remoteDatabase, &RemoteDatabase::openFile, this, &RemoteDock::refreshLocalFileList);

    // Whenever a new directory listing has been parsed, check if it was a new root dir and, if so, open the user's directory
    connect(remoteModel, &RemoteModel::directoryListingParsed, this, &RemoteDock::newDirectoryNode);

    // Show metadata for a database when we get it
    connect(&remoteDatabase, &RemoteDatabase::gotMetadata, this, &RemoteDock::showMetadata);

    // When the Preferences link is clicked in the no-certificates-label, open the preferences dialog. For other links than the ones we know,
    // just open them in a web browser
    connect(ui->labelNoCert, &QLabel::linkActivated, [this](const QString& link) {
        if(link == "#preferences")
        {
            PreferencesDialog dialog(mainWindow, PreferencesDialog::TabRemote);
            if(dialog.exec())
                mainWindow->reloadSettings();
        } else {
            QDesktopServices::openUrl(QUrl(link));
        }
    });

    // When changing the current branch in the branches combo box, update the tree view accordingly
    connect(ui->comboDatabaseBranch, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int /*index*/) {
        remoteCommitsModel->refresh(current_commit_json, ui->comboDatabaseBranch->currentData().toString().toStdString(), currently_opened_file_info.commit_id);
        ui->treeDatabaseCommits->expandAll();
    });

    // Prepare context menu for list of local clones
    connect(ui->treeLocal->selectionModel(), &QItemSelectionModel::currentChanged, [this](const QModelIndex& index, const QModelIndex&) {
        // Only enable database actions when a database was selected
        bool enable = index.isValid() &&
                !index.sibling(index.row(), RemoteLocalFilesModel::ColumnFile).data().isNull();
        ui->actionDeleteDatabase->setEnabled(enable);
    });
    connect(ui->actionDeleteDatabase, &QAction::triggered, [this]() {
       deleteLocalDatabase(ui->treeLocal->currentIndex());
    });
    ui->treeLocal->addAction(ui->actionDeleteDatabase);

    // Initial setup
    reloadSettings();
}

RemoteDock::~RemoteDock()
{
    delete ui;
}

void RemoteDock::reloadSettings()
{
    // Clear list of client certificates and add a dummy entry which does nothing except serve as
    // an explanation to the user.
    ui->comboUser->clear();
    ui->comboUser->addItem(tr("Select an identity to connect"), "dummy");

    // Load list of client certs
    QStringList client_certs = Settings::getValue("remote", "client_certificates").toStringList();
    for(const QString& file : client_certs)
    {
        auto certs = QSslCertificate::fromPath(file);
        for(const QSslCertificate& cert : certs)
            ui->comboUser->addItem(cert.subjectInfo(QSslCertificate::CommonName).at(0), file);
    }

    // Add public certificate for anonymous read-only access to dbhub.io
    ui->comboUser->addItem(tr("Public"), ":/user_certs/public.cert.pem");
}

void RemoteDock::setNewIdentity(const QString& identity)
{
    // Do nothing if the dummy entry was selected
    if(ui->comboUser->currentData() == "dummy")
        return;

    // Check if the dummy item is still there and remove it if it is
    if(ui->comboUser->itemData(0) == "dummy")
        ui->comboUser->removeItem(0);

    // Get certificate file name
    QString cert = ui->comboUser->itemData(ui->comboUser->findText(identity), Qt::UserRole).toString();
    if(cert.isEmpty())
        return;

    // Open root directory. Get host name from client cert
    remoteModel->setNewRootDir(remoteDatabase.getInfoFromClientCert(cert, RemoteDatabase::CertInfoServer), cert);

    // Reset list of local checkouts
    remoteLocalFilesModel->setIdentity(cert);
    refreshLocalFileList();

    // Enable buttons if necessary
    enableButtons();
}

void RemoteDock::fetchDatabase(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    // Get item
    const RemoteModelItem* item = remoteModel->modelIndexToItem(idx);

    // Only open database file
    if(item->value(RemoteModelColumnType).toString() == "database")
        fetchDatabase(item->value(RemoteModelColumnUrl).toString());
}

void RemoteDock::fetchDatabase(QString url_string)
{
    // If no URL was provided ask the user. Default to the current clipboard contents
    if(url_string.isEmpty())
    {
        url_string = QInputDialog::getText(this,
                                           qApp->applicationName(),
                                           tr("This downloads a database from a remote server for local editing. "
                                              "Please enter the URL to clone from. You can generate this URL by clicking "
                                              "the 'Clone Database in DB4S' button on the web page of the database."),
                                           QLineEdit::Normal,
                                           QApplication::clipboard()->text());
    }

    if(url_string.isEmpty())
        return;

    // Check the URL
    QUrl url(url_string);
    if(url.authority() != QUrl(remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoServer)).authority())
    {
        QMessageBox::warning(this, qApp->applicationName(), tr("Invalid URL: The host name does not match the host name of the current identity."));
        return;
    }
    if(!QUrlQuery(url).hasQueryItem("branch"))
    {
        QMessageBox::warning(this, qApp->applicationName(), tr("Invalid URL: No branch name specified."));
        return;
    }
    if(!QUrlQuery(url).hasQueryItem("commit"))
    {
        QMessageBox::warning(this, qApp->applicationName(), tr("Invalid URL: No commit ID specified."));
        return;
    }

    // Clone the database
    remoteDatabase.fetch(url.toString(), RemoteDatabase::RequestTypeDatabase, remoteModel->currentClientCertificate());
}

void RemoteDock::enableButtons()
{
    bool db_opened = mainWindow->getDb().isOpen() && mainWindow->getDb().currentFile() != ":memory:";
    bool logged_in = !remoteModel->currentClientCertificate().isEmpty();

    ui->buttonCloneDatabase->setEnabled(logged_in);
    ui->buttonPushDatabase->setEnabled(db_opened && logged_in);
}

void RemoteDock::pushDatabase()
{
    // If the currently active identity is the read-only public access to dbhub.io, don't show the Push Database dialog because it won't work anyway.
    // Instead switch to an explanation offering some advice to create and import a proper certificate.
    if(remoteModel->currentClientCertificate() == ":/user_certs/public.cert.pem")
    {
        ui->stack->setCurrentIndex(1);
        return;
    }

    // Show a warning when trying to push a database with unsaved changes
    if(mainWindow->getDb().getDirty())
    {
        if(QMessageBox::warning(this,
                                QApplication::applicationName(),
                                tr("The database has unsaved changes. Are you sure you want to push it before saving?"),
                                QMessageBox::Yes | QMessageBox::Cancel,
                                QMessageBox::Cancel) == QMessageBox::Cancel)
            return;
    }

    // The default suggestion for a database name is the local file name. If it is a remote file (like when it initially was fetched using DB4S),
    // the extra bit of information at the end of the name gets removed first.
    QString name = QFileInfo(mainWindow->getDb().currentFile()).fileName();
    name = name.remove(QRegExp("_[0-9]+.remotedb$"));

    // Show the user a dialog for setting all the commit details
    QString host = remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoServer);
    RemotePushDialog pushDialog(this, remoteDatabase, host, remoteModel->currentClientCertificate(), name);
    if(pushDialog.exec() != QDialog::Accepted)
        return;

    // Build push URL
    QString url = host;
    url.append(remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoUser));
    url.append("/");
    url.append(pushDialog.name());

    // Push database
    remoteDatabase.push(mainWindow->getDb().currentFile(), url, remoteModel->currentClientCertificate(), pushDialog.name(),
                        pushDialog.commitMessage(), pushDialog.licence(), pushDialog.isPublic(), pushDialog.branch(), pushDialog.forcePush());
}

void RemoteDock::newDirectoryNode(const QModelIndex& parent)
{
    // Was this a new root dir?
    if(!parent.isValid())
    {
        // Then check if there is a directory with the current user name

        // Get current user name
        QString user = remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoUser);

        for(int i=0;i<remoteModel->rowCount();i++)
        {
            QModelIndex child = remoteModel->index(i, RemoteModelColumnName);
            if(child.data().toString() == user)
            {
                ui->treeRemote->expand(child);
                break;
            }
        }
    }
}

void RemoteDock::reject()
{
    // We override this, to ensure the Escape key doesn't make this dialog
    // dock go away
    return;
}

void RemoteDock::switchToMainView()
{
    ui->stack->setCurrentIndex(0);
}

void RemoteDock::refreshLocalFileList()
{
    remoteLocalFilesModel->refresh();

    // Expand node for current user
    QString user = remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoUser);
    for(int i=0;i<remoteLocalFilesModel->rowCount();i++)
    {
        QModelIndex child = remoteLocalFilesModel->index(i, RemoteLocalFilesModel::ColumnName);
        if(child.data().toString() == user)
        {
            ui->treeLocal->expand(child);
            break;
        }
    }
}

void RemoteDock::openLocalFile(const QModelIndex& idx)
{
    if(!idx.isValid())
        return;

    QString file = idx.sibling(idx.row(), RemoteLocalFilesModel::ColumnFile).data().toString();
    if(!file.isEmpty())
        emit openFile(Settings::getValue("remote", "clonedirectory").toString() + "/" + file);
}

void RemoteDock::fileOpened(const QString& filename)
{
    // Clear data first
    currently_opened_file_info.clear();
    remoteCommitsModel->clear();
    ui->comboDatabaseBranch->clear();
    ui->labelDatabaseUser->setText(QString());
    ui->labelDatabaseFile->setText(QString());
    ui->labelDatabaseBranch->setText(QString());

    // Do nothing if the file name is empty (indicating a closed database) or this is an in-memory database
    if(filename.isEmpty() || filename == ":memory:")
        return;

    // Check if it is a tracked remote database file and retrieve the information we have on it
    if(filename.startsWith(Settings::getValue("remote", "clonedirectory").toString()))
        currently_opened_file_info = remoteDatabase.localGetLocalFileInfo(filename);

    // Is this actually a clone of a remote database?
    if(!currently_opened_file_info.file.empty())
    {
        // Copy information to view
        ui->labelDatabaseUser->setText(currently_opened_file_info.user_name());
        ui->labelDatabaseFile->setText(QString::fromStdString(currently_opened_file_info.name));
        ui->labelDatabaseBranch->setText(QString::fromStdString(currently_opened_file_info.branch));

        // Make sure the current identity matches the identity used to clone this file in the first place.
        // A mismatch is possible when the local database file has been opened using a recent files menu item or some similar technique.
        if(QString::fromStdString(currently_opened_file_info.identity) != QFileInfo(remoteModel->currentClientCertificate()).fileName())
            ui->comboUser->setCurrentIndex(ui->comboUser->findData("/" + QString::fromStdString(currently_opened_file_info.identity), Qt::UserRole, Qt::MatchEndsWith));

        // Query more information on database from server
        refreshMetadata(currently_opened_file_info.user_name(), QString::fromStdString(currently_opened_file_info.name));

        // Switch to "Current Database" tab
        ui->tabs->setCurrentIndex(2);
    }
}

void RemoteDock::refreshMetadata(const QString& username, const QString& dbname)
{
    QUrl url(remoteDatabase.getInfoFromClientCert(remoteModel->currentClientCertificate(), RemoteDatabase::CertInfoServer) + "/metadata/get");
    QUrlQuery query;
    query.addQueryItem("username", username);
    query.addQueryItem("folder", "/");
    query.addQueryItem("dbname", dbname);
    url.setQuery(query);
    remoteDatabase.fetch(url.toString(), RemoteDatabase::RequestTypeMetadata, remoteModel->currentClientCertificate());
}

void RemoteDock::showMetadata(const std::vector<RemoteMetadataBranchInfo>& branches, const std::string& commits,
                              const std::vector<RemoteMetadataReleaseInfo>& releases, const std::vector<RemoteMetadataReleaseInfo>& tags,
                              const std::string& /*default_branch*/)
{
    current_commit_json = commits;

    // Fill branches combo box
    ui->comboDatabaseBranch->clear();
    for(const auto& branch : branches)
        ui->comboDatabaseBranch->addItem(QString::fromStdString(branch.name), QString::fromStdString(branch.commit_id));
    for(const auto& release : releases)
        ui->comboDatabaseBranch->addItem(QString::fromStdString(release.name) + " (" + tr("release") + ")", QString::fromStdString(release.commit_id));
    for(const auto& tag : tags)
        ui->comboDatabaseBranch->addItem(QString::fromStdString(tag.name) + " (" + tr("tag") + ")", QString::fromStdString(tag.commit_id));
    ui->comboDatabaseBranch->setCurrentIndex(ui->comboDatabaseBranch->findText(ui->labelDatabaseBranch->text()));
}

void RemoteDock::deleteLocalDatabase(const QModelIndex& index)
{
    if(!index.isValid())
        return;

    QString filename = index.sibling(index.row(), RemoteLocalFilesModel::ColumnFile).data().toString();
    QString path = Settings::getValue("remote", "clonedirectory").toString() + "/" + filename;

    // Warn when trying to delete a currently opened database file
    if(mainWindow->getDb().currentFile() == path)
    {
        QMessageBox::warning(this, QApplication::applicationName(), tr("The database you are trying to delete is currently opened. "
                                                                       "Please close it before deleting."));
        return;
    }

    // Let user confirm deleting the database
    if(QMessageBox::warning(this, QApplication::applicationName(), tr("This deletes the local version of this database with all the "
                                                                      "changes you have not committed yet. Are you sure you want to "
                                                                      "delete this database?"),
                            QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel)
    {
        return;
    }

    // Delete the file
    remoteLocalFilesModel->removeRow(index.row(), index.parent());
}
