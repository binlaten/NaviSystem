/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#ifndef UVGDXMANAGER_H
#define UVGDXMANAGER_H

#include <QObject>
#include <QDateTime>

class UvGdxManagerPrivate;

class UvGdxManager : public QObject
{
    Q_OBJECT
public:
    explicit UvGdxManager();
    ~UvGdxManager();
    bool init();

signals:
    void nightModeChanged(bool);
    void systemTimeUpdated(const QDate &date, int week, const QTime &time);

public slots:
    void startup();
    void gpsExited();
    void updateToGpsTime(const QDate &date, int week, const QTime &time);
    void updateDistance(int dist);

private:
    void parseTouch(const QByteArray &data);
    void parseCarInfo(const QByteArray &data);
    void parseSystemTime(const QByteArray &data);

private slots:
    void onCommandArrived(unsigned char, QByteArray);

private:
    UvGdxManagerPrivate *d;
};

#endif // UVGDXMANAGER_H
