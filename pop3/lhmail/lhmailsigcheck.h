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

#       ifndef SIGNATURE_CHECKER_H
#       define SIGNATURE_CHECKER_H

#       include <qstring.h>
#       include "lhmaildefs.h"

class LHMail;

/****************************************************************************/

class LHMailSigCheck {
public:

        enum Result { Ok, Error, SignatureParsingError };

        /**
         *  Checks whether passed LHMail object have SMIME sign or not.
         */

        bool haveSign (LHMail *);

        void loadCA (const QString &path) { caCert = path; }
        Result check (LHMail *);

private:

        QString caCert;

};

#       endif


