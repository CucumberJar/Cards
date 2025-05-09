#include "mainwindow.h"
#include <QRandomGenerator>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), scene(new QGraphicsScene(this)), view(new QGraphicsView(scene, this)) {
    setCentralWidget(view);
    view->setFixedSize(1200, 700);
    scene->setSceneRect(0, 0, 1200, 700);
    scene->setBackgroundBrush(QBrush(Qt::darkGreen));  // Или Qt::green
    createAndDealDeck();
}

MainWindow::~MainWindow() {}

void MainWindow::createAndDealDeck() {
    QStringList suits = {"♠", "♣", "♥", "♦"};
    QStringList values = {"A", "6", "7", "8", "9", "10", "J", "Q", "K"};

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
    tableau.resize(9);
    int index = 0;

    for (int col = 0; col < 9; ++col) {
        auto *card1 = new Card("1", 20, "1", Qt::green, nullptr, col);;
        tableau[col].push_back(card1);
        card1->setPos(50 + col * 120, 50);
        scene->addItem(card1);
        card1->setFlag(QGraphicsItem::ItemIsMovable, false);
        for (int row = 0; row < col; ++row) {
            Card *card = deck[index++];
            card->setFaceUp(true);
            card->setZValue(row);
            card->setPos(50 + col * 120, 50 + row * 30);
            scene->addItem(card);
            card->setTNumber(col);
            tableau[col].push_back(card);
        }
    }

    for (int i = 0; i < 4; ++i) {
        auto *foundation = new Foundation(50 + i * 100, 500, nullptr, this);
        scene->addItem(foundation);
        foundations.push_back(foundation);
    }
}


void MainWindow::updateDeck(int col) {
    tableau[col].pop_back();
    tableau[col].back()->setFaceUp(true);
    tableau[col].back()->setFlag(QGraphicsItem::ItemIsMovable, true);
}

void MainWindow::moveCard(int col, Card *card) {
    int old = card->getTNumber();
    card->setTNumber(col);
    if (tableau[col].back()->getValue() == 20) card->setPos(0, 0);
    card->setPos(50 + col * 120, 50 + (tableau[col].size() - 1) * 30);
    card->setZValue(tableau[col].size());
    updateDeck(old);
    tableau[col].back()->setFlag(QGraphicsItem::ItemIsMovable, false);
    tableau[col].push_back(card);
    card->setFaceUp(true);

}



