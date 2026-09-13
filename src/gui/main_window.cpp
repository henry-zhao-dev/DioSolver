#include "main_window.h"

#include "dialog.h"
#include "int_line_edit.h"
#include "lde_frame.h"

#include <QLabel>
#include <QPushButton>
#include <diosolver/diophantine.h>

namespace {
class ButtonFrame : public QFrame {
  public:
    explicit ButtonFrame(MainWindow *win);
};
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), mainLayout(new QVBoxLayout(this)),
      ldeFrame(new LDEFrame(this)) {

    auto *titleLabel = new QLabel("Solve LDE: ax + by = c", this);
    titleLabel->setObjectName("title");
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(ldeFrame);

    auto *buttonFrame = new ButtonFrame(this);
    mainLayout->addStretch();
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonFrame);

    resize(450, height());
}

void MainWindow::clearLDE() {
    mainLayout->removeWidget(ldeFrame);
    ldeFrame->setParent(nullptr);
    ldeFrame->deleteLater();

    ldeFrame = new LDEFrame(this);
    mainLayout->insertWidget(2, ldeFrame);
}

LDE MainWindow::getLDE() const {
    const int a = ldeFrame->aValue();
    const int b = ldeFrame->bValue();
    const int c = ldeFrame->cValue();
    const Interval xInterval = ldeFrame->xInterval();
    const Interval yInterval = ldeFrame->yInterval();

    return (LDE){.a = a, .b = b, .c = c, .xi = xInterval, .yi = yInterval};
}

ButtonFrame::ButtonFrame(MainWindow *win) : QFrame(win) {
    auto *hboxLayout = new QHBoxLayout(this);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    auto *aboutButton = new QPushButton("About", this);
    connect(aboutButton, &QPushButton::clicked, this, [win] {
        auto *dialog = new AboutDialog(win);
        dialog->show();
    });
    hboxLayout->addWidget(aboutButton);

    auto *clearButton = new QPushButton("Clear", this);
    connect(clearButton, &QPushButton::clicked, win, &MainWindow::clearLDE);
    hboxLayout->addWidget(clearButton);
    hboxLayout->addStretch();

    auto *solveButton = new QPushButton("Solve", this);
    connect(solveButton, &QPushButton::clicked, this, [win] {
        auto *dialog = new ResultDialog(win, win->getLDE());
        dialog->show();
    });
    solveButton->setDefault(true);
    hboxLayout->addWidget(solveButton);
}
