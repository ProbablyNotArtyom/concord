CC	:= gcc
OBJDIR	:= obj
LIBDIR	:= lib

SRC		:= $(wildcard github-v3-user-agent.cpp github-v3-git-database.cpp json-scanf.c settings.c)
_OBJS	:= $(patsubst %.cpp, %.o, $(SRC))
OBJS1   += $(patsubst %.c, %.o, $(_OBJS))
OBJS 	:= $(addprefix $(OBJDIR)/, $(OBJS1))

LIBDISCORD_CFLAGS	:= -I./
LIBDISCORD_LDFLAGS	:=  -L./$(LIBDIR) -ldiscord -lcurl


LIBDISCORD_LDFLAGS += -lbearssl -static


LIBS_CFLAGS	:= $(LIBDISCORD_CFLAGS)
LIBS_LDFLAGS	:= $(LIBDISCORD_LDFLAGS)

LIBDISCORD_SLIB	:= $(LIBDIR)/libdiscord.a

CFLAGS := -std=c++03 -Wno-write-strings -fpermissive -O0 -g -DLIBDISCORD_DEBUG -D__stensal__

CFLAGS += -D_DEFAULT_SOURCE

PREFIX ?= /usr/local

.PHONY : all mkdir install clean purge

all : mkdir $(OBJS) $(LIBDISCORD_SLIB) bot-git

mkdir :
	mkdir -p $(OBJDIR) $(LIBDIR)
	echo $(OBJS)

$(OBJDIR)/settings.o : settings.c
	$(CC) $(CFLAGS) $(LIBS_CFLAGS) -c -o $@ $<

$(OBJDIR)/json-scanf.o : json-scanf.c
	$(CC) $(CFLAGS) $(LIBS_CFLAGS) -c -o $@ $<
$(OBJDIR)/jscon-common.o : jscon-common.c
	$(CC) $(CFLAGS) $(LIBS_CFLAGS) -c -o $@ $<

$(OBJDIR)/github-v3-user-agent.o: github-v3-user-agent.cpp
	$(CXX) $(CFLAGS) $(LIBS_CFLAGS) -c -o $@ $<

$(OBJDIR)/github-v3-git-database.o: github-v3-git-database.cpp
	$(CXX) $(CFLAGS) $(LIBS_CFLAGS) -c -o $@ $<

$(OBJDIR)/github-v3-repositories.o: github-v3-repositories.cpp
	$(CXX) $(CFLAGS) $(LIBS_CFLAGS) -c -o $@ $<

bot-git: bot-git.cpp $(OBJS)
	$(CXX) $(CFLAGS) $(LIBS_CFLAGS) -o bot-git.exe $< $(OBJS) -lcurl

$(LIBDISCORD_SLIB) : $(OBJS)
	$(AR) -cvq $@ $(OBJS)

clean :
	rm -rf $(OBJDIR) $(LIBDIR) *.exe 

purge : clean
	rm -rf $(LIBDIR)
