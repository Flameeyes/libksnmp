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
#ifndef KSNMPDEVICE_H
#define KSNMPDEVICE_H

#include <libksnmp/session.h>

namespace KSNMP {

/**
@author Flameeyes
@brief Generic SNMP device support

This class is used as an abstraction for devices compliat to some MIBs.

@sa Device1213
*/
class Device
{
protected:
	/**
	@brief Session descriptor for device
	*/
	Session *m_session;
	
	/**
	@brief Cached values for snmp variables
	
	This dictionary is oid-indexed to give a cache to values, so that
	already-requested values (for example when using cache*() functions)
	doesn't get re-requested to the device itself.
	*/
	QMap<QString, QVariant> m_cache;
public:
	/**
	@brief Constructor
	@param[in] session Session to get and set the values from and to
	
	This is the only constructor provided, because a session is needed to access
	the data from the device.
	
	@note Before get any variable, you should open the session.
	*/
	Device(Session *session);
	~Device();
	
	/**
	@brief Requests for a given variable.
	@param[in] oid OID or mnemonic of the variable to get
	@param[in] nocache If true, the cache won't be used (but the new value will be cached)
	@return The variant representing the value requested, or a null variant if error.
	
	This is a wrapper for Session::getVariable() function, but adds cache support,
	for cache*() functions of sub-devices.
	
	@note Caching is done by mnemonic, so please use it if you want to have the value
	cached :)
	*/
	QVariant requestValue(const QString &oid, bool nocache = false);
	
	/**
	@brief Clears the cache
	
	This function is used to completely clear the cache for the device.
	*/
	void clearCache()
	{
		m_cache.clear();
	}
};

};

#endif
