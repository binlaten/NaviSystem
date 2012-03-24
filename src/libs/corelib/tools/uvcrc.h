/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvcrc.h
 * \brief Functions for CRC check algorithms.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/06/2011
 */
#ifndef UVCRC_H
#define UVCRC_H

#include "../corelib_global.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

extern CORE_LIB_EXPORT unsigned short uvChecksum(const unsigned char *data, int length);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif // UVCRC_H
