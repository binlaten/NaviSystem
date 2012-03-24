/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvabstractframetransceiver.cpp
 * \brief Implement file of class UvAbstractFrameTransceiver.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#include "corelib/global/uvglobal.h"
#include "corelib/io/uvserialport.h"
#include "uvabstractframetransceiver.h"

/*!
 * \class UvAbstractFrameTransceiverPrivate
 * \brief A class to store private data for class UvAbstractFrameTransceiver.
 */
class UvAbstractFrameTransceiverPrivate
{
public:
    UvSerialPort *serialPort; /*!< Serial port object pointer. */
    QByteArray receiveBuffer; /*!< A buffer to store received data. */
    bool hasReceived; /*!< Has received any data from serial port. */
};

/*!
 * \fn UvAbstractFrameTransceiver::UvAbstractFrameTransceiver(UvSerialPort *serialPort)
 * \brief Construct an abstract frame transceiver that transfers and receives data from
 * serial port.
 */
UvAbstractFrameTransceiver::UvAbstractFrameTransceiver(UvSerialPort *serialPort)
{
    d = new UvAbstractFrameTransceiverPrivate;
    UV_CHK_PTR(d);
    d->serialPort = serialPort;
    UV_CHK_PTR(d->serialPort);
    connect(d->serialPort, SIGNAL(readyRead()), this, SLOT(receive()));
    d->hasReceived = false;
}

/*!
 * \fn UvAbstractFrameTransceiver::~UvAbstractFrameTransceiver()
 * \brief Standard destructor.
 */
UvAbstractFrameTransceiver::~UvAbstractFrameTransceiver()
{
    UV_SAFE_DEL(d);
}

/*!
 * \fn bool UvAbstractFrameTransceiver::hasReceived() const
 * \brief Has received any data from serial port or not.
 */
bool UvAbstractFrameTransceiver::hasReceived() const
{
    return d->hasReceived;
}

/*!
 * \fn QByteArray UvAbstractFrameTransceiver::readFrame()
 * \brief Get the received full frame.
 */
QByteArray UvAbstractFrameTransceiver::readFrame()
{
    return d->receiveBuffer;
}

/*!
 * \fn qint64 UvAbstractFrameTransceiver::writeFrame(const QByteArray &frame)
 * \brief Transfer a frame to serial port.
 */
qint64 UvAbstractFrameTransceiver::writeFrame(const QByteArray &frame)
{
#ifndef UV_NO_DEBUG
    qDebug("--------------------------Write a frame");
    Q_FOREACH(char c, frame)
    {
        qDebug("--- %02X ---", (uchar)c);
    }
    qDebug("Write a frame--------------------------");
#endif
    return d->serialPort->write(frame);
}

/*!
 * \fn void UvAbstractFrameTransceiver::receive()
 * Slot for receiving the data come from the serial port.
 */
void UvAbstractFrameTransceiver::receive()
{
    QByteArray data = d->serialPort->read();
    if (!d->hasReceived)
    {
        if (data.size() > 0)
        {
            d->hasReceived = true;
        }
    }
    while (data.size())
    {
        if (packFrame(&d->receiveBuffer, &data))
        {//pack out a full frame
#ifndef UV_NO_DEBUG
            qDebug("\n--------------------------Received a full frame");
            Q_FOREACH(char c, d->receiveBuffer)
            {
                qDebug("--- %02X ---", (uchar)c);
            }
            qDebug("Received a full frame--------------------------\n");
#endif
            emit frameReady();
            d->receiveBuffer.clear();
        }
    }
}
