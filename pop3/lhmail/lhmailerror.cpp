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
 *      25/07/2005                                                          *
 *                                                                          *
 *  Copyright:                                                              *
 *  ----------                                                              *
 *                                                                          *
 *      LeftHand sp. z o. o.    info@lefthand.com.pl                        *
 *                                                                          *
 ****************************************************************************/

#       include "lhmailerror.h"

/****************************************************************************/

LHMailError::MesgMap LHMailError::mesgMap;

/****************************************************************************/

QString LHMailError::mapMessages (int id)
{
        return mesgMap[id];
}

/**
 *  Do not include trailing dot.
 */

void LHMailError::init ()
{                                 //za¿ó³æ gê¶l± ja¼ñ
        mesgMap.append (tr ("Nieznany b³±d")); //0
        mesgMap.append (tr ("B³±d po³±czenia SMTP")); //1
        mesgMap.append (tr ("Nie mo¿na odebraæ danych")); //2
        mesgMap.append (tr ("Nie mo¿na nawi±zaæ po³±czenia")); //3
        mesgMap.append (tr ("Nie mo¿na znale¼æ hosta")); //4
        mesgMap.append (tr ("Nieoczekiwana odpowied¼ od serwera")); //5
}


