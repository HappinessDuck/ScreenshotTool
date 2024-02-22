#ifndef DRAWBOX_H
#define DRAWBOX_H
/************************************************************\
 * 模块名称： 截图工具-windows
 * 模块描述：
 *          截图工具
 * 模块功能：
 *
 * 著作权信息
 *      作者：wqj
 *      联系方式：QQ(1107097641)
 * 版本信息
 *       日期             版本           描述
 *   2024年01月04日      v1.0.0        截图到剪贴板
 *   2024年01月10日      v1.1.0        截图保存到指定文件路径，全局键盘钩子截图快捷键，ESC键取消截图
\************************************************************/

#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QClipboard>
#include <QScreen>
#include <QLabel>
#include <QMouseEvent>
#include <QPen>
#include <QPushButton>
#include <QHBoxLayout>
#include <QKeyEvent>

enum DrawStatus {
    waitDraw=0,
    drawing,
    drawed,
    drawCancel,
};


class DrawBox : public QWidget
{
    Q_OBJECT
public:
    explicit DrawBox(QWidget *parent = nullptr);

    void setFullPixmap(QPixmap &px);

    void canvasInit();
    void initToolBar();
//    void showToolBar();
    void hideToolBar();

    void exitDraw();
    QRect getRectF(QPoint p1, QPoint p2);


public slots:
    void slt_saveFile();
    void slt_saveClipboard();

private:

    QClipboard *clipboard;      //剪贴板
    QPixmap fullPixmap;         //原始全屏图片
    int screen_width=0;
    int screen_height=0;


    QPoint currentPos;  //鼠标当前点

    QPoint pointS;     //鼠标绘制起点
    QPoint pointE;     //鼠标绘制终点
    QRect shotArea;    //截图区域x,y,w,h


    DrawStatus rectFlag=DrawStatus::waitDraw;
    quint8 drawEditFlag=0;          //绘图修改

    //draw
    QPen drawPen;                   //线宽颜色风格

    QWidget *toolBar;           //工具条
    QHBoxLayout *layout = new QHBoxLayout;
    QPushButton *btn_saveFile;  //下载截图按钮
    QPushButton *btn_saveClipboard;//完成按钮
//    QPushButton *btn_cancel;    //取消

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);

    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);
signals:
    void closed();
};

#endif // DRAWBOX_H
