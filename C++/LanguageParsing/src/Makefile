CXX_DIR = ../lib/cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -g


# Testing files and targets.
run-tests:	regex_tests scanner_tests i1_assessment_tests i2_assessment_tests
	./regex_tests
	./scanner_tests
	./i1_assessment_tests
	./i2_assessment_tests

regex_tests:	regex_tests.cpp regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o regex_tests regex.o regex_tests.cpp

regex_tests.cpp:	regex.h regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cpp regex_tests.h

scanner_tests:	scanner_tests.cpp scanner.o regex.o readInput.o 
	g++ $(FLAGS) -I$(CXX_DIR) -g -o scanner_tests \
		scanner.o regex.o readInput.o scanner_tests.cpp

scanner_tests.cpp:	scanner.o scanner_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cpp scanner_tests.h
	
##########################
i1_assessment_tests:	i1_assessment_tests.cpp scanner.o regex.o readInput.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o i1_assessment_tests \
		scanner.o regex.o readInput.o i1_assessment_tests.cpp

i1_assessment_tests.cpp:	scanner.o i1_assessment_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o i1_assessment_tests.cpp i1_assessment_tests.h
###############################
	
i2_assessment_tests:	i2_assessment_tests.cpp scanner.o regex.o readInput.o ExtToken.o Parser.o ParseResult.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o i2_assessment_tests \
		scanner.o regex.o readInput.o i2_assessment_tests.cpp ExtToken.o Parser.o ParseResult.o

i2_assessment_tests.cpp:	ParseResult.h Parser.h ExtToken.h scanner.o i1_assessment_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o i2_assessment_tests.cpp i2_assessment_tests.h



# Program files.
scanner.o:	scanner.cpp scanner.h regex.h readInput.o 
	g++ $(FLAGS) -c -g scanner.cpp

readInput.o:	readInput.cpp readInput.h 
	g++ $(FLAGS) -c -g readInput.cpp

regex.o:	regex.cpp regex.h 
	g++ $(FLAGS) -c -g regex.cpp 
	
ExtToken.o: ExtToken.cpp ExtToken.h Parser.h
	g++ $(FLAGS) -c ExtToken.cpp
	
Parser.o: Parser.cpp AST.h Parser.h scanner.h ExtToken.h
	g++ $(FLAGS) -c Parser.cpp 

#AST.o: //write this for iteration 3	

ParseResult.o: ParseResult.cpp ParseResult.h
	g++ $(FLAGS) -c ParseResult.cpp


clean:
	rm -Rf *.o \
		*.dSYM \
		regex_tests regex_tests.cpp \
		scanner_tests scanner_tests.cpp \
		i1_assessment_tests i2_assessment_tests i1_assessment_tests.cpp i2_assessment_tests.cpp\
		a.out  
