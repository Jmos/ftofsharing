/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#include "lhmime.h"

#include "mime-types.h"

#include <qtextcodec.h>
#include <qdatetime.h>
#include <qfileinfo.h>
#include <qmap.h>
#include <qstringlist.h>

//#       include <iostream>
#       include <fstream>

class LHMimePrivate
{
public:
	static QMap<QString, QString> app_mime;
};

QMap<QString, QString> LHMimePrivate::app_mime;

struct str
{
	char *s;	/* the string's content */
	size_t l;	/* the stings's length */
};

static const uint ENCODED_HEADER_WORD_MAX_LEN = 75;

char *
strtob64(const char *p);            

char *
memtob64(const void *vp, size_t isz);

char* 
c_mime_str_toqp(const char* in, int in_body);

static int
mustquote_hdr(int c);

char*
mime_fromhdr(struct str *in, struct str *out, int flags);

QString LHMime::getCString(const QString& str, const char* codec_name)
{
	QTextCodec* qtc = QTextCodec::codecForCStrings();
	QTextCodec* n_qtc = QTextCodec::codecForName(codec_name);
	if (!n_qtc)
	{
		//qDebug()<<"LHMime::getCString, No codec: "<<codec_name<<", string: "<<str;
	}
	else
	{
		//qDebug("LHMime::getCString, using codec: %s, string: %s", n_qtc->mimeName(), (const char*) str);
		QTextCodec::setCodecForCStrings(n_qtc);
	}
	QString cstr = (QString)str; //str.local8Bit();
	if (n_qtc)
	{
		QTextCodec::setCodecForCStrings(qtc);
	}
	return cstr;    
}

QString LHMime::fromCString(const QString& str, const char* codec_name)
{
	QTextCodec* qtc = QTextCodec::codecForCStrings();
	QTextCodec* n_qtc = QTextCodec::codecForName(codec_name);
	if (!n_qtc)
	{
		qWarning("LHMime::fromCString, No codec: %s", codec_name);
	}
	else
	{
		//qDebug("LHMime::fromCString, using codec: %s ", n_qtc->mimeName());
		QTextCodec::setCodecForCStrings(n_qtc);
	}
	QString rstr = (QString)str; //str.local8Bit();
	if (n_qtc)
	{
		QTextCodec::setCodecForCStrings(qtc);
	}
	return rstr;    
}

QString LHMime::encodeBase64(const QString& str, const char* codec_name)
{
	//This can be problem on Windows!
	QString cstr = LHMime::getCString(str, codec_name);

	const char * in = cstr.toAscii().constData();
	//char * in = (char*) malloc (strlen(in0) + 1);
	//strcpy(in, in0);
	char * out = strtob64(in);
	QString ret(out);
	free(out);
	//free(in);
	return ret;
};

char* LHMime::encodeBase64(const char* str)
{
	return strtob64(str);
};

char* LHMime::encodeBase64Data(const char* str, uint len)
{
	return memtob64(str, len);
};

QString LHMime::encodeBase64Data(const QByteArray& barr)
{
	QString ret;
	char* enc = memtob64(barr.data(), barr.size());
	ret=QString::fromAscii(enc, qstrlen(enc));
	return ret;
}

QString LHMime::encodeMailHeader(const QString& str, const char* codec_name, const char enc)
{
	QStringList words;
	words = str.split(" ");
	QString ret;
	bool first = TRUE;
	
	for (QStringList::Iterator it = words.begin(); it != words.end(); ++it)
	{
		QString word;
		if (!first)
		{
			word = " " + *it;
		}
		else
		{
			word = *it;
			first = FALSE;
		}
		
		//first check if we have to quote
		uint quote = testQuoteHeader(word);
		if (quote>0)
		{
			if (enc == 'q' || enc == 'b')
			{
				ret += encodeHeaderQB(word, enc, codec_name);
			}
			else if (enc == 'c')
			{
				if (quote > str.length()/2 || str.length() > 15)
				{
					ret += encodeHeaderQB(word, 'b', codec_name);
				}
				else
				{
					ret += encodeHeaderQB(word, 'q', codec_name);
				}
			}
			else
			{
				qFatal("encodeMailHeader, wrong enc: %c", enc); 
			}
		}
		else
		{
			
			ret += getCString(word, codec_name);
		}
	}
	return ret;
}

QString LHMime::decodeMailHeader(const QString& str)
{
	QString ret;
	
	struct str in;
	struct str out;

	in.s = str.toLatin1().data();
	in.l = str.length();

	char* encoding = mime_fromhdr(&in, &out, 0);
	
	if (encoding)
	{
		//qDebug("decodeMailHeader, encoding: %s", encoding);
		QString cstr;
		cstr=QString::fromAscii(out.s, out.l+1);
		ret=fromCString(cstr, encoding);
	}
	else
	{
		ret = QString::fromLatin1(out.s);
		free(out.s);
	}

	if (encoding)
	{
		free(encoding);
	}

	return ret;
}

static void mime_fromqp(struct str *in, struct str *out, int ishdr);

QString LHMime::decodeMailBody(const QString& str)
{
	//QString ret;
	
	struct str in;
	struct str out;

	QString r_str = str;
	r_str.replace("=\r\n", "");
	
	in.s = r_str.toAscii().data();
	in.l = str.length();

	mime_fromqp(&in, &out, 0);
	
	QString cstr;
	cstr=QString::fromAscii(out.s, out.l+1);
	cstr.resize(out.l+1);
	cstr[(int)out.l] = QChar('0');
	
	return cstr;
}

void 
mime_fromb64(struct str *in, struct str *out, int is_text);

QByteArray LHMime::decodeBase64(const QString& str)
{
	//QString ret;
	
	struct str in;
	struct str out;

	in.s = str.toLatin1().data(); //qstrdup((const char*) str);
	in.l = str.length();

	mime_fromb64(&in, &out, 0);
	
	QByteArray barr;
	barr=QByteArray::fromRawData(out.s, out.l+1);

	return barr;
}


time_t rfctime(char *date);

QDateTime LHMime::decodeRfcTime(const QString& date)
{
	time_t tt = rfctime(date.toLatin1().data());
	QDateTime ret;
	ret.setTime_t(tt);
	return ret;
}

/**
 * TODO: more sophistocated algorithm cecking len of word
 * now it is perfectly legal, but some mail clients may have problems
 * with reading encoded headers
 */ 
QString LHMime::encodeHeaderQB(const QString& str, const char enc, const char* codec_name)
{
	if (codec_name == 0)
	{
		codec_name = QTextCodec::codecForLocale()->name();
	}
	QString start = QString("=?") + codec_name + "?" + enc + "?";
	uint len = start.length() + 3;
	int text_len = ENCODED_HEADER_WORD_MAX_LEN - len;
	if (text_len < 3)
	{
		qFatal("LHMime::encodeHeaderQ, Can not encode header word");
	}
	uint max_in_text_len = text_len / 3; //the worst case
	uint in_text_len = str.length();
	uint word_len = max_in_text_len;
	uint left = 0;

	QString ret;

	//qDebug("word_len: %d, in_text_len: %d", word_len, in_text_len);

	while (1)
	{
		if (enc == 'q')
		{
			ret += start + encodeQ(str.mid(left,word_len), codec_name) + "?= ";
		}
		else if (enc == 'b')
		{
			ret += start + encodeBase64(str.mid(left,word_len), codec_name) + "?= ";
		}
		if (left + word_len >= in_text_len)
		{
			break;
		}
		left += word_len;
		//word_len += max_in_text_len;
	};
	return ret;
}

QString LHMime::encodeQ(const QString& str, const char* codec_name, int in_body)
{
	QString cstr = LHMime::getCString(str, codec_name);
	const char * in = cstr.toAscii().data();
	qDebug("LHMime::encodeQ, IN: \n %s", in);
	char * out = c_mime_str_toqp(in, in_body);
	qDebug("LHMime::encodeQ, OUT: \n %s", out);
	QString ret; 
	ret=QString::fromAscii(out, qstrlen(out)+1);
	
	return ret;
}

char* LHMime::encodeQ(const char* str, int in_body)
{
	return c_mime_str_toqp(str, in_body);
}


/**
 * returnes how many characters have to be quoted
 */

uint LHMime::testQuoteHeader(const QString& str)
{
	QString cstr = str.toLocal8Bit();
	const char * in = cstr.toAscii().constData();
	uint ret=0;
	char* p = (char*) in;

	for (; *p != 0; p++)
	{
		if (mustquote_hdr((int) *p))
		{
			ret++;
		}
	}
	return ret;
}

QString LHMime::getBoundaryString()
{
	QString bs(20);
	bs.sprintf("Boundary--%.10d", rand());
	return bs;
}

const char* find_in_mime_types(const char* ext);

const char* LHMime::getFileContentType(const QString& fname)
{
	QFileInfo fi(fname);
	QString ext;
	const char* c_type = 0;

	//step first take full extension and check mime_types table
	ext = fi.completeSuffix().toLower();  // ext = "tar.gz"
	c_type = find_in_mime_types(ext.toLatin1());

	//if nothing found try only last part
	ext = fi.suffix().toLower();   // ext = "gz"
	c_type = find_in_mime_types(ext.toLatin1());

	return c_type;
}

const char* find_in_mime_types(const char* ext)
{
	const char* c_ext = 0;
	const char* c_type = 0;
	uint i = 0;

	while (1)
	{
		c_ext = mime_types[i];
		//qDebug("find_in_mime_types, c_ext: %s, ext: %s", c_ext, ext);
		if (c_ext == NULL)
		{
			break;
		}
		if (qstrcmp(c_ext, ext)==0)
		{
			c_type = mime_types[i+1]; 
			//qDebug("find_in_mime_types, FOUND c_ext: %s, ext: %s, c_type: %s", c_ext, ext, c_type);
			break;
		}
		i+=2;
	}
	return c_type;
}


// we need this in english and toString method always uses locale
//toString("ddd, dd MMMM yyyy hh:mm:ss +0zzz")
// Tue, 11 Jan 2005 12:54:59 +0100

static const char * const shortMonthNames[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
		
static const char * const shortDayNames[] = {
        "Mon", "Tue", "Wen", "Thu", "Fri", "Sat", "Sun" };

QString getShortDay(uint day)
{
	return QString(shortDayNames[day-1]);
}

QString getShortMonth(uint month)
{
	return QString(shortMonthNames[month-1]);
}

QString uintToString2pad(uint val)
{
	QString ret(2);
	
	if (val < 10)
	{
		ret.sprintf("0%d",val);
	}
	else if (val < 100)
	{
		ret.sprintf("%d", val);
	}
	else
	{
		qFatal("uintToString2pad, val to big: %d", val);
	}
	return ret;
}
	
QString uintToString4pad(uint val)
{
	QString ret(2);
	
	if (val < 10)
	{
		ret.sprintf("000%d",val);
	}
	else if (val < 100)
	{
		ret.sprintf("00%d", val);
	}
	else if (val < 1000)
	{
		ret.sprintf("0%d", val);
	}
	else if (val < 10000)
	{
		ret.sprintf("%d", val);
	}
	else
	{
		qFatal("uintToString4pad, val to big: %d", val);
	}
	return ret;
}

QString LHMime::encodeRfcTime(const QDateTime& date)
{
	QString ret = 
		getShortDay(date.date().dayOfWeek()) + ", " +
		uintToString2pad(date.date().day()) +  " " +
		getShortMonth(date.date().month()) + " " +
		uintToString4pad(date.date().year()) + " " +
	    uintToString2pad(date.time().hour()) + ":" +
	    uintToString2pad(date.time().minute()) + ":" +
	    uintToString2pad(date.time().second()) + " +100";
	return ret;
}

QString LHMime::getAppForMimeType(const QString& content_type)
{
	return LHMimePrivate::app_mime[content_type];
}

void LHMime::setAppForMimeType(const QString& content_type, const QString& app)
{
	LHMimePrivate::app_mime[content_type] = app;
}

//----------------------------------------------
/**
 * PRIVATE
 * Based on Nail sources
 */

/*
 * Nail - a mail user agent derived from Berkeley Mail.
 *
 * Copyright (c) 2000-2004 Gunnar Ritter, Freiburg i. Br., Germany.
 */
/*
 * Copyright (c) 1980, 1993
 *      The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Nail - a mail user agent derived from Berkeley Mail.
 *
 * Copyright (c) 2000-2004 Gunnar Ritter, Freiburg i. Br., Germany.
 */
/*
 * These base64 routines are derived from the metamail-2.7 sources which
 * state the following copyright notice:
 *
 * Copyright (c) 1991 Bell Communications Research, Inc. (Bellcore)
 *
 * Permission to use, copy, modify, and distribute this material 
 * for any purpose and without fee is hereby granted, provided 
 * that the above copyright notice and this permission notice 
 * appear in all copies, and that the name of Bellcore not be 
 * used in advertising or publicity pertaining to this 
 * material without the specific, prior written permission 
 * of an authorized representative of Bellcore.  BELLCORE 
 * MAKES NO REPRESENTATIONS ABOUT THE ACCURACY OR SUITABILITY 
 * OF THIS MATERIAL FOR ANY PURPOSE.  IT IS PROVIDED "AS IS", 
 * WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES.
 */

#define smalloc malloc
#define srealloc realloc
#define sfree free

static const char b64table[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const signed char b64index[] = {
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,62, -1,-1,-1,63,
	52,53,54,55, 56,57,58,59, 60,61,-1,-1, -1,-1,-1,-1,
	-1, 0, 1, 2,  3, 4, 5, 6,  7, 8, 9,10, 11,12,13,14,
	15,16,17,18, 19,20,21,22, 23,24,25,-1, -1,-1,-1,-1,
	-1,26,27,28, 29,30,31,32, 33,34,35,36, 37,38,39,40,
	41,42,43,44, 45,46,47,48, 49,50,51,-1, -1,-1,-1,-1
};

#define char64(c)  ((c) < 0 ? -1 : b64index[(int)(c)])

//static signed char *ctob64(unsigned char *p, int pad);

/*
 * Convert three characters to base64.
 */
/*static */signed char *
ctob64(unsigned char *p, int pad)
{
	static signed char b64[4];

	b64[0] = b64table[p[0] >> 2];
	b64[1] = b64table[((p[0] & 0x3) << 4) | ((p[1] & 0xF0) >> 4)];
	if (pad == 2)
	{
		b64[2] = b64[3] = '=';
	}
	else if (pad == 1)
	{
		b64[2] = b64table[((p[1] & 0xF) << 2) | ((p[2] & 0xC0) >> 6)];
		b64[3] = '=';
	}
	else
	{
		b64[2] = b64table[((p[1] & 0xF) << 2) | ((p[2] & 0xC0) >> 6)];
		b64[3] = b64table[p[2] & 0x3F];
	}
	return b64;
}

char *
memtob64(const void *vp, size_t isz);

char *
strtob64(const char *p)
{
	return memtob64(p, strlen(p));
}

char *
memtob64(const void *vp, size_t isz)
{
	char    q[3];
	const char  *p = (const char*) vp;
	signed char *h;
	size_t  c = 0;
	int i, l = 0, sz = 0, pads, j;
	char    *rs = NULL;

        // 0 length input.
	if (isz == 0)
	{
		rs = (char*)malloc(1);
		*rs = '\0';
		return rs;
	};
	
	do
	{
                for (j = 0; j < 3; j++)
                        q[j] = 0;

                // Prepare 3 bytes buffer (insert data from main input buffer vp).
		for (pads = 2, i = 0; i <= 2; i++, pads--)
		{
			q[i] = p[c++];
			if (c == isz)
				break;
		};

		h = ctob64((unsigned char *)q, pads);
                
		if (l + 5 >= sz)
			rs = (char*)realloc(rs, sz = l + 100);
                        
		for (i = 0; i < 4; i++)
			rs[l++] = h[i];
                        
	} while (c < isz);
	
	rs[l] = '\0';
        
	return rs;
}

/*
 * Check if c must be quoted inside a message's body.
 */
static int 
mustquote_body(int c)
{
	if (c=='\n' || c=='\r')
		return 0;

	if (c < 040 || c == '=' || c >= 0177)
		return 1;
	return 0;
}

static int
mustquote_inhdrq(int c)
{
	if (c != '\n'
		&& (c <= 040 || c == '=' || c == '?' || c == '_' || c >= 0177))
		return 1;
	return 0;
}

static int
mustquote_hdr(int c)
{
	if (c != '\n' && (c < 040 || c >= 0177))
		return 1;
	if (c == '=' || c == '?' || c == '_')
		return 1;
	return 0;
}


enum
{
	C_CNTRL = 0000,
	C_BLANK = 0001,
	C_WHITE = 0002,
	C_SPACE = 0004,
	C_PUNCT = 0010,
	C_OCTAL = 0020,
	C_DIGIT = 0040,
	C_UPPER = 0100,
	C_LOWER = 0200
};

const unsigned char class_char[] = {
/*      000 nul 001 soh 002 stx 003 etx 004 eot 005 enq 006 ack 007 bel */
	C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,
/*      010 bs  011 ht  012 nl  013 vt  014 np  015 cr  016 so  017 si  */
	C_CNTRL,C_BLANK,C_WHITE,C_SPACE,C_SPACE,C_SPACE,C_CNTRL,C_CNTRL,
/*      020 dle 021 dc1 022 dc2 023 dc3 024 dc4 025 nak 026 syn 027 etb */
	C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,
/*      030 can 031 em  032 sub 033 esc 034 fs  035 gs  036 rs  037 us  */
	C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,C_CNTRL,
/*      040 sp  041  !  042  "  043  #  044  $  045  %  046  &  047  '  */
	C_BLANK,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,
/*      050  (  051  )  052  *  053  +  054  ,  055  -  056  .  057  /  */
	C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,
/*      060  0  061  1  062  2  063  3  064  4  065  5  066  6  067  7  */
	C_OCTAL,C_OCTAL,C_OCTAL,C_OCTAL,C_OCTAL,C_OCTAL,C_OCTAL,C_OCTAL,
/*      070  8  071  9  072  :  073  ;  074  <  075  =  076  >  077  ?  */
	C_DIGIT,C_DIGIT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,
/*      100  @  101  A  102  B  103  C  104  D  105  E  106  F  107  G  */
	C_PUNCT,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,
/*      110  H  111  I  112  J  113  K  114  L  115  M  116  N  117  O  */
	C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,
/*      120  P  121  Q  122  R  123  S  124  T  125  U  126  V  127  W  */
	C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,C_UPPER,
/*      130  X  131  Y  132  Z  133  [  134  \  135  ]  136  ^  137  _  */
	C_UPPER,C_UPPER,C_UPPER,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,
/*      140  `  141  a  142  b  143  c  144  d  145  e  146  f  147  g  */
	C_PUNCT,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,
/*      150  h  151  i  152  j  153  k  154  l  155  m  156  n  157  o  */
	C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,
/*      160  p  161  q  162  r  163  s  164  t  165  u  166  v  167  w  */
	C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,C_LOWER,
/*      170  x  171  y  172  z  173  {  174  |  175  }  176  ~  177 del */
	C_LOWER,C_LOWER,C_LOWER,C_PUNCT,C_PUNCT,C_PUNCT,C_PUNCT,C_CNTRL
};

#define asciichar(c) ((unsigned)(c) <= 0177)
#define alnumchar(c) (asciichar(c)&&(class_char[c]&\
                        (C_DIGIT|C_OCTAL|C_UPPER|C_LOWER)))
#define alphachar(c) (asciichar(c)&&(class_char[c]&(C_UPPER|C_LOWER)))
#define blankchar(c) (asciichar(c)&&(class_char[c]&(C_BLANK)))
#define cntrlchar(c) (asciichar(c)&&(class_char[c]==C_CNTRL))
#define digitchar(c) (asciichar(c)&&(class_char[c]&(C_DIGIT|C_OCTAL)))
#define lowerchar(c) (asciichar(c)&&(class_char[c]&(C_LOWER)))
#define punctchar(c) (asciichar(c)&&(class_char[c]&(C_PUNCT)))
#define spacechar(c) (asciichar(c)&&(class_char[c]&(C_BLANK|C_SPACE|C_WHITE)))
#define upperchar(c) (asciichar(c)&&(class_char[c]&(C_UPPER)))
#define whitechar(c) (asciichar(c)&&(class_char[c]&(C_BLANK|C_WHITE)))
#define octalchar(c) (asciichar(c)&&(class_char[c]&(C_OCTAL)))

#define upperconv(c) (lowerchar(c) ? (c)-'a'+'A' : (c))
#define lowerconv(c) (upperchar(c) ? (c)-'A'+'a' : (c))


static const char basetable[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*
 * Convert c to a hexadecimal character string and store it in hex.
 */
static char *
ctohex(int c, char *hex)
{
	unsigned char d;

	hex[2] = '\0';
	d = c % 16;
	hex[1] = basetable[d];
	if (c > d)
		hex[0] = basetable[(c - d) / 16];
	else
		hex[0] = basetable[0];
	return hex;
}

/*
 * Write to a stringstruct converting to quoted-printable.
 * The mustquote function determines whether a character must be quoted.
 */
static void 
mime_str_toqp(struct str *in, struct str *out, int (*mustquote)(int), int inhdr)
{
	char *p, *q, *upper;

	out->s = (char*) malloc(in->l * 3 + 1);
	q = out->s;
	out->l = in->l;
	upper = in->s + in->l;
	for (p = in->s; p < upper; p++)
	{
		if (mustquote(*p&0377) || p+1 < upper && *(p + 1) == '\n' &&
			blankchar(*p & 0377))
		{
			if (inhdr && *p == ' ')
			{
				*q++ = '_';
			}
			else
			{
				out->l += 2;
				*q++ = '=';
				ctohex(*p&0377, q);
				q += 2;
			}
		}
		else
		{
			*q++ = *p;
		}
	}
	*q = '\0';
}


char* c_mime_str_toqp(const char* in, int in_body)
{
        //qDebug()<<tr("*** %1,%2 : QAZWSX %3").arg(__FILE__).arg(__LINE__).arg(in));

        if (!in)
                return NULL;

	struct str s_in;
	s_in.l = strlen(in);
	s_in.s = (char*) in;
	struct str s_out;
	if (!in_body)
	{
		mime_str_toqp(&s_in, &s_out, mustquote_inhdrq, 1);
	}
	else
	{
		mime_str_toqp(&s_in, &s_out, mustquote_body, 0);
	}
	return s_out.s;
}


/*
 * Decode from base64.
 */
void 
mime_fromb64(struct str *in, struct str *out, int is_text)
{
	char *p, *q, *upper;
	signed char c, d, e, f, g;
	int done = 0, newline = 0;

	out->s = (char*) smalloc(in->l * 3 / 4 + 2);
	out->l = 0;
	upper = in->s + in->l;
	for (p = in->s, q = out->s; p < upper;)
	{
		while (c = *p++, whitechar(c));
		if (p >= upper)	break;
		if (done) continue;
		while (d = *p++, whitechar(d));
		if (p >= upper)	break;
		while (e = *p++, whitechar(e));
		if (p >= upper)	break;
		while (f = *p++, whitechar(f));
		if (c == '=' || d == '=')
		{
			done = 1;
			continue;
		}
		c = char64(c);
		d = char64(d);
		g = ((c << 2) | ((d & 0x30) >> 4));
		if (is_text)
		{
			if (g == '\r')
			{
				newline = 1;
			}
			else if (g == '\n' && newline)
			{
				q--;
				out->l--;
				newline = 0;
			}
			else
			{
				newline = 0;
			}
		}
		*q++ = g;
		out->l++;
		if (e == '=')
		{
			done = 1;
		}
		else
		{
			e = char64(e);
			g = (((d & 0xF) << 4) | ((e & 0x3C) >> 2));
			if (is_text)
			{
				if (g == '\r')
				{
					newline = 1;
				}
				else if (g == '\n' && newline)
				{
					q--;
					out->l--;
					newline = 0;
				}
				else
				{
					newline = 0;
				}
			}
			*q++ = g;
			out->l++;
			if (f == '=')
			{
				done = 1;
			}
			else
			{
				f = char64(f);
				g = (((e & 0x03) << 6) | f);
				if (is_text)
				{
					if (g == '\r')
					{
						newline = 1;
					}
					else if (g == '\n' && newline)
					{
						q--;
						out->l--;
						newline = 0;
					}
					else
					{
						newline = 0;
					}
				}
				*q++ = g;
				out->l++;
			}
		}
	}
	return;
}

/*
 * Write to a stringstruct converting from quoted-printable.
 */
static void 
mime_fromqp(struct str *in, struct str *out, int ishdr)
{

        if (!in->s) {
                out->s = NULL;
                return;
        }

	char *p, *q, *upper;
	char quote[4];

	out->l = in->l;
	out->s = (char*) smalloc(out->l + 1);
	upper = in->s + in->l;

        qDebug ("*** %s,%d : QAZWSX %d, %d", __FILE__, __LINE__, in->l, ((int)(*upper)));

        //std::ofstream file ("out.txt");

	for (p = in->s, q = out->s; p < upper && p != '\0'; p++)
	{

		if (*p == '=')
		{
			do
			{
				p++;
				out->l--;
			} while (blankchar(*p & 0377) && p < upper);
                        
			if (p == upper)
				break;
                                
			if (*p == '\n')
			{
				out->l--;
				continue;
			}
                        
			if (p + 1 >= upper)
				break;
                                
			quote[0] = *p++;
			quote[1] = *p;
			quote[2] = '\0';
			*q = (char)strtol(quote, NULL, 16);
			q++;
			out->l--;
		}
		else if (ishdr && *p == '_')
			*q++ = ' ';
		else
			*q++ = *p;
	}
	return;
}

enum tdflags
{
	TD_NONE     = 0,	/* no display conversion */
	TD_ISPR     = 01,	/* use isprint() checks */
	TD_ICONV    = 02,	/* use iconv() */
	TD_DELCTRL  = 04	/* delete control characters */
};

enum conversion
{
	CONV_NONE,			/* no conversion */
	CONV_7BIT,			/* no conversion, is 7bit */
	CONV_FROMQP,			/* convert from quoted-printable */
	CONV_TOQP,			/* convert to quoted-printable */
	CONV_FROMB64,			/* convert from base64 */
	CONV_FROMB64_T,			/* convert from base64/text */
	CONV_TOB64,			/* convert to base64 */
	CONV_FROMHDR,			/* convert from RFC1522 format */
	CONV_TOHDR,			/* convert to RFC1522 format */
	CONV_TOHDR_A			/* convert addresses for header */
};


static char defcharset[] = "iso-8859-1";

#define	mime_fromhdr_inc(inc) { \
		size_t diff = q - out->s; \
		out->s = (char*) srealloc(out->s, (maxstor += inc) + 1); \
		q = &(out->s)[diff]; \
	}

/*
 * Convert header fields from RFC 1522 format
 * @ret charset
 */
char *
mime_fromhdr(struct str *in, struct str *out, int flags)
{
	char *p, *q, *op, *upper, *cs, *cbeg, *lastwordend = NULL;
	struct str cin, cout;
	int convert;
	size_t maxstor, lastoutl = 0;

	cs = 0;
	maxstor = in->l;
	out->s = (char*) smalloc(maxstor + 1);
	out->l = 0;
	upper = in->s + in->l;
	for (p = in->s, q = out->s; p < upper; p++)
	{
		op = p;
		if (*p == '=' && *(p + 1) == '?')
		{
			p += 2;
			cbeg = p;
			while (p < upper && *p != '?')
				p++;	/* strip charset */
			if (p >= upper)
				goto notmime;
			cs = (char*) smalloc(++p - cbeg); //was salloc
			memcpy(cs, cbeg, p - cbeg - 1);
			cs[p - cbeg - 1] = '\0';
			
			//qDebug("mime_fromhdr, cs: %s", cs);

			switch (*p)
			{
			case 'B':
			case 'b':
				convert = CONV_FROMB64;
				break;
			case 'Q':
			case 'q':
				convert = CONV_FROMQP;
				break;
			default:	/* invalid, ignore */
				goto notmime;
			}
			if (*++p != '?')
				goto notmime;
			cin.s = ++p;
			cin.l = 1;
			for (;;)
			{
				if (p == upper)
					goto fromhdr_end;
				if (*p++ == '?' && *p == '=')
					break;
				cin.l++;
			}
			cin.l--;
			switch (convert)
			{
			case CONV_FROMB64:
				mime_fromb64(&cin, &cout, 1);
				break;
			case CONV_FROMQP:
				mime_fromqp(&cin, &cout, 1);
				break;
			}
			if (lastwordend)
			{
				q = lastwordend;
				out->l = lastoutl;
			}
			while (cout.l > maxstor - out->l)
				mime_fromhdr_inc(cout.l -
								 (maxstor - out->l));
			memcpy(q, cout.s, cout.l);
			q += cout.l;
			out->l += cout.l;
			free(cout.s);
			lastwordend = q;
			lastoutl = out->l;
		}
		else
		{
			notmime:
			p = op;
			while (out->l >= maxstor)
				mime_fromhdr_inc(16);
			*q++ = *p;
			out->l++;
			if (!blankchar(*p&0377))
				lastwordend = NULL;
		}
	}
	fromhdr_end:
	*q = '\0';

	return cs;
}

char *
nexttoken(char *cp)
{
	for (;;) {
		if (*cp == '\0')
			return NULL;
		if (*cp == '(') {
			int nesting = 0;

			while (*cp != '\0') {
				switch (*cp++) {
				case '(':
					nesting++;
					break;
				case ')':
					nesting--;
					break;
				}
				if (nesting <= 0)
					break;
			}
		} else if (blankchar(*cp & 0377) || *cp == ',')
			cp++;
		else
			break;
	}
	return cp;
}

static const char *weekday_names[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

const char *month_names[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec", NULL
};

#define	leapyear(year)	((year % 100 ? year : year / 100) % 4 == 0)

time_t
combinetime(int year, int month, int day, int hour, int minute, int second)
{
	time_t t;

	if (second < 0 || minute < 0 || hour < 0 || day < 1)
		return -1;
	t = second + minute * 60 + hour * 3600 + (day - 1) * 86400;
	if (year < 70)
		year += 2000;
	else if (year < 1900)
		year += 1900;
	if (month > 1)
		t += 86400 * 31;
	if (month > 2)
		t += 86400 * (leapyear(year) ? 29 : 28);
	if (month > 3)
		t += 86400 * 31;
	if (month > 4)
		t += 86400 * 30;
	if (month > 5)
		t += 86400 * 31;
	if (month > 6)
		t += 86400 * 30;
	if (month > 7)
		t += 86400 * 31;
	if (month > 8)
		t += 86400 * 31;
	if (month > 9)
		t += 86400 * 30;
	if (month > 10)
		t += 86400 * 31;
	if (month > 11)
		t += 86400 * 30;
	year -= 1900;
	t += (year - 70) * 31536000 + ((year - 69) / 4) * 86400 -
		((year - 1) / 100) * 86400 + ((year + 299) / 400) * 86400;
	return t;
}

time_t
rfctime(char *date)
{
	char *cp = date, *x;
	time_t t;
	int i, year, month, day, hour, minute, second;

	if ((cp = nexttoken(cp)) == NULL)
		goto invalid;
	if (alphachar(cp[0] & 0377) && alphachar(cp[1] & 0377) &&
				alphachar(cp[2] & 0377) && cp[3] == ',') {
		if ((cp = nexttoken(&cp[4])) == NULL)
			goto invalid;
	}
	day = strtol(cp, &x, 10);
	if ((cp = nexttoken(x)) == NULL)
		goto invalid;
	for (i = 0; month_names[i]; i++) {
		if (strncmp(cp, month_names[i], 3) == 0)
			break;
	}
	if (month_names[i] == NULL)
		goto invalid;
	month = i + 1;
	if ((cp = nexttoken(&cp[3])) == NULL)
		goto invalid;
	year = strtol(cp, &x, 10);
	if ((cp = nexttoken(x)) == NULL)
		goto invalid;
	hour = strtol(cp, &x, 10);
	if (*x != ':')
		goto invalid;
	cp = &x[1];
	minute = strtol(cp, &x, 10);
	if (*x == ':') {
		cp = &x[1];
		second = strtol(cp, &x, 10);
	} else
		second = 0;
	if ((t = combinetime(year, month, day, hour, minute, second)) ==
			(time_t)-1)
		goto invalid;
	if ((cp = nexttoken(x)) != NULL) {
		int sign = -1;
		char buf[3];

		switch (*cp) {
		case '-':
			sign = 1;
			/*FALLTHRU*/
		case '+':
			cp++;
		}
		if (digitchar(cp[0] & 0377) && digitchar(cp[1] & 0377) &&
				digitchar(cp[2] & 0377) &&
				digitchar(cp[3] & 0377)) {
			buf[2] = '\0';
			buf[0] = cp[0];
			buf[1] = cp[1];
			t += strtol(buf, NULL, 10) * sign * 3600;
			buf[0] = cp[2];
			buf[1] = cp[3];
			t += strtol(buf, NULL, 10) * sign * 60;
		}
	}
	return t;
invalid:
	return 0;
}

//------------------------------------------------------------
#if 0
/*
 * Get a line like "text/html html" and look if x matches the extension.
 */
static char *
mime_tline(char *x, char *l)
{
	char *type, *n;
	int match = 0;

	if ((*l & 0200) || alphachar(*l & 0377) == 0)
		return NULL;
	type = l;
	while (blankchar(*l & 0377) == 0 && *l != '\0')
		l++;
	if (*l == '\0')
		return NULL;
	*l++ = '\0';
	while (blankchar(*l & 0377) != 0 && *l != '\0')
		l++;
	if (*l == '\0')
		return NULL;
	while (*l != '\0')
	{
		n = l;
		while (whitechar(*l & 0377) == 0 && *l != '\0')
			l++;
		if (*l != '\0')
			*l++ = '\0';
		if (strcmp(x, n) == 0)
		{
			match = 1;
			break;
		}
		while (whitechar(*l & 0377) != 0 && *l != '\0')
			l++;
	}
	if (match != 0)
	{
		n = (char*) malloc(strlen(type) + 1); //salloc
		strcpy(n, type);
		return n;
	}
	return NULL;
}

/*
 * Check the given MIME type file for extension ext.
 */
static char *
mime_type(char *ext, char *filename)
{
	QFile f(filename);
	char *line = NULL;
	size_t linesize = 0;
	char *type = NULL;

	if (!f.open(IO_ReadOnly))
		return NULL;

	while (fgetline(&line, &linesize, NULL, NULL, f, 0))
	{
		if ((type = mime_tline(ext, line)) != NULL)
			break;
	}
	Fclose(f);
	if (line)
		free(line);

	return type;
}


/*
 * Return the Content-Type matching the extension of name.
 */
char *
mime_filecontent(char *name)
{
	char *ext, *content;

	if ((ext = strrchr(name, '.')) == NULL || *++ext == '\0')
		return NULL;
	if ((content = mime_type(ext, expand(mimetypes_user))) != NULL)
		return content;
	if ((content = mime_type(ext, mimetypes_world)) != NULL)
		return content;
	return NULL;
}

#endif
