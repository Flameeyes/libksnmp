INCLUDES = -I$(top_srcdir) $(all_includes)
METASOURCES = AUTO
lib_LTLIBRARIES = libksnmp.la
libksnmp_la_LDFLAGS = $(all_libraries)
libksnmp_la_SOURCES = session.cpp mib1213.cpp device.cpp session1.cpp \
        session2c.cpp

libksnmp_la_LIBADD = $(NETSNMP_LIBS) $(NETSNMP_LDFLAGS)

ksnmpincludedir = $(includedir)/libksnmp
ksnmpinclude_HEADERS = session.h mib1213.h device.h session1.h session2c.h

noinst_HEADERS = fake-netsnmp-config.h
