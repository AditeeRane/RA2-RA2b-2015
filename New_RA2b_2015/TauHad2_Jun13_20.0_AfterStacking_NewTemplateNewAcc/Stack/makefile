DIR = $(shell pwd)

#CXX           = ~/.vim/bundle/ClangComplete/bin/cc_args.py g++
CXX           = g++
#CXXFLAGS      = -Wno-deprecated $(shell root-config --cflags) -I../$(DEL)/external/ -I../$(DEL) -std=c++0x 
CCFLAGS       = $(shell root-config --cflags) -std=c++0x
CXXFLAGS      = $(shell root-config --cflags) -I../.. 
LDFLAGS       = 
LD            = g++

LIBS = $(shell root-config --glibs)

OBJS2          = Stack.o ../../Selection.o
PROGRAM2       = Stack

HEADERS       = ../../Selection.h

%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CCFLAGS) -c $<

%.o : %.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

all : $(PROGRAM2)

clean : *.o $(PROGRAM2) 
	$(RM) *.o $(PROGRAM2)

.PHONY : all


$(PROGRAM2): $(OBJS2)
	@echo "Linking $(PROGRAM2) ..."
	@$(LD) $(OBJS2) $(LIBS) -o $(PROGRAM2)
	@echo "done"

