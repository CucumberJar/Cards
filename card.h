#ifndef CARD_H
#define CARD_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QColor>
#include "mainwindow.h"
#include <QMessageBox>

class Stack;

class Foundation;

class MainWindow;

class Card : public QGraphicsRectItem {
public:
    Card(QString label, int value, QString suit, QColor textColor, QGraphicsItem *parent = nullptr);

    Card(QString label, int value, QString suit, QColor textColor, QGraphicsItem *parent, int tNumber,
         MainWindow *mainWindow1 = nullptr);

    void setTNumber(int number);

    int getTNumber();

    void setFaceUp(bool up);

    bool isFaceUp() const;

    void setStack(Stack *stack);

    Stack *getStack() const;

    QString getSuit() const { return suit; }

    int getValue() const { return value; }

    bool getInFoundation();

    void setInFoundation();


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QVector<QPointF> originalPositions;
    std::vector<Card *> movingGroup;
    QVector<qreal> oldZValues;
    int oldZValue;
    bool inFoundation = false;
    QString label;
    int value;
    QString suit;
    QColor color;
    bool faceUp;
    int tNumber;
    QGraphicsTextItem *textItem;
    QPointF originalPos;
    MainWindow *mainWindow;
    Stack *m_stack = nullptr;

    bool check(Card *card1, Card *card);


};

#endif
