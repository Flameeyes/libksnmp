INCLUDES = $(all_includes)
METASOURCES = AUTO

noinst_PROGRAMS = ksnmpget loopget

ksnmpget_LDADD = $(top_builddir)/libksnmp/libksnmp.la -lqt-mt $(LIB_KDECORE)
ksnmpget_SOURCES = ksnmpget.cpp
ksnmpget_LDFLAGS = $(all_libraries)

loopget_LDFLAGS = $(all_libraries)
loopget_SOURCES = loopget.cpp
loopget_LDADD = $(top_builddir)/libksnmp/libksnmp.la -lqt-mt $(LIB_KDECORE)
