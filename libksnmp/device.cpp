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
