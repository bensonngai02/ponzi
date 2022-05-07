# ponzi
LISP Emulator
----------------
##Compile + Run Program:
    - This is an absolutely not scuffed, genius way of creating our automated tests.
    - Due to fiddling around with Makefiles for too long, we've created a way to run tests with shell + make.

    ### Instructions:
        1. run `./create.sh [test name]` in Terminal
            - For example, to run basic.scheme, run `./create.sh basic`.
            - This creates a new Makefile with the commands + correct files to run
            - Note: Make sure you're directory is simply ./cs429h_s22_pa_[username] 
                    - if permission denied, execute chmod +x ./create.sh in Terminal
        2. run "make compile"
            - This compiles the Scheme language input code into our revolutionary PONZI intermediate representation,
              and executes the instructions correctly, printing the output to a .out file found in /tests/scheme.

    ### Note:
        - Running "make run" takes in the PONZI intermediate representation as input and runs it on our SECD machine,
          printing the correct output to the console.
        - "make clean" will delete all .out files. Make sure to make clean every time before you make compile; otherwise,
           you'll receive a message that "make compile is already up to date".
        - Mainly used for debugging purposes, or if you want to write some PONZI instructions in your own .ponzi file,
          you can run it as well! 