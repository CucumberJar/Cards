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
    faceUp = up;
    textItem->setVisible(up);
    setBrush(up ? Qt::white : Qt::gray);
    update();

    // Обновим флаг возможности перемещения только для открытой карты
    setFlag(QGraphicsItem::ItemIsMovable, faceUp);
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

    originalPositions.clear();
    oldZValues.clear();
    movingGroup.clear();

// СТЕК И КЛИК
    const auto &stack = mainWindow->tableau[tNumber];
    QPointF clickScenePos = event->scenePos();
    qDebug() << "Mouse click at scene pos:" << clickScenePos;

// Найдем карту под курсором — СВЕРХУ ВНИЗ
    int index = -1;
    for (int i = stack.size() - 1; i >= 0; --i) {
        Card *card = stack[i];
        if (!card->isFaceUp()) continue;

        QRectF sceneRect = card->mapToScene(card->boundingRect()).boundingRect();
        if (sceneRect.contains(clickScenePos)) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        qDebug() << "No card under mouse in stack" << tNumber;
        return;
    }

    qDebug() << "Clicked card at index" << index << "in stack" << tNumber;

// Собираем группу карт сверху от найденной
    int baseZ = 1000;
    for (int i = index; i < stack.size(); ++i) {
        Card *card = stack[i];
        if (!card->isFaceUp()) break;
        movingGroup.push_back(card);
        oldZValues.push_back(card->zValue());
        originalPositions.push_back(card->pos());
        card->setZValue(baseZ++);
    }
}


void Card::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (!faceUp) return;
    QPointF delta = event->scenePos() - event->lastScenePos();
    for (Card *c: movingGroup) {
        c->moveBy(delta.x(), delta.y());
    }
}


void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (!faceUp) return;
    bool placed = false;
    QList<QGraphicsItem *> colliding = scene()->collidingItems(this);
    for (QGraphicsItem *item: colliding) {
        if (auto *targetCard = dynamic_cast<Card *>(item)) {
            if (check(targetCard, this)) {
                mainWindow->moveGroup(tNumber, targetCard->getTNumber(), movingGroup);
                placed = true;
                break;
            }
        } else if (auto *foundation = dynamic_cast<Foundation *>(item)) {
            if (movingGroup.size() == 1 && foundation->canAcceptCard(this)) {
                foundation->addCard(this);
                mainWindow->cards++;
                qDebug() << this->getValue();
                this->setZValue(100 + mainWindow->cards);
                if (mainWindow->cards == 36)
                    QMessageBox::information(mainWindow, "Победа", "Ты выиграл!");
                setStack(nullptr);
                placed = true;
                break;
            }
        }
    }
    if (!placed) {
        for (int i = 0; i < movingGroup.size(); ++i) {
            movingGroup[i]->setZValue(oldZValues[i]);
            movingGroup[i]->setPos(originalPositions[i]);
        }
    }
    mainWindow->updateTableauPositions();
    movingGroup.clear();
    oldZValues.clear();
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

bool Card::check(Card *targetCard, Card *movingCard) {
    if (!targetCard || !movingCard) return false;
    if (targetCard->getTNumber() == 7) return false;
    if (targetCard->value == 20) {
        return movingCard->value == 13;
    }
    auto &col = mainWindow->tableau[targetCard->tNumber];
    if (col.empty()) return false;
    Card *lastCard = col.back();
    bool valueOk = (lastCard->value == movingCard->value + 1);
    bool colorOk = (lastCard->color != movingCard->color);

    return valueOk && colorOk;
}

void Card::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    if (tNumber == 7)
        mainWindow->cardToBack(this);
}
