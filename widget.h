#ifndef WIDGET_H
#define WIDGET_H

#include "drawbox.h"
#include "hook.h"
#include <QWidget>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QPointer>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QMenu* m_pTrayMenu;            //系统托盘右键菜单项
    QSystemTrayIcon* m_pSystemTray; //系统托盘图标
    //右键菜单栏选项
    QAction* m_pActionSetting;
    QAction* m_pActionQuit;
    void initTray();

public slots:
    void shot_start();

private slots:
    void onActivated(QSystemTrayIcon::ActivationReason action);
    void closeEvent(QCloseEvent *event);
    void onExitActionTriggered();

    void on_pushButton_clicked();

    void onDrawClosed();


private:
    Ui::Widget *ui;
    QPixmap fullPixmap;
    DrawBox *drawBox = nullptr;


    bool isShot = false;

    Hook hook;//钩子对象

};
#endif // WIDGET_H
