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

#include "SearchEngine.h"
#include <QString>
#include <QTextDocument>
#include <QTextCursor>
#include <QQuickTextDocument>

SearchEngine::SearchEngine(QObject *parent) : QObject(parent)
{
    connect(this, &SearchEngine::searchStringChanged, this, [this](){
        m_syntaxHighlighter.setWordPattern(m_searchString);
        highlightText();
    }, Qt::DirectConnection);

    connect(this, &SearchEngine::contentStringChanged, this, [this](){
        highlightText();
    }, Qt::DirectConnection);

    connect(this, SearchEngine::onPrevHighlightChanged, this, [this](){
        goToPrevHighlight();
    }, Qt::DirectConnection);

    connect(this, SearchEngine::onNextHighlightChanged, this, [this](){
        goToNextHighlight();
    }, Qt::DirectConnection);
}

SearchEngine::~SearchEngine() { }

QObject* SearchEngine::textDocumentObj() const{
    return qobject_cast<QObject*>(m_textDocument);
}

void SearchEngine::setTextDocument(QObject* obj){
    if(auto quickTextDocument = qobject_cast<QQuickTextDocument*>(obj)){
        initializeHighlighter(quickTextDocument->textDocument());
    }
}

void SearchEngine::initializeHighlighter(QTextDocument* textDocument){
    if(m_textDocument != textDocument){
        m_textDocument = textDocument;
        if(m_textDocument){
            m_syntaxHighlighter.setDocument(m_textDocument);
        }
        emit onTextDocumentChanged();
    }
}

QTextDocument* SearchEngine::textDocument() const{
    return m_textDocument;
}

QString SearchEngine::searchString() const{
    return m_searchString;
}

void SearchEngine::setSearchString(QString searchString){
    if(m_searchString != searchString){
        m_searchString = searchString;
        emit searchStringChanged();
    }
}

QString SearchEngine::contentString() const{
    return m_contentString;
}

void SearchEngine::setContentString(QString contentString){
    if(m_contentString != contentString){
        m_contentString = contentString;
        emit contentStringChanged();
    }
}

int SearchEngine::highlightIndex() const{
    return m_size > 0 ? m_highlightIndex : 0;
}

void SearchEngine::setHighlightIndex(int highlightIndex){
    if(m_highlightIndex != highlightIndex){
        m_highlightIndex = highlightIndex;
        emit onHighlightIndexChanged();
    }
}

int SearchEngine::size() const{
    return m_size;
}

void SearchEngine::setSize(int size){
    if(m_size != size){
        m_size = size;
        emit onSizeChanged();
    }
}

void SearchEngine::highlightText()
{
    if(!m_textDocument) return;

    m_syntaxHighlighter.customRehighlight();
    setSize(m_syntaxHighlighter.totalMatches());

    if(m_syntaxHighlighter.activeMatchIndex() < m_size){
        if(m_size > 0){
            m_syntaxHighlighter.setActiveMatchIndex(m_syntaxHighlighter.activeMatchIndex());
        }else{
            m_syntaxHighlighter.setActiveMatchIndex(0);
        }
    }else{
        m_syntaxHighlighter.setActiveMatchIndex(std::max(m_size - 1, 0));
    }

    updateCursorPosition(m_syntaxHighlighter.currentCursorMatch());
}

void SearchEngine::updateCursorPosition(int cursorPosition){
    setCursorPosition(cursorPosition);
    if(cursorPosition > 0){
        setHighlightIndex(m_syntaxHighlighter.activeMatchIndex() + 1);
    }
}

void SearchEngine::goToNextHighlight()
{
    m_syntaxHighlighter.setNextMatchStateActive();
    updateCursorPosition(m_syntaxHighlighter.currentCursorMatch());
}

void SearchEngine::goToPrevHighlight()
{
    m_syntaxHighlighter.setPrevMatchStateActive();
    updateCursorPosition(m_syntaxHighlighter.currentCursorMatch());
}

void SearchEngine::goToHighlightIndex(int index)
{
    m_syntaxHighlighter.setActiveMatchIndex(index - 1);
    updateCursorPosition(m_syntaxHighlighter.currentCursorMatch());
}

int SearchEngine::cursorPosition() const{
    return m_cursorPosition;
}

void SearchEngine::setCursorPosition(int cursorPosition){
    if(m_cursorPosition != cursorPosition){
        m_cursorPosition = cursorPosition;
        emit onCursorPositionChanged();
    }
}
