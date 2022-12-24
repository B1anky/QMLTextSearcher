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

#include "highlighter.h"
#include <QTextDocument>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    m_defaultHighlightingRule.format.setBackground(QColor(128, 128, 128, 128));
    m_defaultHighlightingRule.format.setFontWeight(QFont::Bold);

    m_activeHighlightingRule.format.setBackground(QColor(255, 165, 0));
    m_activeHighlightingRule.format.setFontWeight(QFont::Bold);
}

void Highlighter::setWordPattern(const QString& pattern){
    if(pattern != m_findString){
        m_findString = pattern;
        m_defaultHighlightingRule.pattern = QRegularExpression(pattern, QRegularExpression::PatternOption::CaseInsensitiveOption);
        m_activeMatchIndex = 0;
    }
}

void Highlighter::highlightBlock(const QString& text)
{
    if(!document()) return;

    if(m_findString.isEmpty()){
        setFormat(0, document()->toPlainText().length(), QTextCharFormat());
        m_activeMatchIndex = 0;
    }else{
        m_matches = m_defaultHighlightingRule.pattern.globalMatch(text);
        while(m_matches.hasNext()){
            QRegularExpressionMatch match = m_matches.next();
            if(match.hasPartialMatch() || match.hasMatch()){
                int startOffset = match.capturedStart();
                int endOffset   = match.capturedEnd();
                QTextCharFormat format = m_defaultHighlightingRule.format;
                if(m_activeMatchIndex == m_currentMatchIndex){
                    format = m_activeHighlightingRule.format;
                    m_activeBlock = currentBlock();
                }
                setFormat(startOffset, endOffset - startOffset, format);
                m_matchList.push_back(match);
                ++m_currentMatchIndex;
            }
        }
    }
}

void Highlighter::customRehighlight(){
    if(!document()) return;

    m_matchList.clear();
    m_currentMatchIndex = 0;
    rehighlight();
}

int Highlighter::setNextMatchStateActive(){
    return setActiveMatchIndex(m_activeMatchIndex + 1);
}

int Highlighter::setPrevMatchStateActive(){
    return setActiveMatchIndex(m_activeMatchIndex - 1);
}

int Highlighter::setActiveMatchIndex(int activeMatchIndex){
    int startIndex = -1;
    if(m_activeMatchIndex != activeMatchIndex){
        if(activeMatchIndex >= 0 && activeMatchIndex < m_matchList.size()){
            m_activeMatchIndex = activeMatchIndex;
            customRehighlight();
            startIndex = m_matchList[m_activeMatchIndex].capturedEnd() + m_activeBlock.position();
        }else if(activeMatchIndex >= m_matchList.size() && m_matchList.size() > 0){
            m_activeMatchIndex = 0;
            customRehighlight();
            startIndex = m_matchList[m_activeMatchIndex].capturedEnd() + m_activeBlock.position();
        }else if(activeMatchIndex <= -1 && m_matchList.size() > 0){
            m_activeMatchIndex = m_matchList.size() - 1;
            customRehighlight();
            startIndex = m_matchList[m_activeMatchIndex].capturedEnd() + m_activeBlock.position();
        }
    }
    return startIndex;
}

int Highlighter::totalMatches() const{
    return m_matchList.size();
}

int Highlighter::activeMatchIndex() const{
    return m_activeMatchIndex;
}
