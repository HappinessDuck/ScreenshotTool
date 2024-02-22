#ifndef HOOK_H
#define HOOK_H
#include <windows.h>
#include <QObject>
/************************************************************\
 * 模块名称： 钩子-windows
 * 模块描述：
 *          windows使用全局键盘事件
 * 模块功能：
 *          全局监听键盘鼠标事件
 * 著作权信息
 *      作者：wqj
 *      联系方式：QQ(1107097641)
 * 版本信息
 *       日期             版本           描述
 *   2024年01月11日      v1.0.0        键盘事件钩子
\************************************************************/
class Hook : public QObject
{
    Q_OBJECT
public:
    explicit Hook(QObject *parent = nullptr);

    void installHook();//安装钩子函数
    void unInstallHook();//删除钩子
    void sendSignal();

signals:
    void sendKeyType();

};

#endif // HOOK_H
