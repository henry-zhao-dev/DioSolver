#include "dialog.h"

#include "main_window.h"

#include <QApplication>
#include <QDesktopServices>
#include <QTextBrowser>
#include <QTextEdit>
#include <QUrl>
#include <diosolver/diophantine.h>

Dialog::Dialog(MainWindow *win, const QString &title, bool modal)
    : QDialog(win), dialogLayout(new QVBoxLayout(this)),
      okButton(new QPushButton("OK", this)) {

    setWindowTitle(title);
    setModal(modal);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(okButton, &QPushButton::clicked, this, &Dialog::close);
    dialogLayout->addWidget(okButton, 0, Qt::AlignCenter);
}

AboutDialog::AboutDialog(MainWindow *win)
    : Dialog(win, "About DioSolver", true) {

    auto *editor = new QTextBrowser(this);
    editor->setLineWrapMode(QTextEdit::WidgetWidth);
    editor->setOpenLinks(false);
    editor->setSource(QUrl("qrc:/conf/about.html"));
    connect(editor, &QTextBrowser::anchorClicked, this, [](const QUrl &url) {
        if (url == QUrl("about:qt")) {
            QApplication::aboutQt();
        } else {
            QDesktopServices::openUrl(url);
        }
    });
    dialogLayout->insertWidget(0, editor);

    resize(500, 500);
}

ResultDialog::ResultDialog(MainWindow *win, const LDE &lde)
    : Dialog(win, "Result") {

    auto *editor = new QTextBrowser(this);
    editor->setReadOnly(true);
    dialogLayout->insertWidget(0, editor);

    calist *result = lde_result(lde);
    for (size_t i = 0; i < calist_size(result); ++i) {
        const auto line = static_cast<const char *>(calist_get(result, i));
        editor->insertPlainText(line);
    }
    calist_destroy(result);

    resize(550, 650);
}
