#ifndef TRANSACTIONHISTORYWIDGET_H
#define TRANSACTIONHISTORYWIDGET_H

#include <QWidget>

namespace Ui {
class transactionHistoryWidget;
}

class transactionHistoryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit transactionHistoryWidget(QWidget *parent = nullptr);
    ~transactionHistoryWidget();

private:
    Ui::transactionHistoryWidget *ui;
};

#endif // TRANSACTIONHISTORYWIDGET_H
