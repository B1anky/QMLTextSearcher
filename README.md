# QMLTextSearcher
Based on my previous repo for TextSearcher, this is a QML-based version of the solution.

Provides a `QML`-based `TextArea` with a `Ctrl+F` interface to launch an IDE / web browser like text highlighter and search bar. Changing the highlighting index can be done via either of the Previous / Next buttons or various additional controls added to the search bar. When in focus of the highlight index TextEdit to the left of the `/` symbol, you can type a valid index or use your keyboard's arrow keys to navigate previous / next as well. If the index changes, the view will jump the cursor location to the front of the highlighted word in the `TextArea`.
