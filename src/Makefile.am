AM_CFLAGS=-Wall -Wextra -Wno-variadic-macros -Werror -pedantic -fPIC -O2 -std=gnu89 -g

lib_LTLIBRARIES = liborionsocket.la
liborionsocket_la_SOURCES = socket.c 	\
							http.c 		\
							util.c \
							cookie.c \
							debug.c

liborionsocket_la_LDFLAGS = -version-info 1:9:0

library_includedir=$(includedir)/orion/socket
library_include_HEADERS=socket.h 		\
						err.h 			\
						http.h 			\
						types.h 		\
						api.h			\
						util.h          \
						debug.h         \
						cookie.h 	\
						../orion_config.h
