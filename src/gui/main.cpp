#include "main_window.h"

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

static void loadStyleSheet(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Cannot load application stylesheet";
        return;
    }
    qApp->setStyleSheet(file.readAll());
    file.close();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/font/jetbrains_mono_variable.ttf");
    loadStyleSheet(":/conf/styles.qss");

    MainWindow win;
    win.show();

    return QApplication::exec();
}
