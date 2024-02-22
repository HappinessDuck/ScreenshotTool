#include "hook.h"
#include <QDebug>


Hook::Hook(QObject *parent) : QObject(parent)
{

}

static HHOOK g_hook=nullptr;//钩子对象
static Hook* hook;//Qt界面中调用Hook类的对象

LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam){//钩子消息函数，系统消息队列信息会返回到该函数中
    KBDLLHOOKSTRUCT* pkbhs = (KBDLLHOOKSTRUCT*)lParam;//lParam用于判断按键类型
    if(0x51 == pkbhs->vkCode&&GetAsyncKeyState(VK_MENU))
    {//按下Alt+Q
        hook->sendSignal();//安装钩子的对象发出按键监听信号
    }

    return CallNextHookEx(g_hook,nCode,wParam,lParam);//继续原有的事件队列
}


void Hook::installHook(){//安装钩子函数
    g_hook = SetWindowsHookEx(WH_KEYBOARD_LL,keyProc,nullptr,0);
    hook = this;
}

void Hook::unInstallHook(){//删除钩子函数
    UnhookWindowsHookEx(g_hook);
    g_hook = nullptr;
}

void Hook::sendSignal()
{//发送信号函数
    emit sendKeyType();
}
