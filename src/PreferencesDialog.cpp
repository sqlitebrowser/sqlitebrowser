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
      m_dbFileExtensions(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_DB_FILE_EXTS).toString().split(";;"))
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
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_ENCODING).toString(), Qt::MatchFixedString));
    ui->comboDefaultLocation->setCurrentIndex(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_SAVE_DEFAULT_LOCATION).toInt());
    ui->locationEdit->setText(QDir::toNativeSeparators(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_LOCATION).toString()));
    ui->checkPromptSQLTabsInNewProject->setChecked(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_PROMPT_SQL_TABS_IN_NEWPRJ).toBool());
    ui->checkUpdates->setChecked(Settings::getValue(szINI::SEC_CHCK_VERSION, szINI::KEY_ENABLED).toBool());

    ui->checkHideSchemaLinebreaks->setChecked(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_HIDE_SCHEMA_LINEBREAKS).toBool());
    ui->foreignKeysCheckBox->setChecked(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_FOREIGN_KEYS).toBool());
    ui->spinPrefetchSize->setValue(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_PREFETCH_SIZE).toInt());
    ui->editDatabaseDefaultSqlText->setText(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_SQL_TEXT).toString());

    ui->defaultFieldTypeComboBox->addItems(DBBrowserDB::Datatypes);

    int defaultFieldTypeIndex = Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_FIELD_TYPE).toInt();
    if (defaultFieldTypeIndex < DBBrowserDB::Datatypes.count())
    {
        ui->defaultFieldTypeComboBox->setCurrentIndex(defaultFieldTypeIndex);
    }

    ui->spinStructureFontSize->setValue(Settings::getValue(szINI::SEC_DATABASE, szINI::KEY_FONTSIZE).toInt());

    // Gracefully handle the preferred Data Browser font not being available
    int matchingFont = ui->comboDataBrowserFont->findText(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FONT).toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getDefaultValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FONT).toString());
    ui->comboDataBrowserFont->setCurrentIndex(matchingFont);

    ui->spinDataBrowserFontSize->setValue(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FONTSIZE).toInt());
    loadColorSetting(ui->fr_null_fg, szINI::KEY_SHP_NULL_FG);
    loadColorSetting(ui->fr_null_bg, szINI::KEY_SHP_NULL_BG);
    loadColorSetting(ui->fr_bin_fg, szINI::KEY_SHP_BIN_FG);
    loadColorSetting(ui->fr_bin_bg, szINI::KEY_SHP_BIN_BG);
    loadColorSetting(ui->fr_reg_fg, szINI::KEY_SHP_REG_FG);
    loadColorSetting(ui->fr_reg_bg, szINI::KEY_SHP_REG_BG);
    loadColorSetting(ui->fr_formatted_fg, szINI::KEY_SHP_FMT_FG);
    loadColorSetting(ui->fr_formatted_bg, szINI::KEY_SHP_FMT_BG);

    ui->spinSymbolLimit->setValue(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_SYMBOL_LIMIT).toInt());
    ui->spinCompleteThreshold->setValue(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_COMPLETE_THRESHOLD).toInt());
    ui->checkShowImagesInline->setChecked(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_IMAGE_PREVIEW).toBool());
    ui->txtNull->setText(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_NULL_TEXT).toString());
    ui->txtBlob->setText(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_BLOB_TEXT).toString());
    ui->editFilterEscape->setText(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FILTER_ESCAPE).toString());
    ui->spinFilterDelay->setValue(Settings::getValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FILTER_DELAY).toInt());

    ui->treeSyntaxHighlighting->resizeColumnToContents(1);

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        std::string name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0).toStdString();
        QString colorname = Settings::getValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_COLOUR).toString();
        QColor color = QColor(colorname);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setForeground(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setBackground(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setText(2, colorname);

        // Add font properties except for colour-only entries
        if (name != "null" && name != "currentline" &&
            name != "background" && name != "foreground" && name != "highlight" &&
            name != "selected_fg" && name != "selected_bg") {
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(3, Settings::getValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_BOLD).toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(4, Settings::getValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_ITALIC).toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(5, Settings::getValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_UNDERLINE).toBool() ? Qt::Checked : Qt::Unchecked);
        }
    }

    // Remote settings
    ui->checkUseRemotes->setChecked(Settings::getValue(szINI::SEC_REMOTE, szINI::KEY_ACTIVE).toBool());
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
        const QStringList client_certs = Settings::getValue(szINI::SEC_REMOTE, szINI::KEY_CLIENT_CERT).toStringList();
        for(const QString& file : client_certs)
        {
            const auto certs = QSslCertificate::fromPath(file);
            for(const QSslCertificate& cert : certs)
                addClientCertToTable(file, cert);
        }
    }
    ui->editRemoteCloneDirectory->setText(QDir::toNativeSeparators(Settings::getValue(szINI::SEC_REMOTE, szINI::KEY_CLONE_DIR).toString()));

    // Gracefully handle the preferred Editor font not being available
    matchingFont = ui->comboEditorFont->findText(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_FONT).toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getDefaultValue(szINI::SEC_EDITOR, szINI::KEY_FONT).toString());
    ui->comboEditorFont->setCurrentIndex(matchingFont);

    ui->spinEditorFontSize->setValue(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_FONTSIZE).toInt());
    ui->spinTabSize->setValue(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_TABSIZE).toInt());
    ui->checkIndentationUseTabs->setChecked(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_INDENTATION_USE_TABS).toBool());
    ui->spinLogFontSize->setValue(Settings::getValue(szINI::SEC_LOG, szINI::KEY_FONTSIZE).toInt());
    ui->wrapComboBox->setCurrentIndex(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_WRAP_LINES).toInt());
    ui->quoteComboBox->setCurrentIndex(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_IDENTIFIER_QUOTES).toInt());
    ui->checkAutoCompletion->setChecked(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_AUTO_COMPLETION).toBool());
    ui->checkCompleteUpper->setEnabled(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_AUTO_COMPLETION).toBool());
    ui->checkCompleteUpper->setChecked(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_UPPER_KEYWORDS).toBool());
    ui->checkErrorIndicators->setChecked(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_ERROR_INDICATORS).toBool());
    ui->checkHorizontalTiling->setChecked(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_HORZ_TILING).toBool());
    ui->checkCloseButtonOnTabs->setChecked(Settings::getValue(szINI::SEC_EDITOR, szINI::KEY_CLOSE_BUTTON_ON_TABS).toBool());

    ui->listExtensions->addItems(Settings::getValue(szINI::SEC_EXTENSIONS, szINI::KEY_LIST).toStringList());
    ui->checkRegexDisabled->setChecked(Settings::getValue(szINI::SEC_EXTENSIONS, szINI::KEY_DISABLE_REGEX).toBool());
    ui->checkAllowLoadExtension->setChecked(Settings::getValue(szINI::SEC_EXTENSIONS, szINI::KEY_ENABLE_LOAD_EXTENSION).toBool());
    fillLanguageBox();
    ui->appStyleCombo->setCurrentIndex(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_APPSTYLE).toInt());
    ui->toolbarStyleComboMain->setCurrentIndex(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_APP).toInt());
    ui->toolbarStyleComboStructure->setCurrentIndex(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_STRUCTURE).toInt());
    ui->toolbarStyleComboBrowse->setCurrentIndex(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_BROWSE).toInt());
    ui->toolbarStyleComboSql->setCurrentIndex(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_SQL).toInt());
    ui->toolbarStyleComboEditCell->setCurrentIndex(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_EDIT_CELL).toInt());
    ui->spinGeneralFontSize->setValue(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_FONTSIZE).toInt());
    ui->spinMaxRecentFiles->setValue(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_MAX_RECENT_FILES).toInt());
}

void PreferencesDialog::saveSettings(bool accept)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_ENCODING, ui->encodingComboBox->currentText());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_LOCATION, ui->locationEdit->text());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_SAVE_DEFAULT_LOCATION, ui->comboDefaultLocation->currentIndex());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_HIDE_SCHEMA_LINEBREAKS, ui->checkHideSchemaLinebreaks->isChecked());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_FOREIGN_KEYS, ui->foreignKeysCheckBox->isChecked());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_PREFETCH_SIZE, ui->spinPrefetchSize->value());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_SQL_TEXT, ui->editDatabaseDefaultSqlText->text());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_DEFAULT_FIELD_TYPE, ui->defaultFieldTypeComboBox->currentIndex());
    Settings::setValue(szINI::SEC_DATABASE, szINI::KEY_FONTSIZE, ui->spinStructureFontSize->value());

    Settings::setValue(szINI::SEC_CHCK_VERSION, szINI::KEY_ENABLED, ui->checkUpdates->isChecked());

    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FONT, ui->comboDataBrowserFont->currentText());
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FONTSIZE, ui->spinDataBrowserFontSize->value());
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_IMAGE_PREVIEW, ui->checkShowImagesInline->isChecked());
    saveColorSetting(ui->fr_null_fg, szINI::KEY_SHP_NULL_FG);
    saveColorSetting(ui->fr_null_bg, szINI::KEY_SHP_NULL_BG);
    saveColorSetting(ui->fr_reg_fg, szINI::KEY_SHP_REG_FG);
    saveColorSetting(ui->fr_reg_bg, szINI::KEY_SHP_REG_BG);
    saveColorSetting(ui->fr_formatted_fg, szINI::KEY_SHP_FMT_FG);
    saveColorSetting(ui->fr_formatted_bg, szINI::KEY_SHP_FMT_BG);
    saveColorSetting(ui->fr_bin_fg, szINI::KEY_SHP_BIN_FG);
    saveColorSetting(ui->fr_bin_bg, szINI::KEY_SHP_BIN_BG);
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_SYMBOL_LIMIT, ui->spinSymbolLimit->value());
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_COMPLETE_THRESHOLD, ui->spinCompleteThreshold->value());
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_NULL_TEXT, ui->txtNull->text());
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_BLOB_TEXT, ui->txtBlob->text());
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FILTER_ESCAPE, ui->editFilterEscape->text());
    Settings::setValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FILTER_DELAY, ui->spinFilterDelay->value());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        std::string name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0).toStdString();
        Settings::setValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_COLOUR, ui->treeSyntaxHighlighting->topLevelItem(i)->text(2));
        Settings::setValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_BOLD, ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(3) == Qt::Checked);
        Settings::setValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_ITALIC, ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(4) == Qt::Checked);
        Settings::setValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_UNDERLINE, ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(5) == Qt::Checked);
    }
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_FONT, ui->comboEditorFont->currentText());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_FONTSIZE, ui->spinEditorFontSize->value());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_TABSIZE, ui->spinTabSize->value());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_INDENTATION_USE_TABS, ui->checkIndentationUseTabs->isChecked());
    Settings::setValue(szINI::SEC_LOG, szINI::KEY_FONTSIZE, ui->spinLogFontSize->value());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_WRAP_LINES, ui->wrapComboBox->currentIndex());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_IDENTIFIER_QUOTES, ui->quoteComboBox->currentIndex());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_AUTO_COMPLETION, ui->checkAutoCompletion->isChecked());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_UPPER_KEYWORDS, ui->checkCompleteUpper->isChecked());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_ERROR_INDICATORS, ui->checkErrorIndicators->isChecked());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_HORZ_TILING, ui->checkHorizontalTiling->isChecked());
    Settings::setValue(szINI::SEC_EDITOR, szINI::KEY_CLOSE_BUTTON_ON_TABS, ui->checkCloseButtonOnTabs->isChecked());

    QStringList extList;
    for(int i=0;i<ui->listExtensions->count();++i)
        extList.append(ui->listExtensions->item(i)->text());
    Settings::setValue(szINI::SEC_EXTENSIONS, szINI::KEY_LIST, extList);
    Settings::setValue(szINI::SEC_EXTENSIONS, szINI::KEY_DISABLE_REGEX, ui->checkRegexDisabled->isChecked());
    Settings::setValue(szINI::SEC_EXTENSIONS, szINI::KEY_ENABLE_LOAD_EXTENSION, ui->checkAllowLoadExtension->isChecked());

    // Save remote settings
    Settings::setValue(szINI::SEC_REMOTE, szINI::KEY_ACTIVE, ui->checkUseRemotes->isChecked());
    QStringList old_client_certs = Settings::getValue(szINI::SEC_REMOTE, szINI::KEY_CLIENT_CERT).toStringList();
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
    Settings::setValue(szINI::SEC_REMOTE, szINI::KEY_CLIENT_CERT, new_client_certs);
    Settings::setValue(szINI::SEC_REMOTE, szINI::KEY_CLONE_DIR, ui->editRemoteCloneDirectory->text());

    // Warn about restarting to change language
    QVariant newLanguage = ui->languageComboBox->itemData(ui->languageComboBox->currentIndex());
    if (newLanguage != Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_LANGUAGE))
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("The language will change after you restart the application."));

    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_LANGUAGE, newLanguage);
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_APPSTYLE, ui->appStyleCombo->currentIndex());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_APP, ui->toolbarStyleComboMain->currentIndex());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_STRUCTURE, ui->toolbarStyleComboStructure->currentIndex());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_BROWSE, ui->toolbarStyleComboBrowse->currentIndex());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_SQL, ui->toolbarStyleComboSql->currentIndex());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_TB_STYLE_EDIT_CELL, ui->toolbarStyleComboEditCell->currentIndex());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_DB_FILE_EXTS, m_dbFileExtensions.join(";;") );
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_FONTSIZE, ui->spinGeneralFontSize->value());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_MAX_RECENT_FILES, ui->spinMaxRecentFiles->value());
    Settings::setValue(szINI::SEC_GENERAL, szINI::KEY_PROMPT_SQL_TABS_IN_NEWPRJ, ui->checkPromptSQLTabsInNewProject->isChecked());

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
    int index = ui->languageComboBox->findData(Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_LANGUAGE),
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
    QColor color = QColor(Settings::getValue(szINI::SEC_DATA_BROWSER, settingName + szINI::KEY_ANY_COLOUR).toString());
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
    Settings::setValue(szINI::SEC_DATA_BROWSER, settingName + szINI::KEY_ANY_COLOUR,
        frame->palette().color(frame->backgroundRole()));
}

void PreferencesDialog::adjustColorsToStyle(int style)
{
    Settings::AppStyle appStyle = static_cast<Settings::AppStyle>(style);
    setColorSetting(ui->fr_null_fg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_NULL_FG_COLOUR, appStyle));
    setColorSetting(ui->fr_null_bg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_NULL_BG_COLOUR, appStyle));
    setColorSetting(ui->fr_bin_fg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_BIN_FG_COLOUR, appStyle));
    setColorSetting(ui->fr_bin_bg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_BIN_BG_COLOUR, appStyle));
    setColorSetting(ui->fr_reg_fg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_REG_FG_COLOUR, appStyle));
    setColorSetting(ui->fr_reg_bg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_REG_BG_COLOUR, appStyle));
    setColorSetting(ui->fr_formatted_fg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FMT_FG_COLOUR, appStyle));
    setColorSetting(ui->fr_formatted_bg, Settings::getDefaultColorValue(szINI::SEC_DATA_BROWSER, szINI::KEY_FMT_BG_COLOUR, appStyle));

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        std::string name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0).toStdString();
        QColor color = Settings::getDefaultColorValue(szINI::SEC_SYNTAX_HIGHLIGHTER, name + szINI::KEY_ANY_COLOUR, appStyle);
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
    const QVariant existingLanguage = Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_LANGUAGE);

    if(!fileName.isEmpty())
    {
        if(Settings::importSettings(fileName))
        {
            QMessageBox::information(this, QApplication::applicationName(), tr("The settings file was loaded properly."));
            if (existingLanguage != Settings::getValue(szINI::SEC_GENERAL, szINI::KEY_LANGUAGE))
                QMessageBox::information(this, QApplication::applicationName(),
                                         tr("The language will change after you restart the application."));

            accept();
        } else {
            QMessageBox::critical(this, QApplication::applicationName(), tr("The selected settings file is not a normal settings file.\nPlease check again."));
        }
    }
}
