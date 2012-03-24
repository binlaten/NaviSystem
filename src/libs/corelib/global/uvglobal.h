/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file uvglobal.h
 * \brief Global defines used in this system.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#ifndef UVGLOBAL_H
#define UVGLOBAL_H

/*!
 * \def UV_NO_DEBUG
 * \brief NO debug message output.
 */
#ifndef UV_NO_DEBUG
#  ifdef QT_NO_DEBUG_OUTPUT
#    define UV_NO_DEBUG
#  endif
#endif

#ifndef UV_NO_DEBUG
#include <QDebug>
#endif

/*!
 * \def UV_CHK_PTR(ptr)
 * \brief Check the pointer \a ptr.
 */
#define UV_CHK_PTR(ptr) Q_CHECK_PTR(ptr)

/*!
 * \def UV_SAFE_DEL(ptr)
 * \brief Delete the pointer \a ptr safely.
 */
#define UV_SAFE_DEL(ptr) \
do { \
    if (ptr) \
    { \
        delete ptr; \
        ptr = 0; \
    } \
} while (0)

#endif // UVGLOBAL_H
