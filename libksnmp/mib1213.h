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
	
//@{
/**
@name ifflags Intefaces' fields' flags
@brief Flags used to define which fields should be get from the interface

These flags can be used to select which fields from an interface should be
cached.
They are useful for example when you simply need to get the interface name,
type and index instead of every statistics.


*/
	static const uint64_t getIndex		= 0x0000000000000001;
	static const uint64_t getDescr		= 0x0000000000000002;
	static const uint64_t getAdminStatus	= 0x0000000000000004;
	static const uint64_t getOperStatus	= 0x0000000000000008;
	static const uint64_t getType		= 0x0000000000000010;
	static const uint64_t getSpeed		= 0x0000000000000020;
	static const uint64_t getPhysAddress	= 0x0000000000000040;
	static const uint64_t getMtu		= 0x0000000000000080;
	static const uint64_t getLastChange	= 0x0000000000000100;
	
	static const uint64_t getInOctets	= 0x0000000000000200;
	static const uint64_t getInUcastPkts	= 0x0000000000000400;
	static const uint64_t getInNUcastPkts	= 0x0000000000000800;
	static const uint64_t getInDiscards	= 0x0000000000001000;
	static const uint64_t getInErrors	= 0x0000000000002000;
	static const uint64_t getInUnknwonProtos= 0x0000000000004000;
	
	static const uint64_t getOutOctets	= 0x0000000000008000;
	static const uint64_t getOutUcastPkts	= 0x0000000000010000;
	static const uint64_t getOutNUCastPkgts	= 0x0000000000020000;
	static const uint64_t getOutDiscards	= 0x0000000000040000;
	static const uint64_t getOutErrors	= 0x0000000000080000;
	static const uint64_t getOutQLen	= 0x0000000000100000;
//@}

//@{
/**
@name metaifflags Flags for "meta" fields
@brief Flags used to suppress mnemonic constants of agregation of fields

You can use these flags to define some subset of the entire fields without the
need to specify them one-by-one
*/

	static const uint64_t getAll		= 0xFFFFFFFFFFFFFFFF;
	static const uint64_t getNone		= 0x0000000000000000;
	
	static const uint64_t getID		= getIndex | getDescr | getType;
	static const uint64_t getLoad		= getInOctets | getOutOctets;
//@}

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
	@param[in] what Mask using \ref ifflags to define which field to get
	*/
	Interface getInterface(uint32_t index, bool nocache = false, uint64_t what = getAll);
	
	/**
	@brief Completely re-read all the interfaces and cache them
	
	This function calls the getInterface(index, true) function for every interface in the device.
	It's useful when we need to get all the statistics for the interfaces.
	*/
	void readInterfaces(uint64_t what = getAll);
	
	/**
	@brief Gets the interface number for the given interface name (ifDescr)
	@param name Name of the remote interface (ifDescr.X value)
	@param cache If true, the cache will be used, else it will be forced a complete reload
	@return The ifIndex of the remote interface.
	
	This function is used to translate the remote interface's name to the index, suitable for
	requesting data.
	*/
	uint32_t getIfIndex(const QString &name, bool cache = true);
};

}

#endif
