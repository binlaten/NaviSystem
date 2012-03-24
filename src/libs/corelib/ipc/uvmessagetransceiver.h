/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvmessagetransceiver.h
 * \brief Header file of class UvMessageTransceiver.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/09/2011
 */
#ifndef UVMESSAGETRANSCEIVER_H
#define UVMESSAGETRANSCEIVER_H

#include "../corelib_global.h"

#ifdef Q_OS_WIN
  #include <QWidget>
  #include "windows.h"
#else //POSIX
  #include <QObject>
#endif

class UvMessageTransceiverPrivate;

/*!
 * \class UvMessageTransceiver
 * \brief A message transceiver window which will receive/send from/to another window.
 * \author binlaten(09r1004)
 * \date 01/09/2011
 */
#ifdef Q_WS_WIN
class CORE_LIB_EXPORT UvMessageTransceiver : public QWidget
#else
class CORE_LIB_EXPORT UvMessageTransceiver : public QObject
#endif
{
    Q_OBJECT
public:
    explicit UvMessageTransceiver(const QString &thisName,
                                  const QString &targetName);
    ~UvMessageTransceiver();
    bool init();

signals:
    void messageArrived(unsigned int msgId, QByteArray msgParam);/*!< message arrived. */

public slots:
    void sendMessage(unsigned int msgId, const QByteArray &msgParam);

protected:
#ifdef Q_WS_WIN
    bool winEvent(MSG *, long *);
#endif

private:
    UvMessageTransceiverPrivate *d;
};


#endif // UVMESSAGETRANSCEIVER_H
