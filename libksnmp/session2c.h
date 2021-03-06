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
#ifndef KSNMPSESSION2C_H
#define KSNMPSESSION2C_H

#include <libksnmp/session.h>

namespace KSNMP {

/**
@author Flameeyes
@brief SNMPv2c Session descriptor

This class reperesent a SNMPv2c session, and it's used to communicate with
this protocol to the peer.

@sa Session1
*/
class Session2c : public Session
{
protected:
	/**
	@brief Community string for session
	
	Every SNMP session must have a community name, which is used as a sort
	of 'password' for the SNMP.
	
	The default community strings are 'public' for read-only access, and
	'private' for read-write access. Not to say, these values aren't the
	most secure ones.
	*/
	QString m_community;
	

public:
	/**
	@brief Peer-Specifying constructor
	@param peername Hostname or IP address of the host to open the session to
	@param community Community name to use for the session. If not
		specified it defaults to 'public'.
	
	This constructor sets the peer name to the one requested, prepare
	the connection and waits for connection request.
	The optional community parameter is used to set the community name.
	
	@sa Session2c::Session()
	@sa Session2c::setCommunity()
	@sa Session::setPeerName()
	*/
	Session2c(const QString &peername = "localhost", const QString &community = "public");

	~Session2c();
	
	/**
	@brief Sets the community name
	@param community New community name to set the session to.
	@retval true The session is not open, the community name is changed.
	@retval false The session is already open, the community name can't be changed.
	*/
	bool setCommunity(const QString &community)
	{
		/// \todo need to fix this when the open-session code is done :)
		if ( m_session )
			return false;
		
		m_community = community;
		return true;
	}
	
	virtual bool open();
};

};

#endif
