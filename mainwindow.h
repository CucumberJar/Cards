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
    std::vector<std::vector<Card *>> tableau;

    MainWindow(QWidget *parent = nullptr);

    void updateTableauPositions();

    ~MainWindow();

    void removeCardFromColumn(int col, Card *card);

    void updateDeck(int n);

    void moveGroup(int fromCol, int toCol, std::vector<Card *> group);

    void moveCard(int col, Card *card);

    void cardToBack(Card *card);

    int cards = 0;
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QVector<Card *> deck;
    std::vector<Foundation *> foundations;

    void createAndDealDeck();


};

#endif
