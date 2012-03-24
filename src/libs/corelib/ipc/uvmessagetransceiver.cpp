/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvmessagetransceiver.cpp
 * \brief Implement file of class UvMessageTransceiver.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/09/2011
 */
#include "uvmessagetransceiver.h"
#include "corelib/global/uvglobal.h"

/*!
 * \class UvMessageTransceiverPrivate
 * \brief A class to store private data for class UvMessageTransceiver.
 */
class UvMessageTransceiverPrivate
{
public:
#ifdef Q_OS_WIN
    WId targetWId; /*!< id of target window */
#endif
    QString thisName; /*!< name of this transceiver window */
    QString targetName; /*!< name of target window to which send message */
};

/*!
 * \fn UvMessageTransceiver::UvMessageTransceiver(const QString &thisName,
                                                  const QString &targetName)
 * \brief Construct a message transceiver with specified this window name and target window name
 * \param [in] thisName This window name for receiving message.
 * \param [in] targetName Target window name to which send message.
 */
UvMessageTransceiver::UvMessageTransceiver(const QString &thisName,
                                           const QString &targetName)
{
    d = new UvMessageTransceiverPrivate;
    UV_CHK_PTR(d);
#ifdef Q_OS_WIN
    d->targetWId = 0;
    setWindowTitle(d->thisName);
    setWindowFlags(Qt::CustomizeWindowHint);
    setWindowFlags(Qt::WindowStaysOnBottomHint);
    setGeometry(-100, -100, 1, 1);
    show();
#endif
    d->thisName = thisName;
    d->targetName = targetName;
}

/*!
 * \fn UvMessageTransceiver::~UvMessageTransceiver()
 * \brief Standard destructor.
 */
UvMessageTransceiver::~UvMessageTransceiver()
{
    UV_SAFE_DEL(d);
}

/*!
 * \fn bool UvMessageTransceiver::init()
 * \brief Initialize message transceiver so that it can send message to target window.
 */
bool UvMessageTransceiver::init()
{
#ifdef Q_WS_WIN
    while (!(d->targetWId = ::FindWindow(0, (WCHAR*)(d->targetName.utf16()))))
    {
        static int findCnt = 0;
        qDebug("can not found target window %s - %d!", d->targetName.toAscii().constData(), findCnt);
        Sleep(50);
        if (++findCnt > 100)
        {//no such window
            qWarning("Error: find target window %s failed!", d->targetName.toAscii().constData());
            return false;
        }
    }
#endif
    return true;
}

#ifdef Q_WS_WIN
/*!
 * \fn bool UvMessageTransceiver::winEvent(MSG * message, long * result)
 * \brief Handle Windows message.
 */
bool UvMessageTransceiver::winEvent(MSG * message, long * result)
{
    Q_UNUSED(result);
    if (message->message >= 0x9000)
    { // only custom message will be handled by this transceiver, others by Qt
        QByteArray msgParam(8, 0);
        msgParam[0] = ((message->wParam >> 24) & 0xFF);
        msgParam[1] = ((message->wParam >> 16) & 0xFF);
        msgParam[2] = ((message->wParam >> 8) & 0xFF);
        msgParam[3] = (message->wParam & 0xFF);
        msgParam[4] = ((message->lParam >> 24) & 0xFF);
        msgParam[5] = ((message->lParam >> 16) & 0xFF);
        msgParam[6] = ((message->lParam >> 8) & 0xFF);
        msgParam[7] = (message->lParam & 0xFF);
        qDebug("-----------Receivd Message-------------------");
        qDebug("message id = 0x%08x", message->message);
        qDebug("message wParam = 0x%08x", (uint)message->wParam);
        qDebug("message lParam = 0x%08x", (uint)message->lParam);
        qDebug("---message Param---");
        for(int i = 0; i < msgParam.size(); i++)
        {
            qDebug("Param[%d] = 0x%02x", i, (uchar)msgParam.at(i));
        }
        emit messageArrived(message->message, msgParam);
    }
    return false;
}
#endif

/*!
 * \fn void UvMessageTransceiver::sendMessage(unsigned int msgId, const QByteArray &msgParam)
 * \brief Send message to target window.
 * \param [in] msgId message ID
 * \param [in] msgParam message parameter
 */
void UvMessageTransceiver::sendMessage(unsigned int msgId, const QByteArray &msgParam)
{
#ifdef Q_WS_WIN
    if (!d->targetWId)
    {
        init();
    }
    WPARAM wParam = 0;
    wParam = (((((msgParam.at(0) << 8)
                 + msgParam.at(1)) << 8)
               + msgParam.at(2)) << 8)
            + msgParam.at(3);
    LPARAM lParam = 0;
    lParam = (((((msgParam.at(4) << 8)
                 + msgParam.at(5)) << 8)
               + msgParam.at(6)) << 8)
            + msgParam.at(7);
    qDebug("-----------Sending Message-------------------");
    qDebug("message id = 0x%08x", msgId);
    qDebug("message wParam = 0x%08x", (uint)wParam);
    qDebug("message lParam = 0x%08x", (uint)lParam);
    ::PostMessage(d->targetWId, msgId, wParam, lParam);
#endif
}
