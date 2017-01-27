#C++ Sanitizers Review Test Suite
[![Build Status](https://travis-ci.org/gayanpathirage/SanitizersReviewTestSuite.svg?branch=master)](https://travis-ci.org/gayanpathirage/SanitizersReviewTestSuite)

<a href="https://scan.coverity.com/projects/gayanpathirage-sanitizersreviewtestsuite">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/11565/badge.svg"/>
</a>

##Overview
This repository contains C++ test code and support scripts developed to easily compare Memory Sanitizer tools like
Valgrind, ASAN, Dr.Memory, etc


**Review Results**

@ [To be updated in the wiki](https://github.com/gayanpathirage/SanitizersReviewTestSuite/wiki)


##Licence
The MIT License (MIT)


##Branch Model
This repository is following GitHub Flow branch model
Read more @ [GitHub Flow](https://guides.github.com/introduction/flow/) before you commit changes to this repository.



##Requirements
The following are required for building this source code.

* GCC 4.9.3 or above


##Building
**Normal Build**

```
/usr/local/gcc-4.9.3/bin/g++ -o MCTester -O -g -Wall main.cpp
```

**ASAN Build**

```
/usr/local/gcc-4.9.3/bin/g++ -o MCTester_ASAN -static-libstdc++ -static-libasan -O -g -fsanitize=address -fno-omit-frame-pointer -Wall main.cpp
```

##Running
e.g. Run only Test 1;

**Valgrind**
```
/mnt/ValgrindInstallations/SuSE-11-SP3/valgrind-3.11.0/vg-in-place --tool=memcheck  MCTester 1 1
```

**ASAN**
```
MCTester_ASAN 1 1
```

**Dr.Memory**
```
/mnt/DrMemory-Linux-1.10.0-2/bin>./drmemory -light -- MCTester 1 1
```

##Additional Settings
**Run Time flags of ASAN**

```
export ASAN_OPTIONS=detect_leaks=1:verbose=1
```

detect_leaks will enable LeakSanitizer (LSAN) with ASAN

**Options to enable when running Valgrind**


with --keep-stacktraces=none --undef-value-errors=no options to be inline with ASAN features

**Options to enable when running Dr.Memory**


-light : This will not detect uninitialized read and memory leak errors (to be inline with ASAN features)

-light -count_leaks : This will not detect uninitialized read errors, but lists leak errors.

##Results
* **Valgrind** : [Gist](https://gist.github.com/gayanpathirage/3731b6493a7b74daa6d5fc59289cc35a)
* **ASAN** : [Gist](https://gist.github.com/gayanpathirage/6a791caf81e39ea8d4926aeeaf71641d)
* **Dr.Memory** : [Gist](https://gist.github.com/gayanpathirage/f4ce60f5a5f50e53b6f0358bd3a834e1)

##Major features
* Test suite can be run all tests e.g. MCTester_ASAN <No-Args>
* Test suite can run range of tests e.g. MCTester_ASAN 1 - 10
* Test suite can run tests starting from a test MCTester_ASAN 10 - this will execute all 10,11,12,....
* Test suite can run individual test e.g. MCTester_ASAN 1 1 - will execute only Test 1
