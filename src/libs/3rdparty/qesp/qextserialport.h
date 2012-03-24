
#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#include "qesp_global.h"

/*POSIX CODE*/
#ifdef _TTY_POSIX_
    #include "posix_qextserialport.h"
    #define QextBaseType Posix_QextSerialPort

/*MS WINDOWS CE CODE*/
#elif defined(_TTY_WINCE_)
    #include "wince_qextserialport.h"
    #define QextBaseType Wince_QextSerialPort

/*MS WINDOWS CODE*/
#else
    #include "win_qextserialport.h"
    #define QextBaseType Win_QextSerialPort

#endif

class QESP_LIB_EXPORT QextSerialPort: public QextBaseType
{
	Q_OBJECT
	
	public:
		typedef QextSerialBase::QueryMode QueryMode;

	    QextSerialPort();
	    QextSerialPort(const QString & name, QueryMode mode = QextSerialPort::Polling);
	    QextSerialPort(PortSettings const& s, QueryMode mode = QextSerialPort::Polling);
	    QextSerialPort(const QString & name, PortSettings const& s, QueryMode mode = QextSerialPort::Polling);
	    QextSerialPort(const QextSerialPort& s);
	    QextSerialPort& operator=(const QextSerialPort&);
	    virtual ~QextSerialPort();

};

#endif
