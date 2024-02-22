#include "drawbox.h"
#include <QDebug>
#include <QPainter>
#include <QHBoxLayout>
#include <QDateTime>
#include <QFileDialog>
#include <QStandardPaths>
DrawBox::DrawBox(QWidget *parent) : QWidget(parent)
{
    //    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);

    QScreen *primaryScreen = QGuiApplication::primaryScreen();

    //    QPixmap originalPixmap = screen->grabWindow(0,rect.topLeft().x(),rect.topLeft().y(),rect.width(),rect.height());

    QRect screenGeometry = primaryScreen->geometry();
    screen_width=screenGeometry.width();
    screen_height=screenGeometry.height();

    //    this->setWindowFlags(Qt::WindowStaysOnTopHint);     //窗口置顶q


    this-> setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    this->raise();                      //软置顶
    this->showFullScreen();             //画布全屏显示

    clipboard = QApplication::clipboard();   //获取系统剪贴板指针

    setMouseTracking(true);                 //鼠标移动捕捉


    canvasInit();

    initToolBar();

    //        drawEditFlag=0;
}

void DrawBox::setFullPixmap(QPixmap &px)
{
    fullPixmap = px;
}

void DrawBox::canvasInit()
{
    //    currentPos.setX(0);
    //    currentPos.setY(0);
    shotArea.setRect(0,0,0,0);
    pointS.rx()=0;
    pointS.ry()=0;
    pointE.rx()=0;
    pointE.ry()=0;

    //set pen
    drawPen.setBrush(Qt::green);
    drawPen.setWidthF(2);
    drawPen.setStyle(Qt::SolidLine);
}

void DrawBox::initToolBar()
{

    toolBar = new QWidget(this);
    btn_saveClipboard=new QPushButton("Copy");
    btn_saveFile=new QPushButton("Save");

    btn_saveClipboard->setFixedSize(50, 50);
    btn_saveFile->setFixedSize(50, 50);

    btn_saveClipboard->setStyleSheet("background-color: rgb(255, 255, 255);");
    btn_saveFile->setStyleSheet("background-color: rgb(255, 255, 255);");

    layout->addWidget(btn_saveFile);
    layout->addWidget(btn_saveClipboard);


    toolBar->setLayout(layout);

    //隐藏工具栏
    toolBar->setVisible(false);


    connect(btn_saveClipboard,SIGNAL(clicked(bool)),this,SLOT(slt_saveClipboard()));
    connect(btn_saveFile,SIGNAL(clicked(bool)),this,SLOT(slt_saveFile()));



}

void DrawBox::hideToolBar()
{
    toolBar->setVisible(false);
}

void DrawBox::exitDraw()
{
    rectFlag=DrawStatus::waitDraw;
    this->close();
}


//根据2点坐标获取矩形
QRect DrawBox::getRectF(QPoint p1, QPoint p2)
{
    float x1,y1,x2,y2;
    if(p1.x()<p2.x())
    {
        x1=p1.x();
        x2=p2.x();
    }
    else
    {
        x1=p2.x();
        x2=p1.x();
    }

    if(p1.y()<p2.y())
    {
        y1=p1.y();
        y2=p2.y();
    }
    else
    {
        y1=p2.y();
        y2=p1.y();
    }

    QPoint ps(x1,y1);
    QPoint pe(x2,y2);
    QRect rect(ps.x(), ps.y(), pe.x() - ps.x(), pe.y() - ps.y());
    return rect;
}


void DrawBox::slt_saveClipboard()
{
    clipboard->setPixmap(fullPixmap.copy(shotArea));
    exitDraw();
}

void DrawBox::slt_saveFile()
{
    QPixmap croppedPixmap = fullPixmap.copy(shotArea);


    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyyMMdd_hhmmsszzz");
    QString savefile="wx_"+current_date+".jpg";


    // 获取桌面路径
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    // 设置默认保存路径和文件名
    QString defaultPath = desktopPath + "/" + savefile;

    // 打开文件管理器，让用户选择保存路径
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Save Image", defaultPath, "JPEG Image (*.jpg)");

    if (!filePath.isEmpty()) {
        // 将QPixmap转换为QImage，并保存为JPG格式的图像文件
        QImage image = croppedPixmap.toImage();
        if (image.save(filePath, "JPG")) {
            qDebug() << "Image saved successfully";
        } else {
            qDebug() << "Failed to save image";
        }
    }
    exitDraw();
}


void DrawBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        // 按下即开始画
        if(rectFlag==DrawStatus::waitDraw)
        {
            //            qDebug()<<"start draw:"<<event->x()<<event->y();
            currentPos.setX(event->x());
            currentPos.setY(event->y());

            pointS.rx()=event->x();
            pointS.ry()=event->y();
            pointE.rx()=event->x();
            pointE.ry()=event->y();
            shotArea = getRectF(pointS, pointE);
            rectFlag=DrawStatus::drawing;
        }
        hideToolBar();

        update();
    }



}

void DrawBox::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()&Qt::LeftButton)
    {
        if(rectFlag==DrawStatus::drawing)
        {
            currentPos.setX(event->x());
            currentPos.setY(event->y());
            pointE.setX(event->x());
            pointE.setY(event->y());
            shotArea = getRectF(pointS, pointE);
        }

        update();
    }


}

void DrawBox::mouseReleaseEvent(QMouseEvent *event)
{

    //    if(event->button() == Qt::RightButton)
    //    {
    //        this->hide();
    //    }

    if (event->button() == Qt::LeftButton)
    {
        if(rectFlag==DrawStatus::drawing)
        {
            //鼠标释放，截图完毕
            pointE.setX(event->x());
            pointE.setY(event->y());
            shotArea = getRectF(pointS, pointE);
            rectFlag=DrawStatus::drawed;         //矩形绘制完成
            qDebug()<<"draw end:"<<event->x()<<event->y();
            //            showToolBar();

            toolBar->move(shotArea.topLeft().x(), shotArea.topLeft().y()+shotArea.height());
            toolBar->setVisible(true);



        }
    }
    else if (event->button() == Qt::RightButton)
    {
        // 按下鼠标右键取消截图，重新截图
        if(rectFlag == DrawStatus::waitDraw)
        {
            qDebug()<<"exit draw";
            exitDraw();
        }

        // 按下鼠标右键取消截图，重新截图
        if(rectFlag == DrawStatus::drawed)
        {
            qDebug()<<"draw cancel";
            canvasInit();
            rectFlag=DrawStatus::drawCancel;

            hideToolBar();

        }

    }
    update();
}

void DrawBox::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    QPixmap tempmask(screen_width, screen_width);
    tempmask.fill((QColor(0, 0, 0, 50)));//绘制画布，设置透明度10

    painter.drawPixmap(0,0,fullPixmap);     //先绘制全屏原图背景
    painter.drawPixmap(0,0,tempmask);       //然后绘制半透明背景，用来降低亮度

    if(rectFlag!=DrawStatus::drawCancel && rectFlag!=DrawStatus::waitDraw)
    {
        //矩形左上角坐标
        int shotAreaX = shotArea.topLeft().x();
        int shotAreaY = shotArea.topLeft().y();

        //矩形右下角坐标
        int shotAreaLowerX = shotAreaX + shotArea.width();
        int shotAreaLowerY = shotAreaY + shotArea.height();


        // 创建一个字体对象
        QFont font("Arial", 16); // 字体为Arial，大小为16
        // 设置字体
        painter.setFont(font);
        // 设置画笔颜色为白色（绘制文字的颜色）
        painter.setPen(Qt::white);

        // 设置画刷颜色为黑色（背景颜色）
        painter.setBrush(Qt::black);

        // 绘制矩形作为文字的背景
        painter.drawRect(shotAreaLowerX+20, shotAreaLowerY+10, 200, 50);

        // 在鼠标右下角添加坐标点信息
        QString positionStr = QString("POS:(%1, %2)").arg(currentPos.x()).arg(currentPos.y());
        painter.drawText(QRect(shotAreaLowerX+20, shotAreaLowerY+10, 200, 50), Qt::AlignLeft, positionStr);

        // 在鼠标右下角添加坐标点rgb信息
        QPixmap screenshot = QApplication::primaryScreen()->grabWindow(0);
        QRgb color = screenshot.toImage().pixel(currentPos);
        int red = qRed(color);
        int green = qGreen(color);
        int blue = qBlue(color);
        QString colorString = QString("RGB:(%1,%2,%3)").arg(red).arg(green).arg(blue);
        painter.drawText(QRect(shotAreaLowerX+20, shotAreaLowerY+30, 200, 50), Qt::AlignLeft, colorString);

        //绘制尺寸
        painter.drawText(QRect(shotAreaX, shotAreaY - 20, 200, 50), Qt::AlignLeft, QString("size:%1 X %2").arg(shotArea.width()).arg(shotArea.height()));

    }





    //draw
    switch(rectFlag)        //截图状态机
    {
    case DrawStatus::waitDraw:
    {
        break;
    }
    case DrawStatus::drawing:
    {
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(shotArea);            //然后绘制矩形框
        painter.drawPixmap(shotArea,fullPixmap,shotArea);     //然后将矩形框中的半透明图像替换成原图
        break;
    }
    case DrawStatus::drawed:
    {
        painter.setPen(QPen(Qt::green,2,Qt::DashLine));//设置画笔形式
        //painter.setBrush(Qt::white);
        painter.drawRect(shotArea);            //然后绘制矩形框
        painter.drawPixmap(shotArea,fullPixmap,shotArea);     //然后将矩形框中的半透明图像替换成原图
        break;
    }
    case DrawStatus::drawCancel:
    {
        rectFlag=DrawStatus::waitDraw;
        break;
    }
    default:
    {
        break;
    }
    }
}

void DrawBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
    {
        this->close();
    }
}

void DrawBox::closeEvent(QCloseEvent *event)
{
    emit closed();
}
