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
import QtQuick.Controls.Material 2.3

import SearchEngine 1.0


Window {
    id: window
    objectName: "window"
    width: 640
    height: 480
    visible: true
    title: qsTr("QML Text Search")
    property bool showSearchBar: false
    property string searchStringOnHide: ""
    property bool darkMode: false
    Material.theme: darkMode ? Material.Dark : Material.Light
    Material.accent: darkMode ? "Orange" : "Teal"
    Material.primary: darkMode ? "White" : "BlueGrey"
    color: window.Material.background

    SearchEngine {
        id: searchEngine
        objectName: "searchEngine"
        textDocumentObj: contentTextArea.textDocument

        onHighlightIndexChanged: {
            indexTextInput.text = parseInt(searchEngine.highlightIndex)
        }

        onCursorPositionChanged: {
            contentTextArea.cursorPosition = searchEngine.cursorPosition
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
            Layout.preferredHeight: 30
            Layout.fillWidth: true

            Switch{
                id: darkModeSwitch
                Layout.alignment: Qt.AlignBottom
                bottomPadding: 0
                text: window.darkMode ? "Light Mode" : "Dark Mode"
                onToggled: {
                    window.darkMode = !window.darkMode
                }
            }

            Item{
                id: searchBar
                Layout.fillWidth: true
                Layout.preferredHeight: 20
                visible: showSearchBar

                RowLayout {
                    anchors.fill: parent

                    Rectangle {
                        id: searchBarRect
                        color: window.Material.background
                        border.color: window.Material.foreground
                        border.width: 1
                        Layout.fillWidth: true
                        Layout.preferredHeight: 30
                        radius: 2

                        TextInput {
                            id: searchTextInput
                            anchors.fill: parent
                            color: window.Material.foreground
                            text: ""
                            focus: true
                            leftPadding: 10
                            rightPadding: 10
                            verticalAlignment: TextEdit.AlignVCenter
                            selectByMouse: true
                            property string placeholderText: "Search for..."

                            Text {
                                anchors.fill: parent
                                text: searchTextInput.placeholderText
                                color: window.Material.foreground
                                visible: !searchTextInput.text
                                         && !searchTextInput.activeFocus
                                leftPadding: 10
                                rightPadding: 10
                                verticalAlignment: TextEdit.AlignVCenter
                            }

                            onTextEdited: {
                                searchEngine.searchString = text
                            }
                        }
                    }

                    ToolSeparator{
                        Layout.preferredHeight: searchBarRect.height
                        topPadding: 0
                        bottomPadding: 0
                    }

                    TextInput {
                        id: indexTextInput
                        text: "0"
                        Layout.preferredHeight: searchBarRect.height
                        Layout.alignment: TextEdit.AlignTop
                        verticalAlignment: TextEdit.AlignVCenter
                        topPadding: 0
                        bottomPadding: 0
                        leftPadding: 0
                        rightPadding: 0
                        font: sizeLabel.font
                        color: window.Material.foreground

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
                       rightPadding: 10
                       Layout.preferredHeight: searchBarRect.height
                       verticalAlignment: TextEdit.AlignVCenter
                   }

                    Button {
                        id: backButton
                        text: "▲"
                        Layout.preferredHeight: searchBarRect.height
                        Layout.preferredWidth: 30
                        Layout.alignment: TextEdit.AlignTop
                        ToolTip.text: "Previous Match"
                        ToolTip.visible: hovered

                        function highlightPrev() {
                            searchEngine.onPrevHighlightChanged()
                            indexTextInput.text = parseInt(searchEngine.highlightIndex)
                        }
                        onPressed: {
                            highlightPrev()
                        }
                    }

                    Button {
                        id: forwardButton
                        text: "▼"
                        Layout.preferredHeight: searchBarRect.height
                        Layout.preferredWidth: 30
                        Layout.alignment: TextEdit.AlignTop
                        ToolTip.text: "Next Match"
                        ToolTip.visible: hovered

                        function highlightNext() {
                            searchEngine.onNextHighlightChanged()
                            indexTextInput.text = parseInt(searchEngine.highlightIndex)
                        }
                        onPressed: {
                            highlightNext()
                        }
                    }

                    Button {
                        id: hideButton
                        text: "X"
                        Layout.preferredHeight: searchBarRect.height
                        Layout.preferredWidth: 30
                        Layout.alignment: TextEdit.AlignTop
                        ToolTip.text: "Close search bar"
                        ToolTip.visible: hovered

                        onPressed: {
                            window.showSearchBar = false
                            window.searchStringOnHide = searchEngine.searchString
                            searchEngine.searchString = ""
                        }
                    }
                }
            }
        }
        Rectangle {
            id: textAreaRect
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: window.Material.background
            border.color: window.Material.foreground
            border.width: 1
            radius: 2

            Flickable {
                id: scrollView
                anchors.fill: parent
                focus: true

                ScrollBar.vertical: ScrollBar {
                    id: verticalScrollBar
                    size: 0.3
                    position: 0.2
                    active: true
                    orientation: Qt.Vertical

                    contentItem: Rectangle {
                        implicitWidth: 6
                        implicitHeight: 100
                        radius: width / 2
                        color: verticalScrollBar.pressed ? Qt.darker(Material.accent) : Material.accent
                    }
                }

                TextArea.flickable: TextArea {
                    id: contentTextArea
                    objectName: "contentTextArea"

                    text: ""
                    leftPadding: 10
                    rightPadding: scrollView.ScrollBar.vertical.visible ? scrollView.ScrollBar.vertical.width : 10
                    bottomPadding: 5
                    wrapMode: TextEdit.Wrap
                    selectByMouse: true
                    selectByKeyboard: true
                    textFormat: TextEdit.PlainText

                    onTextChanged: {
                        searchEngine.contentString = text
                    }

                    Shortcut {
                        sequence: "Ctrl+F"
                        onActivated: {
                            window.showSearchBar = true
                            searchEngine.searchString = window.searchStringOnHide
                            contentTextArea.cursorPosition = searchEngine.cursorPosition
                            searchTextInput.focus = true
                        }
                    }
                }
            }
        }
    }
}
