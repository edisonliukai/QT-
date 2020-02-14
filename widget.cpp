#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(300,540);
    this->setWindowTitle("Snake");
    //设置窗口为黑色
    setStyleSheet("QWidget{background:black}");
    //setWindownOpacity(0,8);//设置窗口透明度
    snake.append(QRectF(200,500,snakeNodeWidth,snakeNodeHeight));
        addTopRectF();
        addTopRectF();
        //首先生成一个奖励节点
        rewardNode.append(QRectF(100,100,snakeNodeWidth,snakeNodeWidth));

        timer = new QTimer;
        connect(timer, SIGNAL(timeout()),this,SLOT(timeOut()));
        //timer->start(time);

        rewardTimer = new QTimer;
        connect(rewardTimer,SIGNAL(timeout()),this,SLOT(rewardTimeOut()));
        //rewardTimer->start(time*30);


}
//界面刷新
void Widget::timeOut()
{
    int flage = 1;
    for(int i=0; i<rewardNode.length(); i++){
        if(rewardNode.at(i).contains(snake.at(0).topLeft()+QPointF(snakeNodeWidth/2,snakeNodeHeight/2))){
        //if(snake.at(0).contains(rewardNode.at(i).x()+rewardNode.at(i).width()/2,rewardNode.at(i).y()+rewardNode.at(i).height()/2)){
            if(rewardNode.at(i).width()>snakeNodeWidth){//额外奖励
                flage += 2;
            }
            flage++;//正常奖励
            rewardNode.removeAt(i);
            break;
        }
    }
    while(flage--){
        switch (moveFlage) {
        case Up:
            addTopRectF();
            break;
        case Down:
            addDownRectF();
            break;
        case Right:
            addRightRectF();
            break;
        case Left:
            addLeftRectF();
            break;
        default:
            break;
        }
    }
    deleteLastRectF();

    update();
}
//随机奖励生成
void Widget::rewardTimeOut()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    if(rewardNode.length() > 5){
        rewardNode.removeAt(qrand()%5);
    }
    rewardNode.append(QRectF(qrand()%(this->width()/20)*20,qrand()%(this->height()/20)*20,snakeNodeWidth,snakeNodeWidth));
    if(qrand()%5 == 3){
        rewardNode.append(QRectF(qrand()%(this->width()/20)*20-5,qrand()%(this->height()/20)*20-5,snakeNodeWidth*2,snakeNodeWidth*2));
    }

}
//向上移动
void Widget::addTopRectF()
{

        snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(0,-snakeNodeHeight),snake.at(0).topRight()));

}
//向下移动
void Widget::addDownRectF()
{

        snake.insert(0,QRectF(snake.at(0).bottomLeft(),snake.at(0).bottomRight()+QPointF(0,snakeNodeHeight)));

}
//向左移动
void Widget::addLeftRectF()
{



  snake.insert(0,QRectF(snake.at(0).topLeft()+QPointF(-snakeNodeWidth,0),snake.at(0).bottomLeft()));

}
//向右移动
void Widget::addRightRectF()
{

  snake.insert(0,QRectF(snake.at(0).topRight(),snake.at(0).bottomRight()+QPointF(snakeNodeWidth,0)));

}
//删除结尾数据
void Widget::deleteLastRectF()
{
    snake.removeLast();

}
//绘图
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    QFont font("方正舒体",12,QFont::ExtraLight,false);

    //反锯齿
    painter.setRenderHint(QPainter::Antialiasing);

    pen.setColor(Qt::black);
    brush.setColor(Qt::green);
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);

    for(int i=0; i<snake.length(); i++){
        painter.drawRect(snake.at(i));
    }
    brush.setColor(Qt::red);
    painter.setBrush(brush);
    for(int i=0; i<rewardNode.length(); i++){
        painter.drawEllipse(rewardNode.at(i));
    }
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.setFont(font);
    painter.drawText(20,20,QString("当前得分：")+QString("%1").arg(snake.length()));
    painter.drawText(400,20,QString("开发者：刘凯  时间： 2020年02月14日15时20分"));
    if(snakeStrike()){
        gameEnd();
        painter.setFont(font);
        painter.drawText((this->width()-300)/2,(this->height()-30)/2,QString("GAME OVER!"));
    }
    if(snake.at(0).x()-snakeNodeWidth < 0){//向左边撞墙

        gameEnd();
        painter.setFont(font);
        painter.drawText((this->width()-300)/2,(this->height()-30)/2,QString("GAME OVER!"));
    }
    if(snake.at(0).x()+snakeNodeWidth*2 > this->width()){//向右边撞墙

        gameEnd();
        painter.setFont(font);
        painter.drawText((this->width()-300)/2,(this->height()-30)/2,QString("GAME OVER!"));
    }
    if(snake.at(0).y()+snakeNodeHeight*2 > this->height()){//向下撞墙

        gameEnd();
        painter.setFont(font);
        painter.drawText((this->width()-300)/2,(this->height()-30)/2,QString("GAME OVER!"));
    }
    if(snake.at(0).y()-snakeNodeHeight < 0){//向上撞墙
        //snake.insert(0,QRectF(QPointF(snake.at(0).x(),this->height()-snakeNodeHeight),
        //QPointF(snake.at(0).x()+snakeNodeWidth,this->height())));
        gameEnd();
        painter.setFont(font);
        painter.drawText((this->width()-300)/2,(this->height()-30)/2,QString("GAME OVER!"));
    }
    QWidget::paintEvent(event);

}
//按键事件
void Widget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
    case Qt::Key_Up:
        if(moveFlage != Down){
            moveFlage = Up;
        }
        break;
    case Qt::Key_Down:
        if(moveFlage != Up){
            moveFlage = Down;
        }
        break;
    case Qt::Key_Right:
        if(moveFlage != Left){
            moveFlage = Right;
        }
        break;
    case Qt::Key_Left:
        if(moveFlage != Right){
            moveFlage = Left;
        }
        break;
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if(gameOver){
            snake.clear();
            rewardNode.clear();
            moveFlage = Up;
            snake.append(QRectF(200,500,snakeNodeWidth,snakeNodeHeight));
            addTopRectF();
            addTopRectF();
            //首先生成一个奖励节点
            rewardNode.append(QRectF(100,100,snakeNodeWidth,snakeNodeWidth));
            timer->start(time);
            rewardTimer->start(time*30);
            gameOver = false;
        }
        break;
    case Qt::Key_Space:
        if(gameStart && !gameOver){
            timer->start(time);
            rewardTimer->start(time*30);
            gameStart = false;
        }else if(!gameStart && !gameOver){
            timer->stop();
            rewardTimer->stop();
            gameStart = true;
        }
        break;
    default:
        break;
    }
}
//判断蛇身是否相撞
bool Widget::snakeStrike()
{
    for(int i=0; i<snake.length(); i++){
        for(int j=i+1; j<snake.length(); j++){
            if(snake.at(i) == snake.at(j)){
                return true;
            }
        }
    }
    return false;

}
void Widget::gameEnd()
{


    timer->stop();
    rewardTimer->stop();
    gameOver = true;
}

Widget::~Widget()
{
    delete ui;
}

