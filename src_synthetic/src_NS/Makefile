LDFLAGS = -lpthread 

PYFLAGI = -I/usr/include/python2.7
PYFLAGL = -L/usr/lib64/python2.7/config -lpython2.7

CEP_OBJ = PatternMatcher.o Query.o _shared/PredicateMiner.o _shared/MurmurHash3.o _shared/MonitorThread.o freegetopt/getopt.o lex.yy.o NormalDistGenChangePattern.o
 #CEP_MATCH_OBJ = $(CEP_OBJ) cep_match/MonitorThread.o cep_match/cep_match.o
CEP_MATCH_OBJ = $(CEP_OBJ) cep_match/cep_match.o
#CEP_CLEANDATA_OBJ = $(CEP_OBJ) cep_cleanData/cep_cleanData.o

all: bin/cep_match 
default: all

clean:
	rm *.o
	rm */*.o

bin/cep_match: $(CEP_MATCH_OBJ)
	#g++ -std=c++14 -o bin/cep_match $(CEP_MATCH_OBJ) $(PYFLAGI) $(PYFLAGL) $(LDFLAGS)
	g++ -std=c++14 -o bin/cep_match $(CEP_MATCH_OBJ) $(LDFLAGS)

#bin/cep_cleanData: $(CEP_CLEANDATA_OBJ)
#	g++ -std=c++14 -o bin/cep_cleanData $(CEP_CLEANDATA_OBJ) $(LDFLAGS)




%.o: %.c
	gcc -c $< -o $@

%.o: %.cpp
	g++ -std=c++14 -O3 -mavx2 -c $< -o $@ 


