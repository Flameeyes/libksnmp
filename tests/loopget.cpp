/***************************************************************************
 *   Copyright (C) 2005 by Diego 'Flameeyes' Pettenò                       *
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

#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kdebug.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STDINT_H
# include <stdint.h>
#elif defined HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include <libksnmp/session1.h>
#include <libksnmp/session2c.h>

#include <iostream>

static const char description[] =
    I18N_NOOP("Test for libksnmp, get a variable in a defined loop");

static const char version[] = "20050211";

static KCmdLineOptions options[] =
{
	{ "c <community>", I18N_NOOP("Name of the community to use, default 'public'"), "public" },
	{ "V <version>", I18N_NOOP("SNMP version to use, one of 1 or 2c, default 1"), "1" },
	{ "+agent", I18N_NOOP("Address of the agent to request the data from"), NULL },
	{ "+oid", I18N_NOOP("Oid to request via SNMP"), NULL },
	{ "+[numloop]", I18N_NOOP("Number of iterations in the loop"), "100" },
	{ "+[delay]", I18N_NOOP("Delay between reads, in milliseconds"), "1000" },
//	{ "+[URL]", I18N_NOOP( "Document to open." ), 0 },
	KCmdLineLastOption
};

int main(int argc, char **argv)
{
	KAboutData about("ksnmpget", I18N_NOOP("ksnmpget"), version, description,
			KAboutData::License_GPL, "(C) 2005 Diego Pettenò", 0, 0, "flameeyes@users.berlios.de");
	about.addAuthor( "Diego Pettenò", 0, "flameeyes@users.berlios.de" );
	KCmdLineArgs::init(argc, argv, &about);
	KCmdLineArgs::addCmdLineOptions( options );
	
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	KSNMP::init("ksnmpget");
	int ret = 0;
	
	uint32_t status;
	QVariant retval;
	
	KSNMP::Session *session = NULL;
	if ( args->getOption("V") == "1" )
		session = new KSNMP::Session1("", args->getOption("c"));
	else if ( args->getOption("V") == "2c" )
		session = new KSNMP::Session2c("", args->getOption("c"));
	else {
		kdError() << "You must select one of 1 or 2c versions." << endl;
		ret = -1;
		goto endofmain;
	}
	
	if ( args->count() != 4 )
	{
		kdError() << "Invalid arguments count." << endl;
		ret = -2;
		goto endofmain;
	}
	
	session->setPeerName( args->arg(0) );
	
	if ( ! session->open() )
	{
		kdError() << "Unable to open SNMP session." << endl;
		ret = -3;
		goto endofmain;
	}
	
	const int loop = atoi( args->arg(2) );
	const int delay = atoi( args->arg(3) ) * 1000;
	
	for(int i = 0; i < loop; i++)
	{
		if ( ! session->getVariable(args->arg(1), retval, status) )
		{
			kdError() << "Error getting OID" << endl;
			ret = (int)status;
			goto endofmain;
		}
		std::cout << i << "\t" << retval.toCString() << std::endl;
		usleep(delay);
	}
	
endofmain:
	args->clear();
	KSNMP::shutdown();
	
	return ret;
}

