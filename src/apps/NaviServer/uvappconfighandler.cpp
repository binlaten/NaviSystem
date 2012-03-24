/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#include "corelib/global/uvglobal.h"
#include "uvappconfighandler.h"
#include "uvappconfig.h"
#include <QtXml>

class UvAppConfigHandlerPrivate
{
public:
    UvAppConfig *config;// application configue
    bool metUcfgTag; // met UCFG file tag or not
    QString errorString; // error message
};

UvAppConfigHandler::UvAppConfigHandler(UvAppConfig *cfg)
{
    d = new UvAppConfigHandlerPrivate;
    UV_CHK_PTR(d);
    d->config = cfg;
    UV_CHK_PTR(d->config);
    d->metUcfgTag = false;
}

UvAppConfigHandler::~UvAppConfigHandler()
{
    UV_SAFE_DEL(d);
}

bool UvAppConfigHandler::startElement(const QString &/*namespaceURI*/,
                                      const QString &/*localName*/,
                                      const QString &qName,
                                      const QXmlAttributes &attributes)
{
    if (!d->metUcfgTag && qName != "UCFG")
    { // first element must be UCFG file tag
        d->errorString = QObject::tr("The file is not an Ultra-View  applicaion config file.");
        return false;
    }
    if (qName == "UCFG")
    {
        QString version = attributes.value("version");
        if (!version.isEmpty() && version != "1.0.0")
        {
            d->errorString = QObject::tr("The file is not an UCFG version 1.0 file.");
            return false;
        }
        d->metUcfgTag = true;
    }
    else if (qName == "SerialPort")
    {

    }
    else if (qName == "GDX")
    {
        d->config->setGdxPortNeedOpen((attributes.value("open") == "yes"));
        d->config->setGdxPortName(attributes.value("Name"));
        d->config->setGdxPortBaudRate(attributes.value("BaudRate").toInt());
        d->config->setGdxPortDataBits(attributes.value("DataBits").toInt());
        d->config->setGdxPortStopBits(attributes.value("StopBits").toInt());
        d->config->setGdxPortParity(attributes.value("Parity"));
        d->config->setGdxPortFlowControl(attributes.value("FlowControl"));
        d->config->setGdxPortTimeout(attributes.value("Timeout_Millisec").toInt());
    }
    return true;
}

bool UvAppConfigHandler::endElement(const QString &/*namespaceURI*/,
                                    const QString &/*localName*/,
                                    const QString &/*qName*/)
{
    return true;
}

bool UvAppConfigHandler::characters(const QString &/*str*/)
{
    return true;
}

bool UvAppConfigHandler::fatalError(const QXmlParseException & exception)
{
    qWarning() << "Fatal error on line" << exception.lineNumber()
               << ", column" << exception.columnNumber() << ":"
               << exception.message();

    return false;
}

QString UvAppConfigHandler::errorString() const
{
    return d->errorString;
}
