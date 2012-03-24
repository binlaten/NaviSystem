/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvgdxprocessor.cpp
 * \brief Implement file of class UvGdxProcessor.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/06/2011
 */
#include "corelib/global/uvglobal.h"
#include "corelib/tools/uvcrc.h"
#include "uvgdxtransceiver.h"
#include "uvgdxprocessor.h"
#ifdef COMMAND_WITH_ACK
#include <QMutex>
#include <QMutexLocker>
#endif

/*!
 * \class UvGdxProcessorPrivate
 * \brief A class to store private data for class UvGdxProcessor.
 */
class UvGdxProcessorPrivate
{
public:
#ifdef COMMAND_WITH_ACK
    bool waitingAck; /*!< now is waiting for receiving ACK frame. */
    int waitedTime; /*!< time has waited for receiving ACK frame. */
    int timeoutCounter; /*!< counter records timeout times for receiving ACK frame. */
    QByteArray waitingAckCmd; /*!< the frame has sent and been waiting for receiving ACK frame. */
    QMutex waitMutex; /*!< mutex for processing ACK mechanism. */
#endif
};

/*!
 * \fn UvGdxProcessor::UvGdxProcessor(UvAbstractFrameTransceiver * transceiver)
 * \brief Construct a GDX command processor.
 */
UvGdxProcessor::UvGdxProcessor(UvAbstractFrameTransceiver * transceiver) :
    UvAbstractFrameProcessor(transceiver)
{
    d = new UvGdxProcessorPrivate;
    UV_CHK_PTR(d);
#ifdef COMMAND_WITH_ACK
    d->waitingAck = false;
    d->waitedTime = 0;
    d->timeoutCounter = 0;
#endif
}

/*!
 * \fn UvGdxProcessor::~UvGdxProcessor()
 * \brief Standard destructor.
 */
UvGdxProcessor::~UvGdxProcessor()
{
    UV_SAFE_DEL(d);
}

/*!
 * \fn void UvGdxProcessor::sendCommand(unsigned char type, const QByteArray &data)
 * \brief Send a GDX command with specified command \a type and
 * command \a data. This command must be acknowledged under ACK-GDX protocol
 */
void UvGdxProcessor::sendCommand(unsigned char type, const QByteArray &data)
{
    QByteArray ba;
    ba.append((uchar)GDX_HEADER); // header
    ba.append(type); // type
#ifdef COMMAND_WITH_ACK
    ba.append(data.size() + 1); // length
    ba.append((char)0x80); // command with ack
#else
    ba.append(data.size()); // length
#endif
    ba.append(data); // data
    ushort crcSum = uvChecksum((uchar *)ba.data(), ba.size());
    ba.append(crcSum & 0xFF); // crc low 8 bit
    ba.append((crcSum >> 8) & 0xFF); // crc high 8 bit
    ba.append((uchar)GDX_TAIL); // tail
    transferFrame(ba);
}

/*!
 * \fn void UvGdxProcessor::postCommand(unsigned char type, const QByteArray &data)
 * \brief Send a GDX command with specified command \a type and
 * command \a data. This command do not need acknowledge under ACK-GDX protocol
 */
void UvGdxProcessor::postCommand(unsigned char type, const QByteArray &data)
{
#ifdef COMMAND_WITH_ACK
    QByteArray ba;
    ba.append((uchar)GDX_HEADER); // header
    ba.append(type); // type
    ba.append(data.size() + 1); // length
    ba.append((char)0x00); // command without ack
    ba.append(data); // data
    ushort crcSum = uvChecksum((uchar *)ba.data(), ba.size());
    ba.append(crcSum & 0xFF); // crc low 8 bit
    ba.append((crcSum >> 8) & 0xFF); // crc high 8 bit
    ba.append((uchar)GDX_TAIL); // tail
    transferFrame(ba);
#else
    sendCommand(type, data);
#endif
}

/*!
 * \fn void UvGdxProcessor::parse()
 * \brief Parse a GDX frame, check it and find its type and data.
 */
void UvGdxProcessor::parse()
{
    QByteArray ba = getReceivedFrame();
    if (check(ba))
    {
#ifdef COMMAND_WITH_ACK
        if ((ba.at(3) & 0x01) == 0)
        { // cmd frame
            if ((ba.at(3) & 0x80) == 0x80)
            { // need ack
                QByteArray ack = ba;
                ack[3] = (ba.at(3) & ~80) | 0x01; // set ack flag
                ushort crcSum = uvChecksum((uchar *)ba.data(), ba.at(2) + 3); // recalculate crc value
                ack[ba.size() - 3] = crcSum & 0xFF;
                ack[ba.size() - 2] = (crcSum >> 8) & 0xFF;
                transceiver()->writeFrame(ack); // send ack frame directly
            }
            emit commandArrived(ba.at(1), ba.mid(4, ba.at(2) - 1));
        }
        else if (((ba.at(3) & 0x01) == 1) && ((ba.at(3) & 0x80) == 0x00))
        { // ack frame
            QMutexLocker locker(&d->waitMutex);
            if (d->waitingAck)
            { // waiting for ack
                if ((d->waitingAckCmd.at(1) == ba.at(1)) &&
                    (d->waitingAckCmd.mid(4, d->waitingAckCmd.at(2) - 1) ==
                     ba.mid(4, ba.at(2) - 1)))
                { // both cmd type and cmd data are right
                    d->waitingAck = false;
                    d->waitingAckCmd = QByteArray();
                    d->timeoutCounter = 0;
                    d->waitedTime = 0;
                }
            }
        }
#else
        emit commandArrived(ba.at(1), ba.mid(3, ba.at(2)));
#endif
    }
}

/*!
 * \fn void UvGdxProcessor::send()
 * \brief Send a GDX frame.
 */
void UvGdxProcessor::send()
{
#ifdef COMMAND_WITH_ACK
    if (d->waitingAck)
    { // waiting for ack
        QMutexLocker locker(&d->waitMutex);
        if (d->waitingAck)
        {
            d->waitedTime += sendingDelayTime();
            if (d->waitedTime >= ACK_TIMEOUT_MS)
            { // time out
                if (++d->timeoutCounter >= ACK_TIMEOUT_MAX)
                { // send falied, reset all
                    qWarning("Error: can NOT receive ack frame for a long time!!");
                    d->waitingAck = false;
                    d->waitingAckCmd = QByteArray();
                    d->timeoutCounter = 0;
                    d->waitedTime = 0;
                }
                else
                { // send command again
                    transceiver()->writeFrame(d->waitingAckCmd);
                    d->waitedTime = 0;
                }
            }
        }
    }
    else
    {
        QByteArray ba = getTransferFrame();
        if (((ba.at(3) & 0x80) == 0x80) &&
            ((ba.at(3) & 0x01) == 0))
        { // cmd need ack
            QMutexLocker locker(&d->waitMutex);
            d->waitingAck = true;
            d->waitingAckCmd = ba;
        }
        transceiver()->writeFrame(ba);
    }
#else
    transceiver()->writeFrame(getTransferFrame());
#endif
}

/*!
 * \fn bool UvGdxProcessor::check(const QByteArray &ba)
 * \brief Check a GDX frame with CRC check.
 * \return If check passed return true, else false.
 */
bool UvGdxProcessor::check(const QByteArray &ba)
{
    int ds = ba.size();
    uchar * rd = (uchar *)ba.data();
    ushort crcSum = uvChecksum(rd, rd[2] + 3);
    uchar lSum = crcSum & 0xFF;
    if (lSum != rd[ds - 3])
    {
        qDebug("Error: CRC low 8 bits checked failed!");
        return false;
    }
    uchar hSum = (crcSum >> 8) & 0xFF;
    if (hSum != rd[ds - 2])
    {
        qDebug("Error: CRC high 8 bits checked failed!");
        return false;
    }
    return true;
}
