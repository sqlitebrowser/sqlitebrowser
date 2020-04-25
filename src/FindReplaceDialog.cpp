#include "FindReplaceDialog.h"
#include "ui_FindReplaceDialog.h"
#include "ExtendedScintilla.h"

#include <QWhatsThis>

FindReplaceDialog::FindReplaceDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::FindReplaceDialog),
      m_scintilla(nullptr),
      foundIndicatorNumber(0),
      findInProgress(false)
{
    // Create UI
    ui->setupUi(this);
}

FindReplaceDialog::~FindReplaceDialog()
{
    delete ui;
}

void FindReplaceDialog::setExtendedScintilla(ExtendedScintilla* scintilla)
{
    m_scintilla = scintilla;

    // Create indicator for find-all and replace-all occurrences
    foundIndicatorNumber = m_scintilla->indicatorDefine(QsciScintilla::StraightBoxIndicator);
    m_scintilla->setIndicatorForegroundColor(Qt::magenta, foundIndicatorNumber);
    m_scintilla->setIndicatorDrawUnder(true, foundIndicatorNumber);

    bool isWriteable = ! m_scintilla->isReadOnly();
    ui->replaceWithText->setEnabled(isWriteable);
    ui->replaceButton->setEnabled(isWriteable);
    ui->replaceAllButton->setEnabled(isWriteable);

    connect(m_scintilla, &ExtendedScintilla::destroyed, this, &FindReplaceDialog::hide);
    connect(ui->findText, &QLineEdit::editingFinished, this, &FindReplaceDialog::cancelFind);
    connect(ui->regexpCheckBox, &QCheckBox::toggled, this, &FindReplaceDialog::cancelFind);
    connect(ui->caseCheckBox, &QCheckBox::toggled, this, &FindReplaceDialog::cancelFind);
    connect(ui->wholeWordsCheckBox, &QCheckBox::toggled, this, &FindReplaceDialog::cancelFind);
    connect(ui->wrapCheckBox, &QCheckBox::toggled, this, &FindReplaceDialog::cancelFind);
    connect(ui->backwardsCheckBox, &QCheckBox::toggled, this, &FindReplaceDialog::cancelFind);
    connect(ui->selectionCheckBox, &QCheckBox::toggled, this, &FindReplaceDialog::cancelFind);
    connect(ui->selectionCheckBox, &QCheckBox::toggled, ui->wrapCheckBox, &QCheckBox::setDisabled);
}

bool FindReplaceDialog::findFirst(bool wrap, bool forward)
{
    if (ui->selectionCheckBox->isChecked())
        return m_scintilla->findFirstInSelection
            (ui->findText->text(),
             ui->regexpCheckBox->isChecked(),
             ui->caseCheckBox->isChecked(),
             ui->wholeWordsCheckBox->isChecked(),
             forward,
             /* show */ true,
             /* posix */ true,
             /* cxx11 */ true);
    else
        return m_scintilla->findFirst
            (ui->findText->text(),
             ui->regexpCheckBox->isChecked(),
             ui->caseCheckBox->isChecked(),
             ui->wholeWordsCheckBox->isChecked(),
             wrap,
             forward,
             /* line */ -1,
             /* index */ -1,
             /* show */ true,
             /* posix */ true,
             /* cxx11 */ true);
}

bool FindReplaceDialog::findNext()
{
    clearIndicators();

    if (findInProgress)
        findInProgress = m_scintilla->findNext();
    else
        findInProgress = findFirst(ui->wrapCheckBox->isChecked(),
                                   !ui->backwardsCheckBox->isChecked());

    if (!findInProgress)
        ui->messageLabel->setText(tr("The searched text was not found"));

    return findInProgress;

}

void FindReplaceDialog::showFindReplaceDialog(bool hasReplace)
{
    ui->replaceWithText->setVisible(hasReplace);
    ui->replaceButton->setVisible(hasReplace);
    ui->replaceWithLabel->setVisible(hasReplace);
    ui->replaceAllButton->setVisible(hasReplace);
    show();
}

void FindReplaceDialog::show()
{
    ui->findText->setFocus();

    // If there is multi-line selected text set automatically the selection
    // check box. If it's only part of a line, use it as text to find.
    if (m_scintilla->hasSelectedText())
      if (m_scintilla->selectedText().contains("\n"))
            ui->selectionCheckBox->setChecked(true);
      else {
            ui->findText->setText(m_scintilla->selectedText());
            ui->selectionCheckBox->setChecked(false);
      }
    else
        ui->selectionCheckBox->setChecked(false);

    ui->findText->selectAll();
    QDialog::show();
}

void FindReplaceDialog::replace()
{
    if (m_scintilla->hasSelectedText())
        m_scintilla->replace(ui->replaceWithText->text());
    findNext();
}

void FindReplaceDialog::indicateSelection()
{
     int fromRow, fromIndex, toRow, toIndex;
     m_scintilla->getSelection(&fromRow, &fromIndex, &toRow, &toIndex);
     m_scintilla->fillIndicatorRange(fromRow, fromIndex, toRow, toIndex, foundIndicatorNumber);
}

void FindReplaceDialog::searchAll(bool replace)
{
    clearIndicators();
    if (!ui->selectionCheckBox->isChecked())
        m_scintilla->setCursorPosition(0, 0);

    bool found = findFirst(/* wrap */ false, /* fordward */ true);

    int occurrences = 0;
    while (found) {
        if (replace)
            m_scintilla->replace(ui->replaceWithText->text());
        indicateSelection();
        ++occurrences;
        found = m_scintilla->findNext();
    }

    if (!ui->selectionCheckBox->isChecked())
        m_scintilla->clearSelection();

    QString message;
    switch (occurrences) {
    case 0:
        message = tr("The searched text was not found.");
        break;
    case 1:
        if (replace)
            message = tr("The searched text was replaced one time.");
        else
            message = tr("The searched text was found one time.");
        break;
    default:
        if (replace)
            message = tr("The searched text was replaced %1 times.").arg(occurrences);
        else
            message = tr("The searched text was found %1 times.").arg(occurrences);

        break;
    }

    ui->messageLabel->setText(message);
}

void FindReplaceDialog::findAll()
{
    searchAll(/* replace */ false);
}

void FindReplaceDialog::replaceAll()
{
    searchAll(/* replace */ true);
}

void FindReplaceDialog::cancelFind()
{
    m_scintilla->findFirst(QString(), false, false, false, false);
    clearIndicators();
    findInProgress = false;
    ui->messageLabel->setText("");
}
void FindReplaceDialog::help()
{
    QWhatsThis::enterWhatsThisMode();
}

void FindReplaceDialog::clearIndicators()
{
    m_scintilla->clearIndicatorRange(0, 0, m_scintilla->lines(), m_scintilla->lineLength(m_scintilla->lines()), foundIndicatorNumber);
    ui->messageLabel->setText("");
}

void FindReplaceDialog::close()
{
    m_scintilla->clearSelection();
    // Reset any previous find so it does not interfere with the next time
    // the dialog is open.
    cancelFind();
    QDialog::close();
}

void FindReplaceDialog::buttonBox_clicked(QAbstractButton* button)
{
    if (button == ui->buttonBox->button(QDialogButtonBox::Help))
        help();
    else if (button == ui->buttonBox->button(QDialogButtonBox::Close))
        close();

}
void FindReplaceDialog::reject()
{
    close();
    QDialog::reject();
}
