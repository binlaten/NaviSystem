/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#include "uvappconfig.h"
#include "uvappconfighandler.h"
#include "corelib/global/uvglobal.h"
#include <QtXml>

class UvAppConfigPrivate
{
public:
    QString filePath; // path of application config file
    bool gdxPortNeedOpen; // whether need open gdx serial port or nor
    QString gdxPortName; // gdx serial port name
    UvPortSettings gdxPortSettings; // gdx serial port settings
};

UvAppConfig::UvAppConfig()
{
    d = new UvAppConfigPrivate;
    UV_CHK_PTR(d);
    d->gdxPortNeedOpen = false;
}

UvAppConfig::UvAppConfig(const QString &filePath)
{
    d = new UvAppConfigPrivate;
    UV_CHK_PTR(d);
    d->gdxPortNeedOpen = false;
    d->filePath = filePath;
}

UvAppConfig::~UvAppConfig()
{
    UV_SAFE_DEL(d);
}

bool UvAppConfig::load()
{
    UvAppConfigHandler handler(this);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.setErrorHandler(&handler);

    QFile file(d->filePath);
    if (!file.exists())
    { // config file file not exist
        qWarning("Error: can NOT find config file: %s", d->filePath.toAscii().constData());
        return false;
    }

    if (!file.open(QFile::ReadOnly | QFile::Text))
    { // open file failed
        qWarning("Error: can NOT open config file: %s", d->filePath.toAscii().constData());
        return false;
    }

    QXmlInputSource xmlInputSource(&file);
    if (reader.parse(xmlInputSource))
    { // parse ok
        qDebug("config file parse OK!");
        return true;
    }
    else
    { // parse failed
        qWarning("Error: config file parse failed!");
        return false;
    }
    return true;
}

void UvAppConfig::setConfigFilePath(const QString &filePath)
{
    d->filePath = filePath;
}

bool UvAppConfig::gdxPortNeedOpen() const
{
    return d->gdxPortNeedOpen;
}

void UvAppConfig::setGdxPortNeedOpen(bool open)
{
    d->gdxPortNeedOpen = open;
}

QString UvAppConfig::gdxPortName() const
{
    return d->gdxPortName;
}

void UvAppConfig::setGdxPortName(const QString &name)
{
    d->gdxPortName = name;
}

UvPortSettings UvAppConfig::gdxPortSettings() const
{
    return d->gdxPortSettings;
}

void UvAppConfig::setGdxPortBaudRate(int baudRate)
{
    d->gdxPortSettings.BaudRate = toUvBaudRate(baudRate);
}

void UvAppConfig::setGdxPortDataBits(int dataBits)
{
    d->gdxPortSettings.DataBits = toUvDataBits(dataBits);
}

void UvAppConfig::setGdxPortStopBits(int stopBits)
{
    d->gdxPortSettings.StopBits = toUvStopBits(stopBits);
}

void UvAppConfig::setGdxPortParity(const QString &parity)
{
    d->gdxPortSettings.Parity = toUvParity(parity);
}

void UvAppConfig::setGdxPortFlowControl(const QString &flowControl)
{
    d->gdxPortSettings.FlowControl = toUvFlowControl(flowControl);
}

void UvAppConfig::setGdxPortTimeout(int ms)
{
    d->gdxPortSettings.Timeout_Millisec = ms;
}

UvBaudRate UvAppConfig::toUvBaudRate(int baudRate) const
{
    UvBaudRate br = (UvBaudRate)-1;
    switch(baudRate)
    {
    case 4800:
        br = UV_BAUD_4800;
        break;
    case 9600:
        br = UV_BAUD_9600;
        break;
    case 14400:
        br = UV_BAUD_14400;
        break;
    case 19200:
        br = UV_BAUD_19200;
        break;
    case 38400:
        br = UV_BAUD_38400;
        break;
    case 57600:
        br = UV_BAUD_57600;
        break;
    case 115200:
        br = UV_BAUD_115200;
        break;
    default:
        qWarning("Error: Invalid Baud Rate!");
        break;
    }
    return br;
}

UvDataBits UvAppConfig::toUvDataBits(int dataBits) const
{
    UvDataBits db = (UvDataBits)-1;
    switch(dataBits)
    {
    case 8:
        db = UV_DATABIT_8;
        break;
    default:
        qWarning("Error: Invalid Data Bits!");
        break;
    }
    return db;
}

UvStopBits UvAppConfig::toUvStopBits(int stopBits) const
{
    UvStopBits sb = (UvStopBits)-1;
    switch(stopBits)
    {
    case 1:
        sb = UV_STOPBIT_1;
        break;
    default:
        qWarning("Error: Invalid Stop Bits!");
        break;
    }
    return sb;
}

UvParity UvAppConfig::toUvParity(const QString &parity) const
{
    UvParity p = (UvParity)-1;
    if (parity == "ODD")
    {
        p = UV_PARITY_ODD;
    }
    else if (parity == "EVEN")
    {
        p = UV_PARITY_EVEN;
    }
    else if (parity == "NONE")
    {
        p = UV_PARITY_NONE;
    }
    else
    {
        qWarning("Error: Invalid Parity!");
    }
    return p;
}

UvFlowControl UvAppConfig::toUvFlowControl(const QString &flowControl) const
{
    UvFlowControl fc = (UvFlowControl)-1;
    if (flowControl == "OFF")
    {
        fc = UV_FLOW_OFF;
    }
    else if (flowControl == "HARDWARE")
    {
        fc = UV_FLOW_HARDWARE;
    }
    else
    {
        qWarning("Error: Invalid Flow Control!");
    }
    return fc;
}
