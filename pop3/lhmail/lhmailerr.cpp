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

#       include "lhmailerr.h"

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
{                                 //za���� g��l� ja��
        mesgMap.append (tr ("Nieznany b��d")); //0
        mesgMap.append (tr ("B��d po��czenia SMTP")); //1
        mesgMap.append (tr ("Nie mo�na odebra� danych")); //2
        mesgMap.append (tr ("Nie mo�na nawi�za� po��czenia")); //3
        mesgMap.append (tr ("Nie mo�na znale�� hosta")); //4
        mesgMap.append (tr ("Nieoczekiwana odpowied� od serwera")); //5
}


