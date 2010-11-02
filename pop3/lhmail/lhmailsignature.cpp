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

#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/lhash.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>

#ifndef OPENSSL_NO_ENGINE
#include <openssl/engine.h>
#endif
#include <openssl/err.h>

// Not certainly (?)
#include <signal.h>

#include "lhmailsignature.h"
#include "lhmailcert.h"
#include "lhmailkey.h"

/****************************************************************************/

LHMailSignature::LHMailSignature ()
{
}

/****************************************************************************/

LHMailSignature::~LHMailSignature ()
{
}

/****************************************************************************/

void LHMailSignature::init ()
{
        /**
         *  Some debug purpose (?)
         */

	CRYPTO_mem_ctrl (CRYPTO_MEM_CHECK_ON);

        /**
         *  Some init purpose (?)
         */

        signal(SIGPIPE,SIG_IGN); // ?
        OpenSSL_add_all_algorithms();
        ERR_load_crypto_strings ();
        ENGINE_load_builtin_engines();
        // setup_ui_method(); // ?
        signal(SIGPIPE,SIG_IGN); // ?
}

/****************************************************************************/

// void LHMailSignature::loadPrv (const QString &certFile, const QString &keyFile, const QString &messageBody)
void LHMailSignature::loadPrv (const LHMailCert &certArg, const LHMailKey &keyArg, const QString &messageBody)
// void LHMailSignature::loadPrv (LHMailCert *certArg, LHMailKey *keyArg, const QString &messageBody)
{
        /**
         *  Set stderr as error output (?)
         */

        BIO *bio_err;
        if ((bio_err = BIO_new (BIO_s_file ())) != NULL)
                BIO_set_fp (bio_err, stderr, BIO_NOCLOSE | BIO_FP_TEXT);

        /**
         *  I think engine is a sort of security device such as magnetic card
         *  reader (?)
         */

        /*
	ENGINE *e = NULL;
	char *engine = NULL;
        */
        // To complex for me to perform this. setup_engine is defined in openssl apps.h and is very complicated.
        // e = setup_engine (bio_err, engine, 0);

        /**
         *  Certificate loading.
         */

        /*
        X509 *signer = NULL;
        if (!(signer = load_cert (bio_err, certFile.ascii (), FORMAT_PEM, NULL, e, "Signer certificate")))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
        */
        X509 *signer = certArg.unwrap ();
        

        /**
         *  Private key load.
         */
        /*
        EVP_PKEY *key = NULL;
        key = load_key (bio_err, keyFile.ascii (), FORMAT_PEM, 0, NULL, e, "signing key file");
        if (!key)
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "Key load failed");
        */
        EVP_PKEY *key = keyArg.unwrap ();

        /**
         *  Clear text file load. File should contain message body.
         */
         
        BIO *in = NULL;

        //QString inFile = "/home/iwasz/mykey/iwasz/file.txt";
        //if (!(in = BIO_new_file (inFile.ascii (), "r")))
        //        qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "Body text file load failed");

        // Instead of loading messageBody from a file, we provide it in method argument.
        in = BIO_new_mem_buf ((void *)messageBody.toAscii().data(), messageBody.length ());

        /**
         *  Redirect app's output to stdout.
         */

        BIO *out = NULL;
        // out = BIO_new_fp (stdout, BIO_NOCLOSE);
        out = BIO_new(BIO_s_mem());

        /**
         *  All needed variables are initialised. Now attempt to perform main routine
         *  task - sign the message.
         */

        int flags = 0;
        PKCS7 *p7 = NULL;
        p7 = PKCS7_sign(signer, key, NULL, in, flags);
        if (!p7)
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!p7 - mesaage sign failed.");

        /**
         *  Finally output smime data.
         */

        // SMIME_write_PKCS7(out, p7, in, 0);
        PEM_write_bio_PKCS7 (out, p7);

        /*
        BUF_MEM *buf = NULL;
        if ((buf=BUF_MEM_new()) == NULL)
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                
        if (!BUF_MEM_grow(buf,size))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
        */

        char *dat;
        long int len = BIO_get_mem_data(out, &dat);

        QByteArray array(dat, len);
        // array->resize (len + 1);

        data = "";
        data.append(array);
        array=QByteArray::fromRawData(dat,len);

        /**
         *  AS for now I don't know how to output raw base64 SMIME data, without Content-type
         *  statements or without headers like ---BEGIN---. So I strip unnecessary characters from
         *  end and begin of data block.
         */

        data.remove ("-----BEGIN PKCS7-----");
        data.remove ("-----END PKCS7-----");
        data.replace ("\n", "\r\n");

        // qDebug (data.ascii ());

        /**
         *  Free memory.
         */

	PKCS7_free(p7);
	BIO_free(in);
	BIO_free_all(out);

        /**
         *  Uninit stuff (?)
         */

	CRYPTO_mem_leaks (bio_err);

        /**
         *  Free bio_err
         */

        BIO_free (bio_err);
}

/**
 *  
 */

void LHMailSignature::setCert (const LHMailCert &certArg)
{
        cert = certArg;
}

/****************************************************************************/

void LHMailSignature::setKey (const LHMailKey &keyArg)
{
        key = keyArg;
}

/****************************************************************************/
/*
void LHMailSignature::setMail (LHMail *m)
{
        mail = m;
}
*/

void LHMailSignature::setBody (const QString &b)
{
        body = b;
}

/****************************************************************************/

void LHMailSignature::load ()
{
        loadPrv (cert, key, body);

#       if 0
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "------------------------------");
        qDebug ("*** %s,%d : (%s)", __FILE__, __LINE__, (const char *)body);
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "------------------------------");
#       endif
}


