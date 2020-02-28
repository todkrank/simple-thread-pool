# Simple dynamic thread pool

* c++11 libraries only
* doesn't use <future>, std::bind, and templates
* uses lambda-functions because any number of parameters can be passed to lambda and then lambda itself will be pushed into a queue of work tasks, although this has not been implemented.
* compile with *-std=c++11 -pthread* flags

## How to run

* the program awaits for integer input, N, then runs a task of printing all numbers from 0 to N to std::cout in available thread. Entering "0" signals the program to finish pending tasks and exit.
