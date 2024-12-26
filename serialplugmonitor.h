#ifndef SERIALPLUGMONITOR_H
#define SERIALPLUGMONITOR_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QStyle>
#include <QIcon>
#include <QSerialPortInfo>
#include <QSet>
#include <QDebug>

class SerialPlugMonitor : public QObject {
    Q_OBJECT

public:
    SerialPlugMonitor(QSystemTrayIcon *trayIcon, QObject *parent = nullptr)
        : QObject(parent), trayIcon(trayIcon) {
        updateSerialPorts();
        connect(&timer, &QTimer::timeout, this, &SerialPlugMonitor::checkSerialPorts);
        timer.start(1000);
    }

private slots:
    void checkSerialPorts() {
        QSet<QString> currentPorts;
        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
            currentPorts.insert(info.portName());
        }

        for (const QString &portName : currentPorts) {
            if (!availablePorts.contains(portName)) {
                trayIcon->showMessage("SerialPort Changed", QString("SerialPort %1 Connected").arg(portName), QSystemTrayIcon::Information, 1000);
                qDebug() << "SerialPort " << portName << " Connected";
            }
        }

        for (const QString &portName : availablePorts) {
            if (!currentPorts.contains(portName)) {
                trayIcon->showMessage("SerialPort Changed", QString("SerialPort %1 Disconnected").arg(portName), QSystemTrayIcon::Warning, 1000);
                qDebug() << "SerialPort " << portName << " Disconnected";
            }
        }

        availablePorts = currentPorts;
    }

private:
    void updateSerialPorts() {
        availablePorts.clear();
        for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
            availablePorts.insert(info.portName());
        }
    }

    QSystemTrayIcon *trayIcon;
    QSet<QString> availablePorts;
    QTimer timer;
};
#endif // SERIALPLUGMONITOR_H
