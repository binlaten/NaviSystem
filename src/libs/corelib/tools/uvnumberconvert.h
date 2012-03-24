/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvnumberconvert.h
 * \brief Functions for number converting algorithms.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/13/2011
 */
#ifndef UVNUMBERCONVERT_H
#define UVNUMBERCONVERT_H

#include "../corelib_global.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

extern CORE_LIB_EXPORT     int uvBcd2Dec(unsigned char bcd);
extern CORE_LIB_EXPORT     unsigned char uvDec2Bcd(int dec);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif // UVNUMBERCONVERT_H
