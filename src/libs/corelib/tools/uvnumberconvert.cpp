/****************************************************************************
**
** Copyright (C) 2011 UltraView Technology Limited. All rights reserved.
** Contact: UltraView (09r1004@mail.ultra-view.com)
**
****************************************************************************/

#include "uvnumberconvert.h"

/*!
 * \fn int uvBcd2Dec(unsigned char bcd)
 * Convert BCD code \a bcd to corresponding decimalization number.
 * \param [in] bcd BCD code need to be converted.
 * \return decimalization number of \a bcd.
 */
int uvBcd2Dec(unsigned char bcd)
{
    unsigned char h4 = (bcd >> 4) & 0x0F;
    unsigned char l4 = bcd & 0x0F;
    if ((h4 < 0x0a) && (l4 < 0x0a))
    {
        return ((h4 << 3) + (h4 << 1) + l4);
    }
    else
    { // bad bcd code
        return 0;
    }
}

/*!
 * \fn unsigned char uvDec2Bcd(int dec)
 * Convert decimalization number(0 ~ 99) \a dec to corresponding BCD code.
 * \param [in] dec decimalization number need to be converted.
 * \return BCD code of \a dec.
 */
unsigned char uvDec2Bcd(int dec)
{
    if ((dec < 0) || (dec > 99))
    { // bad dec
        return 0;
    }
    else
    {
        unsigned char h4 = (dec / 10) & 0x0F;
        unsigned char l4 = (dec % 10) & 0x0F;
        return (h4 << 4 | l4);
    }
}
