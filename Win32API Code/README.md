# Win32-API-Code

I’m currently creating my own debugger as well as code for DLL and Code injection, although as of currently due to windows 10, is difficult to test due to conflict of security settings and system architectures the code compiles in. You are free to browse around the code if you wish and see how I wrote it.

For now, please check out the “Opening A Process.cpp” and “OpeningWinHandle.cpp” files which you can run without permission interference, of course in administrative mode. 

I primarily utilize C++ for a lot of Win32 API programs, interacting with the Windows Kernel on a low level. I design programs that utilize: Opening Processes and windows, process enumeration, thread enumeration, DLL and Code injection, as well as debuggers and fuzzers, and hardware interaction. The win32 API is so big and vast there's no worry of running out of programs to write using it. I love API programming. As for C, I use it for basically the same purpose, except I use it to interact with the Linux Kernel on a low level.

You can edit the file path “Opening A Process.cpp”(The line of code showing the calc.exe file path) in order to open a different process. This key function is essentially a module for in a sense DLL and code injections in that the process needs to be open in order to properly enumerate and edit the memory pages of the process. This version of the code will just open the calculator on your system by default.

The “OpeningWinHandle.cpp” program simply opens a window with the handle name “Futaba”. I plan to use this for developing companion type programs of course but in the meantime it interacts with your system to load a dummy window, of course for safety reasons. 
