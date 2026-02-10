#ifndef INTERACTABLEFRAME_H
#define INTERACTABLEFRAME_H

#include <QWidget>

namespace Ui {
class interactableFrame;
}

class interactableFrame : public QWidget
{
    Q_OBJECT

public:
    explicit interactableFrame(QWidget *parent = nullptr);
    ~interactableFrame();

private:
    Ui::interactableFrame *ui;
};

#endif // INTERACTABLEFRAME_H
