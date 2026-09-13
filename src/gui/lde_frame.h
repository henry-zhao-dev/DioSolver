#ifndef DIOSOLVER_LDE_FRAME_H
#define DIOSOLVER_LDE_FRAME_H

#include "int_line_edit.h"

#include <QGroupBox>
#include <diosolver/interval.h>

class IntervalFrame;
class QCheckBox;
class QComboBox;

class DomainBox : public QGroupBox {
    Q_OBJECT

  public:
    explicit DomainBox(const QString &title, QWidget *parent = nullptr);

    [[nodiscard]] Interval interval() const;

  private:
    QComboBox *intervalBox{};
    IntervalFrame *intervalFrame{};
};

class IntervalFrame : public QFrame {
    Q_OBJECT

  public:
    explicit IntervalFrame(DomainBox *domainBox);

    [[nodiscard]] Interval customInterval() const;

  private:
    IntLineEdit *fromField{};
    IntLineEdit *toField{};

    QCheckBox *leftEndpointBox{};
    QCheckBox *rightEndpointBox{};
};

class LDEFrame : public QFrame {
    Q_OBJECT

  public:
    explicit LDEFrame(QWidget *parent = nullptr);

    [[nodiscard]] int aValue() const {
        return aField->value();
    }

    [[nodiscard]] int bValue() const {
        return bField->value();
    }

    [[nodiscard]] int cValue() const {
        return cField->value();
    }

    [[nodiscard]] Interval xInterval() const {
        return xBox->interval();
    }

    [[nodiscard]] Interval yInterval() const {
        return yBox->interval();
    }

  private:
    IntLineEdit *aField{};
    IntLineEdit *bField{};
    IntLineEdit *cField{};

    DomainBox *xBox{};
    DomainBox *yBox{};
};

#endif // DIOSOLVER_LDE_FRAME_H
