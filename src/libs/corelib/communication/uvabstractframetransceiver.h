/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvabstractframetransceiver.h
 * \brief Header file of class UvAbstractFrameTransceiver.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#ifndef UVABSTRACTFRAMETRANSCEIVER_H
#define UVABSTRACTFRAMETRANSCEIVER_H

#include "../corelib_global.h"
#include <QObject>

class UvSerialPort;
class UvAbstractFrameTransceiverPrivate;

/*!
 * \class UvAbstractFrameTransceiver
 * \brief An abstract class of all kinds of frame transceiver.
 *
 * This Transceiver will pick up a full frame from data buffer which is
 * received from serial port, and transfer a full frame data to serial port.
 *
 * \author binlaten(09r1004)
 * \date 01/05/2011
 */
class CORE_LIB_EXPORT UvAbstractFrameTransceiver : public QObject
{
    Q_OBJECT
public:
    explicit UvAbstractFrameTransceiver(UvSerialPort *serialPort);
    ~UvAbstractFrameTransceiver();
    QByteArray readFrame();
    qint64 writeFrame(const QByteArray &frame);
    bool hasReceived() const;
signals:
    void frameReady(); /*!< A full frame has received. */
protected:
    virtual bool packFrame(QByteArray * dstBuffer,
                           QByteArray * srcBuffer) = 0; /*!< Pack to a full frame. */
private slots:
    void receive();
private:
    UvAbstractFrameTransceiverPrivate *d;
};

#endif // UVABSTRACTFRAMETRANSCEIVER_H
