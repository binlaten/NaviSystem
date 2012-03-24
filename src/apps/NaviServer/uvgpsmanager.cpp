/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#include "uvgpsmanager.h"
#include "uvnaviserver_global.h"
#include "corelib/global/uvglobal.h"
#include "corelib/tools/uvnumberconvert.h"
#include "corelib/ipc/uvmessagetransceiver.h"

/* following defines for message from Navigation to AudioGPS */
#define WM_NG_GPS_CAR_INFO	0x9001		//car information
#define WM_NG_DAY_NIGHT_MOD	0x9002		//switch day-night mode
#define WM_NG_GPS_TO_SAVE	0x9004		//tell gps to save
#define WM_NG_SET_VOLUME	0x900E		//set volume value
#define WM_NG_GET_VERSION	0x9010		//get version
#define WM_NG_ZOOM_IN_MAP	0x9100		//zoom in map
#define WM_NG_ZOOM_OUT_MAP	0x9101		//zoom out map
#define WM_NG_SHOW_DEST		0x9102		//show destination
#define WM_NG_SHOW_MAP		0x9103		//show map
#define WM_NG_SHOW_LOCAL	0x9104		//show local position
#define WM_NG_DIRECTION		0x9105		//move direction
#define WM_NG_ENTER			0x9106		//ok
#define WM_NG_RETURN		0x9107		//cancel
#define WM_NG_INC_VOLUME	0x9108		//increase volume
#define WM_NG_DEC_VOLUME	0x9109		//decrease volume
#define WM_NG_GPS_MENU		0x910A		//show gps main menu
#define WM_NG_EXIT_GPS		0x910B		//exit gps
#define WM_NG_SHOW_4S		0x910C		//show 4s position
#define WM_NG_FOCUS_RIGHT	0x910D		//focus move clockwisely
#define WM_NG_FOCUS_LEFT	0x910E		//focus move anti-clockwise
#define WM_NG_OIL_LIGHT		0x910F		//low oil hint
#define WM_NG_MUTE			0x9111		//mute
#define WM_NG_VOICE			0x9112		//hint voice

/* following defines for message from AudioGPS to Navigation */
#define WM_GN_EXIT_GPS		0x9000		//GPS exited
#define WM_GN_HEART_BEAT	0x9003		//GPS heart beat ack message
#define WM_GN_GPS_SAVED		0x9005		//GPS has saved
#define WM_GN_GPS_TIME		0x9006		//GPS time
#define WM_GN_TALK_START	0x900B		//GPS talk start
#define WM_GN_TALK_END  	0x900C		//GPS talk end
#define WM_GN_DEST_DIST		0x900D		//destination distance
#define WM_GN_CUR_VOLUME	0x900F		//current volume
#define WM_GN_RETURN_VER	0x9011		//version

class UvGpsManagerPrivate
{
public:
    UvMessageTransceiver *transceiver;
    bool gpsAppHasLaunched;
};

UvGpsManager::UvGpsManager()
{
    d = new UvGpsManagerPrivate;
    UV_CHK_PTR(d);
    d->transceiver = 0;
    d->gpsAppHasLaunched = false;
}

UvGpsManager::~UvGpsManager()
{
    UV_SAFE_DEL(d->transceiver);
    UV_SAFE_DEL(d);
}

bool UvGpsManager::launchApp()
{
#ifdef Q_OS_WINCE
    if (!d->gpsAppHasLaunched)
    {
        qDebug("\nlaunch GPS application...");
        SHELLEXECUTEINFO ShExeInfo = {0};
        ShExeInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
        ShExeInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
        ShExeInfo.hwnd         = NULL;
        ShExeInfo.lpVerb       = NULL;
        ShExeInfo.lpFile       = _T(GPS_APP_NAME);
        ShExeInfo.lpParameters = L"";
        ShExeInfo.lpDirectory  = NULL;
        ShExeInfo.nShow        = SW_SHOW;
        ShExeInfo.hInstApp     = NULL;
        d->gpsAppHasLaunched = ShellExecuteEx(&ShExeInfo);
        if (!d->gpsAppHasLaunched)
        {
            qWarning("Error: launch gps application failed!!");
            return false;
        }
    }
#endif
    return true;
}

bool UvGpsManager::initMsg()
{
    d->transceiver = new UvMessageTransceiver(MY_WND_NAME, GPS_WND_NAME);
    UV_CHK_PTR(d->transceiver);
    if (!d->transceiver->init())
    {
        qWarning("Error: init gps message transceiver failed!!");
        return false;
    }
    connect(d->transceiver, SIGNAL(messageArrived(uint,QByteArray)),
            this, SLOT(onMessageArrived(uint,QByteArray)));
    return true;
}

bool UvGpsManager::init()
{
    if (!launchApp())
    {
        return false;
    }
    if (!initMsg())
    {
        return false;
    }
    return true;
}

void UvGpsManager::onMessageArrived(unsigned int msgId, const QByteArray &msgParam)
{
    switch(msgId)
    {
    case WM_GN_EXIT_GPS:
        qDebug("Navi received message from GPS --- GPS exited");
        emit gpsExited();
        break;
    case WM_GN_GPS_TIME:
        qDebug("Navi received message from GPS --- GPS time");
        parseGpsTime(msgParam);
        break;
     case WM_GN_DEST_DIST:
        qDebug("Navi received message from GPS --- GPS dest dist");
        parseGpsDistance(msgParam);
        break;
    }
}

void UvGpsManager::parseGpsTime(const QByteArray &msgParam)
{
    QDate date(uvBcd2Dec(msgParam.at(1)) * 100 + uvBcd2Dec(msgParam.at(0)), // year
               uvBcd2Dec(msgParam.at(2)), // month
               uvBcd2Dec(msgParam.at(3))); // day
    int week = uvBcd2Dec(msgParam.at(4)); // week
    QTime time(uvBcd2Dec(msgParam.at(5)), // hour
               uvBcd2Dec(msgParam.at(6)), // minute
               uvBcd2Dec(msgParam.at(7))); // second
#ifndef QT_NO_DEBUG
    qDebug() << date << week << time;
#endif
    emit gpsTime(date, week, time);
}

void UvGpsManager::parseGpsDistance(const QByteArray &msgParam)
{
    int dist = 0;
    if (msgParam.at(0) & 0x80)
    { // is valid distance value
        if (msgParam.at(0) & 0x40)
        { // by kilometer
            dist = (((int)msgParam.at(1)) << 16) +
                    (((int)msgParam.at(2)) << 8) +
                    msgParam.at(3) + 1;
        }
        else
        { // by meter
            dist = 1;
        }
    }
    else
    { // is invalid distance value
        dist = 0;
    }
    qDebug("GpsDistance: %d", dist);
    emit gpsDistance(dist);
}

void UvGpsManager::showNightMode(bool night)
{
    QByteArray msgParam(8, 0);
    msgParam[3] = night;
    d->transceiver->sendMessage(WM_NG_DAY_NIGHT_MOD, msgParam);
}
