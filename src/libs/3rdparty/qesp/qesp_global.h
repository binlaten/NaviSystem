/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file qesp_global.h
 * \brief This file sets global things up for c++ dynamic or static library - qesp.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#ifndef QESP_GLOBAL_H
#define QESP_GLOBAL_H

#include <QtCore/qglobal.h>

/*!
 * \def QESP_LIB_EXPORT
 * \brief A special export keyword for qesp library.
 */
#if defined(QT_LIB_STATIC) // qt library is static
#  define QESP_LIB_EXPORT // qesp library will be static too
#else // qt library is shared
#  if defined(QESP_SHARED_LIBRARY) // bulid shared
#    define QESP_LIB_EXPORT Q_DECL_EXPORT
#  elif  defined(QESP_STATIC_LIBRARY) // build or import static
#    define QESP_LIB_EXPORT
#  else // import shared
#    define QESP_LIB_EXPORT Q_DECL_IMPORT
#  endif
#endif

/*!
 * Platform-specific defines.
 */
/*@{*/
#if defined(Q_OS_WIN32)
#  define _TTY_WIN_
#elif defined(Q_OS_WINCE)
#  define _TTY_WINCE_
#else
#  define _TTY_POSIX_
#endif
/*!@}*/

#endif // QESP_GLOBAL_H
