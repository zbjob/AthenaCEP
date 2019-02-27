LDFLAGS = -lpthread 

CEP_OBJ = PatternMatcher.o Query.o _shared/PredicateMiner.o _shared/MurmurHash3.o _shared/MonitorThread.o freegetopt/getopt.o lex.yy.o
# CEP_MATCH_OBJ = $(CEP_OBJ) cep_match/MonitorThread.o cep_match/cep_match.o
CEP_MATCH_OBJ = $(CEP_OBJ) cep_match/cep_match.o
CEP_SYMBOLMATCH_OBJ = $(CEP_OBJ) cep_symbolmatch/BatchWorker.o cep_symbolmatch/cep_symbolmatch.o
CEP_BIN2CSV_OBJ = $(CEP_OBJ) cep_bin2csv/cep_bin2csv.o
CEP_CSV2BIN_OBJ = $(CEP_OBJ) cep_csv2bin/cep_csv2bin.o

all: bin/cep_symbolmatch bin/cep_match bin/cep_csv2bin bin/cep_bin2csv
default: all

clean:
	rm *.o
	rm */*.o

bin/cep_match: $(CEP_MATCH_OBJ)
	g++ -std=c++14  -o bin/cep_match $(CEP_MATCH_OBJ) $(LDFLAGS)

bin/cep_symbolmatch: $(CEP_SYMBOLMATCH_OBJ)
	g++ -std=c++14 -o bin/cep_symbolicmatch $(CEP_SYMBOLMATCH_OBJ) $(LDFLAGS)


bin/cep_bin2csv: $(CEP_BIN2CSV_OBJ)
	g++ -std=c++14 -o bin/cep_bin2csv $(CEP_BIN2CSV_OBJ) $(LDFLAGS)

bin/cep_csv2bin: $(CEP_CSV2BIN_OBJ)
	g++ -std=c++14 -o bin/cep_csv2bin $(CEP_CSV2BIN_OBJ) $(LDFLAGS)

%.o: %.c
	gcc -c $< -o $@

%.o: %.cpp
	g++ -std=c++14 -mavx2 -c $< -o $@


