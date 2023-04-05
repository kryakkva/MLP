#################################################
##              Makefile template              ##
#################################################

SOURCES = ./headers/* ./sources/*


#     These special targets are called phony     #
.PHONY: all dist clean install uninstall dvi build test

build: ########################
	cmake -S . -B ./../build
	cmake --build ./../build

install: build ########################
	cp -rf ./../build/MLP.app $(HOME)/Applications/

uninstall: ########################
	rm -rf $(HOME)/Applications/MLP.app

############ Cleaning up unused files ############
clean: ########################
	rm -rf ./../build

################## Style check ###################
style: ########################
	clang-format -n -verbose -style=Google $(SOURCES)

test: ########################
	@$(CC) $(TEST_FLAG) $(SRCS_TEST) $(SOURCE_TEST) -o g_tests
	./g_tests

cppcheck: ########################
	cppcheck --std=c++17 --suppress=syntaxError $(SOURCES)
##############  Project archiving   ##############
dist: clean ########################
	tar -czf MLP.tgz ./*

##############     show instuction      ##############
dvi: ########################
	open documentation/README.md
