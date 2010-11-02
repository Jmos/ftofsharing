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

#       include <openssl/engine.h>
#       include <openssl/err.h>
#       include <openssl/bio.h>
#       include <openssl/crypto.h>
#       include <openssl/lhash.h>
#       include <openssl/conf.h>
#       include <openssl/x509.h>
#       include <openssl/pem.h>

#       include "lhmailcert.h"

/****************************************************************************/

LHMailCert::~LHMailCert ()
{
        free ();
}

/****************************************************************************/

void LHMailCert::free ()
{
        //if (cert)
	//        X509_free (cert);
}

/**
 *  Load certificate in PEM format (only PEM. format arg is ommited).
 */

static X509 *load_cert(BIO *err, const char *file, int format,
	const char *pass, ENGINE *e, const char *cert_descrip)
	{
	ASN1_HEADER *ah=NULL;
	BUF_MEM *buf=NULL;
	X509 *x=NULL;
	BIO *cert;

	if ((cert=BIO_new(BIO_s_file())) == NULL)
		{
		ERR_print_errors(err);
                // qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                return NULL;
		}

        if (BIO_read_filename(cert,file) <= 0)
			{
			// BIO_printf(err, "Error opening %s %s\n", cert_descrip, file);
			// ERR_print_errors(err);
                        // qFatal ("*** %s,%d : %s", __FILE__, __LINE__, file);
                        return NULL;
			}


	else if (format == FORMAT_PEM)
		x = PEM_read_bio_X509_AUX (cert,NULL, NULL, (char *)("spleenwort"));


        if (x == NULL)
		{
		// BIO_printf(err,"unable to load certificate\n");
		// ERR_print_errors(err);
                return NULL;
	        }
                
	if (ah != NULL) ASN1_HEADER_free(ah);
	if (cert != NULL) BIO_free(cert);
	if (buf != NULL) BUF_MEM_free(buf);
	return(x);
}

/****************************************************************************/

LHMailCert::LHMailCert (const LHMailCert &c)
{
        cert = X509_dup (c.unwrap ());
}

/****************************************************************************/

bool LHMailCert::loadFromFile (const QString &path)
{
        /**
         *  Private key load.
         */

        BIO *bio_err;
	ENGINE *e = NULL;

        cert = load_cert (bio_err, path.toAscii (), FORMAT_PEM, NULL, e, "Signer certificate");

        if (!cert)
                return false;

        return true;
}

