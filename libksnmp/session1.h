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
#ifndef KSNMPSESSION1_H
#define KSNMPSESSION1_H

#include <libksnmp/session.h>

namespace KSNMP {

/**
@author Flameeyes
@brief SNMPv1 Session descriptor

This class reperesent a SNMPv1 session, and it's used to communicate with
this protocol to the peer.

@sa Session2c
*/
class Session1 : public Session
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
	
	@sa Session1::Session()
	@sa Session1::setCommunity()
	@sa Session::setPeerName()
	*/
	Session1(const QString &peername = "localhost", const QString &community = "public");

	~Session1();
	
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
