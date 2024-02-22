//#pragma execution_character_set("utf-8")//解决中文乱码
#include "widget.h"
#include "ui_widget.h"
#include <QCloseEvent>
#include <QDebug>
#include <QScreen>
#include <QShortcut>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    initTray();

    //注册windows全局键盘钩子
    hook.installHook();
    connect(&hook, &Hook::sendKeyType, this, &Widget::shot_start);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initTray()
{
    //创建菜单对象和托盘图标对象
    m_pTrayMenu = new QMenu(this);
    m_pSystemTray = new QSystemTrayIcon(this);
    //需要添加图标文件，不然托盘不会显示图标
    m_pSystemTray->setIcon(QIcon("://app.jpg"));

    //创建菜单项
    m_pActionSetting = new QAction(tr("Setting"), this);
    m_pActionQuit = new QAction(tr("Exit"), this);

    //添加菜单项
    m_pTrayMenu->addAction(m_pActionSetting);
    m_pTrayMenu->addAction(m_pActionQuit);
    m_pSystemTray->setContextMenu(m_pTrayMenu);

    //    connect(a1,SIGNAL(triggered(bool)),this,SLOT(onA1(bool)));
    connect(m_pActionQuit,SIGNAL(triggered(bool)),this,SLOT(onExitActionTriggered()));

    // 关联拖盘事件
    connect(m_pSystemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(onActivated(QSystemTrayIcon::ActivationReason)));
    m_pSystemTray->show();

    //    // 要在show之后调用,托盘弹窗
    //    m_pSystemTray->showMessage("showMessage","exe is start",QSystemTrayIcon::MessageIcon::Information,500);

}

void Widget::shot_start()
{
    if(!isShot)
    {
        isShot = true;
        QScreen *screen = QGuiApplication::primaryScreen();
        fullPixmap = screen->grabWindow(0);
        drawBox = new DrawBox();     //创建画布
        drawBox->setFullPixmap(fullPixmap);  //传递全屏背景图片
        connect(drawBox, &DrawBox::closed, this, &Widget::onDrawClosed);
    }
}


//拦截桌面窗口事件
void Widget::closeEvent(QCloseEvent *event)
{
    qDebug()<<"exit***";

//    QApplication::quit(); // 退出整个程序
//    return;
    if (m_pSystemTray->isVisible())
    { // 如果系统托盘可见
        hide(); // 隐藏窗口
        event->ignore(); // 忽略关闭事件的默认处理方式
    }/* else {
        event->accept(); // 否则接受关闭事件的默认处理方式，即关闭窗口
    }*/
}

void Widget::onActivated(QSystemTrayIcon::ActivationReason action)
{
    if(action==QSystemTrayIcon::ActivationReason::DoubleClick)
    {
        show();
    }
    //    else if(action==QSystemTrayIcon::ActivationReason::Context || action==QSystemTrayIcon::ActivationReason::Trigger){
    //        qDebug()<<"鼠标单击";
    //    }
    //    if(action==QSystemTrayIcon::ActivationReason::Unknown){
    //        qDebug()<<"未知点击";
    //    }

    //    else if(action==QSystemTrayIcon::ActivationReason::DoubleClick){
    //        qDebug()<<"鼠标双击";
    //    }
    //    else if(action==QSystemTrayIcon::ActivationReason::MiddleClick){
    //        qDebug()<<"鼠标中间单击";
    //    }else{
    //        return;
    //    }
}

void Widget::onExitActionTriggered()
{
    QApplication::quit(); // 退出整个程序
}


void Widget::on_pushButton_clicked()
{
    shot_start();

}

void Widget::onDrawClosed()
{
    delete drawBox;
    drawBox = nullptr;
    isShot = false;
}
