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
 *      18/08/2005                                                          *
 *                                                                          *
 *  Copyright:                                                              *
 *  ----------                                                              *
 *                                                                          *
 *      LeftHand sp. z o. o.    info@lefthand.com.pl                        *
 *                                                                          *
 ****************************************************************************/

#       ifndef LH_MAIL_MAILKEY_H
#       define LH_MAIL_MAILKEY_H

#       include <openssl/ssl.h>
#       include <qstring.h>

#       include "lhmaildefs.h"

class LHMailKey {
public:

        LHMailKey () : key (NULL) {}
        LHMailKey (const LHMailKey &);

        LHMailKey &operator = (const LHMailKey &);
        
        ~LHMailKey ();

        /**
         *  Returns underlying OpenSSL object.
         */

        EVP_PKEY *unwrap () const { return key; }

        /**
         *  Not yet implemented.
         */

        void wrap (EVP_PKEY *) {}
        
        /**
         *  Get data in PEM format.
         */

        QString getData () const { return ""; }

        /**
         *  Set data in PEM format.
         */

        void setData (const QString &) {}

        /**
         *  Load from file. without additional arguments PEM format is assumed.
         */

        bool loadFromFile (const QString &/*,format*/);

private:

        void free ();
        EVP_PKEY *key;

};

#       endif

