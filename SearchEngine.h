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

#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QObject>
#include <Highlighter.h>

class SearchEngine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString NOTIFY searchStringChanged)
    Q_PROPERTY(QString contentString READ contentString WRITE setContentString NOTIFY contentStringChanged)
    Q_PROPERTY(int highlightIndex READ highlightIndex WRITE setHighlightIndex NOTIFY onHighlightIndexChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY onSizeChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY onCursorPositionChanged)
    Q_PROPERTY(int linePosition READ linePosition WRITE setLinePosition NOTIFY onLinePositionChanged)
    Q_PROPERTY(QObject* textDocumentObj READ textDocumentObj WRITE setTextDocument)
    Q_PROPERTY(QTextDocument* textDocument READ textDocument NOTIFY onTextDocumentChanged)

public:
    explicit SearchEngine(QObject *parent = nullptr);
    ~SearchEngine();

    void initializeHighlighter(QTextDocument* textDocument);
    QObject* textDocumentObj() const;
    void setTextDocument(QObject* obj);
    QTextDocument* textDocument() const;

    QString searchString() const;
    void setSearchString(QString searchString);

    QString contentString() const;
    void setContentString(QString contentString);

    int highlightIndex() const;
    void setHighlightIndex(int highlightIndex);

    int size() const;
    void setSize(int size);

    int cursorPosition() const;
    void setCursorPosition(int cursorPosition);

    int linePosition() const;
    void setLinePosition(int linePosition);

private:
    void highlightText();
    void TextEditRefreshHighlighter();
    void goToPrevHighlight();
    void goToNextHighlight();
    void goToHighlightIndex(int index);
    void jumpToLine(int cursorPosition);

signals:
    void searchStringChanged();
    void contentStringChanged();
    void onSizeChanged();
    void onHighlightIndexChanged();
    void onPrevHighlightChanged();
    void onNextHighlightChanged();
    void onCursorPositionChanged();
    void onLinePositionChanged();
    void onTextDocumentChanged();

private:
    QString m_searchString;
    QString m_contentString;

    int m_highlightIndex = 0;
    int m_size = 0;
    int m_cursorPosition = 0;
    int m_linePosition = 0;

    Highlighter m_syntaxHighlighter;
    QTextDocument* m_textDocument = nullptr;
};

#endif // SEARCHENGINE_H
