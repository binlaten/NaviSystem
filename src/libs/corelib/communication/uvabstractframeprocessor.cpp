/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/
/*!
 * \file uvabstractframeprocessor.cpp
 * \brief Implement file of class UvAbstractFrameProcessor.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#include "corelib/global/uvglobal.h"
#include "uvabstractframeprocessor.h"
#include "uvabstractframetransceiver.h"
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QMutexLocker>

/*!
 * class UvAbstractFrameProcessorPrivate
 * \brief A class to store private data for class UvAbstractFrameProcessor.
 */
class UvAbstractFrameProcessorPrivate
{
public:
    UvAbstractFrameTransceiver * transceiver; /*!< Frame transceiver object pointer. */
    UvFrameParsingThread * parsingThread; /*!< Thread for parsing frames had been received. */
    UvFrameSendingThread * sendingThread; /*!< Thread for sending frames need to be transfered. */

    QQueue<QByteArray *> receivedFrames; /*!< Received frames queue. */
    QQueue<QByteArray *> transferFrames; /*!< Transfer frames queue. */

    QMutex receiveMutex; /*!< Received frames queue mutex. */
    QWaitCondition receiveQueueReady; /*!< Received frames queue ready. */
    QMutex transferMutex; /*!< transfer frames queue mutex. */
    QWaitCondition transferQueueReady; /*!< Transfer frames queue ready. */

    unsigned long sendingDelayTime; /*!< delay time after every frame sent. */
};

/*!
 * \fn UvAbstractFrameProcessor::UvAbstractFrameProcessor(UvAbstractFrameTransceiver *transceiver)
 * \brief Construct a frame processor.
 */
UvAbstractFrameProcessor::UvAbstractFrameProcessor(UvAbstractFrameTransceiver *transceiver)
{
    d = new UvAbstractFrameProcessorPrivate;
    UV_CHK_PTR(d);

    d->transceiver = transceiver;
    UV_CHK_PTR(d->transceiver);
    connect(d->transceiver, SIGNAL(frameReady()), this, SLOT(receiveFrame()));

    //create and start frame parsing and sending thread
    d->parsingThread = new UvFrameParsingThread(this);
    UV_CHK_PTR(d->parsingThread);
    d->parsingThread->start();
    d->sendingThread = new UvFrameSendingThread(this);
    UV_CHK_PTR(d->sendingThread);
    d->sendingThread->start();

    d->sendingDelayTime = 0;
}

/*!
 * \fn UvAbstractFrameTransceiver::~UvAbstractFrameTransceiver()
 * \brief Standard destructor.
 */
UvAbstractFrameProcessor::~UvAbstractFrameProcessor()
{
    d->transceiver = 0;
    d->parsingThread->wait(1000);
    d->sendingThread->wait(1000);
    UV_SAFE_DEL(d->parsingThread);
    UV_SAFE_DEL(d->sendingThread);
    UV_SAFE_DEL(d);
}

/*!
 * \fn unsigned long UvAbstractFrameProcessor::sendingDelayTime() const
 * \brief Get the sending delay time. After one frame sent, the next
 * one will be sent in such a sending delay time.
 */
unsigned long UvAbstractFrameProcessor::sendingDelayTime() const
{
    return d->sendingDelayTime;
}

/*!
 * \fn void UvAbstractFrameProcessor::setSendingDelayTime(unsigned long ms)
 * \brief Set the sending delay time to \a ms. After one frame sent,
 * the next one will be sent in such a transfer delay time.
 */
void UvAbstractFrameProcessor::setSendingDelayTime(unsigned long ms)
{
    d->sendingDelayTime = ms;
}

/*!
 * \fn UvAbstractFrameTransceiver * UvAbstractFrameProcessor::transceiver() const
 * \brief Return the frame transceiver.
 */
UvAbstractFrameTransceiver * UvAbstractFrameProcessor::transceiver() const
{
    return d->transceiver;
}

/*!
 * \fn QByteArray UvAbstractFrameProcessor::getReceivedFrame()
 * \brief Get a frame from the received queue.
 */
QByteArray UvAbstractFrameProcessor::getReceivedFrame()
{
    QByteArray *prf;
    {
        QMutexLocker locker(&d->receiveMutex);
        if (d->receivedFrames.isEmpty())
        {
            d->receiveQueueReady.wait(&d->receiveMutex);
        }
        prf = d->receivedFrames.dequeue();
    }
    QByteArray ba(*prf);
    UV_SAFE_DEL(prf);
    return ba;
}

/*!
 * \fn QByteArray UvAbstractFrameProcessor::getTransferFrame()
 * \brief Get a frame from the transfer queue.
 */
QByteArray UvAbstractFrameProcessor::getTransferFrame()
{
    QByteArray * ptf;
    {
        QMutexLocker locker(&d->transferMutex);
        if (d->transferFrames.isEmpty())
        {
            d->transferQueueReady.wait(&d->transferMutex);
        }
        ptf = d->transferFrames.dequeue();
    }
    QByteArray ba(*ptf);
    UV_SAFE_DEL(ptf);
    return ba;
}

/*!
 * \fn void UvAbstractFrameProcessor::receiveFrame()
 * \brief Slot for receiving the full frame from transceiver,
 * and add this frame to received queue.
 */
void UvAbstractFrameProcessor::receiveFrame()
{
    QByteArray * prf = new QByteArray(d->transceiver->readFrame());
    UV_CHK_PTR(prf);
    {
        QMutexLocker locker(&d->receiveMutex);
        d->receivedFrames.enqueue(prf);
    }
    d->receiveQueueReady.wakeOne();
}

/*!
 * \fn void UvAbstractFrameProcessor::transferFrame(const QByteArray & frame)
 * \brief Slot for transfering a frame to transfer queue.
 */
void UvAbstractFrameProcessor::transferFrame(const QByteArray & frame)
{
    QByteArray * ptf = new QByteArray(frame);
    UV_CHK_PTR(ptf);
    {
        QMutexLocker locker(&d->transferMutex);
        d->transferFrames.enqueue(ptf);
    }
    d->transferQueueReady.wakeOne();
}


/////////////////////////////////////////////////////////////////////////////////

/*!
 * \fn UvFrameParsingThread::UvFrameParsingThread(UvAbstractFrameProcessor * uafp)
 * Constructor with a parameter indicates the one needs this thread.
 */
UvFrameParsingThread::UvFrameParsingThread(UvAbstractFrameProcessor * uafp) :
    processor(uafp)

{
}

/*!
 * \fn UvFrameParsingThread::~UvFrameParsingThread()
 * Standard destructor.
 */
UvFrameParsingThread::~UvFrameParsingThread()
{
    processor = 0;
}

/*!
 * \fn void UvFrameParsingThread::run()
 * Reimplement from QThread, pasrse one frame in the received queue.
 */
void UvFrameParsingThread::run()
{
    forever
    {
        processor->parse();
    }
}

/*!
 * \fn UvFrameSendingThread::UvFrameSendingThread(UvAbstractFrameProcessor * uafp)
 * Constructor with a parameter indicates the one needs this thread.
 */
UvFrameSendingThread::UvFrameSendingThread(UvAbstractFrameProcessor * uafp) :
    processor(uafp)
{
}

/*!
 * \fn UvFrameSendingThread::~UvFrameSendingThread()
 * Standard destructor.
 */
UvFrameSendingThread::~UvFrameSendingThread()
{
    processor = 0;
}

/*!
 * \fn void UvFrameSendingThread::run()
 * Reimplement from QThread, send one frame in the transfer queue.
 */
void UvFrameSendingThread::run()
{
    forever
    {
        processor->send();
        msleep(processor->sendingDelayTime());
    }
}
