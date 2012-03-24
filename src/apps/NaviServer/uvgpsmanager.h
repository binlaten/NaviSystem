/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#ifndef UVGPSMANAGER_H
#define UVGPSMANAGER_H

#include <QObject>
#include <QDateTime>

class UvGpsManagerPrivate;

class UvGpsManager : public QObject
{
    Q_OBJECT
public:
    explicit UvGpsManager();
    ~UvGpsManager();
    bool init();

signals:
    void gpsExited();
    void gpsTime(const QDate &date, int week, const QTime &time);
    void gpsDistance(int dist);

public slots:
    void showNightMode(bool night);

private slots:
    void onMessageArrived(unsigned int msgId, const QByteArray &msgParam);

private:
    bool launchApp();
    bool initMsg();

    void parseGpsTime(const QByteArray &msgParam);
    void parseGpsDistance(const QByteArray &msgParam);

private:
    UvGpsManagerPrivate *d;
};

#endif // UVGPSMANAGER_H
