#ifndef DIOSOLVER_DIALOG_H
#define DIOSOLVER_DIALOG_H

#include <QBoxLayout>
#include <QDialog>
#include <QPushButton>

struct LDE;
class MainWindow;

class Dialog : public QDialog {
    Q_OBJECT

  public:
    Dialog(MainWindow *win, const QString &title, bool modal = false);

  protected:
    QVBoxLayout *dialogLayout;
    QPushButton *okButton;
};

class AboutDialog : public Dialog {
    Q_OBJECT

  public:
    explicit AboutDialog(MainWindow *win);
};

class ResultDialog : public Dialog {
    Q_OBJECT

  public:
    ResultDialog(MainWindow *win, const LDE &lde);
};

#endif // DIOSOLVER_DIALOG_H
