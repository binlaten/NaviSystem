/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#ifndef UVNAVISERVER_GLOBAL_H
#define UVNAVISERVER_GLOBAL_H

#ifdef Q_OS_WIN
    #include "windows.h"
#endif

#define SCREEN_WIDTH        800 /*!< screen width */
#define SCREEN_HEIGHT       480 /*!< screen height */

#define GPS_APP_NAME        "StorageCard\\AudioGPS.exe" /*!< GPS applicaion name */
#define GPS_WND_NAME        "NaviSystem" /*!< GPS window name, for sending message */

#define MY_WND_NAME        "MCONTROL" /*!< my window name for receiving message */

#define APP_CONFIG_FILE     "config.ucfg" /*!< application config file path */
class UvAppConfig;
extern UvAppConfig *g_appConfig; /*!< application config, this is a global variable! */

#endif // UVNAVISERVER_GLOBAL_H
