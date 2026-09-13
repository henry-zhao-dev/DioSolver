#include "int_line_edit.h"

#include <QIntValidator>

IntLineEdit::IntLineEdit(QWidget *parent) : QLineEdit(parent) {
    const auto *intValidator = new QIntValidator(this);
    setValidator(intValidator);
}

int IntLineEdit::value() const {
    return text().toInt();
}
