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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#define __STDC_CONSTANT_MACROS
#ifdef HAVE_STDINT_H
# include <stdint.h>
#elif defined HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include "session.h"
#include "device.h"
#include "mib1213.h"

#include <klocale.h>
#include <qapplication.h>
#include <unistd.h>

namespace KSNMP {

QString Device1213::adminStatus(const uint32_t n)
{
	switch(n)
	{
	case 1: return i18n("Up");
	case 2: return i18n("Down");
	default: return i18n("Error");
	}
}

QString Device1213::operStatus(const uint32_t n)
{
	switch(n)
	{
	case 1: return i18n("Up");
	case 2: return i18n("Down");
	case 3: return i18n("Testing");
	default: return i18n("Error");
	}
}

QString Device1213::ifType(const uint32_t n)
{
	switch(n)
	{
	case 2: return i18n("Regular 1822");
	case 3: return i18n("HDH 1822");
	case 4: return i18n("DDN X.25");
	case 5: return i18n("RFC877 X.25");
	case 6: return i18n("Ethernet CSMA/CD");
	case 7: return i18n("ISO 8802.3 | IEEE 802.3 CSMA/CD");
	case 8: return i18n("ISO 8802.4 | IEEE 802.4 Token Bus");
	case 9: return i18n("ISO 8802.5 | IEEE 802.5 Token Ring");
	case 10: return i18n("ISO 8802.6 | IEEE 802.6 MAN");
	case 11: return i18n("Star LAN");
	case 12: return i18n("Proteon 10Mbit");
	case 13: return i18n("Proteon 80Mbit");
	case 14: return i18n("HyperChannel");
	case 15: return i18n("FDDI");
	case 16: return i18n("LAP-B");
	case 17: return i18n("SDLC");
	case 18: return i18n("T1");
	case 19: return i18n("E1");
	case 20: return i18n("Basic ISDN");
	case 21: return i18n("Primary ISDN");
	case 22: return i18n("Proposed Point-to-Point serial");
	case 23: return i18n("PPP");
	case 24: return i18n("Software loopback");
	case 25: return i18n("EON | CLNP over IP");
	case 26: return i18n("Ethernet 3Mbit");
	case 27: return i18n("XNS over IP");
	case 28: return i18n("Generic SLIP");
	case 29: return i18n("ULTRA technologies");
	case 30: return i18n("T3");
	case 31: return i18n("SMDS");
	case 32: return i18n("Frame Relay");
	default: return i18n("Other");
	}
}

QString forwarding(const uint32_t n)
{
	switch(n)
	{
	case 1: return i18n("Forwarding");
	case 2: return i18n("Not forwarding");
	default: return i18n("Error");
	}
}

QString Device1213::physAddress(const QByteArray &ba)
{
	QString result;
	for(uint i = 0; i < ba.size()-1; i++)
		result += QString::number( ba[i], 16 ) + ":";
	
	result.remove( result.length() -1, 1 );
	
	return result;
}

Device1213::Device1213(Session *session) :
	Device(session)
{
}

void Device1213::cacheSystem()
{
	QStringList variables;
	variables << "sysDescr.0" << "sysContact.0" << "sysName.0" << "sysLocation.0" << "sysUpTime.0";
	
	QValueVector<QVariant> vars(variables.size()); uint32_t bogus;
	if ( ! m_session->getVariables(variables, vars, bogus) )
		return;

	m_cache["sysDescr.0"] = vars[0];
	m_cache["sysContact.0"] = vars[1];
	m_cache["sysName.0"] = vars[2];
	m_cache["sysLocation.0"] = vars[3];
	m_cache["sysUpTime.0"] = vars[4];
}

void Device1213::cacheIpSettings()
{
	QStringList variables;
	variables << "ipForwarding.0" << "ipDefaultTTL.0";
	
	QValueVector<QVariant> vars(variables.size());
	uint32_t status;
	if ( ! m_session->getVariables(variables, vars, status) )
		return;
	
	m_cache["ipForwarding.0"] = vars[0];
	m_cache["ipDefaultTTL.0"] = vars[1];
}

Device1213::Interface Device1213::getInterface(uint32_t index, bool nocache, uint64_t what)
{
	static const Interface bogus;
	if ( index > requestValue("ifNumber.0").toUInt() || ! index )
		return bogus;
	
	if ( nocache )
		m_ifCache.remove(index);
	
	if ( m_ifCache.contains(index) )
		return m_ifCache[index];
	
	m_ifCache[index].ifIndex = index;
	
	QStringList variables;
	if ( what & getDescr )
		variables << "ifDescr." + QString::number(index);
	if ( what & getType )
		variables << "ifType." + QString::number(index);
	if ( what & getMtu )
		variables << "ifMtu." + QString::number(index);
	if ( what & getSpeed )
		variables << "ifSpeed." + QString::number(index);
	if ( what & getPhysAddress )
		variables << "ifPhysAddress." + QString::number(index);
	if ( what & getAdminStatus )
		variables << "ifAdminStatus." + QString::number(index);
	if ( what & getOperStatus )
		variables << "ifOperStatus." + QString::number(index);
	if ( what & getLastChange )
		variables << "ifLastChange." + QString::number(index);
	if ( what & getInOctets )
		variables << "ifInOctets." + QString::number(index);
	if ( what & getInUcastPkts )
		variables << "ifInUcastPkts." + QString::number(index);
	if ( what & getInNUcastPkts )
		variables << "ifInNUcastPkts." + QString::number(index);
	if ( what & getInDiscards )
		variables << "ifInDiscards." + QString::number(index);
	if ( what & getInErrors )
		variables << "ifInErrors." + QString::number(index);
	if ( what & getInUnknownProtos )
		variables << "ifInUnknownProtos." + QString::number(index);
	if ( what & getOutOctets )
		variables << "ifOutOctets." + QString::number(index);
	if ( what & getOutUcastPkts )
		variables << "ifOutUcastPkts." + QString::number(index);
	if ( what & getOutNUcastPkts )
		variables << "ifOutNUcastPkts." + QString::number(index);
	if ( what & getOutDiscards )
		variables << "ifOutDiscards." + QString::number(index);
	if ( what & getOutErrors )
		variables << "ifOutErrors." + QString::number(index);
	if ( what & getOutQLen )
		variables << "ifOutQLen." + QString::number(index);
	
	QValueVector<QVariant> vars(variables.size());
	uint32_t status;
	if ( ! m_session->getVariables(variables, vars, status) )
		return bogus;
	
	int i = 0;
	if ( what & getDescr )
		m_ifCache[index].ifDescr = vars[i++].toString();
	if ( what & getType )
		m_ifCache[index].ifType = vars[i++].toInt();
	if ( what & getMtu )
		m_ifCache[index].ifMtu = vars[i++].toInt();
	if ( what & getSpeed )
		m_ifCache[index].ifSpeed = vars[i++].toInt();
	if ( what & getPhysAddress )
		m_ifCache[index].ifPhysAddress = vars[i++].toByteArray();
	if ( what & getAdminStatus )
		m_ifCache[index].ifAdminStatus = vars[i++].toInt();
	if ( what & getOperStatus )
		m_ifCache[index].ifOperStatus = vars[i++].toInt();
	if ( what & getLastChange )
		m_ifCache[index].ifLastChange = vars[i++].toInt();
	if ( what & getInOctets )
		m_ifCache[index].ifInOctets = vars[i++].toInt();
	if ( what & getInUcastPkts )
		m_ifCache[index].ifInUcastPkts = vars[i++].toInt();
	if ( what & getInNUcastPkts )
		m_ifCache[index].ifInNUcastPkts = vars[i++].toInt();
	if ( what & getInDiscards )
		m_ifCache[index].ifInDiscards = vars[i++].toInt();
	if ( what & getInErrors )
		m_ifCache[index].ifInErrors = vars[i++].toInt();
	if ( what & getInUnknownProtos )
		m_ifCache[index].ifInUnknownProtos = vars[i++].toInt();
	if ( what & getOutOctets )
		m_ifCache[index].ifOutOctets = vars[i++].toInt();
	if ( what & getOutUcastPkts )
		m_ifCache[index].ifOutUcastPkts = vars[i++].toInt();
	if ( what & getOutNUcastPkts )
		m_ifCache[index].ifOutNUcastPkts = vars[i++].toInt();
	if ( what & getOutDiscards )
		m_ifCache[index].ifOutDiscards = vars[i++].toInt();
	if ( what & getOutErrors )
		m_ifCache[index].ifOutErrors = vars[i++].toInt();
	if ( what & getOutQLen )
		m_ifCache[index].ifOutQLen = vars[i++].toInt();
	
	return m_ifCache[index];
}

void Device1213::readInterfaces(uint64_t what)
{
	uint maxIf = requestValue("ifNumber.0").toUInt();
	for(uint i = 1; i <= maxIf; i++)
		getInterface(i, true, what);
}

uint32_t Device1213::getIfIndex(const QString &name, bool cache)
{
	if ( ! cache )
		readInterfaces(getID);
	
	uint maxIf = requestValue("ifNumber.0").toUInt();
	for(uint i = 1; i < maxIf; i++)
		if ( m_ifCache[i].ifDescr == name )
			return i;
	
	return 0;
}

}
