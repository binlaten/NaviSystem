/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/
/*!
 * \file uvabstractframeprocessor.h
 * \brief Header file of class UvAbstractFrameProcessor.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#ifndef UVABSTRACTFRAMEPROCESSOR_H
#define UVABSTRACTFRAMEPROCESSOR_H

#include "../corelib_global.h"
#include <QObject>
#include <QThread>

class UvAbstractFrameTransceiver;
class UvAbstractFrameProcessorPrivate;

/*!
 * \class UvAbstractFrameProcessor
 * \brief An abstract class of all kinds of frame processor.
 *
 * This processor has two queues to store received and transfer frames and
 * two threads to process these frames wheneven any queue is not empty, while
 * how to process these frames is determined by a concrete frame processor.
 *
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
class CORE_LIB_EXPORT UvAbstractFrameProcessor : public QObject
{
    Q_OBJECT
    friend class UvFrameParsingThread;
    friend class UvFrameSendingThread;
public:
    explicit UvAbstractFrameProcessor(UvAbstractFrameTransceiver * transceiver);
    ~UvAbstractFrameProcessor();
    unsigned long sendingDelayTime() const;
    void setSendingDelayTime(unsigned long ms);
signals:
    void commandArrived(unsigned char, QByteArray); /*!< command arrived. */
public slots:
    virtual void sendCommand(unsigned char, const QByteArray &) = 0; /*!< Slot for sending command. */
protected:
    UvAbstractFrameTransceiver * transceiver() const;
    QByteArray getReceivedFrame();
    QByteArray getTransferFrame();
    virtual void parse() = 0; /*!< Parse a received frame to a command. */
    virtual void send() = 0; /*!< Send a full frame. */
protected slots:
    void receiveFrame();
    void transferFrame(const QByteArray &);
private:
    UvAbstractFrameProcessorPrivate *d;
};

/*!
 * \class UvFrameParsingThread
 * \brief This thread parse a frame of the received frame queue.
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
class UvFrameParsingThread: public QThread
{
    Q_OBJECT
public:
    explicit UvFrameParsingThread(UvAbstractFrameProcessor * cp);
    ~UvFrameParsingThread();
signals:
protected:
    void run();
private:
    UvAbstractFrameProcessor * processor;
};

/*!
 * \class UvFrameSendingThread
 * \brief This thread send a frame of the transfer frame queue.
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
class UvFrameSendingThread: public QThread
{
    Q_OBJECT
public:
    explicit UvFrameSendingThread(UvAbstractFrameProcessor * cp);
    ~UvFrameSendingThread();
signals:
protected:
    void run();
private:
    UvAbstractFrameProcessor * processor;
};

#endif // UVABSTRACTFRAMEPROCESSOR_H
