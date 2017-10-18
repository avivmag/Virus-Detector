#format is target-name: target dependencies
#{-tab-}actions

# All Targets
all: virusDetector

virusDetector: virusDetector.o
	gcc -m32 -g -Wall -o virusDetector virusDetector.o

virusDetector.o: virusDetector.c
	gcc -g -Wall -m32 -ansi -c -o virusDetector.o virusDetector.c

.PHONY: clean

clean: 
	rm -f *.o virusDetector
