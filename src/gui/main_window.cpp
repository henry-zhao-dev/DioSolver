#include "main_window.h"

#include "dialog.h"

#include <QGridLayout>
#include <QIntValidator>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include <diosolver/diophantine.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("DioSolver");

    QWidget *mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    mainLayout = new QVBoxLayout(mainWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *titleLabel = new QLabel("Solve LDE: ax + by = c", this);
    titleLabel->setObjectName("title");
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);

    ldeFrame = new LDEFrame(this);
    mainLayout->addWidget(ldeFrame);

    ButtonFrame *buttonFrame = new ButtonFrame(this);
    mainLayout->addStretch();
    mainLayout->addSpacing(50);
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

LDE MainWindow::solveLDE() {
    int a = ldeFrame->aValue();
    int b = ldeFrame->bValue();
    int c = ldeFrame->cValue();
    Interval xIntvl = ldeFrame->xInterval();
    Interval yIntvl = ldeFrame->yInterval();
    return (LDE){a, b, c, xIntvl, yIntvl};
}

IntLineEdit::IntLineEdit(QWidget *parent) : QLineEdit(parent) {
    QIntValidator *intValidator = new QIntValidator(this);
    setValidator(intValidator);
}

int IntLineEdit::value() {
    return text().toInt();
}

LDEFrame::LDEFrame(MainWindow *win) : QFrame(win) {
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setVerticalSpacing(30);

    aField = new IntLineEdit(this);
    gridLayout->addWidget(new QLabel("a:", this), 0, 0);
    gridLayout->addWidget(aField, 0, 1);

    bField = new IntLineEdit(this);
    gridLayout->addWidget(new QLabel("   b:", this), 0, 2);
    gridLayout->addWidget(bField, 0, 3);

    cField = new IntLineEdit(this);
    gridLayout->addWidget(new QLabel("   c:", this), 0, 4);
    gridLayout->addWidget(cField, 0, 5);

    xBox = new DomainBox("Domain of x", this);
    gridLayout->addWidget(xBox, 1, 0, 1, 6);

    yBox = new DomainBox("Domain of y", this);
    gridLayout->addWidget(yBox, 2, 0, 1, 6);
}

int LDEFrame::aValue() {
    return aField->value();
}

int LDEFrame::bValue() {
    return bField->value();
}

int LDEFrame::cValue() {
    return cField->value();
}

Interval LDEFrame::xInterval() {
    return xBox->interval();
}

Interval LDEFrame::yInterval() {
    return yBox->interval();
}

DomainBox::DomainBox(const QString &title, LDEFrame *frame)
    : QGroupBox(title, frame) {
    static const QStringList defined_intvls = {
        "Real", "Positive", "Negative", "Nonpositive", "Nonnegative", "Custom"};

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(5);
    vboxLayout->setContentsMargins(10, 10, 10, 10);

    intvlBox = new QComboBox(this);
    intvlBox->addItems(defined_intvls);
    vboxLayout->addWidget(intvlBox);

    intvlFrame = new IntervalFrame(this);
    intvlFrame->setVisible(false);
    vboxLayout->addWidget(intvlFrame);

    connect(intvlBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        static const int lastIndex = defined_intvls.size() - 1;
        intvlFrame->setVisible(index == lastIndex);
    });
}

Interval DomainBox::interval() {
    int index = intvlBox->currentIndex();
    switch (index) {
    case 0:
        return REAL;
    case 1:
        return POS;
    case 2:
        return NEG;
    case 3:
        return NONPOS;
    case 4:
        return NONNEG;
    }
    return intvlFrame->customInterval();
}

IntervalFrame::IntervalFrame(DomainBox *domainBox) : QFrame(domainBox) {
    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->setVerticalSpacing(5);

    fromBox = new IntLineEdit(this);
    fromBox->setPlaceholderText("-Infinity");
    gridLayout->addWidget(new QLabel("From:  ", this), 0, 0);
    gridLayout->addWidget(fromBox, 0, 1);

    leftBox = new QCheckBox("Include Left Endpoint", this);
    gridLayout->addWidget(leftBox, 0, 2);

    toBox = new IntLineEdit(this);
    toBox->setPlaceholderText("Infinity");
    gridLayout->addWidget(new QLabel("To:", this), 1, 0);
    gridLayout->addWidget(toBox, 1, 1);

    rightBox = new QCheckBox("Include Right Endpoint", this);
    gridLayout->addWidget(rightBox, 1, 2);
}

Interval IntervalFrame::customInterval() {
    int low = fromBox->text().isEmpty() ? NEG_INF : fromBox->value();
    int high = toBox->text().isEmpty() ? POS_INF : toBox->value();
    bool left_open = !leftBox->isChecked();
    bool right_open = !rightBox->isChecked();

    return make_interval(low, high, left_open, right_open);
}

ButtonFrame::ButtonFrame(MainWindow *win) : QFrame(win) {
    QHBoxLayout *hboxLayout = new QHBoxLayout(this);
    hboxLayout->setContentsMargins(0, 0, 0, 0);

    QPushButton *aboutButton = new QPushButton("About", this);
    connect(aboutButton, &QPushButton::clicked, this, [win] {
        AboutDialog *dialog = new AboutDialog(win);
        dialog->show();
    });
    hboxLayout->addWidget(aboutButton);

    QPushButton *clearButton = new QPushButton("Clear", this);
    connect(clearButton, &QPushButton::clicked, win, &MainWindow::clearLDE);
    hboxLayout->addWidget(clearButton);
    hboxLayout->addStretch();

    QPushButton *solveButton = new QPushButton("Solve", this);
    connect(solveButton, &QPushButton::clicked, this, [win] {
        ResultDialog *dialog = new ResultDialog(win, win->solveLDE());
        dialog->show();
    });
    solveButton->setDefault(true);
    hboxLayout->addWidget(solveButton);
}
