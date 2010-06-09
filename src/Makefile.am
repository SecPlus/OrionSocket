AM_CFLAGS=-Wall -Werror -ggdb
AM_LDFLAGS=-version-info 1:0:1

lib_LTLIBRARIES = liborionsocket.la
liborionsocket_la_SOURCES = socket.c 	\
							http.c 		\
							util.c

library_includedir=$(includedir)/orion/socket
library_include_HEADERS=socket.h 		\
						err.h 			\
						http.h 			\
						types.h 		\
						api.h			\
						util.h
