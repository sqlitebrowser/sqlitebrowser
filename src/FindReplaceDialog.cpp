#include "FindReplaceDialog.h"
#include "ui_FindReplaceDialog.h"

#include <QWhatsThis>

FindReplaceDialog::FindReplaceDialog(QWidget* parent)
    : QDialog(parent),
      ui(new Ui::FindReplaceDialog)
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

    connect(m_scintilla, SIGNAL(destroyed()), this, SLOT(hide()));
}

bool FindReplaceDialog::findNext()
{
    clearIndicators();

    bool found = m_scintilla->findText
            (ui->findText->text(),
             ui->regexpCheckBox->isChecked(),
             ui->caseCheckBox->isChecked(),
             ui->wholeWordsCheckBox->isChecked(),
             ui->wrapCheckBox->isChecked(),
             !ui->backwardsCheckBox->isChecked());
    if (!found)
        ui->messageLabel->setText(tr("The searched text was not found"));

    return found;

}

void FindReplaceDialog::show()
{
    ui->findText->setFocus();
    ui->findText->selectAll();
    QDialog::show();
}

void FindReplaceDialog::replace()
{
    m_scintilla->replace(ui->replaceWithText->text());
    findNext();
}

void FindReplaceDialog::indicateSelection()
{
     int fromRow, fromIndex, toRow, toIndex;
     m_scintilla->getSelection(&fromRow, &fromIndex, &toRow, &toIndex);
     m_scintilla->fillIndicatorRange(fromRow, fromIndex, toRow, toIndex, foundIndicatorNumber);

}
void FindReplaceDialog::findAll()
{
    clearIndicators();
    int occurrences = 0;
    m_scintilla->setCursorPosition(0, 0);
    while (m_scintilla->findText
            (ui->findText->text(),
             ui->regexpCheckBox->isChecked(),
             ui->caseCheckBox->isChecked(),
             ui->wholeWordsCheckBox->isChecked(),
             false,
             true)) {
        indicateSelection();
        ++occurrences;
    }
    m_scintilla->clearSelection();

    QString message;
    switch (occurrences) {
    case 0:
        message = tr("The searched text was not found.");
        break;
    case 1:
        message = tr("The searched text was found one time.");
        break;
    default:
        message = tr("The searched text was found %1 times.").arg(occurrences);
        break;
    }

    ui->messageLabel->setText(message);
}

void FindReplaceDialog::replaceAll()
{
    clearIndicators();
    int occurrences = 0;
    m_scintilla->setCursorPosition(0, 0);
    while (m_scintilla->findText
            (ui->findText->text(),
             ui->regexpCheckBox->isChecked(),
             ui->caseCheckBox->isChecked(),
             ui->wholeWordsCheckBox->isChecked(),
             false,
             true)) {
        m_scintilla->replace(ui->replaceWithText->text());
        indicateSelection();
        ++occurrences;
    }
    m_scintilla->clearSelection();

    QString message;
    switch (occurrences) {
    case 0:
        message = tr("The searched text was not found.");
        break;
    case 1:
        message = tr("The searched text was replaced one time.");
        break;
    default:
        message = tr("The searched text was replaced %1 times.").arg(occurrences);
        break;
    }

    ui->messageLabel->setText(message);

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
    clearIndicators();
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
