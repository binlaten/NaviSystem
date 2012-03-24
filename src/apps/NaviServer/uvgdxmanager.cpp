/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#include "uvgdxmanager.h"
#include "uvnaviserver_global.h"
#include "uvappconfig.h"
#include "corelib/global/uvglobal.h"
#include "corelib/io/uvserialport.h"
#include "corelib/communication/uvgdxtransceiver.h"
#include "corelib/communication/uvgdxprocessor.h"
#include "corelib/tools/uvnumberconvert.h"

#define GD210_OPEL // this gdx manager is used on GD210 for OPEL NaviSystem

#ifdef GD210_OPEL
/*! following type defined for command from GD210 to DM65B */
#define CMD_GD_NAVI_STARTED         0x80
#define CMD_GD_GPS_EXITED           0x82
#define CMD_GD_GPS_TIME             0x84
#define CMD_GD_GPS_DIST             0x86

/*! following type defined for command from DM65B to GD210 */
#define CMD_DG_TOUCH                0x81
#define CMD_DG_CAR_INFO             0x83
#define CMD_DG_SYSTEM_TIME          0x85

#else
/*! following type defined for command from GD201 to MCU */
#define CMD_GD_NAVI_STARTED         0x0a
#define CMD_GD_GPS_EXITED           0x0f
#define CMD_GD_GPS_TIME             0x12
#define CMD_GD_GPS_DIST             0x1d

/*! following type defined for command from MCU to GD210 */
#define CMD_DG_TOUCH                0x03
#define CMD_DG_CAR_INFO             0x15
#define CMD_DG_SYSTEM_TIME          0x12

#endif // GD210_OPEL

class UvGdxManagerPrivate
{
public:
    UvSerialPort *serialPort;
    UvGdxTransceiver *transceiver;
    UvGdxProcessor *processor;
};

UvGdxManager::UvGdxManager()
{
    d = new UvGdxManagerPrivate;
    UV_CHK_PTR(d);
    d->serialPort = 0;
    d->transceiver = 0;
    d->processor = 0;
}

UvGdxManager::~UvGdxManager()
{
    UV_SAFE_DEL(d->processor);
    UV_SAFE_DEL(d->transceiver);
    UV_SAFE_DEL(d->serialPort);
    UV_SAFE_DEL(d);
}

bool UvGdxManager::init()
{
    if (!g_appConfig)
    {
        qWarning("Error: app config has not init!!");
        return false;
    }
    if (!g_appConfig->gdxPortNeedOpen())
    {
        qWarning("Warning: according to the config file, gdx serial port will not open!!");
        return false;
    }
    d->serialPort = new UvSerialPort(g_appConfig->gdxPortName(), g_appConfig->gdxPortSettings());
    UV_CHK_PTR(d->serialPort);
    if (!d->serialPort->open())
    {
        qWarning("Error: Serial Port open failed!");
        return false;
    }
    d->transceiver = new UvGdxTransceiver(d->serialPort);
    UV_CHK_PTR(d->transceiver);
    d->processor = new UvGdxProcessor(d->transceiver);
    UV_CHK_PTR(d->processor);
    d->processor->setSendingDelayTime(5);
    connect(d->processor, SIGNAL(commandArrived(unsigned char,QByteArray)),
            this, SLOT(onCommandArrived(unsigned char,QByteArray)));
    return true;
}

void UvGdxManager::onCommandArrived(unsigned char type, QByteArray data)
{
    switch (type)
    {
    case CMD_DG_TOUCH:
        parseTouch(data);
        break;
    case CMD_DG_CAR_INFO:
        parseCarInfo(data);
        break;
    case CMD_DG_SYSTEM_TIME:
        parseSystemTime(data);
        break;
    default:
        break;
    }
}

void UvGdxManager::parseTouch(const QByteArray &data)
{
    if (data.size() == 4)
    {
#ifdef Q_OS_WINCE
        ulong posX = 0;
        ulong posY = 0;
        DWORD mouseFlags = 0;
        switch((uchar)data.at(3) & 0xC0)
        {
        case 0x00://pen up
            mouseFlags = MOUSEEVENTF_LEFTUP;
            break;
        case 0x80://pen down
            posX = (((uchar)data.at(1) << 8) + (uchar)data.at(0)) * 65536UL / SCREEN_WIDTH;
            posY = ((((uchar)data.at(3) & 0x1F) << 8) + (uchar)data.at(2)) * 65536UL / SCREEN_HEIGHT;
            mouseFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
            break;
        case 0x40://pen move
            posX = (((uchar)data.at(1) << 8) + (uchar)data.at(0)) * 65536UL / SCREEN_WIDTH;
            posY = ((((uchar)data.at(3) & 0x1F) << 8) + (uchar)data.at(2)) * 65536UL / SCREEN_HEIGHT;
            mouseFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
            break;
        default:
            return;
        }
        mouse_event(mouseFlags, posX, posY, 0, 0);
#endif
    }
    else
    {
        qWarning("Warnning: command data length error!");
    }
}

void UvGdxManager::parseCarInfo(const QByteArray &data)
{
    if (data.size() == 1)
    {
        bool nightMode = data.at(0) & 0x80;
        emit nightModeChanged(nightMode);
    }
    else
    {
        qWarning("Warnning: command data length error!");
    }
}

void UvGdxManager::parseSystemTime(const QByteArray &data)
{
    if (data.size() == 7)
    {
        int year = 2000 + uvBcd2Dec(data.at(0));
        int month = uvBcd2Dec(data.at(1));
        int day = uvBcd2Dec(data.at(2));
        int week = uvBcd2Dec(data.at(3));
        int hour = uvBcd2Dec(data.at(4));
        int minute = uvBcd2Dec(data.at(5));
        int second = uvBcd2Dec(data.at(6));
#ifdef Q_OS_WINCE
        SYSTEMTIME st;
        st.wYear = year;
        st.wMonth = month;
        st.wDay = day;
        st.wHour = hour;
        st.wDayOfWeek = week;
        if((st.wHour < 0) || (st.wHour > 24))
        {
            st.wHour = 1;
        }
        st.wMinute = minute;
        if((st.wMinute < 0) || (st.wMinute > 59))
        {
            st.wMinute = 1;
        }
        st.wSecond = second;
        SetLocalTime(&st);
#else
        emit systemTimeUpdated(QDate(year, month, day), week, QTime(hour, minute, second));
#endif
    }
    else
    {
        qWarning("Warnning: command data length error!");
    }
}

void UvGdxManager::startup()
{
    d->processor->sendCommand(CMD_GD_NAVI_STARTED, QByteArray());
}

void UvGdxManager::gpsExited()
{
    d->processor->sendCommand(CMD_GD_GPS_EXITED, QByteArray());
}

void UvGdxManager::updateToGpsTime(const QDate &date, int week, const QTime &time)
{
    QByteArray ba;
    ba.append(uvDec2Bcd(date.year() % 100)); // year
    ba.append(uvDec2Bcd(date.month())); // month
    ba.append(uvDec2Bcd(date.day())); // day
    ba.append(uvDec2Bcd(week)); // week
    ba.append(uvDec2Bcd(time.hour())); // hour
    ba.append(uvDec2Bcd(time.minute())); // minute
    ba.append(uvDec2Bcd(time.second())); // second
    d->processor->sendCommand(CMD_GD_GPS_TIME, ba);
}

void UvGdxManager::updateDistance(int dist)
{
    QByteArray ba;
    ba.append(((uint)dist >> 16) & 0xFF);
    ba.append(((uint)dist >> 8) & 0xFF);
    ba.append((uint)dist & 0xFF);
    if (dist == 0)
    {
        d->processor->sendCommand(CMD_GD_GPS_DIST, ba);
    }
    else
    {
        d->processor->postCommand(CMD_GD_GPS_DIST, ba);
    }
}
