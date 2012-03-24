/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

/*!
 * \file corelib_global.h
 * \brief This file sets global things up for c++ dynamic or static library - corelib.
 * \version 1.0.0
 * \author binlaten(09r1004)
 * \date created: 01/05/2011
 */
#ifndef CORELIB_GLOBAL_H
#define CORELIB_GLOBAL_H

#include <QtCore/qglobal.h>

/*!
 * \def CORE_LIB_EXPORT
 * \brief A special export keyword for corelib library
 */
#if defined(QT_LIB_STATIC) // qt library is static
#  define CORE_LIB_EXPORT // core library will be static too
#else // qt library is shared
#  if defined(CORE_SHARED_LIBRARY) // bulid shared
#    define CORE_LIB_EXPORT Q_DECL_EXPORT
#  elif  defined(CORE_STATIC_LIBRARY) // build or import static
#    define CORE_LIB_EXPORT
#  else // import shared
#    define CORE_LIB_EXPORT Q_DECL_IMPORT
#  endif
#endif

#endif // CORELIB_GLOBAL_H
