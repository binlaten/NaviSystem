/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvgdxprocessor.h
 * \brief Header file of class UvGdxProcessor.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/06/2011
 */
#ifndef UVGDXPROCESSOR_H
#define UVGDXPROCESSOR_H

#include "../corelib_global.h"
#include "uvabstractframeprocessor.h"

#define COMMAND_WITH_ACK // command has sent must be acknowledged
#ifdef COMMAND_WITH_ACK
#define ACK_TIMEOUT_MS      20 // receiver should ack within this time by ms
#define ACK_TIMEOUT_MAX     3 // max times for ack timeout
#endif

class UvGdxProcessorPrivate;

/*!
 * \class UvGdxProcessor
 * \brief A processor for GDX serial protocol.
 * \author binlaten(09r1004)
 * \date created: 01/06/2011
 */
class CORE_LIB_EXPORT UvGdxProcessor : public UvAbstractFrameProcessor
{
    Q_OBJECT
public:
    explicit UvGdxProcessor(UvAbstractFrameTransceiver * transceiver);
    ~UvGdxProcessor();

signals:

public slots:
    virtual void sendCommand(unsigned char, const QByteArray &);
    void postCommand(unsigned char, const QByteArray &);

protected:
    virtual void parse();
    virtual void send();

private:
    bool check(const QByteArray &);

private:
    UvGdxProcessorPrivate *d;
};

#endif // UVGDXPROCESSOR_H
