/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#ifndef UVAPPCONFIGHANDLER_H
#define UVAPPCONFIGHANDLER_H

#include <QXmlDefaultHandler>

class UvAppConfig;
class UvAppConfigHandlerPrivate;

class UvAppConfigHandler : public QXmlDefaultHandler
{
public:
    UvAppConfigHandler(UvAppConfig *cfg);
    ~UvAppConfigHandler();
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI,
                    const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;
private:
    UvAppConfigHandlerPrivate *d;
};

#endif // UVAPPCONFIGHANDLER_H
