#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>

class QTextEdit;

class HelpDialog : public QDialog {
Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
};

#endif // HELPDIALOG_H
