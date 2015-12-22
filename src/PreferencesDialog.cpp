#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
#include "sqlitedb.h"
#include "FileDialog.h"

#include <QDir>
#include <QSettings>
#include <QColorDialog>
#include <QMessageBox>
#include <QKeyEvent>

QHash<QString, QVariant> PreferencesDialog::m_hCache;

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
    ui->encodingComboBox->setCurrentIndex(ui->encodingComboBox->findText(getSettingsValue("db", "defaultencoding").toString(), Qt::MatchFixedString));
    ui->comboDefaultLocation->setCurrentIndex(getSettingsValue("db", "savedefaultlocation").toInt());
    ui->locationEdit->setText(getSettingsValue("db", "defaultlocation").toString());
    ui->checkUpdates->setChecked(getSettingsValue("checkversion", "enabled").toBool());
    ui->checkHideSchemaLinebreaks->setChecked(getSettingsValue("db", "hideschemalinebreaks").toBool());
    ui->foreignKeysCheckBox->setChecked(getSettingsValue("db", "foreignkeys").toBool());
    ui->spinPrefetchSize->setValue(getSettingsValue("db", "prefetchsize").toInt());
    ui->editDatabaseDefaultSqlText->setText(getSettingsValue("db", "defaultsqltext").toString());

    ui->comboDataBrowserFont->setCurrentIndex(ui->comboEditorFont->findText(getSettingsValue("databrowser", "font").toString()));
    ui->spinDataBrowserFontSize->setValue(getSettingsValue("databrowser", "fontsize").toInt());
    loadColorSetting(ui->fr_null_fg, "null_fg");
    loadColorSetting(ui->fr_null_bg, "null_bg");
    loadColorSetting(ui->fr_reg_fg, "reg_fg");
    loadColorSetting(ui->fr_reg_bg, "reg_bg");
    loadColorSetting(ui->fr_bin_fg, "bin_fg");
    loadColorSetting(ui->fr_bin_bg, "bin_bg");

    ui->txtNull->setText(getSettingsValue("databrowser", "null_text").toString());
    ui->editFilterEscape->setText(getSettingsValue("databrowser", "filter_escape").toString());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        QString name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0);
        QString colorname = getSettingsValue("syntaxhighlighter", name + "_colour").toString();
        QColor color = QColor(colorname);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setTextColor(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setBackgroundColor(2, color);
        ui->treeSyntaxHighlighting->topLevelItem(i)->setText(2, colorname);
        if (name != "null") {
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(3, getSettingsValue("syntaxhighlighter", name + "_bold").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(4, getSettingsValue("syntaxhighlighter", name + "_italic").toBool() ? Qt::Checked : Qt::Unchecked);
            ui->treeSyntaxHighlighting->topLevelItem(i)->setCheckState(5, getSettingsValue("syntaxhighlighter", name + "_underline").toBool() ? Qt::Checked : Qt::Unchecked);
        }
    }
    ui->comboEditorFont->setCurrentIndex(ui->comboEditorFont->findText(getSettingsValue("editor", "font").toString()));
    ui->spinEditorFontSize->setValue(getSettingsValue("editor", "fontsize").toInt());
    ui->spinTabSize->setValue(getSettingsValue("editor", "tabsize").toInt());
    ui->spinLogFontSize->setValue(getSettingsValue("log", "fontsize").toInt());
    ui->checkErrorIndicators->setChecked(getSettingsValue("editor", "error_indicators").toBool());
    ui->checkHorizontalTiling->setChecked(getSettingsValue("editor", "horizontal_tiling").toBool());

    ui->listExtensions->addItems(getSettingsValue("extensions", "list").toStringList());
    ui->checkRegexDisabled->setChecked(getSettingsValue("extensions", "disableregex").toBool());
    fillLanguageBox();
}

void PreferencesDialog::saveSettings()
{
    setSettingsValue("db", "defaultencoding", ui->encodingComboBox->currentText());
    setSettingsValue("db", "defaultlocation", ui->locationEdit->text());
    setSettingsValue("db", "savedefaultlocation", ui->comboDefaultLocation->currentIndex());
    setSettingsValue("db", "hideschemalinebreaks", ui->checkHideSchemaLinebreaks->isChecked());
    setSettingsValue("db", "foreignkeys", ui->foreignKeysCheckBox->isChecked());
    setSettingsValue("db", "prefetchsize", ui->spinPrefetchSize->value());
    setSettingsValue("db", "defaultsqltext", ui->editDatabaseDefaultSqlText->text());

    setSettingsValue("checkversion", "enabled", ui->checkUpdates->isChecked());

    setSettingsValue("databrowser", "font", ui->comboDataBrowserFont->currentText());
    setSettingsValue("databrowser", "fontsize", ui->spinDataBrowserFontSize->value());
    saveColorSetting(ui->fr_null_fg, "null_fg");
    saveColorSetting(ui->fr_null_bg, "null_bg");
    saveColorSetting(ui->fr_reg_fg, "reg_fg");
    saveColorSetting(ui->fr_reg_bg, "reg_bg");
    saveColorSetting(ui->fr_bin_fg, "bin_fg");
    saveColorSetting(ui->fr_bin_bg, "bin_bg");
    setSettingsValue("databrowser", "null_text", ui->txtNull->text());
    setSettingsValue("databrowser", "filter_escape", ui->editFilterEscape->text());

    for(int i=0; i < ui->treeSyntaxHighlighting->topLevelItemCount(); ++i)
    {
        QString name = ui->treeSyntaxHighlighting->topLevelItem(i)->text(0);
        setSettingsValue("syntaxhighlighter", name + "_colour", ui->treeSyntaxHighlighting->topLevelItem(i)->text(2));
        setSettingsValue("syntaxhighlighter", name + "_bold", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(3) == Qt::Checked);
        setSettingsValue("syntaxhighlighter", name + "_italic", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(4) == Qt::Checked);
        setSettingsValue("syntaxhighlighter", name + "_underline", ui->treeSyntaxHighlighting->topLevelItem(i)->checkState(5) == Qt::Checked);
    }
    setSettingsValue("editor", "font", ui->comboEditorFont->currentText());
    setSettingsValue("editor", "fontsize", ui->spinEditorFontSize->value());
    setSettingsValue("editor", "tabsize", ui->spinTabSize->value());
    setSettingsValue("log", "fontsize", ui->spinLogFontSize->value());
    setSettingsValue("editor", "error_indicators", ui->checkErrorIndicators->isChecked());
    setSettingsValue("editor", "horizontal_tiling", ui->checkHorizontalTiling->isChecked());

    QStringList extList;
    foreach(QListWidgetItem* item, ui->listExtensions->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard))
        extList.append(item->text());
    setSettingsValue("extensions", "list", extList);
    setSettingsValue("extensions", "disableregex", ui->checkRegexDisabled->isChecked());

    // Warn about restarting to change language
    QVariant newLanguage = ui->languageComboBox->itemData(ui->languageComboBox->currentIndex());
    if (newLanguage != getSettingsValue("General", "language"))
        QMessageBox::information(this, QApplication::applicationName(),
                                 tr("The language will change after you restart the application."));

    setSettingsValue("General", "language", newLanguage);

    accept();
}

QVariant PreferencesDialog::getSettingsValue(const QString& group, const QString& name)
{
    // Have a look in the cache first
    QHash<QString, QVariant>::iterator cacheIndex = m_hCache.find(group + name);
    if(cacheIndex != m_hCache.end())
    {
        return cacheIndex.value();
    } else {
        // Nothing found in the cache, so get the value from the settings file or get the default value if there is no value set yet
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        QVariant value = settings.value(group + "/" + name, getSettingsDefaultValue(group, name));

        // Store this value in the cache for further usage and return it afterwards
        m_hCache.insert(group + name, value);
        return value;
    }
}

void PreferencesDialog::setSettingsValue(const QString& group, const QString& name, const QVariant& value, bool dont_save_to_disk)
{
    // Sometime the value has to be saved for the current session only but get discarded when the application exits.
    // In order to achieve this this flag can be set which disables the save to disk mechanism and only leaves the save to cache part active.
    if(dont_save_to_disk == false)
    {
        // Set the group and save the given value
        QSettings settings(QApplication::organizationName(), QApplication::organizationName());
        settings.beginGroup(group);
        settings.setValue(name, value);
        settings.endGroup();
    }

    // Also change it in the cache
    m_hCache[group + name] = value;
}

QVariant PreferencesDialog::getSettingsDefaultValue(const QString& group, const QString& name)
{
    // db/defaultencoding?
    if(group == "db" && name == "defaultencoding")
        return "UTF-8";

    // db/savedefaultlocation?
    if(group == "db" && name == "savedefaultlocation")
        return 2;

    // db/defaultlocation?
    if(group == "db" && name == "defaultlocation")
        return QDir::homePath();

    // db/lastlocation?
    if(group == "db" && name == "lastlocation")
        return getSettingsValue("db", "defaultlocation");

    // db/hideschemalinebreaks?
    if(group == "db" && name == "hideschemalinebreaks")
        return false;

    // db/foreignkeys?
    if(group == "db" && name == "foreignkeys")
        return true;

    // db/prefetchsize?
    if(group == "db" && name == "prefetchsize")
        return 50000;

    // db/defaultsqltext?
    if(group == "db" && name == "defaultsqltext")
        return "";

    // MainWindow/geometry?
    if(group == "MainWindow" && name == "geometry")
        return "";

    // MainWindow/windowState?
    if(group == "MainWindow" && name == "windowState")
        return "";

    // SQLLogDock/Log?
    if(group == "SQLLogDock" && name == "Log")
        return "Application";

    // General/recentFileList?
    if(group == "General" && name == "recentFileList")
        return QStringList();

    // General/language?
    if(group == "General" && name == "language")
        return QLocale::system().name();

    // checkversion/enabled
    if(group == "checkversion" && name == "enabled")
        return true;

    // Data Browser/NULL Fields
    if(group == "databrowser")
    {
        if(name == "font")
            return "Sans Serif";
        if(name == "fontsize")
            return 10;
        if (name == "null_text")
            return "NULL";
        if (name == "filter_escape")
            return "\\";
        if (name == "null_fg_colour")
            return QColor(Qt::lightGray).name();
        if (name == "null_bg_colour")
            return QColor(Qt::white).name();
        if (name == "reg_fg_colour")
            return QColor(Qt::black).name();
        if (name == "reg_bg_colour")
            return QColor(Qt::white).name();
        if (name == "bin_fg_colour")
            return QColor(Qt::lightGray).name();
        if (name == "bin_bg_colour")
            return QColor(Qt::white).name();
    }

    // syntaxhighlighter?
    if(group == "syntaxhighlighter")
    {
        // Bold? Only tables, functions and keywords are bold by default
        if(name.right(4) == "bold")
            return name == "keyword_bold" || name == "table_bold" || name == "function_bold";

        // Italic? Nothing by default
        if(name.right(6) == "italic")
            return false;

        // Underline? Nothing by default
        if(name.right(9) == "underline")
            return false;

        // Colour?
        if(name.right(6) == "colour")
        {
            if(name == "keyword_colour")
                return QColor(Qt::darkBlue).name();
            else if(name == "function_colour")
                return QColor(Qt::blue).name();
            else if(name == "table_colour")
                return QColor(Qt::darkCyan).name();
            else if(name == "comment_colour")
                return QColor(Qt::darkGreen).name();
            else if(name == "identifier_colour")
                return QColor(Qt::darkMagenta).name();
            else if(name == "string_colour")
                return QColor(Qt::red).name();
            else if(name == "currentline_colour")
                return QColor(236, 236, 245).name();
        }
    }

    // editor/font?
    if(group == "editor" && name == "font")
        return "Monospace";

    // editor/fontsize or log/fontsize?
    if((group == "editor" || group == "log") && name == "fontsize")
        return 9;

    if(group == "editor")
    {
        if(name == "tabsize")
        {
            return 4;
        }
    }

    // editor/error_indicators?
    if(group == "editor" && name == "error_indicators")
        return true;

    // editor/horizontal_tiling?
    if(group == "editor" && name == "horizontal_tiling")
        return false;

    // extensions/list?
    if(group == "extensions" && name == "list")
        return QStringList();

    // extensions/disableregex?
    if(group == "extension" && name == "disableregex")
        return false;

    // PlotDock/lineType or pointShape?
    if(group == "PlotDock")
    {
        // QCPGraph::lsLine
        if(name == "lineType")
            return 1;

        // QCPScatterStyle::ssDisk
        if(name == "pointShape")
            return 4;
    }

    // Unknown combination of group and name? Return an invalid QVariant!
    return QVariant();
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
    int index = ui->languageComboBox->findData(getSettingsValue("General", "language"),
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
        QColor(getSettingsValue("databrowser", settingName + "_colour").toString()));
    frame->setPalette(palette);
}

void PreferencesDialog::saveColorSetting(QFrame *frame, const QString & settingName)
{
    setSettingsValue("databrowser", settingName + "_colour",
        frame->palette().color(frame->backgroundRole()));
}
