/***************************************************************************
 *   Copyright (C) 2004 by Diego "Flameeyes" Pettenò                       *
 *   dgp85@users.sourceforge.net                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "session.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fake-netsnmp-config.h"
#include <net-snmp/net-snmp-includes.h>

#include <qcstring.h>

namespace KSNMP {

/**
@brief Transform an SNMP variable into a QVariant object
@param var SNMP Variable to transform
@return A QVariant object with the content of the given variant

This function is the main bridge between net-snmp and QT. It makes
simple to use normal variables to retrieve snmp variant values.
*/
static QVariant snmpvarToVariant(struct variable_list *var)
{
	/// @todo Missing a lot of default values and quite all the other values
	if ( ! var ) return QVariant();
	switch( var->type )
	{
	case ASN_OCTET_STR: {
		QByteArray ba(var->val_len+1);
		ba.duplicate((const char*)var->val.string, var->val_len);
		ba.resize(var->val_len+1);
		ba[var->val_len] = '\0';
		return QVariant(ba);
		}
	case ASN_INTEGER:
	case ASN_COUNTER:
	case ASN_GAUGE:
	case ASN_TIMETICKS:
		return QVariant((uint)*(var->val.integer));
	}
	
	return QVariant();
}

void init(const QString &appname)
{
	init_snmp(appname.latin1());
	init_mib();
}

void shutdown()
{
	shutdown_mib();
}

Session::Session() :
	m_peerName("localhost"),
	m_session(NULL)
{
}

Session::~Session()
{
	close();
}

void Session::close()
{
	if ( m_session )
	{
		snmp_close(m_session);
		m_session = false;
	}
}

void Session::initSession(snmp_session *session)
{
	snmp_sess_init(session); // Setup defaults
	session->peername = new char[m_peerName.length()+1];
	strncpy(session->peername, m_peerName.latin1(), m_peerName.length()+1);
	
	// Casts are to move the variables to the original type.
	session->retries = (int)m_retries;
	session->timeout = (long)m_timeout;
}

bool Session::getVariable(const QString &oid, QVariant &retvar, uint32_t &status)
{
	struct snmp_pdu *response = NULL;
	if ( ! m_session )
	{
		status = 0xFFFFFFFF;
		return false;
	}
	
	// Buffer requestd for net-snmp library
	u_long anOID[MAX_OID_LEN];
	size_t anOID_len = MAX_OID_LEN;
	
	// Prepare the PDU for a GET command
	struct snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_GET);
	get_node(oid.latin1(), anOID, &anOID_len);
	snmp_add_null_var(pdu, anOID, anOID_len);
	
	status = snmp_synch_response(m_session, pdu, &response);
	
	//! @todo Error handling should be changed in a more OO way.
	if ( status != STAT_SUCCESS )
	{
		snmp_sess_perror("snmpget", m_session);
		return false;
	}
	
	if ( response->errstat != SNMP_ERR_NOERROR )
	{
		qWarning("Error in packet: %s", snmp_errstring(response->errstat));
		snmp_free_pdu(response);
		return false;
	}

	retvar = snmpvarToVariant(response->variables);
	snmp_free_pdu(response);
	
	return true;
}

bool Session::getVariables(const QStringList &oids, QValueVector<QVariant> &retvars, uint32_t &status)
{
	int maxIterations = oids.count() / 5;
	for(int i = 0; i < maxIterations; i++)
		if ( ! getVariables(oids, retvars, status, i*5) )
			return false;
	
	return getVariables(oids, retvars, status, maxIterations*5);
}

bool Session::getVariables(const QStringList &oids, QValueVector<QVariant> &retvars, uint32_t &status, int startIndex)
{
	struct snmp_pdu *response = NULL;
	if ( ! m_session )
	{
		status = 0xFFFFFFFF;
		return false;
	}
	
	struct snmp_pdu *pdu = snmp_pdu_create(SNMP_MSG_GET);

	u_long anOID[MAX_OID_LEN];
	
	QStringList::const_iterator it = oids.begin();
	for(int i = 0; i < startIndex; i++) it++; // rough hack, but works
	
	for(int i = 0; it != oids.end() && i < 5; it++, i++ )
	{
		size_t anOID_len = MAX_OID_LEN;
		get_node((*it).latin1(), anOID, &anOID_len);
		snmp_add_null_var(pdu, anOID, anOID_len);
	}
	
	status = snmp_synch_response(m_session, pdu, &response);
	
	//! @todo Error handling should be changed in a more OO way.
	if ( status != STAT_SUCCESS )
	{
		snmp_sess_perror("snmpget", m_session);
		return false;
	}
	
	if ( response->errstat != SNMP_ERR_NOERROR )
	{
		qWarning("Error in packet: %s", snmp_errstring(response->errstat));
		snmp_free_pdu(response);
		return false;
	}

	variable_list *var = response->variables;
	int i = startIndex;
	while( var )
	{
		retvars[i] = snmpvarToVariant(var);
		i++;
		var = var->next_variable;
	}
	
	snmp_free_pdu(response);
	
	return true;
}

};
