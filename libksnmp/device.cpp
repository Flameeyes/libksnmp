/***************************************************************************
 *   Copyright (C) 2004 by Diego 'Flameeyes' Pettenò                       *
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
#include "config.h"
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
#elif defined HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include "device.h"

namespace KSNMP {

Device::Device(Session *session)
	: m_session(session)
{
}

Device::~Device()
{
}

QVariant Device::requestValue(const QString &oid, bool nocache)
{
	if ( nocache )
		m_cache.remove(oid);
	
	if ( m_cache.contains(oid) )
		return m_cache[oid];
	
	QVariant retvar; uint32_t bogus;
	m_session->getVariable(oid, retvar, bogus);
	
	return retvar;
}

}
