#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "card.h"
#include "foundation.h"


class Foundation;

class Card;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void updateDeck(int n);

    void moveCard(int col, Card *card);

    int cards = 0;
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QVector<Card *> deck;
    std::vector<std::vector<Card *>> tableau;
    std::vector<Foundation *> foundations;
    void createAndDealDeck();
};

#endif
