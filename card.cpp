#include "card.h"
#include "foundation.h"
#include <QMessageBox>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QFont>
#include <QPen>
#include <QDebug>

Card::Card(QString label, int value, QString suit, QColor textColor, QGraphicsItem *parent, int tNumber,
           MainWindow *mainWindow1)
        : QGraphicsRectItem(parent),
          label(label),
          value(value),
          suit(suit),
          tNumber(tNumber),
          faceUp(false),
          mainWindow(mainWindow1),
          color(textColor),
          m_stack(nullptr) {

    setRect(0, 0, 80, 100);
    setBrush(Qt::gray);
    if (value == 20) {
        setBrush(Qt::green);
        setZValue(0);
    } else
        setPen(QPen(Qt::black, 2));
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setAcceptedMouseButtons(Qt::LeftButton);

    textItem = new QGraphicsTextItem(label, this);
    textItem->setDefaultTextColor(color);
    textItem->setFont(QFont("Arial", 14, QFont::Bold));
    textItem->setPos(10, 10);
    textItem->setVisible(false);
}

void Card::setFaceUp(bool up) {
    if (faceUp == up || getValue() == 20) return;
    //   qDebug() << "Cart111";
    faceUp = up;
    // qDebug() << "Card" << label << (up ? "face up" : "face down");

    // Печатаем дополнительные данные для диагностики
    //   qDebug() << "Brush color:" << (up ? "white" : "gray");
    //  qDebug() << "Value:" << value;
    //  qDebug() << "Text visibility:" << (up ? "visible" : "hidden");

    textItem->setVisible(up);
    setBrush(Qt::white);
    update();  // Перерисовать
}


bool Card::isFaceUp() const {
    return faceUp;
}

void Card::setStack(Stack *stack) {
    m_stack = stack;
}

Stack *Card::getStack() const {
    return m_stack;
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (!faceUp) return;
    oldZValue = zValue();
    setZValue(20);
    originalPos = pos();
    QGraphicsRectItem::mousePressEvent(event);
}

void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (!faceUp) return;
    QGraphicsRectItem::mouseMoveEvent(event);
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    if (!faceUp) return;
    QList<QGraphicsItem *> colliding = scene()->collidingItems(this);
    bool placed = false;

    for (QGraphicsItem *item: colliding) {
        if (auto *card1 = dynamic_cast<Card *>(item)) {
            if (check(card1, this)) {
                mainWindow->moveCard(card1->tNumber, this);
                placed = true;
            }
            break;
        } else if (auto *foundation = dynamic_cast<Foundation *>(item)) {
            qDebug() << "ISfoundation:" << label;
            if (foundation->canAcceptCard(this)) {
                foundation->addCard(this);
                qDebug() << "CARDS" << mainWindow->cards;
                mainWindow->cards++;
                qDebug() << "CARDS" << mainWindow->cards;
                if (mainWindow->cards == 36) QMessageBox::information(mainWindow, "Победа", "Ты выиграл!");
                setStack(nullptr);
                qDebug() << "Card placed in foundation:" << label;
                placed = true;
                break;
            }
        }
    }
    if (!placed) {
        setZValue(oldZValue);
        setPos(originalPos);
    }
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void Card::setTNumber(int number) {
    tNumber = number;
}

int Card::getTNumber() {
    return tNumber;
}

void Card::setInFoundation() {
    inFoundation = true;
}

bool Card::check(Card *card1, Card *card) {
    if (!card) return false;
    if (card1->value == 20) {
        return true;
    }
    bool canAccept = card1->getValue() == card->value + 1 &&
                     card1->color != card->color && card->getTNumber() != card1->tNumber;
    return canAccept;
}



