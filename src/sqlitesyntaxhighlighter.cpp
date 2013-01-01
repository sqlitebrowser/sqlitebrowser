#include "sqlitesyntaxhighlighter.h"

SQLiteSyntaxHighlighter::SQLiteSyntaxHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    tableFormat.setForeground(Qt::darkCyan);
    tableFormat.setFontWeight(QFont::Bold);

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns
            << "\\bREINDEX\\b" << "\\bINDEXED\\b" << "\\bINDEX\\b"
            << "\\bDESC\\b" << "\\bESCAPE\\b" << "\\bEACH\\b"
            << "\\bCHECK\\b" << "\\bKEY\\b" << "\\bBEFORE\\b"
            << "\\bFOREIGN\\b" << "\\bFOR\\b" << "\\bIGNORE\\b"
            << "\\bREGEXP\\b" << "\\bEXPLAIN\\b" << "\\bINSTEAD\\b"
            << "\\bADD\\b" << "\\bDATABASE\\b" << "\\bAS\\b"
            << "\\bSELECT\\b" << "\\bTABLE\\b" << "\\bLEFT\\b"
            << "\\bTHEN\\b" << "\\bEND\\b" << "\\bDEFERRABLE\\b"
            << "\\bELSE\\b" << "\\bEXCEPT\\b" << "\\bTRANSACTION\\b"
            << "\\bACTION\\b" << "\\bON\\b" << "\\bNATURAL\\b"
            << "\\bALTER\\b" << "\\bRAISE\\b" << "\\bEXCLUSIVE\\b"
            << "\\bEXISTS\\b" << "\\bSAVEPOINT\\b" << "\\bINTERSECT\\b"
            << "\\bTRIGGER\\b" << "\\bREFERENCES\\b" << "\\bCONSTRAINT\\b"
            << "\\bINTO\\b" << "\\bOFFSET\\b" << "\\bOF\\b"
            << "\\bSET\\b" << "\\bTEMPORARY\\b" << "\\bTEMP\\b"
            << "\\bOR\\b" << "\\bUNIQUE\\b" << "\\bQUERY\\b"
            << "\\bATTACH\\b" << "\\bHAVING\\b" << "\\bGROUP\\b"
            << "\\bUPDATE\\b" << "\\bBEGIN\\b" << "\\bINNER\\b"
            << "\\bRELEASE\\b" << "\\bBETWEEN\\b" << "\\bNOTNULL\\b"
            << "\\bNOT\\b" << "\\bNO\\b" << "\\bNULL\\b"
            << "\\bLIKE\\b" << "\\bCASCADE\\b" << "\\bASC\\b"
            << "\\bDELETE\\b" << "\\bCASE\\b" << "\\bCOLLATE\\b"
            << "\\bCREATE\\b" << "\\bCURRENT_DATE\\b" << "\\bDETACH\\b"
            << "\\bIMMEDIATE\\b" << "\\bJOIN\\b" << "\\bINSERT\\b"
            << "\\bMATCH\\b" << "\\bPLAN\\b" << "\\bANALYZE\\b"
            << "\\bPRAGMA\\b" << "\\bABORT\\b" << "\\bVALUES\\b"
            << "\\bVIRTUAL\\b" << "\\bLIMIT\\b" << "\\bWHEN\\b"
            << "\\bWHERE\\b" << "\\bRENAME\\b" << "\\bAFTER\\b"
            << "\\bREPLACE\\b" << "\\bAND\\b" << "\\bDEFAULT\\b"
            << "\\bAUTOINCREMENT\\b" << "\\bTO\\b" << "\\bIN\\b"
            << "\\bCAST\\b" << "\\bCOLUMN\\b" << "\\bCOMMIT\\b"
            << "\\bCONFLICT\\b" << "\\bCROSS\\b" << "\\bCURRENT_TIMESTAMP\\b"
            << "\\bCURRENT_TIME\\b" << "\\bPRIMARY\\b" << "\\bDEFERRED\\b"
            << "\\bDISTINCT\\b" << "\\bIS\\b"  << "\\bDROP\\b"
            << "\\bFAIL\\b" << "\\bFROM\\b" << "\\bFULL\\b"
            << "\\bGLOB\\b" << "\\bBY\\b" << "\\bIF\\b"
            << "\\bISNULL\\b" << "\\bORDER\\b" << "\\bRESTRICT\\b"
            << "\\bOUTER\\b" << "\\bRIGHT\\b" << "\\bROLLBACK\\b"
            << "\\bROW\\b" << "\\bUNION\\b"
            << "\\bUSING\\b" << "\\bVACUUM\\b" << "\\bVIEW\\b"
            << "\\bINITIALLY\\b" << "\\bALL\\b" << "\\bSELECT\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.pattern.setCaseSensitivity(Qt::CaseInsensitive);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("'.*'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}

void SQLiteSyntaxHighlighter::setTableNames(const QStringList &tablenames)
{
    tableNameRules.clear();
    foreach(const QString& tblname, tablenames) {
        HighlightingRule rule;
        rule.pattern = QRegExp(tblname);
        rule.format = tableFormat;
        tableNameRules.append(rule);
    }
}

void SQLiteSyntaxHighlighter::highlightBlockVector(const QString& text, const QVector<HighlightingRule>& vec)
{
    foreach (const HighlightingRule &rule, vec) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}

void SQLiteSyntaxHighlighter::highlightBlock(const QString &text)
{
    highlightBlockVector(text, tableNameRules);
    highlightBlockVector(text, highlightingRules);
}
