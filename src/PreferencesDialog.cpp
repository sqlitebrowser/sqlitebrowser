#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "FileDialog.h"
#include "Settings.h"
#include "Application.h"
#include "MainWindow.h"
#include "RemoteNetwork.h"
#include "FileExtensionManager.h"
#include "ProxyDialog.h"

#include <QDir>
#include <QColorDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QStandardPaths>
#include <QStyledItemDelegate>

PreferencesDialog::PreferencesDialog(QWidget* parent, Tabs tab)
    : QDialog(parent),
      ui(new Ui::PreferencesDialog),
      m_proxyDialog(new ProxyDialog(this)),
      m_dbFileExtensions(Settings::getValue("General", "DBFileExtensions").toString().split(";;"))
{
    ui->setupUi(this);
    ui->treeSyntaxHighlighting->setColumnHidden(0, true);
    ui->tableClientCerts->setColumnHidden(0, true);

    ui->fr_bin_bg->installEventFilter(this);
    ui->fr_bin_fg->installEventFilter(this);
    ui->fr_reg_bg->installEventFilter(this);
    ui->fr_reg_fg->installEventFilter(this);
    ui->fr_null_bg->installEventFilter(this);
    ui->fr_null_fg->installEventFilter(this);
    ui->fr_formatted_bg->installEventFilter(this);
    ui->fr_formatted_fg->installEventFilter(this);

    connect(ui->comboDataBrowserFont, static_cast<void (QFontComboBox::*)(int)>(&QFontComboBox::currentIndexChanged), this, &PreferencesDialog::updatePreviewFont);
    connect(ui->spinDataBrowserFontSize, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &PreferencesDialog::updatePreviewFont);

#ifndef CHECKNEWVERSION
    ui->labelUpdates->setVisible(false);
    ui->checkUpdates->setVisible(false);
#endif

#ifdef Q_OS_MACX
    listUpBuiltinExtension();
#else
    ui->labeBuiltinExtensions->setVisible(false);
    ui->listBuiltinExtensions->setVisible(false);
#endif

    loadSettings();

    connect(ui->appStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(adjustColorsToStyle(int)));

    // Avoid different heights due to having check boxes or not
    ui->treeSyntaxHighlighting->setUniformRowHeights(true);

    // Set current tab
    ui->tabWidget->setCurrentIndex(tab);

    // Connect 'Export Settings' and 'Import Settings' buttons
    connect(ui->buttonExportSettings, &QPushButton::clicked, this, &PreferencesDialog::exportSettings);
    connect(ui->buttonImportSettings, &QPushButton::clicked, this, &PreferencesDialog::importSettings);
}

/*
 *  Destroys the object and frees any allocated resources
 */
PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::chooseLocation()
{
    QString s = FileDialog::getExistingDirectory(
                NoSpecificType,
                this,
                tr("Choose a directory"),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!s.isEmpty())
        ui->locationEdit->setText(s);
}

void PreferencesDialog::loadSettings()
{
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(Settings::getValue("db", "defaultencoding").toString(), Qt::MatchFixedString));
    ui->comboDefaultLocation->setCurrentIndex(Settings::getValue("db", "savedefaultlocation").toInt());
    ui->locationEdit->setText(QDir::toNativeSeparators(Settings::getValue("db", "defaultlocation").toString()));
    ui->checkPromptSQLTabsInNewProject->setChecked(Settings::getValue("General", "promptsqltabsinnewproject").toBool());
    ui->checkUpdates->setChecked(Settings::getValue("checkversion", "enabled").toBool());

    ui->checkHideSchemaLinebreaks->setChecked(Settings::getValue("db", "hideschemalinebreaks").toBool());
    ui->foreignKeysCheckBox->setChecked(Settings::getValue("db", "foreignkeys").toBool());
    ui->spinPrefetchSize->setValue(Settings::getValue("db", "prefetchsize").toInt());
    ui->editDatabaseDefaultSqlText->setText(Settings::getValue("db", "defaultsqltext").toString());

    ui->defaultFieldTypeComboBox->addItems(DBBrowserDB::Datatypes);

    int defaultFieldTypeIndex = Settings::getValue("db", "defaultfieldtype").toInt();
    if (defaultFieldTypeIndex < DBBrowserDB::Datatypes.count())
    {
        ui->defaultFieldTypeComboBox->setCurrentIndex(defaultFieldTypeIndex);
    }

    ui->spinStructureFontSize->setValue(Settings::getValue("db", "fontsize").toInt());

    // Gracefully handle the preferred Data Browser font not being available
    int matchingFont = ui->comboDataBrowserFont->findText(Settings::getValue("databrowser", "font").toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getDefaultValue("databrowser", "font").toString());
    ui->comboDataBrowserFont->setCurrentIndex(matchingFont);

    ui->spinDataBrowserFontSize->setValue(Settings::getValue("databrowser", "fontsize").toInt());
    loadColorSetting(ui->fr_null_fg, "null_fg");
    loadColorSetting(ui->fr_null_bg, "null_bg");
    loadColorSetting(ui->fr_bin_fg, "bin_fg");
    loadColorSetting(ui->fr_bin_bg, "bin_bg");
    loadColorSetting(ui->fr_reg_fg, "reg_fg");
    loadColorSetting(ui->fr_reg_bg, "reg_bg");
    loadColorSetting(ui->fr_formatted_fg, "formatted_fg");
    loadColorSetting(ui->fr_formatted_bg, "formatted_bg");

    ui->spinSymbolLimit->setValue(Settings::getValue("databrowser", "symbol_limit").toInt());
    ui->spinCompleteThreshold->setValue(Settings::getValue("databrowser", "complete_threshold").toInt());
    ui->checkShowImagesInline->setChecked(Settings::getValue("databrowser", "image_preview").toBool());
    ui->txtNull->setText(Settings::getValue("databrowser", "null_text").toString());
    ui->txtBlob->setText(Settings::getValue("databrowser", "blob_text").toString());
    ui->editFilterEscape->setText(Settings::getValue("databrowser", "filter_escape").toString());
    ui->spinFilterDelay->setValue(Settings::getValue("databrowser", "filter_delay").toInt());

    ui->treeSyntaxHighlighting->resizeColumnToContents(1);

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        std::string name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0).toStdString();
        QString colorname = Settings::getValue("syntaxhighlighter", name + "_colour").toString();
        QColor color = QColor(colorname);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setForeground(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setBackground(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setText(2, colorname);

        // Add font properties except for colour-only entries
        if (name != "null" && name != "currentline" &&
            name != "background" && name != "foreground" && name != "highlight" &&
            name != "selected_fg" && name != "selected_bg") {
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(3, Settings::getValue("syntaxhighlighter", name + "_bold").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(4, Settings::getValue("syntaxhighlighter", name + "_italic").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(5, Settings::getValue("syntaxhighlighter", name + "_underline").toBool() ? Qt::Checked : Qt::Unchecked);
        }
    }

    // Remote settings
    ui->checkUseRemotes->setChecked(Settings::getValue("remote", "active").toBool());
    {
        auto ca_certs = RemoteNetwork::get().caCertificates();
        ui->tableCaCerts->setRowCount(ca_certs.size());
        for(int i=0;i<ca_certs.size();i++)
        {
            QSslCertificate cert = ca_certs.at(i);

            QTableWidgetItem* cert_cn = new QTableWidgetItem(cert.subjectInfo(QSslCertificate::CommonName).at(0));
            cert_cn->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 0, cert_cn);

            QTableWidgetItem* cert_o = new QTableWidgetItem(cert.subjectInfo(QSslCertificate::Organization).at(0));
            cert_o->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 1, cert_o);

            QTableWidgetItem* cert_from = new QTableWidgetItem(cert.effectiveDate().toString());
            cert_from->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 2, cert_from);

            QTableWidgetItem* cert_to = new QTableWidgetItem(cert.expiryDate().toString());
            cert_to->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 3, cert_to);

            QTableWidgetItem* cert_serialno = new QTableWidgetItem(QString(cert.serialNumber()));
            cert_serialno->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableCaCerts->setItem(i, 4, cert_serialno);
        }
    }
    {
        const QStringList client_certs = Settings::getValue("remote", "client_certificates").toStringList();
        for(const QString& file : client_certs)
        {
            const auto certs = QSslCertificate::fromPath(file);
            for(const QSslCertificate& cert : certs)
                addClientCertToTable(file, cert);
        }
    }
    ui->editRemoteCloneDirectory->setText(QDir::toNativeSeparators(Settings::getValue("remote", "clonedirectory").toString()));

    // Gracefully handle the preferred Editor font not being available
    matchingFont = ui->comboEditorFont->findText(Settings::getValue("editor", "font").toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getDefaultValue("editor", "font").toString());
    ui->comboEditorFont->setCurrentIndex(matchingFont);

    ui->spinEditorFontSize->setValue(Settings::getValue("editor", "fontsize").toInt());
    ui->spinTabSize->setValue(Settings::getValue("editor", "tabsize").toInt());
    ui->checkIndentationUseTabs->setChecked(Settings::getValue("editor", "indentation_use_tabs").toBool());
    ui->spinLogFontSize->setValue(Settings::getValue("log", "fontsize").toInt());
    ui->wrapComboBox->setCurrentIndex(Settings::getValue("editor", "wrap_lines").toInt());
    ui->quoteComboBox->setCurrentIndex(Settings::getValue("editor", "identifier_quotes").toInt());
    ui->checkAutoCompletion->setChecked(Settings::getValue("editor", "auto_completion").toBool());
    ui->checkCompleteUpper->setEnabled(Settings::getValue("editor", "auto_completion").toBool());
    ui->checkCompleteUpper->setChecked(Settings::getValue("editor", "upper_keywords").toBool());
    ui->checkErrorIndicators->setChecked(Settings::getValue("editor", "error_indicators").toBool());
    ui->checkHorizontalTiling->setChecked(Settings::getValue("editor", "horizontal_tiling").toBool());
    ui->checkCloseButtonOnTabs->setChecked(Settings::getValue("editor", "close_button_on_tabs").toBool());

    ui->listExtensions->addItems(Settings::getValue("extensions", "list").toStringList());
#ifdef Q_OS_MACX
    for (int i=0;i<ui->listBuiltinExtensions->count();++i)
    {
        QListWidgetItem* item = ui->listBuiltinExtensions->item(i);
        item->setCheckState(Settings::getValue("extensions", "builtin").toMap().value(item->text()).toBool() ? Qt::Checked : Qt::Unchecked);
    }
#endif
    ui->checkRegexDisabled->setChecked(Settings::getValue("extensions", "disableregex").toBool());
    ui->checkAllowLoadExtension->setChecked(Settings::getValue("extensions", "enable_load_extension").toBool());
    fillLanguageBox();
    ui->appStyleCombo->setCurrentIndex(Settings::getValue("General", "appStyle").toInt());
    ui->toolbarStyleComboMain->setCurrentIndex(Settings::getValue("General", "toolbarStyle").toInt());
    ui->toolbarStyleComboStructure->setCurrentIndex(Settings::getValue("General", "toolbarStyleStructure").toInt());
    ui->toolbarStyleComboBrowse->setCurrentIndex(Settings::getValue("General", "toolbarStyleBrowse").toInt());
    ui->toolbarStyleComboSql->setCurrentIndex(Settings::getValue("General", "toolbarStyleSql").toInt());
    ui->toolbarStyleComboEditCell->setCurrentIndex(Settings::getValue("General", "toolbarStyleEditCell").toInt());
    ui->spinGeneralFontSize->setValue(Settings::getValue("General", "fontsize").toInt());
    ui->spinMaxRecentFiles->setValue(Settings::getValue("General", "maxRecentFiles").toInt());
}

void PreferencesDialog::saveSettings(bool accept)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    Settings::setValue("db", "defaultencoding", ui->encodingComboBox->currentText());
    Settings::setValue("db", "defaultlocation", ui->locationEdit->text());
    Settings::setValue("db", "savedefaultlocation", ui->comboDefaultLocation->currentIndex());
    Settings::setValue("db", "hideschemalinebreaks", ui->checkHideSchemaLinebreaks->isChecked());
    Settings::setValue("db", "foreignkeys", ui->foreignKeysCheckBox->isChecked());
    Settings::setValue("db", "prefetchsize", ui->spinPrefetchSize->value());
    Settings::setValue("db", "defaultsqltext", ui->editDatabaseDefaultSqlText->text());
    Settings::setValue("db", "defaultfieldtype", ui->defaultFieldTypeComboBox->currentIndex());
    Settings::setValue("db", "fontsize", ui->spinStructureFontSize->value());

    Settings::setValue("checkversion", "enabled", ui->checkUpdates->isChecked());

    Settings::setValue("databrowser", "font", ui->comboDataBrowserFont->currentText());
    Settings::setValue("databrowser", "fontsize", ui->spinDataBrowserFontSize->value());
    Settings::setValue("databrowser", "image_preview", ui->checkShowImagesInline->isChecked());
    saveColorSetting(ui->fr_null_fg, "null_fg");
    saveColorSetting(ui->fr_null_bg, "null_bg");
    saveColorSetting(ui->fr_reg_fg, "reg_fg");
    saveColorSetting(ui->fr_reg_bg, "reg_bg");
    saveColorSetting(ui->fr_formatted_fg, "formatted_fg");
    saveColorSetting(ui->fr_formatted_bg, "formatted_bg");
    saveColorSetting(ui->fr_bin_fg, "bin_fg");
    saveColorSetting(ui->fr_bin_bg, "bin_bg");
    Settings::setValue("databrowser", "symbol_limit", ui->spinSymbolLimit->value());
    Settings::setValue("databrowser", "complete_threshold", ui->spinCompleteThreshold->value());
    Settings::setValue("databrowser", "null_text", ui->txtNull->text());
    Settings::setValue("databrowser", "blob_text", ui->txtBlob->text());
    Settings::setValue("databrowser", "filter_escape", ui->editFilterEscape->text());
    Settings::setValue("databrowser", "filter_delay", ui->spinFilterDelay->value());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        std::string name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0).toStdString();
        Settings::setValue("syntaxhighlighter", name + "_colour", ui->treeSyntaxHighlighting->topLevelItem(i)->text(2));
        Settings::setValue("syntaxhighlighter", name + "_bold", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(3) == Qt::Checked);
        Settings::setValue("syntaxhighlighter", name + "_italic", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(4) == Qt::Checked);
        Settings::setValue("syntaxhighlighter", name + "_underline", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(5) == Qt::Checked);
    }
    Settings::setValue("editor", "font", ui->comboEditorFont->currentText());
    Settings::setValue("editor", "fontsize", ui->spinEditorFontSize->value());
    Settings::setValue("editor", "tabsize", ui->spinTabSize->value());
    Settings::setValue("editor", "indentation_use_tabs", ui->checkIndentationUseTabs->isChecked());
    Settings::setValue("log", "fontsize", ui->spinLogFontSize->value());
    Settings::setValue("editor", "wrap_lines", ui->wrapComboBox->currentIndex());
    Settings::setValue("editor", "identifier_quotes", ui->quoteComboBox->currentIndex());
    Settings::setValue("editor", "auto_completion", ui->checkAutoCompletion->isChecked());
    Settings::setValue("editor", "upper_keywords", ui->checkCompleteUpper->isChecked());
    Settings::setValue("editor", "error_indicators", ui->checkErrorIndicators->isChecked());
    Settings::setValue("editor", "horizontal_tiling", ui->checkHorizontalTiling->isChecked());
    Settings::setValue("editor", "close_button_on_tabs", ui->checkCloseButtonOnTabs->isChecked());

    QStringList extList;
    for(int i=0;i<ui->listExtensions->count();++i)
        extList.append(ui->listExtensions->item(i)->text());
    Settings::setValue("extensions", "list", extList);
    Settings::setValue("extensions", "disableregex", ui->checkRegexDisabled->isChecked());
    Settings::setValue("extensions", "enable_load_extension", ui->checkAllowLoadExtension->isChecked());

#ifdef Q_OS_MACX
    QVariantMap builtinExtList;
    for (int i=0;i<ui->listBuiltinExtensions->count();++i)
        builtinExtList.insert(ui->listBuiltinExtensions->item(i)->text(), ui->listBuiltinExtensions->item(i)->checkState());
    Settings::setValue("extensions", "builtin", QVariant::fromValue(builtinExtList));
#endif

    // Save remote settings
    Settings::setValue("remote", "active", ui->checkUseRemotes->isChecked());
    QStringList old_client_certs = Settings::getValue("remote", "client_certificates").toStringList();
    QStringList new_client_certs;
    for(int i=0;i<ui->tableClientCerts->rowCount();i++)
    {
        // Loop through the new list of client certs

        // If this certificate was already imported, remove it from the list of old certificates. All remaining certificates on this
        // list will be deleted later on.
        QString path = ui->tableClientCerts->item(i, 0)->text();
        if(old_client_certs.contains(path))
        {
            // This is a cert that is already imported
            old_client_certs.removeAll(path);
            new_client_certs.push_back(path);
        } else {
            // This is a new certificate. Copy file to a safe place.

            // Generate unique destination file name
            QString copy_to = QStandardPaths::writableLocation(
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
                        QStandardPaths::AppDataLocation
#else
                        QStandardPaths::GenericDataLocation
#endif
                        ).append("/").append(QFileInfo(path).fileName());
            int suffix = 0;
            do
            {
                suffix++;
            } while(QFile::exists(copy_to + QString::number(suffix)));

            // Copy file
            copy_to.append(QString::number(suffix));
            QDir().mkpath(QStandardPaths::writableLocation(
#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
                              QStandardPaths::AppDataLocation
#else
                              QStandardPaths::GenericDataLocation
#endif
                              ));
            QFile::copy(path, copy_to);

            new_client_certs.push_back(copy_to);
        }
    }
    for(const QString& file : qAsConst(old_client_certs))
    {
        // Now only the deleted client certs are still in the old list. Delete the cert files associated with them.
        QFile::remove(file);
    }
    Settings::setValue("remote", "client_certificates", new_client_certs);
    Settings::setValue("remote", "clonedirectory", ui->editRemoteCloneDirectory->text());

    // Warn about restarting to change language
    QVariant newLanguage = ui->languageComboBox->itemData(ui->languageComboBox->currentIndex());
    if (newLanguage != Settings::getValue("General", "language"))
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("The language will change after you restart the application."));

    Settings::setValue("General", "language", newLanguage);
    Settings::setValue("General", "appStyle", ui->appStyleCombo->currentIndex());
    Settings::setValue("General", "toolbarStyle", ui->toolbarStyleComboMain->currentIndex());
    Settings::setValue("General", "toolbarStyleStructure", ui->toolbarStyleComboStructure->currentIndex());
    Settings::setValue("General", "toolbarStyleBrowse", ui->toolbarStyleComboBrowse->currentIndex());
    Settings::setValue("General", "toolbarStyleSql", ui->toolbarStyleComboSql->currentIndex());
    Settings::setValue("General", "toolbarStyleEditCell", ui->toolbarStyleComboEditCell->currentIndex());
    Settings::setValue("General", "DBFileExtensions", m_dbFileExtensions.join(";;") );
    Settings::setValue("General", "fontsize", ui->spinGeneralFontSize->value());
    Settings::setValue("General", "maxRecentFiles", ui->spinMaxRecentFiles->value());
    Settings::setValue("General", "promptsqltabsinnewproject", ui->checkPromptSQLTabsInNewProject->isChecked());

    m_proxyDialog->saveSettings();

    if(accept)
        PreferencesDialog::accept();

    QApplication::restoreOverrideCursor();
}

void PreferencesDialog::showColourDialog(QTreeWidgetItem* item, int column)
{
    QString text = item->text(column);
    if(!text.size() || text.at(0) != '#')
        return;

    QColor colour = QColorDialog::getColor(text, this);
    if(colour.isValid())
    {
        item->setForeground(column, colour);
        item->setBackground(column, colour);
        item->setText(column, colour.name());
    }
}

bool PreferencesDialog::eventFilter(QObject *obj, QEvent *event)
{
    // Use mouse click and enter press on the frames to pop up a colour dialog
    if (obj == ui->fr_bin_bg  || obj == ui->fr_bin_fg ||
        obj == ui->fr_reg_bg  || obj == ui->fr_reg_fg ||
        obj == ui->fr_formatted_bg || obj == ui->fr_formatted_fg ||
        obj == ui->fr_null_bg || obj == ui->fr_null_fg)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *key = static_cast<QKeyEvent *>(event);
            // Not interesting, so send to the parent (might be shortcuts)
            if((key->key() != Qt::Key_Enter) && (key->key() != Qt::Key_Return))
            {
                return QDialog::eventFilter(obj, event);
            }
        }
        else if (event->type() != QEvent::MouseButtonPress)
        {
            // Not a key event neither a mouse event, send to the parent
            return QDialog::eventFilter(obj, event);
        }

        QFrame *frame = qobject_cast<QFrame *>(obj);
        QColor oldColour = frame->palette().color(frame->backgroundRole());
        QColor colour = QColorDialog::getColor(oldColour, frame);

        if (colour.isValid())
        {
            setColorSetting(frame, colour);
        }
        // Consume
        return true;
    }

    // Send any other events to the parent
    return QDialog::eventFilter(obj, event);
}

void PreferencesDialog::addExtension()
{
    QString file = FileDialog::getOpenFileName(
                OpenExtensionFile,
                this,
                tr("Select extension file"),
                tr("Extensions(*.so *.dylib *.dll);;All files(*)"));

    if(QFile::exists(file))
        ui->listExtensions->addItem(file);
}

void PreferencesDialog::removeExtension()
{
    if(ui->listExtensions->currentIndex().isValid())
        ui->listExtensions->takeItem(ui->listExtensions->currentIndex().row());
}

void PreferencesDialog::listUpBuiltinExtension()
{
    const QDir dir(qApp->applicationDirPath() + "/../Extensions/");
    QStringList files = dir.entryList(QStringList() << "*.dylib", QDir::Files);
    for (const QString& file: files) {
        QListWidgetItem* item = new QListWidgetItem(file, ui->listBuiltinExtensions);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        // The check status is redetermined after the 'loadSettings()' function call.
        item->setCheckState(Qt::Unchecked);
        ui->listBuiltinExtensions->addItem(item);
    }
}

void PreferencesDialog::fillLanguageBox()
{
    QDir translationsDir(QCoreApplication::applicationDirPath() + "/translations",
                         "sqlb_*.qm");

    QLocale systemLocale = QLocale::system();

    // Add default language
    if (systemLocale.name() == "en_US")
    {
        ui->languageComboBox->addItem(QIcon(":/flags/en_US"),
                                      "English (United States) [System Language]",
                                      "en_US");
    }
    else
    {
        ui->languageComboBox->addItem(QIcon(":/flags/en_US"),
                                      "English (United States) [Default Language]",
                                      "en_US");
    }

    // Get available *.qm files from translation dir near executable as well as from resources
    QFileInfoList file_infos = translationsDir.entryInfoList();
    file_infos += QDir(":/translations").entryInfoList();
    for(const QFileInfo& file : qAsConst(file_infos))
    {
        QLocale locale(file.baseName().remove("sqlb_"));

        // Skip invalid locales
        if(locale.name() == "C")
            continue;

        // Skip translations that were already loaded
        if (ui->languageComboBox->findData(locale.name(), Qt::UserRole, Qt::MatchExactly) != -1)
            continue;

        QString language = QLocale::languageToString(locale.language()) + " (" +
                           QLocale::countryToString(locale.country()) + ")";

        if (locale == systemLocale)
            language += " [System language]";

        ui->languageComboBox->addItem(QIcon(":/flags/" + locale.name()), language, locale.name());
    }

    ui->languageComboBox->model()->sort(0);

    // Try to select the language for the stored locale
    int index = ui->languageComboBox->findData(Settings::getValue("General", "language"),
                                               Qt::UserRole, Qt::MatchExactly);

    // If there's no translation for the current locale, default to English
    if(index < 0)
        index = ui->languageComboBox->findData("en_US", Qt::UserRole, Qt::MatchExactly);

    QString chosenLanguage = ui->languageComboBox->itemText(index);
    QVariant chosenLocale = ui->languageComboBox->itemData(index);
    QIcon chosenIcon = ui->languageComboBox->itemIcon(index);

    // There's no "move" method, so we remove and add the chosen language again at the top
    ui->languageComboBox->removeItem(index);
    ui->languageComboBox->insertItem(0, chosenIcon, chosenLanguage, chosenLocale);
    ui->languageComboBox->setCurrentIndex(0);

    // This is a workaround needed for QDarkStyleSheet.
    // See https://github.com/ColinDuquesnoy/QDarkStyleSheet/issues/169
    QStyledItemDelegate* styledItemDelegate = new QStyledItemDelegate(ui->languageComboBox);
    ui->languageComboBox->setItemDelegate(styledItemDelegate);

}

void PreferencesDialog::loadColorSetting(QFrame *frame, const std::string& settingName)
{
    QColor color = QColor(Settings::getValue("databrowser", settingName + "_colour").toString());
    setColorSetting(frame, color);
}

void PreferencesDialog::setColorSetting(QFrame *frame, const QColor &color)
{
    QPalette::ColorRole role;
    QLineEdit *line;

    if (frame == ui->fr_bin_bg) {
        line = ui->txtBlob;
        role = line->backgroundRole();
    } else if (frame ==  ui->fr_bin_fg) {
        line = ui->txtBlob;
        role = line->foregroundRole();
    } else if (frame ==  ui->fr_reg_bg) {
        line = ui->txtRegular;
        role = line->backgroundRole();
    } else if (frame ==  ui->fr_reg_fg) {
        line = ui->txtRegular;
        role = line->foregroundRole();
    } else if (frame ==  ui->fr_formatted_bg) {
        line = ui->txtFormatted;
        role = line->backgroundRole();
    } else if (frame ==  ui->fr_formatted_fg) {
        line = ui->txtFormatted;
        role = line->foregroundRole();
    } else if (frame ==  ui->fr_null_bg) {
        line = ui->txtNull;
        role = line->backgroundRole();
    } else if (frame ==  ui->fr_null_fg) {
        line = ui->txtNull;
        role = line->foregroundRole();
    } else
        return;

    QPalette palette = frame->palette();
    palette.setColor(frame->backgroundRole(), color);
    frame->setPalette(palette);

    frame->setStyleSheet(QString(".QFrame {background-color: %2}").arg(color.name()));

    palette = line->palette();
    palette.setColor(role, color);
    line->setPalette(palette);

    line->setStyleSheet(QString(".QLineEdit {color: %1; background-color: %2}").arg(palette.color(line->foregroundRole()).name(),
                                                                       palette.color(line->backgroundRole()).name()));
}

void PreferencesDialog::saveColorSetting(QFrame* frame, const std::string& settingName)
{
    Settings::setValue("databrowser", settingName + "_colour",
        frame->palette().color(frame->backgroundRole()));
}

void PreferencesDialog::adjustColorsToStyle(int style)
{
    Settings::AppStyle appStyle = static_cast<Settings::AppStyle>(style);
    setColorSetting(ui->fr_null_fg, Settings::getDefaultColorValue("databrowser", "null_fg_colour", appStyle));
    setColorSetting(ui->fr_null_bg, Settings::getDefaultColorValue("databrowser", "null_bg_colour", appStyle));
    setColorSetting(ui->fr_bin_fg, Settings::getDefaultColorValue("databrowser", "bin_fg_colour", appStyle));
    setColorSetting(ui->fr_bin_bg, Settings::getDefaultColorValue("databrowser", "bin_bg_colour", appStyle));
    setColorSetting(ui->fr_reg_fg, Settings::getDefaultColorValue("databrowser", "reg_fg_colour", appStyle));
    setColorSetting(ui->fr_reg_bg, Settings::getDefaultColorValue("databrowser", "reg_bg_colour", appStyle));
    setColorSetting(ui->fr_formatted_fg, Settings::getDefaultColorValue("databrowser", "formatted_fg_colour", appStyle));
    setColorSetting(ui->fr_formatted_bg, Settings::getDefaultColorValue("databrowser", "formatted_bg_colour", appStyle));

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        std::string name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0).toStdString();
        QColor color = Settings::getDefaultColorValue("syntaxhighlighter", name + "_colour", appStyle);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setForeground(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setBackground(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setText(2, color.name());
    }
}

void PreferencesDialog::activateRemoteTab(bool active)
{
    ui->tabWidget->setTabEnabled(ui->tabWidget->indexOf(ui->tabRemote), active);
}

void PreferencesDialog::addClientCertificate()
{
    // Get certificate file to import and abort here if no file gets selected
    // NOTE: We assume here that this file contains both, certificate and private key!
    QString path = FileDialog::getOpenFileName(OpenCertificateFile, this, tr("Import certificate file"), "*.pem");
    if(path.isEmpty())
        return;

    // Open file and check if any certificates were imported
    auto certs = QSslCertificate::fromPath(path);
    if(certs.size() == 0)
    {
        QMessageBox::warning(this, qApp->applicationName(), tr("No certificates found in this file."));
        return;
    }

    // Add certificates to list
    for(int i=0;i<certs.size();i++)
        addClientCertToTable(path, certs.at(i));
}

void PreferencesDialog::removeClientCertificate()
{
    // Any row selected?
    int row = ui->tableClientCerts->currentRow();
    if(row == -1)
        return;

    // Double check
    if(QMessageBox::question(this, qApp->applicationName(), tr("Are you sure you want do remove this certificate? All certificate "
                                                               "data will be deleted from the application settings!"),
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        ui->tableClientCerts->removeRow(row);
    }
}

void PreferencesDialog::addClientCertToTable(const QString& path, const QSslCertificate& cert)
{
    // Do nothing if the file doesn't even exist
    if(!QFile::exists(path))
        return;

    // Add new row
    int row = ui->tableClientCerts->rowCount();
    ui->tableClientCerts->setRowCount(row + 1);

    // Fill row with data
    QTableWidgetItem* cert_file = new QTableWidgetItem(path);
    cert_file->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 0, cert_file);

    QTableWidgetItem* cert_subject_cn = new QTableWidgetItem(cert.subjectInfo(QSslCertificate::CommonName).at(0));
    cert_subject_cn->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 1, cert_subject_cn);

    QTableWidgetItem* cert_issuer_cn = new QTableWidgetItem(cert.issuerInfo(QSslCertificate::CommonName).at(0));
    cert_issuer_cn->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 2, cert_issuer_cn);

    QTableWidgetItem* cert_from = new QTableWidgetItem(cert.effectiveDate().toString());
    cert_from->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 3, cert_from);

    QTableWidgetItem* cert_to = new QTableWidgetItem(cert.expiryDate().toString());
    cert_to->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 4, cert_to);

    QTableWidgetItem* cert_serialno = new QTableWidgetItem(QString(cert.serialNumber()));
    cert_serialno->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->tableClientCerts->setItem(row, 5, cert_serialno);
}

void PreferencesDialog::chooseRemoteCloneDirectory()
{
    QString s = FileDialog::getExistingDirectory(
                NoSpecificType,
                this,
                tr("Choose a directory"),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!s.isEmpty() && QDir().mkpath(s))
        ui->editRemoteCloneDirectory->setText(s);
}

void PreferencesDialog::updatePreviewFont()
{
    if (ui->spinDataBrowserFontSize->value() != 0) {
        QFont textFont(ui->comboDataBrowserFont->currentText());
        textFont.setPointSize(ui->spinDataBrowserFontSize->value());
        ui->txtRegular->setFont(textFont);
        ui->txtFormatted->setFont(textFont);
        textFont.setItalic(true);
        ui->txtNull->setFont(textFont);
        ui->txtBlob->setFont(textFont);
    }
}

void PreferencesDialog::showFileExtensionManager()
{
    FileExtensionManager *manager = new FileExtensionManager(m_dbFileExtensions, this);

    if(manager->exec() == QDialog::Accepted)
    {
        m_dbFileExtensions = manager->getDBFileExtensions();
    }
}

void PreferencesDialog::buttonBoxClicked(QAbstractButton* button)
{
    if (button == ui->buttonBox->button(QDialogButtonBox::Cancel))
        reject();
    else if (button == ui->buttonBox->button(QDialogButtonBox::Save))
        saveSettings();
    else if (button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        if (QMessageBox::warning(this, QApplication::applicationName(), tr("Are you sure you want to clear all the saved settings?\nAll your preferences will be lost and default values will be used."),
                                 QMessageBox::RestoreDefaults | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::RestoreDefaults)
        {
            Settings::restoreDefaults();
            accept();
        }
    }
}

void PreferencesDialog::configureProxy()
{
    m_proxyDialog->show();
}

void PreferencesDialog::exportSettings()
{
    saveSettings(false);

    const QString fileName = FileDialog::getSaveFileName(CreateSettingsFile, this, tr("Save Settings File"), tr("Initialization File (*.ini)"));
    if(!fileName.isEmpty())
    {
        Settings::exportSettings(fileName);
        QMessageBox::information(this, QApplication::applicationName(), (tr("The settings file has been saved in location :\n") + fileName));
    }
}

void PreferencesDialog::importSettings()
{
    const QString fileName = FileDialog::getOpenFileName(OpenSettingsFile, this, tr("Open Settings File"), tr("Initialization File (*.ini)"));
    const QVariant existingLanguage = Settings::getValue("General", "language");

    if(!fileName.isEmpty())
    {
        if(Settings::importSettings(fileName))
        {
            QMessageBox::information(this, QApplication::applicationName(), tr("The settings file was loaded properly."));
            if (existingLanguage != Settings::getValue("General", "language"))
                QMessageBox::information(this, QApplication::applicationName(),
                                         tr("The language will change after you restart the application."));

            accept();
        } else {
            QMessageBox::critical(this, QApplication::applicationName(), tr("The selected settings file is not a normal settings file.\nPlease check again."));
        }
    }
}
