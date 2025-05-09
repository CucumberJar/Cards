#ifndef FOUNDATION_H
#define FOUNDATION_H

#include <QGraphicsRectItem>
#include <QVector>

#include "card.h"
#include "mainwindow.h"

class MainWindow;

class Card;

class Foundation : public QGraphicsRectItem {
public:
    Foundation(qreal x, qreal y, QGraphicsItem *parent = nullptr, MainWindow *mainWindow = nullptr);

    bool canAcceptCard(Card *card) const;

    void addCard(Card *card);

private:
    int index = 0;
    MainWindow *mainWindow;
    QVector<Card *> m_cards;
};

#endif
