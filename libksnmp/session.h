/***************************************************************************
 *   Copyright (C) 2004-2005 by Diego 'Flameeyes' Pettenò                  *
 *   flameeyes@users.berlios.de                                            *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Library General Public License for more details.                      *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB.            *
 *   If not, write to the Free Software Foundation, Inc., 59 Temple Place  *
 *   - Suite 330, Boston, MA 02111-1307, USA.                              *
 ***************************************************************************/
#ifndef KSNMPSESSION_H
#define KSNMPSESSION_H

#include <qstring.h>
#include <qvariant.h>
#include <qvaluevector.h>
#include <qmutex.h>

// pre-declaration of net-snmp structs so we don't need net-snmp headers
struct snmp_session;

#include <kdeversion.h>

#define LIBKSNMP_VERSION \
	KDE_MAKE_VERSION(0, 3, 0)

namespace KSNMP {

/**
@brief Available version of SNMP requests

This enum is used to select between different version of SNMP sessions.
As for now, we have three kind of SNMP sessions: 1 and 2c which are community
authenticated, and version 3 which is more complex.
This enum is used to select the kind of session you are requesting.

@note Only SNMP v1 or v2c are supported right now.
*/
enum SNMPVer {
	V1,	///< Version 1
	V2c	///< Version 2c
};

/**
@brief Initialize the library
@param[in] appname Name of the application

This function is used to initialize the net-snmp library.
It prevents the base application to mess up with damned fake-netsnmp-config header.
*/
void init(const QString &appname);

/**
@brief Shutdown the library

This function is used to shutdown the net-snmp library.
It prevents the base application to mess up with damned fake-netsnmp-config header.
*/
void shutdown();

/**
@author Flameeyes
@class Session session.h "libksnmp/session.h"
@brief Describe an SNMP session
*/
class Session {
protected:
	/**
	@brief Peer name
	
	The hostname or the IP address to connect to.
	*/
	QString m_peerName;
	
	/**
	@brief Number of retries before timeout.
	
	@note Took from net-snmp documentation
	*/
	uint32_t m_retries;
	
	/**
	@brief Number of μS until first timeout, then exponential backoff
	
	@note Took from net-snmp documentation
	*/
	uint32_t m_timeout;
	
	/**
	@brief Session pointer
	
	This pointer must be initializzed at NULL and will be set to the
	session pointer when the session is opened.
	*/
	snmp_session *m_session;
	
	/**
	@brief Mutex for thread safety
	
	Getting more request from different threads at one time can lead to
	errors in sessions, with this mutex we make sure that only one thread
	at a time take access to the session.
	*/
	QMutex m_mutex;

	/**
	@brief Gets many variables from the SNMP session [reiterated version]
	@param[in] oids List of OIDs or mnemonics of variables to get.
	@param[out] retvars Array of variants where to store the read variables.
	@param[out] status Variable to set to the snmp_synch_response return value.
	@param[in] startIndex Index to start set the values from
	@retval true The variables were read, retvar is filled with the value.
	@retval false Error occoured during request of values, status is set accordling
	
	This protected function is used to split-up calls for too many variables.
	Never call this function directly, the other getVariables() function takes care
	of calling it.
	
	@note If the session is closed, getVariables() returns false and sets status to
		0xFFFFFFFF
	@note To avoid cpu load, pass as retvars an array already of the dimension of
		the variables to read.
	@note Not request too many variables at once, else the request might fail.
	*/
	bool getVariables(const QStringList &oids, QValueVector<QVariant> &retvars, uint32_t &status, int startIndex);
	
	/**
	@brief Protected constructor
	
	It's used by child classes to construct a session.
	*/
	Session();
	
	/**
	@brief Initialize a base session struct
	@return The initialized structure to be used to open the session.
	
	This function is a repalcement for snmp_sess_init() function from net-snmp.
	It calls the above method, but also sets the peername, the retries and timeout.
	
	This method should be called by Session::open() methods from child classes before
	they set their own parameters.
	*/
	void initSession(snmp_session &sess);
public:
	virtual ~Session();
	
	/**
	@brief Sets the peer name
	@param peername New peer name to set the session for.
	@retval true The session is not open, the peer name is changed.
	@retval false The session is already open, the peer name can't be changed.
	*/
	bool setPeerName(const QString &peername)
	{
		if ( m_session )
			return false;
		
		m_peerName = peername;
		return true;
	}
	
	/**
	@brief Sets the number of retries
	@param retries Number of retries to have before timeout requests.
	@retval true The session is not open, setting applied.
	@retval false The session is already open, setting not applied.
	*/
	bool setRetries(const uint32_t retries)
	{
		if ( m_session )
			return false;
		
		m_retries = retries;
		return true;
	}
	
	/**
	@brief Sets the number of μS until first timeout
	@param timeout Number of μS until first timeout.
	@retval true The session is not open, setting applied.
	@retval false The session is already open, setting not applied.
	*/
	bool setTimeout(const uint32_t timeout)
	{
		if ( m_session )
			return false;
		
		m_timeout = timeout;
		return true;
	}
	
	/**
	@brief Initialize the SNMP session and open it.
	@retval true Session opened correctly.
	@retval false Unable to open connection.
	
	This function calls the underlying net-snmp functions to open the
	session with the peer.
	*/
	virtual bool open() = 0;
	
	/**
	@brief Closes the SNMP session
	*/
	void close();
	
	/**
	@brief Gets a single variable from the SNMP session.
	@param[in] oid OID or mnemonic name of the variable to get.
	@param[out] retvar Variable where to store the given value.
	@param[out] status Variable to set to the snmp_synch_response return value.
	@retval true The variable was read, retvar is filled with the value.
	@retval false Error occoured during request of value, status is set accordling
	
	This function is used to retrieve a single variable from the SNMP peer
	connected.
	If you need to get more than one variable, you should use Session::getVariables()
	function, which sends only one PDU at a time.
	
	@note If the session is closed, getVariable() returns false and sets status to
		0xFFFFFFFF
	*/
	bool getVariable(const QString &oid, QVariant &retvar, uint32_t &status);
	
	/**
	@brief Gets many variables from the SNMP session.
	@param[in] oids List of OIDs or mnemonics of variables to get.
	@param[out] retvars Array of variants where to store the read variables.
	@param[out] status Variable to set to the snmp_synch_response return value.
	@retval true The variables were read, retvar is filled with the value.
	@retval false Error occoured during request of values, status is set accordling
	
	This function is similar to getVariable() function, but it sends a single
	PDU for all the requested variables.
	
	@note If the session is closed, getVariables() returns false and sets status to
		0xFFFFFFFF
	@note To avoid cpu load, pass as retvars an array already of the dimension of
		the variables to read.
	@note Not request too many variables at once, else the request might fail.
	*/
	bool getVariables(const QStringList &oids, QValueVector<QVariant> &retvars, uint32_t &status);
};

/**
@brief Quick create a session with the given parameters.
@param peer Hostname or IP address of the host to connect to.
@param community Name of the community to use for authentiation.
@param version Version of protocol to use for the session.
@return A pointer to a newly created session with the requested parameters.
*/
Session *createSession(const QString &peer, const QString &community, SNMPVer version);

};

#endif
