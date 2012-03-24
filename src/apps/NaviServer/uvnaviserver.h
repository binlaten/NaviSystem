/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#ifndef UVNAVISERVER_H
#define UVNAVISERVER_H

#include <QObject>

class UvNaviServerPrivate;

class UvNaviServer : public QObject
{
    Q_OBJECT
public:
    explicit UvNaviServer();
    ~UvNaviServer();
    bool start();
signals:

public slots:

private:
    bool loadAppConfig();
    bool initGdxManager();
    bool initGpsManager();
    bool finishInit();
    bool connectAll();

private:
    UvNaviServerPrivate *d;
};

#endif // UVNAVISERVER_H
