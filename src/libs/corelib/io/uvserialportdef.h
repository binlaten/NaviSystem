/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvserialportdef.h
 * \brief Defines for serial port.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#ifndef UVSERIALPORTDEF_H
#define UVSERIALPORTDEF_H

#include "3rdparty/qesp/qextserialbase.h"

/*!
 * \name Serial port settings types
 * Our serial port settings types for portability
 */
/*@{*/
/*!
 * \typedef UvBaudRate
 * \brief Baud Rate
 */
typedef BaudRateType        UvBaudRate;

/*!
 * \typedef UvDataBits
 * \brief Data Bits
 */
typedef DataBitsType        UvDataBits;

/*!
 * \typedef UvStopBits
 * \brief Stop Bits
 */
typedef StopBitsType        UvStopBits;

/*!
 * \typedef UvParity
 * \brief Parity
 */
typedef ParityType          UvParity;

/*!
 * \typedef UvFlowControl
 * \brief Flow Control
 */
typedef FlowType          UvFlowControl;

/*!
 * \typedef UvPortSettings
 * \brief Port Settings
 */
typedef struct PortSettings UvPortSettings;
/*@}*/

/*! \name Serial Port Settings Flags
 *  These are the currently supported flags for the serial port settings
 */
/*@{*/

/*! \name Baud rate available values */
/*@{*/
#define UV_BAUD_4800        BAUD4800 /*!< Baud rate: 4800 bps */
#define UV_BAUD_9600        BAUD9600 /*!< Baud rate: 9600 bps */
#define UV_BAUD_14400       BAUD14400 /*!< Baud rate: 14400 bps */
#define UV_BAUD_19200       BAUD19200 /*!< Baud rate: 19200 bps */
#define UV_BAUD_38400       BAUD38400 /*!< Baud rate: 38400 bps */
#define UV_BAUD_57600       BAUD57600 /*!< Baud rate: 57600 bps */
#define UV_BAUD_115200      BAUD115200 /*!< Baud rate: 115200 bps */
/*!@}*/

/*! \name Data bit available values */
/*@{*/
#define UV_DATABIT_5        DATA_5 /*!< Data bit: 5 bits */
#define UV_DATABIT_6        DATA_6 /*!< Data bit: 6 bits */
#define UV_DATABIT_7        DATA_7 /*!< Data bit: 7 bits */
#define UV_DATABIT_8        DATA_8 /*!< Data bit: 8 bits */
/*!@}*/

/*! \name Stop bit available values */
/*@{*/
#define UV_STOPBIT_1        STOP_1 /*!< Stop bit: 1 bit */
#define UV_STOPBIT_2        STOP_2 /*!< Stop bit: 2 bit */
/*!@}*/

/*! \name Parity available flags */
/*@{*/
#define UV_PARITY_NONE      PAR_NONE /*!< Parity: none */
#define UV_PARITY_ODD       PAR_ODD /*!< Parity: odd */
#define UV_PARITY_EVEN      PAR_EVEN /*!< Parity: even */
/*!@}*/

/*! \name Flow control flags */
/*@{*/
#define UV_FLOW_OFF         FLOW_OFF /*!< No flow conntrol */
#define UV_FLOW_HARDWARE    FLOW_HARDWARE /*!< Hardware flow conntrol */
/*!@}*/

/*!@}*/

#endif // UVSERIALPORTDEF_H
