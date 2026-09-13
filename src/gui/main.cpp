#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

void loadStyleSheet(const QString &filename) {
    QFile file(filename);
    file.open(QFile::ReadOnly | QFile::Text);
    qApp->setStyleSheet(file.readAll());
    file.close();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/font/jetbrains_mono_variable.ttf");
    loadStyleSheet(":/conf/styles.qss");

    MainWindow win;
    win.show();

    return app.exec();
}
