#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "sqlitedb.h"
#include "FileDialog.h"
#include "Settings.h"

#include <QDir>
#include <QSettings>
#include <QColorDialog>
#include <QMessageBox>
#include <QKeyEvent>

PreferencesDialog::PreferencesDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    ui->treeSyntaxHighlighting->setColumnHidden(0, true);
    ui->labelDatabaseDefaultSqlText->setVisible(false);
    ui->editDatabaseDefaultSqlText->setVisible(false);

    ui->fr_bin_bg->installEventFilter(this);
    ui->fr_bin_fg->installEventFilter(this);
    ui->fr_reg_bg->installEventFilter(this);
    ui->fr_reg_fg->installEventFilter(this);
    ui->fr_null_bg->installEventFilter(this);
    ui->fr_null_fg->installEventFilter(this);

#ifndef CHECKNEWVERSION
    ui->labelUpdates->setVisible(false);
    ui->checkUpdates->setVisible(false);
#endif

    loadSettings();
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
                this,
                tr("Choose a directory"),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(!s.isEmpty())
        ui->locationEdit->setText(s);
}

void PreferencesDialog::loadSettings()
{
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(Settings::getSettingsValue("db", "defaultencoding").toString(), Qt::MatchFixedString));
    ui->comboDefaultLocation->setCurrentIndex(Settings::getSettingsValue("db", "savedefaultlocation").toInt());
    ui->locationEdit->setText(Settings::getSettingsValue("db", "defaultlocation").toString());
    ui->checkUpdates->setChecked(Settings::getSettingsValue("checkversion", "enabled").toBool());
    ui->checkHideSchemaLinebreaks->setChecked(Settings::getSettingsValue("db", "hideschemalinebreaks").toBool());
    ui->foreignKeysCheckBox->setChecked(Settings::getSettingsValue("db", "foreignkeys").toBool());
    ui->spinPrefetchSize->setValue(Settings::getSettingsValue("db", "prefetchsize").toInt());
    ui->editDatabaseDefaultSqlText->setText(Settings::getSettingsValue("db", "defaultsqltext").toString());

    ui->defaultFieldTypeComboBox->addItems(sqlb::Field::Datatypes);

    int defaultFieldTypeIndex = Settings::getSettingsValue("db", "defaultfieldtype").toInt();

    if (defaultFieldTypeIndex < sqlb::Field::Datatypes.count())
    {
        ui->defaultFieldTypeComboBox->setCurrentIndex(defaultFieldTypeIndex);
    }

    // Gracefully handle the preferred Data Browser font not being available
    int matchingFont = ui->comboDataBrowserFont->findText(Settings::getSettingsValue("databrowser", "font").toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getSettingsDefaultValue("databrowser", "font").toString());
    ui->comboDataBrowserFont->setCurrentIndex(matchingFont);

    ui->spinDataBrowserFontSize->setValue(Settings::getSettingsValue("databrowser", "fontsize").toInt());
    loadColorSetting(ui->fr_null_fg, "null_fg");
    loadColorSetting(ui->fr_null_bg, "null_bg");
    loadColorSetting(ui->fr_reg_fg, "reg_fg");
    loadColorSetting(ui->fr_reg_bg, "reg_bg");
    loadColorSetting(ui->fr_bin_fg, "bin_fg");
    loadColorSetting(ui->fr_bin_bg, "bin_bg");

    ui->spinSymbolLimit->setValue(Settings::getSettingsValue("databrowser", "symbol_limit").toInt());
    ui->txtNull->setText(Settings::getSettingsValue("databrowser", "null_text").toString());
    ui->editFilterEscape->setText(Settings::getSettingsValue("databrowser", "filter_escape").toString());
    ui->spinFilterDelay->setValue(Settings::getSettingsValue("databrowser", "filter_delay").toInt());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        QString name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0);
        QString colorname = Settings::getSettingsValue("syntaxhighlighter", name + "_colour").toString();
        QColor color = QColor(colorname);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setTextColor(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setBackgroundColor(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setText(2, colorname);
        if (name != "null") {
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(3, Settings::getSettingsValue("syntaxhighlighter", name + "_bold").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(4, Settings::getSettingsValue("syntaxhighlighter", name + "_italic").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(5, Settings::getSettingsValue("syntaxhighlighter", name + "_underline").toBool() ? Qt::Checked : Qt::Unchecked);
        }
    }

    // Gracefully handle the preferred Editor font not being available
    matchingFont = ui->comboEditorFont->findText(Settings::getSettingsValue("editor", "font").toString(), Qt::MatchExactly);
    if (matchingFont == -1)
        matchingFont = ui->comboDataBrowserFont->findText(Settings::getSettingsDefaultValue("editor", "font").toString());
    ui->comboEditorFont->setCurrentIndex(matchingFont);

    ui->spinEditorFontSize->setValue(Settings::getSettingsValue("editor", "fontsize").toInt());
    ui->spinTabSize->setValue(Settings::getSettingsValue("editor", "tabsize").toInt());
    ui->spinLogFontSize->setValue(Settings::getSettingsValue("log", "fontsize").toInt());
    ui->checkAutoCompletion->setChecked(Settings::getSettingsValue("editor", "auto_completion").toBool());
    ui->checkErrorIndicators->setChecked(Settings::getSettingsValue("editor", "error_indicators").toBool());
    ui->checkHorizontalTiling->setChecked(Settings::getSettingsValue("editor", "horizontal_tiling").toBool());

    ui->listExtensions->addItems(Settings::getSettingsValue("extensions", "list").toStringList());
    ui->checkRegexDisabled->setChecked(Settings::getSettingsValue("extensions", "disableregex").toBool());
    fillLanguageBox();
}

void PreferencesDialog::saveSettings()
{
    Settings::setSettingsValue("db", "defaultencoding", ui->encodingComboBox->currentText());
    Settings::setSettingsValue("db", "defaultlocation", ui->locationEdit->text());
    Settings::setSettingsValue("db", "savedefaultlocation", ui->comboDefaultLocation->currentIndex());
    Settings::setSettingsValue("db", "hideschemalinebreaks", ui->checkHideSchemaLinebreaks->isChecked());
    Settings::setSettingsValue("db", "foreignkeys", ui->foreignKeysCheckBox->isChecked());
    Settings::setSettingsValue("db", "prefetchsize", ui->spinPrefetchSize->value());
    Settings::setSettingsValue("db", "defaultsqltext", ui->editDatabaseDefaultSqlText->text());

    Settings::setSettingsValue("db", "defaultfieldtype", ui->defaultFieldTypeComboBox->currentIndex());

    Settings::setSettingsValue("checkversion", "enabled", ui->checkUpdates->isChecked());

    Settings::setSettingsValue("databrowser", "font", ui->comboDataBrowserFont->currentText());
    Settings::setSettingsValue("databrowser", "fontsize", ui->spinDataBrowserFontSize->value());
    saveColorSetting(ui->fr_null_fg, "null_fg");
    saveColorSetting(ui->fr_null_bg, "null_bg");
    saveColorSetting(ui->fr_reg_fg, "reg_fg");
    saveColorSetting(ui->fr_reg_bg, "reg_bg");
    saveColorSetting(ui->fr_bin_fg, "bin_fg");
    saveColorSetting(ui->fr_bin_bg, "bin_bg");
    Settings::setSettingsValue("databrowser", "symbol_limit", ui->spinSymbolLimit->value());
    Settings::setSettingsValue("databrowser", "null_text", ui->txtNull->text());
    Settings::setSettingsValue("databrowser", "filter_escape", ui->editFilterEscape->text());
    Settings::setSettingsValue("databrowser", "filter_delay", ui->spinFilterDelay->value());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        QString name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0);
        Settings::setSettingsValue("syntaxhighlighter", name + "_colour", ui->treeSyntaxHighlighting->topLevelItem(i)->text(2));
        Settings::setSettingsValue("syntaxhighlighter", name + "_bold", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(3) == Qt::Checked);
        Settings::setSettingsValue("syntaxhighlighter", name + "_italic", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(4) == Qt::Checked);
        Settings::setSettingsValue("syntaxhighlighter", name + "_underline", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(5) == Qt::Checked);
    }
    Settings::setSettingsValue("editor", "font", ui->comboEditorFont->currentText());
    Settings::setSettingsValue("editor", "fontsize", ui->spinEditorFontSize->value());
    Settings::setSettingsValue("editor", "tabsize", ui->spinTabSize->value());
    Settings::setSettingsValue("log", "fontsize", ui->spinLogFontSize->value());
    Settings::setSettingsValue("editor", "auto_completion", ui->checkAutoCompletion->isChecked());
    Settings::setSettingsValue("editor", "error_indicators", ui->checkErrorIndicators->isChecked());
    Settings::setSettingsValue("editor", "horizontal_tiling", ui->checkHorizontalTiling->isChecked());

    QStringList extList;
    foreach(QListWidgetItem* item, ui->listExtensions->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard))
        extList.append(item->text());
    Settings::setSettingsValue("extensions", "list", extList);
    Settings::setSettingsValue("extensions", "disableregex", ui->checkRegexDisabled->isChecked());

    // Warn about restarting to change language
    QVariant newLanguage = ui->languageComboBox->itemData(ui->languageComboBox->currentIndex());
    if (newLanguage != Settings::getSettingsValue("General", "language"))
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("The language will change after you restart the application."));

    Settings::setSettingsValue("General", "language", newLanguage);

    accept();
}

void PreferencesDialog::showColourDialog(QTreeWidgetItem* item, int column)
{
    if(item->text(column).left(1) != "#")
        return;

    QColor colour = QColorDialog::getColor(QColor(item->text(column)), this);
    if(colour.isValid())
    {
        item->setTextColor(column, colour);
        item->setBackgroundColor(column, colour);
        item->setText(column, colour.name());
    }
}

bool PreferencesDialog::eventFilter(QObject *obj, QEvent *event)
{
    // Use mouse click and enter press on the frames to pop up a colour dialog
    if (obj == ui->fr_bin_bg  || obj == ui->fr_bin_fg ||
        obj == ui->fr_reg_bg  || obj == ui->fr_reg_fg ||
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
            QPalette palette = frame->palette();
            palette.setColor(frame->backgroundRole(), colour);
            frame->setPalette(palette);
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
                this,
                tr("Select extension file"),
                tr("Extensions(*.so *.dll);;All files(*)"));

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
    foreach(const QFileInfo &file, file_infos)
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
    int index = ui->languageComboBox->findData(Settings::getSettingsValue("General", "language"),
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
}

void PreferencesDialog::loadColorSetting(QFrame *frame, const QString & settingName)
{
    QPalette palette = frame->palette();
    palette.setColor(frame->backgroundRole(),
        QColor(Settings::getSettingsValue("databrowser", settingName + "_colour").toString()));
    frame->setPalette(palette);
}

void PreferencesDialog::saveColorSetting(QFrame *frame, const QString & settingName)
{
    Settings::setSettingsValue("databrowser", settingName + "_colour",
        frame->palette().color(frame->backgroundRole()));
}
