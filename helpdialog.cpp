#include "HelpDialog.h"
#include <QVBoxLayout>
#include <QTextEdit>

HelpDialog::HelpDialog(QWidget *parent)
        : QDialog(parent) {
    setWindowTitle("Справка");
    resize(500, 400);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QTextEdit *helpText = new QTextEdit(this);
    helpText->setReadOnly(true);
    helpText->setText(
            "Добро пожаловать в игру 'Пасьянс'!\n\n"
            "Правила:\n"
            "- Соберите карты по мастям от туза до короля.\n"
            "- Перемещайте карты между колонками по убыванию и чередуя цвета.\n"
            "- Используйте верхнюю стопку для перетаскивания лишних карт.\n\n"
            "Управление:\n"
            "- Клик и перетаскивание мышью для перемещения карт.\n"
            "- Двойной клик — автоматическое перемещение в foundation, если возможно.\n\n"
            "Удачи!"
    );

    layout->addWidget(helpText);
    setLayout(layout);
}
