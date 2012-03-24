/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvgdxtransceiver.h
 * \brief Header file of class UvGdxTransceiver.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/06/2011
 */
#ifndef UVGDXTRANSCEIVER_H
#define UVGDXTRANSCEIVER_H

#include "../corelib_global.h"
#include "uvabstractframetransceiver.h"

#define GDX_HEADER      0xE1 /*!< GDX protocol header */
#define GDX_TAIL        0xFE /*!< GDX protocol tail */

class UvGdxTransceiverPrivate;

/*!
 * \class UvGdxTransceiver
 * \brief A transceiver for GDX serial communication protocol.
 * \author binlaten(09r1004)
 * \date created: 01/06/2011
 */
class CORE_LIB_EXPORT UvGdxTransceiver : public UvAbstractFrameTransceiver
{
    Q_OBJECT
public:
    explicit UvGdxTransceiver(UvSerialPort *serialPort);
    ~UvGdxTransceiver();

signals:

protected:
    bool packFrame(QByteArray * dstBuffer, QByteArray * srcBuffer);

private:
    UvGdxTransceiverPrivate *d;
};

#endif // UVGDXTRANSCEIVER_H
