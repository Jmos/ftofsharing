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

#       ifndef LH_MAIL_MAILCERT_H
#       define LH_MAIL_MAILCERT_H

#       include <qstring.h>
#       include <openssl/ssl.h>

#       include "lhmaildefs.h"

class LHMailCert {
public:

        LHMailCert () : cert (NULL) {}
        ~LHMailCert ();

        LHMailCert(const LHMailCert &c);

        /**
         *  Returns underlying OpenSSL object.
         */

        X509 *unwrap () const { return cert; }

        /**
         *  Not yet implemented.
         */

        void wrap (X509 *) {}
        
        /**
         *  Returns data in PEM format (to be stored in DB for example).
         */

        QString getData () const { return ""; }

        /**
         *  Sets data in PEM format.
         */
         
        void setData (const QString &) {}

        /**
         *  Loads certificate from file.
         */

        bool loadFromFile (const QString &);

        /**
         *  Gets distinguished name.
         */

        QString getDN () const;

private:

        /**
         *  Frees X509 structure.
         */

        void free ();
        X509 *cert;

};

#       endif

