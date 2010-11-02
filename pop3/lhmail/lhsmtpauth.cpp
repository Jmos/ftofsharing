/****************************************************************************
 *                                                                          *
 *  Author:                                                                 *
 *  -------                                                                 *
 *                                                                          *
 *      Lukasz Iwaszkiewicz   lukasz.iwaszkiewicz@lefthand.com.pl           *
 *                            lukasz.iwaszkiewicz@software.com.pl           *
 *                                                                          *
 *  Date :                                                                  *
 *  ------                                                                  *
 *                                                                          *
 *      15/07/2005                                                          *
 *                                                                          *
 ****************************************************************************/

#       include "lhmime.h"
#       include "lhsmtpauth.h"

/****************************************************************************/

QString LHSmtpAuthLogin::getOutputLine (bool *end) const
{
        state++;

        switch (state - 1) {
        case 0:
                *end = false;
                return "AUTH LOGIN\r\n";

        case 1:
                *end = false;
                return LHMime::encodeBase64Data (getUsername ().toAscii (), getUsername ().length ()) + QString ("\r\n");

        case 2:
                *end = true;
                return LHMime::encodeBase64Data (getPassword ().toAscii (), getPassword ().length ()) + QString ("\r\n");

        default:
                *end = true;
                state = 0;
                return QString::null;
        }

}

