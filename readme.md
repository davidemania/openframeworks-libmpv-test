This is a test openFrameworks application written to explore integration with libmpv

Code is just an experiment, to be used only for testing/research

The environment used to edit and build is Visual Studio Code under Linux Ubuntu 18.04. The project can be ported very easily to Qt Creator if needed.

To compile open the VS workspace and press CTRL-SHIFT-B and use the launch options to run or debug

It is of course possible to build via terminal using make Debug or make Release, then make RunDebug (or RunRelease) to start.

libmpv to be used for testing is included, copy to a place where it can be found by linker

Build simple.c and test it with some movie, it should work and display it using full hardware decoding. OF app will only work with vaapi-copy, and not use full hardware decoding :(

Then... have a look at the openframeworks app and see if you can help :)
