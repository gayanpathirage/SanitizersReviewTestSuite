#C++ Sanitizers Review Test Suite


##Overview
This repository contains C++ test code and support scripts developed to easily compare Memory Sanitizer tools like
Valgrind, ASAN, Dr.Memory, etc

**Review Results**

@ [Sanitizers for C++ (Valgrind, Dr.Memory, ASAN, TSAN, MSAN, .....)](https://confluence.millenniumit.com/pages/viewpage.action?pageId=30521524)



##Branch Model
This repository is following GitHub Flow branch model
Read more @ [GitHub Flow](https://guides.github.com/introduction/flow/) before you commit changes to this repository.



##Requirements
The following are required for building this source code.

* GCC 4.9.3 or above



##Contributors
The following people can/have contribute/d to this project.

* Gayan Pathirage \<<gayancp@millenniumit.com>\>


##Building
**Normal Build**


/usr/local/gcc-4.9.3/bin/g++ -o MCTester -O -g -Wall main.cpp

**ASAN Build**


/usr/local/gcc-4.9.3/bin/g++ -o MCTester_ASAN -static-libstdc++ -static-libasan -O -g -fsanitize=address -fno-omit-frame-pointer -Wall main.cpp

##Running
e.g. Run only Test 1;

**Valgrind**
/mnt/exds/ValgrindInstallations/SuSE-11-SP3/valgrind-3.11.0/vg-in-place --tool=memcheck  MCTester 1 1


**ASAN**
MCTester_ASAN 1 1

**Dr.Memory**
/x01/exd10/DrMemory-Linux-1.10.0-2/bin>./drmemory -light -- MCTester 1 1

##Additional Settings
**Run Time flags of ASAN**


export ASAN_OPTIONS=detect_leaks=1:verbose=1

detect_leaks will enable LeakSanitizer (LSAN) with ASAN

**Options to enable when running Valgrind**


with --keep-stacktraces=none --undef-value-errors=no options to be inline with ASAN features

**Options to enable when running Dr.Memory**


-light : This will not detect uninitialized read and memory leak errors (to be inline with ASAN features)

-light -count_leaks : This will not detect uninitialized read errors, but lists leak errors.

##Major features
* Test suite can be run all tests e.g. MCTester_ASAN <No-Args>
* Test suite can run range of tests e.g. MCTester_ASAN 1 - 10
* Test suite can run tests starting from a test MCTester_ASAN 10 - this will execute all 10,11,12,....
* Test suite can run individual test e.g. MCTester_ASAN 1 1 - will execute only Test 1


##History
Repo created by Gayan Pathirage \<<gayancp@millenniumit.com>\> on April 01, 2016