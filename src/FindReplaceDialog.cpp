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

void FindReplaceDialog::setSqlTextEdit(SqlTextEdit* sqlTextEdit)
{
    m_sqlTextEdit = sqlTextEdit;

    // Create indicator for find-all and replace-all occurrences
    foundIndicatorNumber = m_sqlTextEdit->indicatorDefine(QsciScintilla::StraightBoxIndicator);
    m_sqlTextEdit->setIndicatorForegroundColor(Qt::magenta, foundIndicatorNumber);
    m_sqlTextEdit->setIndicatorDrawUnder(true, foundIndicatorNumber);

    if (m_sqlTextEdit->isReadOnly()) {
        ui->replaceWithText->setEnabled(false);
        ui->replaceButton->setEnabled(false);
        ui->replaceAllButton->setEnabled(false);
    }

}

bool FindReplaceDialog::findNext()
{
    clearIndicators();

    bool found = m_sqlTextEdit->findText
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

void FindReplaceDialog::replace()
{
    m_sqlTextEdit->replace(ui->replaceWithText->text());
    findNext();
}

void FindReplaceDialog::indicateSelection()
{
     int fromRow, fromIndex, toRow, toIndex;
     m_sqlTextEdit->getSelection(&fromRow, &fromIndex, &toRow, &toIndex);
     m_sqlTextEdit->fillIndicatorRange(fromRow, fromIndex, toRow, toIndex, foundIndicatorNumber);

}
void FindReplaceDialog::findAll()
{
    clearIndicators();
    int occurrences = 0;
    m_sqlTextEdit->setCursorPosition(0, 0);
    while (m_sqlTextEdit->findText
            (ui->findText->text(),
             ui->regexpCheckBox->isChecked(),
             ui->caseCheckBox->isChecked(),
             ui->wholeWordsCheckBox->isChecked(),
             false,
             true)) {
        indicateSelection();
        ++occurrences;
    }

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
    m_sqlTextEdit->setCursorPosition(0, 0);
    while (m_sqlTextEdit->findText
            (ui->findText->text(),
             ui->regexpCheckBox->isChecked(),
             ui->caseCheckBox->isChecked(),
             ui->wholeWordsCheckBox->isChecked(),
             false,
             true)) {
        m_sqlTextEdit->replace(ui->replaceWithText->text());
        indicateSelection();
        ++occurrences;
    }

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
    m_sqlTextEdit->clearIndicatorRange(0, 0, m_sqlTextEdit->lines(), m_sqlTextEdit->lineLength(m_sqlTextEdit->lines()), foundIndicatorNumber);
    ui->messageLabel->setText("");
}

void FindReplaceDialog::close()
{
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
