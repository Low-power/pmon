#	$Id: Makefile,v 1.1.1.1 2006/09/14 01:59:06 root Exp $

PROG=	bootelf

SRCS=	bootelf.c 
DESTDIR=/opt/pmon2000/tools

OBJS=	bootelf.o

#CFLAGS+=-I ${.CURDIR} -I .
#CFLAGS+=-I ${.CURDIR} -I . -D DO_SWAP=1
CFLAGS += -Wall

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} -o $@ ${OBJS} ${LIBS}

install:
	cp ${PROG} ${DESTDIR}/bin

clean:
	rm -rf ${OBJS} ${PROG}
