#include "foundation.h"
#include <QPen>

Foundation::Foundation(qreal x, qreal y, QGraphicsItem *parent, MainWindow *mainWindow)
        : QGraphicsRectItem(parent) {
    this->mainWindow = mainWindow;
    setRect(0, 0, 80, 100);
    setPos(x, y);
    setPen(QPen(Qt::DashDotLine));
    setBrush(Qt::NoBrush);
}

bool Foundation::canAcceptCard(Card *card) const {
    if (!card) return false;
    if (m_cards.isEmpty()) {
        return card->getValue() == 1;
    }
    Card *top = m_cards.back();
    bool canAccept = card->getSuit() == top->getSuit() &&
                     card->getValue() == top->getValue() + 1;
    return canAccept;
}

void Foundation::addCard(Card *card) {
    if (!card) return;
    setZValue(15);
    card->setInFoundation();
    m_cards.append(card);
    mainWindow->updateDeck(card->getTNumber());
    card->setPos(pos());
    card->setFaceUp(true);
    card->setZValue(index++);
    card->setFlag(QGraphicsItem::ItemIsMovable, false);
    card->setStack(nullptr);
}
