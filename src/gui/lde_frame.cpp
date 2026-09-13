#include "lde_frame.h"

#include "int_line_edit.h"
#include "main_window.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

DomainBox::DomainBox(const QString &title, QWidget *parent)
    : QGroupBox(title, parent), intervalBox(new QComboBox(this)),
      intervalFrame(new IntervalFrame(this)) {

    intervalBox->addItems({"Real", "Positive", "Negative", "Nonpositive",
                           "Nonnegative", "Custom"});
    intervalFrame->setVisible(false);

    auto *vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(5);
    vboxLayout->setContentsMargins(10, 10, 10, 10);
    vboxLayout->addWidget(intervalBox);
    vboxLayout->addWidget(intervalFrame);

    // Only show interval frame if user chooses "Custom"
    connect(intervalBox, &QComboBox::currentTextChanged, this,
            [this](const QString &text) {
                intervalFrame->setVisible(text == "Custom");
            });
}

Interval DomainBox::interval() const {
    switch (intervalBox->currentIndex()) {
    case 0:
        return REAL_INTERVAL;
    case 1:
        return POSITIVE_INTERVAL;
    case 2:
        return NEGATIVE_INTERVAL;
    case 3:
        return NONPOSITIVE_INTERVAL;
    case 4:
        return NONNEGATIVE_INTERVAL;
    default:
        return intervalFrame->customInterval();
    }
}

IntervalFrame::IntervalFrame(DomainBox *domainBox)
    : QFrame(domainBox), fromField(new IntLineEdit(this)),
      toField(new IntLineEdit(this)),
      leftEndpointBox(new QCheckBox("Include Left Endpoint", this)),
      rightEndpointBox(new QCheckBox("Include Right Endpoint", this)) {

    auto *gridLayout = new QGridLayout(this);
    gridLayout->setVerticalSpacing(5);

    fromField->setPlaceholderText("-Infinity");
    gridLayout->addWidget(new QLabel("From:  ", this), 0, 0);
    gridLayout->addWidget(fromField, 0, 1);

    toField->setPlaceholderText("Infinity");
    gridLayout->addWidget(new QLabel("To:", this), 1, 0);
    gridLayout->addWidget(toField, 1, 1);

    gridLayout->addWidget(leftEndpointBox, 0, 2);
    gridLayout->addWidget(rightEndpointBox, 1, 2);
}

Interval IntervalFrame::customInterval() const {
    const int low = fromField->text().isEmpty() ? NEG_INF : fromField->value();
    const int high = toField->text().isEmpty() ? POS_INF : toField->value();
    const bool left_open = !leftEndpointBox->isChecked();
    const bool right_open = !rightEndpointBox->isChecked();

    return make_interval(low, high, left_open, right_open);
}

LDEFrame::LDEFrame(QWidget *parent)
    : QFrame(parent), aField(new IntLineEdit(this)),
      bField(new IntLineEdit(this)), cField(new IntLineEdit(this)),
      xBox(new DomainBox("Domain of x", this)),
      yBox(new DomainBox("Domain of y", this)) {

    auto *gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setVerticalSpacing(30);

    gridLayout->addWidget(new QLabel("a:", this), 0, 0);
    gridLayout->addWidget(aField, 0, 1);
    gridLayout->addWidget(new QLabel("b:", this), 0, 2);
    gridLayout->addWidget(bField, 0, 3);
    gridLayout->addWidget(new QLabel("c:", this), 0, 4);
    gridLayout->addWidget(cField, 0, 5);

    gridLayout->addWidget(xBox, 1, 0, 1, 6);
    gridLayout->addWidget(yBox, 2, 0, 1, 6);
}
