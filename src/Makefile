CXX_DIR = ../lib/cxxtest
CXXTEST = $(CXX_DIR)/bin/cxxtestgen
CXXFLAGS = --error-printer --abort-on-fail --have-eh

FLAGS = -Wall -g

# Program files.
readInput.o:	readInput.cpp readInput.h
	g++ $(FLAGS) -c readInput.cpp 

regex.o:	regex.cpp regex.h
	g++ $(FLAGS) -c regex.cpp 

scanner.o:	scanner.cpp scanner.h regex.h
	g++ $(FLAGS) -c scanner.cpp 

parser.o: parser.cpp parser.h scanner.h extToken.h AST.h
	g++ $(FLAGS) -c parser.cpp 

extToken.o: extToken.cpp extToken.h scanner.h parser.h
	g++ $(FLAGS) -c extToken.cpp 

parseResult.o: parseResult.cpp parseResult.h 
	g++ $(FLAGS) -c parseResult.cpp 

# Testing files and targets.
run-tests:	regex_tests scanner_tests parser_tests ast_tests codegeneration_tests
	./regex_tests
	./scanner_tests
	./parser_tests
	./ast_tests
	./codegeneration_tests

regex_tests:	regex_tests.cpp regex.o
	g++ $(FLAGS) -I$(CXX_DIR) -o regex_tests regex.o regex_tests.cpp

regex_tests.cpp:	regex.h regex_tests.h
	$(CXXTEST) $(CXXFLAGS) -o regex_tests.cpp regex_tests.h

scanner_tests:	scanner_tests.cpp scanner.o regex.o readInput.o
	g++ $(FLAGS) -I$(CXX_DIR)  -o scanner_tests \
		scanner.o regex.o readInput.o scanner_tests.cpp

scanner_tests.cpp:	scanner.o scanner_tests.h readInput.h
	$(CXXTEST) $(CXXFLAGS) -o scanner_tests.cpp scanner_tests.h

parser_tests: parser_tests.cpp  scanner.o readInput.o extToken.o parser.o parseResult.o regex.o 
	g++ $(FLAGS) -I$(CXX_DIR) -o parser_tests \
		scanner.o readInput.o parser_tests.cpp extToken.o parser.o parseResult.o regex.o

parser_tests.cpp: scanner.o parser_tests.h readInput.h extToken.o parser.o parseResult.o
	$(CXXTEST) $(CXXFLAGS) -o parser_tests.cpp parser_tests.h

ast_tests:	ast_tests.cpp scanner.o readInput.o parser.o extToken.o parseResult.o regex.o extToken.h AST.h
	g++ $(FLAGS) -I$(CXX_DIR) -o ast_tests \
		ast_tests.cpp scanner.o readInput.o parser.o extToken.o parseResult.o regex.o

ast_tests.cpp:	parser.o ast_tests.h readInput.h parseResult.h extToken.h scanner.h AST.h
	$(CXXTEST) $(CXXFLAGS) -o ast_tests.cpp ast_tests.h

codegeneration_tests: codegeneration_tests.cpp scanner.o readInput.o parser.o extToken.o parseResult.o regex.o extToken.h AST.h
	g++ $(FLAGS) -I$(CXX_DIR) -o codegeneration_tests \
		codegeneration_tests.cpp scanner.o readInput.o parser.o extToken.o parseResult.o regex.o

codegeneration_tests.cpp: codegeneration_tests.h AST.h parser.o readInput.h parseResult.h extToken.h scanner.h
	$(CXXTEST) $(CXXFLAGS) -o codegeneration_tests.cpp codegeneration_tests.h 

clean:
	rm -Rf *.o *.dSYM\
		regex_tests regex_tests.cpp \
		scanner_tests scanner_tests.cpp \
		parser_tests parser_tests.cpp \
		readInput \
		ast_tests ast_tests.cpp \
		AST \
		codegeneration_tests codegeneration_tests.cpp \
		../samples/forest_loss_v2 ../samples/forest_loss_v2.cpp ../samples/forest_loss_v2.diff ../samples/forest_loss_v2.output \
		../samples/my_code_1 ../samples/my_code_1.cpp ../samples/my_code_1.diff ../samples/my_code_1.output \
		../samples/my_code_2 ../samples/my_code_2.cpp ../samples/my_code_2.diff ../samples/my_code_2.output \
		../samples/sample_4 ../samples/sample_4.cpp ../samples/sample_4.diff ../samples/sample_4.output \
		../samples/sample_5 ../samples/sample_5.cpp ../samples/sample_5.diff ../samples/sample_5.output \
		../samples/sample_6 ../samples/sample_6.cpp ../samples/sample_6.diff ../samples/sample_6.output \
		../samples/sample_7 ../samples/sample_7.cpp ../samples/sample_7.diff ../samples/sample_7.output \
		../samples/sample_8 ../samples/sample_8.cpp ../samples/sample_8.diff ../samples/sample_8.output \


