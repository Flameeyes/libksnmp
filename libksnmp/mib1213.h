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
#ifndef __KSNMP_MIB1213_H__
#define __KSNMP_MIB1213_H__

#include <libksnmp/device.h>

namespace KSNMP {

/**
@author Flameeyes
@brief RFC 1213 compliant device

This class is used to access a 1213-compliant device, for example the most part
of ADSL routers.

This is a wrapper around the Session class, which provides member for access the
rfc1213 variables.
*/
class Device1213 : public Device {
public:
	/**
	@brief Values of a single interface from SNMP
	
	These are the values which describes an interface in RFC1213 MIB
	
	@note TimeTicks, Counter, Gauge types are simply changed to uint32_t
		standard type.
	*/
	struct Interface {
		uint32_t ifIndex;	///< Interface index
		QString ifDescr;	///< Interface description
		uint32_t ifAdminStatus;	///< Interface administrative status
		uint32_t ifOperStatus;	///< Interface operational status
		uint32_t ifType;	///< Interface type
		uint32_t ifSpeed;	///< Interface speed (in bps)
		QByteArray ifPhysAddress;///< Interface physical address (MAC)
		uint32_t ifMtu;		///< Interface Maximum Transfer Unit
		uint32_t ifLastChange;	///< Interface last changed time
		
		uint32_t ifInOctets;	///< Interface received bytes
		uint32_t ifInUcastPkts;	///< Interface received unicast packets
		uint32_t ifInNUcastPkts;///< Interface received non-unicast packets
		uint32_t ifInDiscards;	///< Interface received discarded packets
		uint32_t ifInErrors;	///< Interface received packets w/ errors
		uint32_t ifInUnknownProtos;///< Interface received packets w/ unknown protocol
		
		uint32_t ifOutOctets;	///< Interface transmitted bytes
		uint32_t ifOutUcastPkts;///< Interface transmitted unicast packets
		uint32_t ifOutNUcastPkts;///< Interface transmitted non-unicast packets
		uint32_t ifOutDiscards;	///< Interface transmitted discarded packets
		uint32_t ifOutErrors;	///< Interface transmitted packets w/ errors
		uint32_t ifOutQLen;	///< Interface Queue Len
	};

protected:
	/**
	@brief Interfaces cache
	
	This is a special cache, similar to Device::m_cache one, but specifical
	for interfaces of RFC1213 devices.
	Index of the dictionary is interface number.
	*/
	QMap<uint32_t, Interface> m_ifCache;

public:
	/**
	@brief Format a physical address as for RFC1213 MIB.
	@param[in] ba Byte array representing the physical address
	@return a HH:HH:HH:HH:... formatted string for the passed byte array
	
	@note Physical addresses in RFC1213 are MAC addresses.
	*/
	static QString physAddress(const QByteArray &ba);
	
	/**
	@brief Gets a translated human-readable string for the adminStatus passed.
	@param[in] n Value of ifAdminStatus for the interface
	*/
	static QString adminStatus(const uint32_t n);

	/**
	@brief Gets a translated human-readable string for the operStatus passed.
	@param[in] n Value of ifOperStatus for the interface
	*/
	static QString operStatus(const uint32_t n);
	
	/**
	@brief Gets a translated human-readable string for the interface type passed
	@param[in] n Value of ifType for the interface
	*/
	static QString ifType(const uint32_t n);
	
	/**
	@brief Gets a translated human-readable string for the forwarding status passed
	@param[in] n Value of ipForwarding variable
	*/
	static QString forwarding(const uint32_t n);
	
	/**
	@brief Constructor
	@param[in] session Session to get and set the values from and to
	
	This is the only constructor provided, because a session is needed to access
	the data from the device.
	
	@note Before get any variable, you should open the session.
	*/
	Device1213(Session *session);
	
	/**
	@brief Cache the base system values
	
	This function is used to cache the values of sys* variables for RFC1213 devices.
	*/
	void cacheSystem();
	
	/**
	@brief Cache IP settings
	
	This function is used to cache the values of ip settings variables for RFC1213 devices.
	*/
	void cacheIpSettings();
	
	/**
	@brief Gets and cache the interface data for the given index
	@param[in] index Index of the interface to get the data for
	@param[in] nocache If true, the interface will be re-read from the device instead of given
		from the cache
	*/
	Interface getInterface(uint32_t index, bool nocache = false);
	
	/**
	@brief Completely re-read all the interfaces and cache them
	
	This function calls the getInterface(index, true) function for every interface in the device.
	It's useful when we need to get all the statistics for the interfaces.
	*/
	void readInterfaces();
};

}

#endif
