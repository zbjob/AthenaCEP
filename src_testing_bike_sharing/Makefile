LDFLAGS = -lpthread 

CEP_OBJ = PatternMatcher.o Query.o _shared/PredicateMiner.o _shared/MurmurHash3.o _shared/MonitorThread.o freegetopt/getopt.o lex.yy.o
# CEP_MATCH_OBJ = $(CEP_OBJ) cep_match/MonitorThread.o cep_match/cep_match.o
CEP_MATCH_OBJ = $(CEP_OBJ) cep_match/cep_match.o

all: bin/cep_match 
default: all

clean:
	rm *.o
	rm */*.o

bin/cep_match: $(CEP_MATCH_OBJ)
	g++ -std=c++14  -o bin/cep_match $(CEP_MATCH_OBJ) $(LDFLAGS)




%.o: %.c
	gcc -c $< -o $@

%.o: %.cpp
	g++ -std=c++14 -O3 -mavx2 -c $< -o $@


