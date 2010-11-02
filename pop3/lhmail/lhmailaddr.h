
/****************************************************************************
**
** Copyright (C) 2004 LeftHand sp.z.o.o.  All rights reserved.
**
*****************************************************************************/

#ifndef LHMAILADDR_H
#define LHMAILADDR_H
#define UNIT_API __declspec(dllexport)
#include <QObject>

class QString;
class LHMailAddrPrivate;

/**
 * This is representation of single mail address. Used generaly by @ref LHMail.
 * @short Representation of single mail address.
 * @see LHMail
 */ 
 #ifdef Q_OS_WIN32
	UNIT_API
#endif

class LHMailAddr : public QObject
{
    Q_OBJECT

public:
		
	/**
	 * Constructor for LHMailAddr object.
	 * @param addr String representing one mail address. Can be in form <code>name@domain<code>.
	 *             <p>or <code> Long Name &lt;name@domain&gt;</code>.
	 *             <p>If provided string is not valid mail address, then object returnes on @ref isValid 0.
	 */ 
	LHMailAddr(const QString &addr);
    ~LHMailAddr();
	
	/**
	 * Returnes string with address in form <code>name@domain</code>.
	 */ 
	QString simple();
	
	/**
	 * Returnes full string provided in constructor @ref LHMailAddress::LHMailAddress.
	 */
	QString full();
	
	/**
	 * Returnes 1 if provided in constructor string is valid, 0 otherwise.
	 */
	int isValid();
	
	/**
	 * Method used to testing if given string is valid mail address.
	 */
	int testAddr(const QString& addr);
	
	/**
	 * Static method used to parse string with many mail addresses separated by ',' or ';'.
	 * <p>Only valid addresses are added to list.
	 * <p>You should run @ref testMulti before.
	 * @return 1 if all addresses are valid, 0 otherwise.
	 */
	static int parseMulti(QList<LHMailAddr *>& mail_vect, const QString &multi_addr);
	
	/**
	 * Static method used to test string with many mail addresses separated by ',' or ';'.
	 * <p>All invalid addresses are added in string form to provided string.
	 * @return Number of invalid mail addresses
	 */
	static int testMulti(QString& wrong, const QString &multi_addr);
	
	/**
	 * Converting mail collection provded as string to HTML representation.
	 * All mails are changed to mailto: links.
	 * This method is used by @ref LHMail::getShowText
	 * @param multi_addr Collection of addresses separated by ',' or ';'
	 * @return HTML string 
	 */ 
	static QString convToHtmlMulti(const QString &multi_addr);
	
	
private:

	LHMailAddrPrivate* d;
};

#endif
