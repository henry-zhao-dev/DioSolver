#ifndef DIOSOLVER_MAIN_WINDOW_H
#define DIOSOLVER_MAIN_WINDOW_H

#include <QBoxLayout>
#include <QMainWindow>

struct LDE;
class LDEFrame;

class MainWindow : public QWidget {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);

    void clearLDE();
    [[nodiscard]] LDE getLDE() const;

  private:
    QVBoxLayout *mainLayout;
    LDEFrame *ldeFrame;
};

#endif // DIOSOLVER_MAIN_WINDOW_H
