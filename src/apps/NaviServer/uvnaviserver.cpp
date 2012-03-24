/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#include "uvnaviserver.h"
#include "uvnaviserver_global.h"
#include "uvappconfig.h"
#include "uvgdxmanager.h"
#include "uvgpsmanager.h"
#include "corelib/global/uvglobal.h"

class UvNaviServerPrivate
{
public:
    UvGdxManager *gdxManager;
    UvGpsManager *gpsManager;
};

UvNaviServer::UvNaviServer()
{
    d = new UvNaviServerPrivate;
    UV_CHK_PTR(d);
    d->gdxManager = 0;
    d->gpsManager = 0;
}

UvNaviServer::~UvNaviServer()
{
    UV_SAFE_DEL(d->gpsManager);
    UV_SAFE_DEL(d->gdxManager);
    UV_SAFE_DEL(d);
}

bool UvNaviServer::start()
{
    qDebug("\n\n\nnavi server start...");
    if (!loadAppConfig())
    {
        return false;
    }
    if (!initGdxManager())
    {
        return false;
    }
    if (!initGpsManager())
    {
        return false;
    }
    if (!finishInit())
    {
        return false;
    }
    return true;
}

bool UvNaviServer::loadAppConfig()
{
    qDebug("\nload application config...");
    if (!g_appConfig)
    {
        g_appConfig = new UvAppConfig(APP_CONFIG_FILE);
        UV_CHK_PTR(g_appConfig);
    }
    if (!g_appConfig->load())
    {
        qWarning("Error: load application config failed!!");
        return false;
    }
    return true;
}

bool UvNaviServer::initGdxManager()
{
    qDebug("\ninit gdx manager...");
    d->gdxManager = new UvGdxManager();
    UV_CHK_PTR(d->gdxManager);
    if (!d->gdxManager->init())
    {
        qWarning("Error: init gdx manager failed!!");
        return false;
    }
    return true;
}

bool UvNaviServer::initGpsManager()
{
    qDebug("\ninit gps manager...");
    d->gpsManager = new UvGpsManager();
    UV_CHK_PTR(d->gpsManager);
    if (!d->gpsManager->init())
    {
        qWarning("Error: init gps manager failed!!");
        return false;
    }
    return true;
}

bool UvNaviServer::finishInit()
{
    qDebug("\nfinish init...");
    if (!connectAll())
    {
        qWarning("Error: connect all failed!!");
        return false;
    }
    d->gdxManager->startup();
    return true;
}

bool UvNaviServer::connectAll()
{
    qDebug("\nconnecting all...");
    if (d->gdxManager && d->gpsManager)
    {
        connect(d->gdxManager, SIGNAL(nightModeChanged(bool)),
                d->gpsManager, SLOT(showNightMode(bool)));

        connect(d->gpsManager, SIGNAL(gpsExited()),
                d->gdxManager, SLOT(gpsExited()));
        connect(d->gpsManager, SIGNAL(gpsTime(QDate,int,QTime)),
                d->gdxManager, SLOT(updateToGpsTime(QDate,int,QTime)));
        connect(d->gpsManager, SIGNAL(gpsDistance(int)),
                d->gdxManager, SLOT(updateDistance(int)));
        return true;
    }
    else
    {
        qWarning("Error: please init gdx or gps manager first!!");
        return false;
    }
}
