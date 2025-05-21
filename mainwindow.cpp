#include "mainwindow.h"
#include <QRandomGenerator>
#include <algorithm>
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), scene(new QGraphicsScene(this)), view(new QGraphicsView(scene, this)) {
    setCentralWidget(view);
    showFullScreen();
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int width = screenGeometry.width();
    int height = screenGeometry.height();
    view->setSceneRect(0, 0, width, height);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->setBackgroundBrush(QBrush(Qt::darkGreen));
    createAndDealDeck();
}

MainWindow::~MainWindow() {}

void MainWindow::moveGroup(int fromCol, int toCol, std::vector<Card *> group) {
    for (Card *card: group) {
        card->setTNumber(toCol);
        tableau[toCol].push_back(card);
    }
    for (Card *card: group) {
        removeCardFromColumn(fromCol, card);
    }
    for (int i = 1; i < tableau[toCol].size(); ++i) {
        tableau[toCol][i]->setZValue(i);
        tableau[toCol][i]->setPos(350 + toCol * 120, 250 + (i - 1) * 30);
    }

    if (!tableau[fromCol].empty()) {
        tableau[fromCol].back()->setZValue(tableau[fromCol].size());
        tableau[fromCol].back()->setFaceUp(true);
    }
}

void MainWindow::removeCardFromColumn(int col, Card *card) {
    auto &stack = tableau[col];
    auto it = std::find(stack.begin(), stack.end(), card);
    if (it != stack.end()) {
        stack.erase(it);
    }
    if (!stack.empty()) {
        Card *top = stack.back();
        if (!top->isFaceUp()) {
            top->setFaceUp(true);
            top->setFlag(QGraphicsItem::ItemIsMovable, true);
        }
    }
    card->setFlag(QGraphicsItem::ItemIsMovable, false);
    card->setFlag(QGraphicsItem::ItemIsSelectable, false);
}


void MainWindow::createAndDealDeck() {
    QStringList suits = {"♠", "♣", "♥", "♦"};
    QStringList values = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    for (const QString &suit: suits) {
        for (int i = 0; i < values.size(); ++i) {
            QString text = values[i] + " " + suit;
            QColor color = (suit == "♥" || suit == "♦") ? Qt::red : Qt::black;
            auto *card = new Card(text, i + 1, suit, color, nullptr, 0, this);
            deck.push_back(card);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    tableau.resize(8);
    int index = 0;
    for (int col = 0; col < 7; ++col) {
        auto *markerCard = new Card("1", 20, "1", Qt::green, nullptr, col, this);
        tableau[col].push_back(markerCard);
        markerCard->setPos(350 + col * 120, 250);
        scene->addItem(markerCard);
        markerCard->setFlag(QGraphicsItem::ItemIsMovable, false);

        for (int row = 0; row < col + 1; ++row) {
            Card *card = deck[index++];
            card->setZValue(row + 1);
            card->setPos(350 + col * 120, 250 + row * 30);
            scene->addItem(card);
            card->setTNumber(col);
            tableau[col].push_back(card);
        }
    }

    for (int i = 28; i < 52; ++i) {
        Card *card = deck[i];
        card->setFaceUp(true);
        card->setZValue(i - 28);
        card->setPos(350, 110);
        scene->addItem(card);
        card->setTNumber(7);
        tableau[7].push_back(card);
    }

    for (int i = 0; i < 7; ++i) {
        if (!tableau[i].empty())
            tableau[i].back()->setFaceUp(true);
    }

    for (int i = 0; i < 4; ++i) {
        auto *foundation = new Foundation(710 + i * 120, 110, nullptr, this);
        scene->addItem(foundation);
        foundations.push_back(foundation);
    }
}


void MainWindow::updateTableauPositions() {
    for (int col = 0; col < tableau.size() - 1; ++col) {
        for (int i = 1; i < tableau[col].size(); ++i) {
            tableau[col][i]->setPos(350 + col * 120, 250 + (i - 1) * 30);
            tableau[col][i]->setZValue(i);
            tableau[col][i]->setFlag(QGraphicsItem::ItemIsMovable,
                                     i == tableau[col].size() - 1 && tableau[col][i]->isFaceUp());
        }
    }
}


void MainWindow::updateDeck(int col) {
    tableau[col].pop_back();
    tableau[col].back()->setFaceUp(true);
    tableau[col].back()->setFlag(QGraphicsItem::ItemIsMovable, true);
}


void MainWindow::cardToBack(Card *card) {
    int col = card->getTNumber();
    auto &stack = tableau[col];
    auto it = std::find(stack.begin(), stack.end(), card);
    if (it != stack.end()) {
        stack.erase(it);
        stack.insert(stack.begin(), card);
    }
    for (int i = 0; i < stack.size(); ++i) {
        stack[i]->setZValue(i);
        stack[i]->setPos(350, 110);
    }
    card->setFaceUp(true);
    scene->removeItem(card);
    scene->addItem(card);
}





