<div align="center">

<h1>unitctest</h1>

![Standard](https://img.shields.io/badge/C-99/11/14/17-blue.svg)
![Standard](https://img.shields.io/badge/C++-11/14/17/20-blue.svg)
![License](https://img.shields.io/github/license/luizinhosuraty/unitctest)
![Lines of code](https://img.shields.io/tokei/lines/github/luizinhosuraty/unitctest?label=LoC-header)

</div>

unitctest is a tiny (~500 LoC for the unitctest header), header-only
unit test framework for C (and C++)

# Usage / Installation

Using unitctest is very easy and straightforward, with no need to install anything.

In order to use unitctest, you basically have two options :
1. Download the source code of this repository and copy folder include/unitctest in your project;
2. Use ```git submodule``` (branch ```release``` presents a cleaner repository)

Once you have downloaded (or added the submodule), include the header in your test C file :

```c
#include <unitctest/unitctest.h>
```

And you are set. Well, almost. For unitctest to work correctly, you must have the following
included at the end of your test file :

```c
TEST_MAIN() // DO NOT add ';' at the end.
```

This is required, among other things, to create the ```main``` function of the test program.
Therefore, this must appear in only a single test source code for a given test program.

Check examples inside folder ```test/```.

# Supported Platforms

unitctest is officially supported on the following platforms:

Operating Systems          |
-------------------------- |
Linux                      |
macOS                      |

If you run into any problems on your platform, please create an
[issue](https://github.com/luizinhosuraty/unitctest/issues), or, even better, send a PR
with fixes. ;)

# Supported Languages / Versions

Support for C++ is still under development.

Language          | Version
----------------- | -----------------
C                 | C99*, C11, C14, C17
C++**             | C++1x, C++2x

*There is a known small issue with logging (related to print type) 

** Some caveats :
 - Comparisons rely on '==' operator of operands;
 - Compiler will signal errors if operand does not have '<<' implemented;
 - Currently mixing cerr with fprintf to take advantage of '<<'.


# Questions / FAQs

## Why another unit test framework ?
Well, the answer is that I wanted to have a simple framework that I could easily change to fulfill
any special need (header-only, small size, etc).

## What are the differences with googletest, and other frameworks ?
The main difference is that unitctest is smaller and header-only. Thus, no installation is required.
Moreover, googletest has more features (and it is heavier, not header-only, ...), as mocking,
which we do not support.

## What about mocking ?
mocking is currently out of the scope for unitctest. However, you can use unitctest along with the 
[Fake Function Framework](https://github.com/meekrosoft/fff).

