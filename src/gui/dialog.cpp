#include "dialog.h"
#include "main_window.h"
#include <diosolver/diophantine.h>

#include <QLabel>
#include <QTextBrowser>
#include <QTextEdit>
#include <QDesktopServices>

#include <cstdlib>

Dialog::Dialog(MainWindow *win, const QString &title) : QDialog(win) {
    setWindowTitle(title);
    setAttribute(Qt::WA_DeleteOnClose);

    dialogLayout = new QVBoxLayout(this);
    dialogLayout->setContentsMargins(25, 25, 25, 25);

    okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &Dialog::close);
    dialogLayout->addWidget(okButton, 0, Qt::AlignCenter);
}

AboutDialog::AboutDialog(MainWindow *win) : Dialog(win, "About DioSolver") {
    setModal(true);

    QTextBrowser *editor = new QTextBrowser(this);
    editor->setMinimumSize(420, 400);
    editor->setOpenExternalLinks(true);
    dialogLayout->insertWidget(0, editor);

    editor->insertHtml("<b><span style='font-size: 16px'>LDE Solver</span></b><br>");
    editor->append("Developed by: Henry Zhao\n");

    editor->append("Relevant Theorems:");
    editor->append("  - Extended Euclidean Algorithm");
    editor->append("  - Bézout's Lemma");
    editor->append("  - Linear Diophantine Equation Theorem, Part 1");
    editor->append("  - Linear Diophantine Equation Theorem, Part 2\n");

    editor->append("Reference:");
    editor->append("  Language and Proofs in Algebra: An Introduction");
    editor->append("  Version 1.3");
    editor->append("  © Faculty of Mathematics, University of Waterloo");
    editor->append("  September 21, 2024\n\n");

    editor->insertHtml("Built with <a href=www.qt.io>Qt 6.8.0</a>");
    editor->insertHtml("<br><br>");
    editor->insertHtml("Visit <a href=github.com/HenryZhao2020>My GitHub</a>");
}

ResultDialog::ResultDialog(MainWindow *win, LDE lde) : Dialog(win, "Result") {
    QTextEdit *editor = new QTextEdit(this);
    editor->setReadOnly(true);
    editor->setMinimumSize(400, 400);
    dialogLayout->insertWidget(0, editor);

    List result = lde_result(lde);
    for (int i = 0; i < result.size; ++i) {
        char *line = list_at(result, i, char*);
        editor->insertPlainText(line);
        free(line);
    }
    list_free(result);
}
