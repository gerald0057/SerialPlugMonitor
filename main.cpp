#include <QApplication>
#include <QMenu>
#include "serialplugmonitor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning("系统托盘不可用！");
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    QSystemTrayIcon trayIcon;
    QIcon icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
    trayIcon.setIcon(icon);
    trayIcon.setVisible(true);

    QMenu menu;
    QAction quitAction("退出", &menu);
    menu.addAction(&quitAction);

    // 连接退出动作
    QObject::connect(&quitAction, &QAction::triggered, &app, &QApplication::quit);

    // 设置托盘图标的上下文菜单
    trayIcon.setContextMenu(&menu);

    SerialPlugMonitor monitor(&trayIcon);

    return app.exec();
}
