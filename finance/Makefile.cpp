################
#    MAKEFILE
################


# Compiler : 
CC=g++
# FLAGS
CCFLAFS=`root-config --cflags --libs` 
# TARGET
TRGT=game



INCLUDES+= I$(ROOTSYS)/include -I..

all : $(TRGT)

$(TRGT):        
        $(CC) -c machine.cxx -o machine.o `root-config --cflags --libs`

        $(CC)  game.cxx machine.o -o $(TRGT) `root-config --cflags --libs`      

clean :         
        rm -fr *.o game
