# PONG! But in C
This is a simple implementation of PONG written in C using [Raylib](https://www.raylib.com/).
<br><br>
It was done as an exercise to see if i could make Pong in one sitting, as well as to get a bit more comfortable with raylib before tackling larger game projects. Altho it was done in two sittings because I got tired.
<br>

To run the game, download raylib, then clone this repo, and run
<br>
```gcc -Wall -Wextra -I/<"path to raylib"> -o main src/*.c  -L<"path to raylib"> -lraylib -lm```<br>
In my own build script I had raylib in the project directory, which is reflected in the build.sh file. 
<br>
It should work on any posix complient system out of the box, on windows you may have to change the path to the sound files.
<hr>
This software is provided as is. Do whatever you want with it, but if something breaks its on you.