#include "widget.h"
#include "ui_widget.h"

QFont fon;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m(nullptr)
{
    ui->setupUi(this);
    this->setFixedSize(690,520);
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background,QColor(0xF5F6F7));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->setWindowTitle("Mine Sweeper");

    minesX = minesY = 0;

    initializeItems();

    showGreeter();
}

Widget::~Widget()
{
    for(unsigned i=0; i<greetButtons.size(); ++i){
        delete greetButtons[i];
    }

    delete ui;
}

void Widget::showGreeter(){
    if(!mines.empty()){
        for(unsigned i=0;i<mines.size(); ++i){
            for(unsigned j=0;j<mines[0].size(); ++j){
                mines[i][j]->setGeometry(0,0,0,0);
            }
        }
    }

    for(int i=4; i<7; ++i){
        greetButtons[i]->setGeometry(0,0,0,0);
    }

    greetButtons[0]->setGeometry(105,20,230,230);
    greetButtons[1]->setGeometry(355,20,230,230);
    greetButtons[2]->setGeometry(105,270,230,230);
    greetButtons[3]->setGeometry(355,270,230,230);

    minesX = minesY = 0;
}

void Widget::showMines(int x, int y){
    minesX = x, minesY = y;
    for(int i=0; i < 4; ++i){
        greetButtons[i]->setGeometry(1,1,0,0);
    }

    if(!mines.empty()){
        for(unsigned i=0;i<mines.size(); ++i){
            for(unsigned j=0;j<mines[0].size(); ++j){
                mines[i][j]->setGeometry(0,0,0,0);
            }
        }
    }

    int w(480/x),h(480/y);
    for(int i = 0; i < x; ++i){
        for(int j = 0; j < y; ++j){;
            mines[i][j]->setGeometry(20+i*h,20+j*h,w-3,h-3);

            mines[i][j]->setStyleSheet("QPushButton{border-image:url(:/Ico/back)}"
                                       "QPushButton:hover{border-image:url(:/Ico/back_1)}"
                                       "QPushButton:pressed{background-color:rgb(255, 255, 255); border-style: inset;}");
            mines[i][j]->setIconSize(QSize(w,h));

            mines[i][j]->setText("");
            //mines[i][j]->setEnabled();
        }
    }

        greetButtons[4]->setGeometry(520,313,150,58);
        greetButtons[5]->setGeometry(520,375,150,58);
        greetButtons[6]->setGeometry(520,437,150,58);

        /////////////////////////////////////////
        ////////////////////////////////////////
        //mines[0][1]->setStyleSheet("QPushButton{border-image:url(:/Ico/back); image:url(:/Ico/flag);}");
        ////////////////////////////////////////
        //fon.setBold(true);
}

void Widget::drawMine(int x, int y){
    mines[x][y]->setStyleSheet("QPushButton{border-image:url(:/Ico/mine)}");
}

void Widget::drawDiscovered(int x, int y,int num){
    fon.setPixelSize(480/minesX*0.8);
    mines[x][y]->setStyleSheet("QPushButton{border-image:url(:/Ico/back_3)}");
    mines[x][y]->setFont(fon);
    QString n;
    if(num > 0){
        n = QString::number(num);
    }
    mines[x][y]->setText(n);
}

void Widget::drawFlag(int x, int y, bool s){
    if(s)
        mines[x][y]->setStyleSheet("QPushButton{border-image:url(:/Ico/back); image:url(:/Ico/flag);}");
    else {
        mines[x][y]->setStyleSheet("QPushButton{border-image:url(:/Ico/back); image:url(:/Ico/back);}");
    }
}

void Widget::refresh(){
    vector<vector<int>> landMap = m->getMap();
    for(int i=0; i < minesX; ++i){
        for(int j=0; j < minesX; ++j){
            if(landMap[i][j] >= 0){
                drawDiscovered(i,j,landMap[i][j]);
            }
            if(landMap[i][j] == -1){
                drawMine(i,j);
            }
        }
    }
}

void Widget::initializeItems(){
    QFont fon;
    QPalette pal(this->palette());
    fon.setPixelSize(20);
    pal.setColor(QPalette::Button,QColor(0xFBFBFC));
    for(int i(0); i<7; ++i){
        QPushButton *p = new QPushButton(this);
        pal = QPalette(p->palette());
        p->setPalette(pal);
        p->setFont(fon);
        greetButtons.push_back(p);
    }


    greetButtons[0]->setText(QString("8 X 8\n 10 mines "));
    greetButtons[1]->setText(QString("16 X 16\n 40 mines "));
    greetButtons[2]->setText(QString("24 X 24\n 99 mines "));
    greetButtons[3]->setText(QString("About"));

    QObject::connect(greetButtons[0], &QPushButton::clicked, this, &Widget::eightWidth);
    QObject::connect(greetButtons[1], &QPushButton::clicked, this, &Widget::sixteenWidth);
    QObject::connect(greetButtons[2], &QPushButton::clicked, this, &Widget::twentyfourWidth);
    QObject::connect(greetButtons[3], &QPushButton::clicked, this, &Widget::informations);


    fon.setPixelSize(16);
    greetButtons[4]->setText(QString("Again"));
    greetButtons[4]->setFont(fon);
    greetButtons[5]->setText(QString("Back"));
    greetButtons[5]->setFont(fon);
    greetButtons[6]->setText(QString("AI"));
    greetButtons[6]->setFont(fon);

    QObject::connect(greetButtons[4], &QPushButton::clicked, this, &Widget::again);
    QObject::connect(greetButtons[5], &QPushButton::clicked, this, &Widget::back);
    QObject::connect(greetButtons[6], &QPushButton::clicked, this, &Widget::ai);

    for(int i=0; i<32; ++i){
        vector<myButton*> tempButtons;
        for(int j=0; j<32;++j){
            myButton *p=new myButton(this);
            p->setGeometry(0,0,0,0);
            QObject::connect(p, &myButton::clicked, this, &Widget::mineClicked);
            QObject::connect(p, &myButton::rightButtonClick, this, &Widget::mineRightClicked);
            tempButtons.push_back(p);
        }
        mines.push_back(tempButtons);
    }
}

//void Widget::mousePressEvent(QMouseEvent *event){
//}

void Widget::eightWidth(){
    showMines(8,8);
    if(m!=nullptr)
        delete m;
    m = new mineSweeper(minesX);

}

void Widget::sixteenWidth(){
    showMines(16,16);
    if(m!=nullptr)
        delete m;
    m = new mineSweeper(minesX);
}

void Widget::twentyfourWidth(){
    showMines(24,24);
    if(m!=nullptr)
        delete m;
    m = new mineSweeper(minesX);
}

void Widget::informations(){
    QMessageBox::information(this,"About","<font size=\"14\">About</font>",QMessageBox::Button());
}

void Widget::again(){
    showMines(minesX,minesY);
    if(m!=nullptr)
        delete m;
    m = new mineSweeper(minesX);
}

void Widget::back(){
    showGreeter();
}

void Widget::ai(){
    autoPlay();
    //this->setWindowTitle(QString::number(p.x())+"," + QString::number(p.y()));
}

void Widget::mineClicked(){
    if(minesX == 0 || !m->clickAble())
        return;
    QPoint p = QCursor::pos();
    QRect r=this->geometry();

    int w(480/minesX),h(480/minesX);
    int x = p.x() - r.x();
    int y = p.y() - r.y();
    int boardX = (x - 20)/w;
    int boardY = (y - 20)/h;
    if(m->click(boardX,boardY)){
        refresh();
    }
    this->setWindowTitle(QString::number(boardX)+"," + QString::number(boardY));

    if(!m->clickAble()){
        if(m->getResult()>0){
            QMessageBox::information(this,"Message","You have cleared all the mines",QMessageBox::Button());
        }
        else {
            QMessageBox::information(this,"Message","You faild, please try again",QMessageBox::Button());
        }
    }
}

void Widget::mineRightClicked(){
    if(minesX == 0|| !m->clickAble())
        return;

    QPoint p = QCursor::pos();
    QRect r=this->geometry();
    int w(480/minesX),h(480/minesX);
    int x = p.x() - r.x();
    int y = p.y() - r.y();
    int boardX = (x - 20)/w;
    int boardY = (y - 20)/h;
    if(m->isFlag(boardX,boardY)){
        m->unputFlags(boardX,boardY);
        drawFlag(boardX, boardY, false);
    }
    else if (m->putFlags(boardX,boardY)) {
        drawFlag(boardX,boardY,true);
    }
    if(!m->clickAble()){
        if(m->getResult()>0){
            QMessageBox::information(this,"Message","You have cleared all the mines",QMessageBox::Button());
        }
        else {
            QMessageBox::information(this,"Message","You faild, please try again",QMessageBox::Button());
        }
    }
}

bool Widget::isInBoard(int x, int y)const{
        return x>=0 && x<minesX && y>=0 && y<minesY;
}

void Widget::autoPlay(){
    bool isPlayed = false;
    vector<vector<int>> board = m->getMap();

    int direction[8][2]={{1,0},
                         {1,1},
                         {0,1},
                         {-1,1},
                         {-1,0},
                         {-1,-1},
                         {0,-1},
                         {1,-1},
                        };

    queue<int> qx;
    queue<int> qy;
    for(int i = 0; i < minesX; ++i){
        for(int j = 0; j < minesX; ++j){
            if(board[i][j] > 0){
                qx.push(i);
                qy.push(j);
            }
        }
    }


    while(!qx.empty()){

        int i = qx.front();
        int j = qy.front();
        int n = board[i][j];
        int tx, ty;

        int ok[8] = {0};
        //unsure number
        int un=0;
        for(int k=0; k<8; ++k){
            tx = i + direction[k][0];
            ty = j + direction[k][1];
            if(isInBoard(tx,ty)){
                if(board[tx][ty] == -10){
                    ok[k] = -10;
                    ++un;
                }
            }
        }
        if(n == un){
            for(int k=0; k<8; ++k){
                if(ok[k] == -10){
                    tx = i + direction[k][0];
                    ty = j + direction[k][1];
                    if(!m->isFlag(tx,ty)){
                        isPlayed = true;
                        m->putFlags(tx,ty);
                        drawFlag(i + direction[k][0], j + direction[k][1], true);

                    }
                }
            }
        }else{
            int c = 0;
            for(int k=0; k<8; ++k){
                tx = i + direction[k][0];
                ty = j + direction[k][1];
                if(isInBoard(tx,ty) && m->isFlag(tx,ty)){
                    ok[k] = 0;
                    ++c;
                }
            }
            if(c == n){
                for(int k=0; k<8; ++k){
                    tx = i + direction[k][0];
                    ty = j + direction[k][1];
                    if(ok[k] == -10){
                        isPlayed = true;
                        m->click(tx,ty);
                        board = m->getMap();
                        drawDiscovered(tx,ty,board[tx][ty]);
                    }
                }
            }
        }

        qx.pop();
        qy.pop();
    }

    if(isPlayed)
        refresh();
    else{
        int x=0,y=0;
        if(m->getArecommand(x,y)){
            drawFlag(x,y,false);
            QMessageBox::information(this,"Message","No more good choice, I have helped you click one more step!",QMessageBox::Button());
            refresh();
        }
    }
    if(!m->clickAble()){
        if(m->getResult()>0){
            QMessageBox::information(this,"Message","You have cleared all the mines",QMessageBox::Button());
        }
        else {
            QMessageBox::information(this,"Message","You faild, please try again",QMessageBox::Button());
        }
    }
}
