#ifndef WIDGET_H
#define WIDGET_H

#include"minesweep.h"
#include <QWidget>
#include <QPushButton>
#include <vector>
#include <QColor>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPoint>
#include <QFont>
using namespace std;
namespace Ui {
class Widget;
}

class myButton:public QPushButton
{
    Q_OBJECT
public:
    myButton(QWidget *parent):QPushButton(parent) {}
    void mousePressEvent(QMouseEvent*e){
        if (Qt::LeftButton == e->button())
        {
            emit clicked();
        }

        else if (Qt::RightButton == e->button())
        {
            emit rightButtonClick();
        }
    }
signals:
    void rightButtonClick();
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    //void mousePressEvent(QMouseEvent *event);

private slots:
    void eightWidth();
    void sixteenWidth();
    void twentyfourWidth();
    void informations();
    void again();
    void back();
    void ai();
    void mineClicked();
    void mineRightClicked();
private:
    bool isInBoard(int x, int y)const;
    void showGreeter();
    void showMines(unsigned x, unsigned y);
    void initializeItems();
    void drawMine(int x, int y);
    void drawDiscovered(int x, int y,int num);
    void drawFlag(int x, int y, bool s);
    void refresh();

    void autoPlay();

private:
    Ui::Widget *ui;
    //QGridLayout *mineGrid;
    vector<QPushButton*> greetButtons;
    vector<vector<myButton*>> mines;
    unsigned minesX,minesY;
    mineSweeper *m;
};

#endif // WIDGET_H
