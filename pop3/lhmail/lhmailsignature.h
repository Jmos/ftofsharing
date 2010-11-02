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
 *      08/08/2005                                                          *
 *                                                                          *
 ****************************************************************************/

#       ifndef LH_MAIL_SIGNATURE_H
#       define LH_MAIL_SIGNATURE_H

#       include <qstring.h>

#       include "lhmail.h"
#       include "lhmaildefs.h"
#       include "lhmailcert.h"
#       include "lhmailkey.h"

/****************************************************************************/

class LHMailKey;
class LHMailCert;

/**
 *  Class representing digital S/MIME signature.
 */

class LHMailSignature {
public:

        LHMailSignature ();
        virtual ~LHMailSignature ();

        /**
         *  OpenSSL init.
         */

        static void init ();

        /**
         *  Returnd base64 encoded S/MIME data.
         */

        QString getData () const { return data; }

        /**
         *  Methods to set cert and key files. Actually they only stores file paths, and
         *  true loading is dalayed to LHMail::atachSignature method invokation.
         */

        void setCert (const LHMailCert &);
        void setKey (const LHMailKey &);
        void setBody (const QString &);

        friend LHMailBase* LHMail::attachSignature (LHMailSignature &signature);

private:

        // Used by LHMail
        void setMail (LHMail *mail);
        // Used by LHMail
        void load ();

        void setData (const QString &s) { data = s; }
        // void loadPrv (const QString &certFile, const QString &keyFile, const QString &messageBody);
        void loadPrv (const LHMailCert &cert, const LHMailKey &key, const QString &messageBody);
        // void loadPrv (LHMailCert *cert, LHMailKey *key, const QString &messageBody);

        QString data;        
        QString body;
        LHMailCert cert;
        LHMailKey key;
        LHMail *mail;
        
};

#       endif

