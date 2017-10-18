# Virus-Detector

A simple virus detector which scans given file and search for predefined virus signatures.
In this case, this program scans the file infected.txt and searches for predefined viruses which are ordered in the file signatures.txt as described in details in the Assigment description file attached.

The development of this simulation was done as part of an assignment in "Computer Architecture" course at Ben-Gurion University in the second semester of 2016.

A detailed description of the virus detector can be found in the assignment description attached.

## Getting Started
### Prerequisites

1. Kubuntu - this program was tested only on kubuntu, but it probably can be ran on any other known nasm and gcc compatible operating systems.
	https://kubuntu.org/getkubuntu/</br>
(The followings are for those who want to compile the files themselves)
2. GNU make
	https://www.gnu.org/software/make/
3. gcc compiler
	via ```sudo apt-get install gcc-4.8``` on ubuntu based os (kubuntu included).
	
Note: this is how I used to build and run the program. There are many other well-known compilers to compile this assembly file for other types of operating systems.

### Running virus detector

1. open terminal and navigate to the program directory
2. Do this step only if virus detector rebuilt is needed: type `make` and press enter.
3. type `./virusDetector` and press enter.
4. enjoy :D.

## Built With

* [GNU make](https://www.gnu.org/software/make/) - A framework used for simple code compilation.
* [gcc](https://gcc.gnu.org/)

## Useful links

* The original source of the assignment: https://www.cs.bgu.ac.il/~caspl162/Lab3/Tasks?format=standalone.
