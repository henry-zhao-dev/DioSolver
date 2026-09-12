#pragma once

#include <QMainWindow>
#include <QBoxLayout>
#include <QLineEdit>
#include <QFrame>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>

class LDEFrame;
class DomainBox;
class IntervalFrame;

struct Interval;
struct LDE;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void clearLDE();
    LDE solveLDE();

private:
    QVBoxLayout *mainLayout;
    LDEFrame *ldeFrame;
};

class IntLineEdit : public QLineEdit {
    Q_OBJECT

public:
    IntLineEdit(QWidget *parent = nullptr);

    int value();
};

class LDEFrame : public QFrame {
    Q_OBJECT

public:
    LDEFrame(MainWindow *win);

    int aValue();
    int bValue();
    int cValue();

    Interval xInterval();
    Interval yInterval();

private:
    IntLineEdit *aField;
    IntLineEdit *bField;
    IntLineEdit *cField;

    DomainBox *xBox;
    DomainBox *yBox;
};

class DomainBox : public QGroupBox {
    Q_OBJECT

public:
    DomainBox(const QString &title, LDEFrame *frame);

    Interval interval();

private:
    QComboBox *intvlBox;
    IntervalFrame *intvlFrame;
};

class IntervalFrame : public QFrame {
    Q_OBJECT

public:
    IntervalFrame(DomainBox *domainBox);

    Interval customInterval();

private:
    IntLineEdit *fromBox;
    IntLineEdit *toBox;

    QCheckBox *leftBox;
    QCheckBox *rightBox;
};

class ButtonFrame : public QFrame {
    Q_OBJECT

public:
    ButtonFrame(MainWindow *win);
};
