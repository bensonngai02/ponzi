# ponzi
SECD Machine LISP Functional Programming Language Emulator for Scheme
----------------

*** NOTE to Dr. Gheith + TAs: Upon submission, 'make compile' - for function tests specifically, seems to work on Mac but not the Linux lab machines. However, 'make compile' should work for most, if not all the other tests (arithmetic, boolean, conditionals). To mitigate this, we have included the output for testing functions in ".output" file format, and also a screen recording of it working in ".mov" file format. Besides that, 'make run' to run the individual .ponzi files should work for all tests. To run this, please comment out the main method in compiler.cxx + uncomment the main method in secd.cxx. The instructions below can be followed. ***

##Compile + Run Program:
    - This is an absolutely not scuffed, genius way of creating our automated tests.
    - Due to fiddling around with Makefiles for too long, we've created a way to run tests with shell + make.

    ### Instructions:
        1. run `./create.sh [test name]` in Terminal
            - For example, to run square.scheme, run `./create.sh square`.
            - This creates a new Makefile with the commands + correct files to run
            - Note: Make sure you're directory is "./cs429h_s22_pa_[username]"
                    - if permission denied, execute chmod +x ./create.sh in Terminal
        2. run `make compile`
            - This compiles the Scheme language input code into our revolutionary PONZI intermediate representation,
              and executes the instructions correctly, printing the output to a .out file found in /tests/scheme.

    ### Note:
        - Running "make run" takes in the PONZI intermediate representation as input and runs it on our SECD machine,
          printing the correct output to the console.
        - "make clean" will delete all .out files. Make sure to make clean every time before you make compile; otherwise,
           you'll receive a message that "make compile is already up to date".
        - Mainly used for debugging purposes, or if you want to write some PONZI instructions in your own .ponzi file,
          you can run it as well! 