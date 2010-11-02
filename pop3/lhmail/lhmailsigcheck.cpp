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

#       include <openssl/bio.h>
#       include <openssl/crypto.h>
#       include <openssl/lhash.h>
#       include <openssl/conf.h>
#       include <openssl/x509.h>
#       include <openssl/pem.h>
#       include <openssl/ssl.h>

#       ifndef OPENSSL_NO_ENGINE
#       include <openssl/engine.h>
#       endif
#       include <openssl/err.h>

// Not certainly (?)
#       include <signal.h>
#       define FORMAT_PEM      3

#       include "lhmailsigcheck.h"
#       include "lhmail.h"
#       include "lhmailheader.h"

/****************************************************************************/
/*
void LHMailSigCheck::loadCA (const QString &path)
{
        (void)path;
}
*/
/****************************************************************************/

static X509_STORE *setup_verify(BIO *bp, const char *CAfile, const char *CApath)
{
	X509_STORE *store;
	X509_LOOKUP *lookup;
	if(!(store = X509_STORE_new())) goto end;
	lookup=X509_STORE_add_lookup(store,X509_LOOKUP_file());
	if (lookup == NULL) goto end;
	if (CAfile) {
		if(!X509_LOOKUP_load_file(lookup,CAfile,X509_FILETYPE_PEM)) {
			BIO_printf(bp, "Error loading file %s\n", CAfile);
			goto end;
		}
	} else X509_LOOKUP_load_file(lookup,NULL,X509_FILETYPE_DEFAULT);
		
	lookup=X509_STORE_add_lookup(store,X509_LOOKUP_hash_dir());
	if (lookup == NULL) goto end;
	if (CApath) {
		if(!X509_LOOKUP_add_dir(lookup,CApath,X509_FILETYPE_PEM)) {
			BIO_printf(bp, "Error loading directory %s\n", CApath);
			goto end;
		}
	} else X509_LOOKUP_add_dir(lookup,NULL,X509_FILETYPE_DEFAULT);

	ERR_clear_error();
	return store;
	end:
	X509_STORE_free(store);
	return NULL;
}

/****************************************************************************/
/*
static void debug (BIO *b)
{
                char *dat;
                int len = BIO_get_mem_data(b, &dat);
                char *data = (char *)malloc (len + 1);
                memcpy (data, dat, len);
                data[len] = '\0';
                printf (data);
}
*/
/****************************************************************************/

LHMailSigCheck::Result LHMailSigCheck::check (LHMail *m)
{
        if (!m)
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "!m");

        // QString infile = "/home/iwasz/test.msg";
        // QString CAfile = "/home/iwasz/mykey/demoCA/cacert.pem";

        /**
         *  Set stderr as error output (?)
         */

        BIO *bio_err;
        if ((bio_err = BIO_new (BIO_s_file ())) != NULL)
                BIO_set_fp (bio_err, stderr, BIO_NOCLOSE | BIO_FP_TEXT);
        
        /**
         *  Read input file.
         */

        BIO *in;
        /*
        // indata from file
        if (!(in = BIO_new_file (infile.ascii (), "r")))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
        */
        // indata from memory (QString).

        in = BIO_new_mem_buf (m->getSendData().toAscii().data(), m->getSendData ().length());

        /**
         *  Create the output BIO.
         */

        BIO *out;
        out = BIO_new_fp(stdout, BIO_NOCLOSE);

        /**
         *  Prepare for veryfing.
         */

	X509_STORE *store = NULL;
        if (!(store = setup_verify (bio_err, caCert.toAscii(), NULL)))
                qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");

                
        X509_STORE_set_verify_cb_func (store, NULL);

        /**
         *  Parse SMIME data read from input file.
         */

	PKCS7 *p7 = NULL;
        BIO *indata = NULL;
        p7 = SMIME_read_PKCS7(in, &indata);

        if (!p7) {
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "SMIME_read_PKCS7 !p7");
                return SignatureParsingError;
        }

        /**
         *  Verify.
         */

        /*
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "+++++++++++++++++++++++++++++++++++++");
        debug (indata);
        qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "+++++++++++++++++++++++++++++++++++++");
        */
        Result ret;
        if (PKCS7_verify (p7, NULL, store, indata, out, 0)) {
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "PKCS7_verify OK");
                ret = Ok;
        }
        else {
                qDebug ("*** %s,%d : %s", __FILE__, __LINE__, "PKCS7_verify error");
                ret = Error;
                ERR_print_errors(bio_err);
        }

        /**
         *  Write signers do file (?)
         */
        /*
        STACK_OF(X509) *signers;
        signers = PKCS7_get0_signers (p7, NULL, 0);

        if (!save_certs (signerfile, signers))
                qFatal ("*** %s,%d : Error writing signers to %s", __FILE__, __LINE__, signerfile);

        sk_X509_free (signers);
        */
        /**
         *  Free everything.
         */

	X509_STORE_free(store);
	PKCS7_free(p7);
	BIO_free(in);
	BIO_free(indata);
	BIO_free_all(out);

        return ret;
}

/****************************************************************************/

bool LHMailSigCheck::haveSign (LHMail *mail)
{
        QString data = mail->header ().getData ("Content-Type");
        data = data.toLower();        
        return data.contains ("signed");
}

