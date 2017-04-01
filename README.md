# riot-cpp-wrappers
Unofficial repository of C++ Wrappers for RIOT-OS core facilities.

The classes provide basic OOP abstractions for embedded Programming with RIOT-OS.

# Design Goals
* Ease of use
* Header only implementation
* Avoiding Exceptions (usage of return values instead)
* Avoiding C++ standard library (exception c-headers and initializer lists)
* No hidden memory allocations

# Module Dependencies
The following Classes need additional modules:
* Semaphore (additional modules: sema)
* BlockingRingbuffer (additional modules: sema)
