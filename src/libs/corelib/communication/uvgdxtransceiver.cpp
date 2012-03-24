/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvgdxtransceiver.cpp
 * \brief Implement file of class UvGdxTransceiver.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/06/2011
 */
#include "corelib/global/uvglobal.h"
#include "uvgdxtransceiver.h"

/*!
 * \class UvGdxTransceiverPrivate
 * \brief A class to store private data for class UvGdxTransceiver.
 */
class UvGdxTransceiverPrivate
{
public:
    uchar leftBytesLength; /*!< Length of left bytes should be received for a full frame. */
};

/*!
 * \fn UvGdxTransceiver::UvGdxTransceiver(UvSerialPort *serialPort)
 * \brief Construct a GDX frame transceiver that transfers and receives
 * data from serial port.
 */
UvGdxTransceiver::UvGdxTransceiver(UvSerialPort *serialPort) :
    UvAbstractFrameTransceiver(serialPort)
{
    d = new UvGdxTransceiverPrivate;
    UV_CHK_PTR(d);
    d->leftBytesLength = 0;
}

/*!
 * \fn UvGdxTransceiver::~UvGdxTransceiver()
 * \brief Standard destructor.
 */
UvGdxTransceiver::~UvGdxTransceiver()
{
    UV_SAFE_DEL(d);
}

/*!
 * \fn bool UvGdxTransceiver::packFrame(QByteArray * dstBuffer,
 *                                      QByteArray * srcBuffer)
 * \brief Pack up the source data \a srcBuffer to a full frame \a dstBuffer
 * under the protocol of GDX. If got a full frame return true, else false.
 */
bool UvGdxTransceiver::packFrame(QByteArray * dstBuffer,
                                 QByteArray * srcBuffer)
{
    int ds = dstBuffer->size();
    for (int i = 0; i < srcBuffer->size(); i++, ds++)
    {
        dstBuffer->append(srcBuffer->at(i));
        if (ds == 0)
        {//got header
            if ((uchar)dstBuffer->at(0) != GDX_HEADER)
            {//bad header
                qDebug("Error: GDX bad header!");
                dstBuffer->clear();
                ds = -1;
            }
        }
        else if (ds == 2)
        {//got length
            d->leftBytesLength = (uchar)dstBuffer->at(2) + 3;
        }
        else if (ds > 2)
        {//got data
            d->leftBytesLength--;
            if (!d->leftBytesLength)
            {//finished receive left data
                if ((uchar)dstBuffer->at(ds) == GDX_TAIL)
                {//good tail
                    srcBuffer->remove(0, i + 1);
                    return true;
                }
                else
                {//bad tail
                    qDebug("Error: GDX bad tail!");
                    dstBuffer->clear();
                    ds = -1;
                }
            }
        }
    }
    srcBuffer->clear();
    return false;
}
