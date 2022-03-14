# UnitTestLib
*Library to easily unit test parts of some Particle device code off device (native gcc compile)*


Github Repository: https://github.com/rickkas7/UnitTestLib
License: MIT

This library contains a small subset of functions available in Device OS. For example:

- String
- Time
- millis()
- Parts of Log.info, etc.

These are designed so you can write unit tests that run on a native gcc compiler, typically on Mac or Linux. On Linux, you can also use this library to run your unit tests on Valgrind, which is useful for checking for memory leaks, buffer overruns, etc.

You will typically:

- Include this repo as a git submodule in your code

```
git submodule add https://github.com/rickkas7/UnitTestLib
```

- Update the submodules. This is also necessary when cloning a fresh copy of the repo.

```
git submodule update --init --recursive
```

- Call make to build this module
- Include libwiringgcc.a (static library) in your unit test binary
- Include this directory in your header search list (-I)

Here's the full Makefile for JsonParserGeneratorRK from the test directory:

```
all : JsonTest
	./JsonTest

JsonTest : JsonTest.cpp ../src/JsonParserGeneratorRK.cpp ../src/JsonParserGeneratorRK.h libwiringgcc
	gcc JsonTest.cpp ../src/JsonParserGeneratorRK.cpp UnitTestLib/libwiringgcc.a -std=c++11 -lc++ -IUnitTestLib -I../src -o JsonTest

check : JsonTest.cpp ../src/JsonParserGeneratorRK.cpp ../src/JsonParserGeneratorRK.h libwiringgcc
	gcc JsonTest.cpp ../src/JsonParserGeneratorRK.cpp UnitTestLib/libwiringgcc.a -g -O0 -std=c++11 -lc++ -IUnitTestLib -I ../src -o JsonTest && valgrind --leak-check=yes ./JsonTest 

libwiringgcc :
	cd UnitTestLib && make libwiringgcc.a 	
	
.PHONY: libwiringgcc
```

- Include Particle.h in your unit test C++ source

```
#include "Particle.h"
```

- To update the submodule if changes are made in this repository

```
git submodule update --remote
```

## Examples

- [JsonParserGeneratorRK](https://github.com/rickkas7/JsonParserGeneratorRK)


## Version History

### 0.0.1 (2022-03-14)

- Extracted from JsonParserGeneratorRK to a separate repository

