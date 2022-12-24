/*
 * Copyright (c) 2022 Brett Sackstein
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextCharFormat>

class QTextDocument;

class Highlighter : public QSyntaxHighlighter{

    Q_OBJECT

public:

    Highlighter(QTextDocument* parent = nullptr);
    void setWordPattern(const QString& word);
    void highlightBlock(const QString &text);
    int setNextMatchStateActive();
    int setPrevMatchStateActive();
    void customRehighlight();

    int totalMatches() const;
    int setActiveMatchIndex(int activeMatchIndex);
    int activeMatchIndex() const;

private:

    struct HighlightingRule{
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QString m_findString;
    HighlightingRule m_defaultHighlightingRule;
    HighlightingRule m_activeHighlightingRule;
    QRegularExpressionMatchIterator m_matches;
    QList<QRegularExpressionMatch> m_matchList;
    int m_currentMatchIndex = 0;
    int m_activeMatchIndex = 0;
    QTextBlock m_activeBlock;
};

#endif // HIGHLIGHTER_H
