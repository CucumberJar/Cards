#include "StartMenu.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>

StartMenu::StartMenu(QWidget *parent)
        : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QFont font("Arial", 18, QFont::Bold);
    playButton = new QPushButton("Играть");
    helpButton = new QPushButton("Справка");
    exitButton = new QPushButton("Выход");
    playButton->setFont(font);
    helpButton->setFont(font);
    exitButton->setFont(font);
    layout->addStretch();
    layout->addWidget(playButton);
    layout->addWidget(helpButton);
    layout->addWidget(exitButton);
    layout->addStretch();
    connect(playButton, &QPushButton::clicked, this, &StartMenu::startGame);
    connect(helpButton, &QPushButton::clicked, this, &StartMenu::showHelp);
    connect(exitButton, &QPushButton::clicked, this, &StartMenu::exitApp);

    setLayout(layout);
}
