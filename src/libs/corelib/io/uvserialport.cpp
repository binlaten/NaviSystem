/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvserialport.cpp
 * \brief Implement file of class UvSerialPort.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#include "corelib/global/uvglobal.h"
#include "3rdparty/qesp/qextserialport.h"
#include "uvserialport.h"

/*!
 * \class UvSerialPortPrivate
 * \brief A class to store private data for class UvSerialPort.
 */
class UvSerialPortPrivate
{
public:
    QextSerialPort * qesp; /*!< Serial port class under our system. */
};

/*!
 * \fn UvSerialPort::UvSerialPort(const QString & name,
 *                                const UvPortSettings & settings)
 * \brief Construct a serial port with specified port name and settings
 * \param [in] name Port name.
 * \param [in] settings Port settings
 */
UvSerialPort::UvSerialPort(const QString & name,
                           const UvPortSettings & settings)
{
    d = new UvSerialPortPrivate;
    UV_CHK_PTR(d);
    d->qesp = new QextSerialPort(name, settings,
                                 QextSerialBase::EventDriven);
    UV_CHK_PTR(d->qesp);
    connect(d->qesp, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
}

/*!
 * \fn UvSerialPort::~UvSerialPort()
 * \brief Standard destructor.
 */
UvSerialPort::~UvSerialPort()
{
    if (isOpen())
    {
        close();
    }
    UV_SAFE_DEL(d->qesp);
    UV_SAFE_DEL(d);
}

/*!
 * \fn bool UvSerialPort::open()
 * \brief Open a serial port in read / write mode.
 * \return Open it succeeded or failed.
 */
bool UvSerialPort::open()
{
    return d->qesp->open(QIODevice::ReadWrite);
}

/*!
 * \fn bool UvSerialPort::close()
 * \brief Close the opened serial port.
 * \return Close it succeeded or failed.
 */
bool UvSerialPort::close()
{
    d->qesp->close();
    return true;
}

/*!
 * \fn bool UvSerialPort::isOpen()
 * \brief Query the serial port is open or not.
 * \return If open returns true, else false.
 */
bool UvSerialPort::isOpen()
{
    return d->qesp->isOpen();
}

/*!
 * \fn UvSerialPort::reopen()
 * \brief Reopen a serial port.
 */
void UvSerialPort::reopen()
{
    if (isOpen())
    {
        close();
    }
    open();
}

/*!
 * \fn QByteArray UvSerialPort::read()
 * \brief Read all the data in serial port receive buffer.
 * \return Data has read.
 */
QByteArray UvSerialPort::read()
{
    if (isOpen())
    {
        return d->qesp->readAll();
    }
    else
    {
        qDebug("Error: serial port read failed! Not open yet!");
        return QByteArray();
    }
}

/*!
 * \fn qint64 UvSerialPort::write(const QByteArray &data)
 * \brief Write a serial data to serial port.
 * \param [in] data Data will be write to serial port.
 * \return Bytes wrote to serial port.
 */
qint64 UvSerialPort::write(const QByteArray &data)
{
    if (isOpen())
    {
        return d->qesp->write(data);
    }
    else
    {
        qDebug("Error: serial port write failed! Not open yet!");
        return -1;
    }
}
