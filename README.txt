This project is originally the 2018 winter assignment of problem solving class in Nanjing University. 
It is coded by NSKernel and is licensed under GPL v3.

Project Highlight:
1. Implemented in C++ with optimized logging system.
2. Non-static base directory. You can set the base directory when the server fires up.
3. Support a wide variety of content type including ttf, mp3, etc., thus the example game "THE EVOLUTION OF TRUST" can run with beautiful fonts and sound effects.
4. Industrial-level code format, good for your eyes;-)
5. EASTER EGG: Partly compiled with RFC2324. Try trigger it (x).

Build:
The project is created with Visual Studio 2017 as a Linux project. Click the build tab in VS2017 and click build solution (You have to connect to a real Linux machine or WSL via SSH). You may also write your own makefile and compile manually. 

Usage:
After you start the program, you will be asked to enter the base directory of your website and you are good to go. A "/add" page is hard coded into the server to do basic test. You can access it with the pattern of "/add?xxx=123456&yyy=234567"(key names xxx and yyy can be anything and will be ignored). You may also delete it if you don't need it.