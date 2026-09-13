#ifndef DIOSOLVER_INT_LINE_EDIT_H
#define DIOSOLVER_INT_LINE_EDIT_H

#include <QLineEdit>

class IntLineEdit : public QLineEdit {
  public:
    explicit IntLineEdit(QWidget *parent = nullptr);

    [[nodiscard]] int value() const;
};

#endif // DIOSOLVER_INT_LINE_EDIT_H
