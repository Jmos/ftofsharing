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
#       include <openssl/asn1.h>
#       include <openssl/dsa.h>
#       include <openssl/dh.h>

#       include "lhmailkey.h"

/****************************************************************************/

LHMailKey::~LHMailKey ()
{
}

/****************************************************************************/

void LHMailKey::free ()
{
        if (key)
	        EVP_PKEY_free(key);
}

/**
 *  Load key in PEM format (only PEM. format arg is ommited).
 */

static EVP_PKEY *load_key(BIO *err, const char *file, int format, int maybe_stdin,
	const char *pass, ENGINE *e, const char *key_descrip)
	{
	BIO *key=NULL;
	EVP_PKEY *pkey=NULL;

	key=BIO_new(BIO_s_file());

        if (key == NULL)
		{
		ERR_print_errors(err);
                // qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                return NULL;
		}

        if (BIO_read_filename(key,file) <= 0)
			{
			BIO_printf(err, "Error opening %s %s\n",
				key_descrip, file);
			ERR_print_errors(err);
                        // qFatal ("*** %s,%d : %s", __FILE__, __LINE__, "");
                        return NULL;
                        
			}

                        
	else if (format == FORMAT_PEM)
		{
		pkey=PEM_read_bio_PrivateKey(key,NULL, NULL, (char*)"spleenwort");
		}

	if (key != NULL) BIO_free(key);

	if (pkey == NULL)
		BIO_printf(err,"unable to load %s\n", key_descrip);

	return(pkey);
	}

/****************************************************************************/

static ::EVP_PKEY *copy (::EVP_PKEY *key)
{
  ::EVP_PKEY * pkey = key;

  // We're using the EVP_PKEY_var even though it depends on this
  // trait function.  This works since we're not actually using
  // any of the EVP_PKEY_var methods that call this copy()
  // trait.  This allows us to maintain exception safety.
  EVP_PKEY *p = ::EVP_PKEY_new ();

  switch (::EVP_PKEY_type (pkey->type))
    {
    case EVP_PKEY_RSA:
      {
        RSA * rsa = ::EVP_PKEY_get1_RSA (pkey);
        if (rsa != 0)
          {
            // Not exception safe!
            ::EVP_PKEY_set1_RSA (p, RSAPrivateKey_dup (rsa));
            ::RSA_free (rsa);
          }
      }
      break;

    case EVP_PKEY_DSA:
      {
        DSA * dsa = ::EVP_PKEY_get1_DSA (pkey);
        if (dsa != 0)
          {
            // Not exception safe!
            ::EVP_PKEY_set1_DSA(p, DSAparams_dup(dsa));
            ::DSA_free(dsa);
          }
      }
      break;

    case EVP_PKEY_DH:
      {
        DH * dh = ::EVP_PKEY_get1_DH (pkey);
        if (dh != 0)
          {
            // Not exception safe!
            ::EVP_PKEY_set1_DH (p, DHparams_dup (dh));
            ::DH_free(dh);
          }
      }
      break;

    default:
      // We should never get here!
      return 0;
    }

  return p;
}

/****************************************************************************/

LHMailKey::LHMailKey (const LHMailKey &k)
{
        key = copy (k.unwrap ());
}

/****************************************************************************/

LHMailKey &LHMailKey::operator= (const LHMailKey &k)
{
        key = copy (k.unwrap ());
        return *this;
}

/****************************************************************************/

bool LHMailKey::loadFromFile (const QString &path)
{
        /**
         *  Private key load.
         */

        BIO *bio_err;
	ENGINE *e = NULL;

        key = load_key (bio_err, path.toAscii (), FORMAT_PEM, 0, NULL, e, "signing key file");

        if (!key)
                return false;

        return true;
}

