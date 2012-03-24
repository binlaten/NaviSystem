/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvserialport.h
 * \brief Header file of class UvSerialPort.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#ifndef UVSERIALPORT_H
#define UVSERIALPORT_H

#include "../corelib_global.h"
#include "uvserialportdef.h"
#include <QObject>

class UvSerialPortPrivate;

/*!
 * \class UvSerialPort
 * \brief A serial port class to communicate with serial port I/O.
 *
 * This class implements these operates to a serial port.We can open/close
 * a serial port, read and write data to it.
 *
 * \author binlaten(09r1004)
 * \date 01/05/2011
 */
class CORE_LIB_EXPORT UvSerialPort : public QObject
{
    Q_OBJECT
public:
    explicit UvSerialPort(const QString & name,
                          const UvPortSettings & portSettings);
    ~UvSerialPort();
    bool open();
    bool close();
    bool isOpen();
    QByteArray read();
    qint64 write(const QByteArray &data);
signals:
    void readyRead(); /*!< Signal that means the serial port has got some data. */
public slots:
    void reopen();
private:
    UvSerialPortPrivate *d;

};

#endif // UVSERIALPORT_H
