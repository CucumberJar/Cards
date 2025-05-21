#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>

class QPushButton;

class StartMenu : public QWidget {
Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = nullptr);

signals:

    void startGame();

    void showHelp();

    void exitApp();

private:
    QPushButton *playButton;
    QPushButton *helpButton;
    QPushButton *exitButton;
};

#endif // STARTMENU_H
