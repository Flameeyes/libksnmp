INCLUDES = -I$(top_srcdir)/. $(all_includes)
METASOURCES = AUTO

noinst_PROGRAMS = ksnmpget loopget name2index

ksnmpget_LDADD = $(top_builddir)/libksnmp/libksnmp.la -lqt-mt $(LIB_KDECORE)
ksnmpget_SOURCES = ksnmpget.cpp
ksnmpget_LDFLAGS = $(all_libraries)

loopget_LDFLAGS = $(all_libraries)
loopget_SOURCES = loopget.cpp
loopget_LDADD = $(top_builddir)/libksnmp/libksnmp.la -lqt-mt $(LIB_KDECORE)
name2index_LDFLAGS = $(all_libraries)
name2index_SOURCES = name2index.cpp
name2index_LDADD = $(top_builddir)/libksnmp/libksnmp.la -lqt-mt $(LIB_KDECORE)
