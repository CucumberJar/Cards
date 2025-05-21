#include <QApplication>
#include "StartMenu.h"
#include "mainwindow.h"
#include "HelpDialog.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    StartMenu menu;
    menu.resize(400, 300);
    menu.setWindowTitle("Меню");
    menu.show();

    QObject::connect(&menu, &StartMenu::startGame, [&]() {
        MainWindow *w = new MainWindow();
        w->show();
        menu.close();
    });

    QObject::connect(&menu, &StartMenu::showHelp, [&]() {
        HelpDialog *help = new HelpDialog(&menu);
        help->exec();
    });

    QObject::connect(&menu, &StartMenu::exitApp, [&]() {
        a.quit();
    });

    return a.exec();
}
