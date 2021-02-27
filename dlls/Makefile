#
# RCBot makefile for Linux
#
# created: 12/16/2000 botman (botman@mailandnews.com)
# updated: 26/02/2021 [APG]Robocop
#

# the following specifies the path to your MOD...
#MOD_DIR = /home/paul/hlds_l/rcbot

CPP = g++ -m32

BASEFLAGS = -Dstricmp=strcasecmp -Dstrcmpi=strcasecmp -DRCBOT_META_BUILD -D__linux__# -D_DEBUG

#CPPFLAGS = ${BASEFLAGS} -g -ggdb -march=i486 -I. -I../engine -I../common -I../pm_shared

CPPFLAGS = ${BASEFLAGS} -mtune=generic -march=i686 -mmmx -msse -msse2 -mfpmath=sse -pipe -O2 \
	-Wno-write-strings -Wno-write-strings -Wno-unused-local-typedefs -static-libstdc++ \
	-funsafe-math-optimizations -I. -I../engine -I../common -I../pm_shared
		
OBJ = dll.o engine.o rcbot_base.o rcbot_commands.o rcbot_file.o rcbot_manager.o rcbot_message.o \
    rcbot_meta_api.o rcbot_mod.o rcbot_profile.o rcbot_strings.o rcbot_util.o  rcbot_visibles.o \
	rcbot_specialists.o rcbot_navigator.o

rcbot_mm.so: ${OBJ}
	${CPP} -fPIC -shared -o $@ ${OBJ} -ldl -lm
	cp -f rcbot_mm.so ./Release

clean:
	-rm -f *.o
	-rm -f rcbot_mm.so	

%.o:	%.cpp
	${CPP} ${CPPFLAGS} -c $< -o $@

%.o:	%.c
	${CPP} ${CPPFLAGS} -c $< -o $@
