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
#include <libksnmp/session1.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fake-netsnmp-config.h"
#include <net-snmp/net-snmp-includes.h>

namespace KSNMP {

Session1::Session1(const QString &peername, const QString &community) : Session()
{
	setPeerName(peername);
	m_community = community;
}

Session1::~Session1()
{
}

bool Session1::open()
{
	netsnmp_session session;
	initSession(&session); // Initialize a "session" that defines who we're going to talk to
	
	session.version = SNMP_VERSION_1;
	
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
