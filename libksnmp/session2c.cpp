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
#include "session2c.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fake-netsnmp-config.h"
#include <net-snmp/net-snmp-includes.h>

namespace KSNMP {

Session2c::Session2c(const QString &peername, const QString &community) : Session()
{
	setPeerName(peername);
	m_community = community;
}

Session2c::~Session2c()
{
}

bool Session2c::open()
{
	QMutexLocker ml(&m_mutex);
	// Initialize a "session" that defines who we're going to talk to
	netsnmp_session session;
	initSession(session);

	session.version = SNMP_VERSION_2c;
	
	session.community = new u_char[m_community.length()+1];
	memcpy(session.community, m_community.latin1(), m_community.length()+1);
	session.community_len = m_community.length();
	
	m_session = snmp_open(&session);
	
	delete session.community;
	delete session.peername;
	
	if ( ! m_session )
		return false;
	
	return true;
}

};
