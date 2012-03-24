/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#include <QtGui/QApplication>
#include "uvnaviserver.h"

/*!
 * \fn int main(int argc, char *argv[])
 * Entry of this application.
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UvNaviServer server;
    server.start();
    return a.exec();
}
