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

#       ifndef LH_MAIL_ERROR_H
#       define LH_MAIL_ERROR_H

#include <QtCore>
#include <QtNetwork>

/**
 *  Class representing an error.
 */

class LHMailError : public QObject {

        Q_OBJECT

public:

        LHMailError (int i = 0, const QVariant &v = QVariant ()) : id (i), additional (v) {}

        bool operator== (const LHMailError &err) { return (err.id == id); }
        bool operator!= (const LHMailError &err) { return (err.id != id); }

        int getId () const { return id; }
        void setId (int i) { id = i; }

        QString getMessage () const { return mapMessages (id); }
        QVariant getAdditional () const { return additional; }
        
        static void init ();

private:

        static QString mapMessages (int id);

        // typedef QMap <int, QString> MesgMap;
        typedef QVector<QString> MesgMap;
        static MesgMap mesgMap;

        int id;
        QVariant additional;

};

#       endif

