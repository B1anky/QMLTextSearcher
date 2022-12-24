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

import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import SearchEngine 1.0

Window {
    id: window
    objectName: "window"
    width: 640
    height: 480
    visible: true
    title: qsTr("QML Text Search")

    SearchEngine {
        id: searchEngine
        objectName: "searchEngine"
        textDocumentObj: contentTextArea.textDocument

        onHighlightIndexChanged: {
            contentTextArea.width = contentTextArea.width + 1
            contentTextArea.width = contentTextArea.width - 1
            indexTextInput.text = parseInt(searchEngine.highlightIndex)
        }
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 5
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.bottomMargin: 10

        RowLayout {
            Layout.preferredHeight: 20
            Layout.fillWidth: true

            Rectangle {
                id: rect1
                border.color: "black"
                border.width: 1
                Layout.fillWidth: true
                Layout.preferredHeight: 20
                Layout.alignment: Qt.AlignTop

                TextInput {
                    id: searchTextInput
                    anchors.fill: parent
                    text: ""
                    leftPadding: 10
                    rightPadding: 10
                    verticalAlignment: TextEdit.AlignVCenter
                    selectByMouse: true
                    property string placeholderText: "Search for..."

                    Text {
                        anchors.fill: parent
                        text: searchTextInput.placeholderText
                        color: "#aaa"
                        visible: !searchTextInput.text
                                 && !searchTextInput.activeFocus
                        leftPadding: 10
                        rightPadding: 10
                        verticalAlignment: TextEdit.AlignVCenter
                    }

                    onTextChanged: {
                        searchEngine.searchString = text
                        contentTextArea.width = contentTextArea.width + 1
                        contentTextArea.width = contentTextArea.width - 1
                    }
                }
            }

            TextInput {
                id: indexTextInput
                text: "0"
                Layout.preferredHeight: rect1.height
                Layout.alignment: TextEdit.AlignTop
                verticalAlignment: TextEdit.AlignVCenter
                selectedTextColor: Qt.rgba(1, 1, 1, 1)

                validator: IntValidator {
                    bottom: searchEngine.size > 0 ? 1 : 0
                    top: searchEngine.size
                }

                onFocusChanged: {
                    if (focus) {
                        selectAll()
                    }
                }

                onTextEdited: {
                    if (acceptableInput) {
                        searchEngine.highlightIndex = parseInt(text)
                    } else {
                        indexTextInput.text = parseInt(
                                    searchEngine.highlightIndex)
                        selectAll()
                    }
                }

                Keys.onUpPressed: {
                    backButton.highlightPrev()
                }

                Keys.onDownPressed: {
                    forwardButton.highlightNext()
                }
            }

            Label {
                id: sizeLabel
                text: "/ " + parseInt(searchEngine.size)
                Layout.preferredHeight: rect1.height
                Layout.alignment: TextEdit.AlignTop
                verticalAlignment: TextEdit.AlignVCenter
            }

            RoundButton {
                id: backButton
                text: "▲"
                Layout.preferredHeight: rect1.height
                Layout.preferredWidth: 30
                Layout.alignment: TextEdit.AlignTop
                radius: 5
                function highlightPrev() {
                    searchEngine.onPrevHighlightChanged()
                    contentTextArea.width = contentTextArea.width + 1
                    contentTextArea.width = contentTextArea.width - 1
                    indexTextInput.text = parseInt(searchEngine.highlightIndex)
                }
                onPressed: {
                    highlightPrev()
                }
            }

            RoundButton {
                id: forwardButton
                text: "▼"
                Layout.preferredHeight: rect1.height
                Layout.preferredWidth: 30
                Layout.alignment: TextEdit.AlignTop
                radius: 5
                function highlightNext() {
                    searchEngine.onNextHighlightChanged()
                    contentTextArea.width = contentTextArea.width + 1
                    contentTextArea.width = contentTextArea.width - 1
                    indexTextInput.text = parseInt(searchEngine.highlightIndex)
                }
                onPressed: {
                    highlightNext()
                }
            }
        }
        Rectangle {
            id: rect2
            Layout.fillWidth: true
            Layout.fillHeight: true
            border.color: "black"
            border.width: 1
            ScrollView {
                id: scrollView
                anchors.fill: parent
                TextArea {
                    id: contentTextArea
                    objectName: "contentTextArea"
                    text: ""
                    wrapMode: TextEdit.Wrap
                    selectByMouse: true
                    selectByKeyboard: true
                    textFormat: TextEdit.PlainText
                    focus: true
                    cursorPosition: searchEngine.cursorPosition

                    onTextChanged: {
                        searchEngine.contentString = text
                    }
                }
            }
        }
    }
}
