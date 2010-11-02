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

#       ifndef LH_MAIL_LH_SMTP_AUTH_H
#       define LH_MAIL_LH_SMTP_AUTH_H

#       include <QString>

/**
 *  Abstraction of RFC 2554 AUTH mechanism. The class is designed to perform
 *  SASL handshakeing with SMTP servers. Concrete classess are to implement
 *  particuar SASL mechanisms such as PLAIN, LOGIN or CRAM-MD5
 */

class LHSmtpAuth {
public:

        virtual ~LHSmtpAuth () {}

        virtual void reset () = 0;
        virtual QString getMethodName () const = 0;
        virtual QString getOutputLine (bool *end) const = 0;

        void setUsername (const QString &s) { user = s; }
        QString getUsername () const { return user; }

        void setPassword (const QString &s) { pass = s; }
        QString getPassword () const { return pass; }

private:

        QString pass;
        QString user;
        
};

/**
 *  Implementation of LOGIN SASL method.
 */

class LHSmtpAuthLogin : public LHSmtpAuth {
public:

        LHSmtpAuthLogin () : state (0) {}
        virtual ~LHSmtpAuthLogin () {}

        virtual void reset () { state = 0; }
        virtual QString getMethodName () const { return "LOGIN"; }
        virtual QString getOutputLine (bool *end) const;

private:

        mutable uint state;

};

#       endif

