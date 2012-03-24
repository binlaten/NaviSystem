/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#ifndef UVAPPCONFIG_H
#define UVAPPCONFIG_H

#include "corelib/io/uvserialportdef.h"

class UvAppConfigPrivate;

class UvAppConfig
{
public:
    UvAppConfig();
    UvAppConfig(const QString &filePath);
    ~UvAppConfig();
    bool load();

    void setConfigFilePath(const QString &filePath);

    bool gdxPortNeedOpen() const;
    void setGdxPortNeedOpen(bool open);

    QString gdxPortName() const;
    void setGdxPortName(const QString &name);

    UvPortSettings gdxPortSettings() const;
    void setGdxPortBaudRate(int baudRate);
    void setGdxPortDataBits(int dataBits);
    void setGdxPortStopBits(int stopBits);
    void setGdxPortParity(const QString &parity);
    void setGdxPortFlowControl(const QString &flowControl);
    void setGdxPortTimeout(int ms);

private:
    UvBaudRate toUvBaudRate(int baudRate) const;
    UvDataBits toUvDataBits(int dataBits) const;
    UvStopBits toUvStopBits(int stopBits) const;
    UvParity toUvParity(const QString &parity) const;
    UvFlowControl toUvFlowControl(const QString &flowControl) const;

private:
    UvAppConfigPrivate *d;
};

#endif // UVAPPCONFIG_H
