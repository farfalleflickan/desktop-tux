# desktop-tux
 WIP [desktop-goose](https://samperson.itch.io/desktop-goose) clone for X11 (currently moving the rendering to OpenGL, I wanted to use Vulkan but decided to go for OpenGL for backwards compatibility). 
 
 Working on getting all the basic mechanics working. Haven't worked with X11/Xlib and documentation is a bit spotty, but I'm figuring it out.  

A short demo of the current status (as of 5/2/2020) can be seen [here](https://github.com/pastapojken/desktop-tux/blob/screenshots/2020-02-05.mp4). 
The recorded footage is 60fps 4480x1080 and shows off the use of both _.xpm_ files and _.png_ files, although using _.png_ without a compositor has issues because of transparancy (note that _.xpm_ does *not* need a compositor for this, but it still helps with the rendering & overall smoothness). 

Once I feel like the overall structure of the program (especially rendering and input handling) is solid, I will start implementing the "AI" for the Tux/goose/whathaveya.

Current, specific "TO-DO":
1. implement OpenGL rendering (finally found a way to achieve a transparent window, see [here](https://github.com/datenwolf/codesamples/blob/master/samples/OpenGL/x11xcb_opengl/x11xcb_opengl.c))
2. find a way to display at least _.gif_ (maybe _.mp4_ and/or _.webm_ as well? ) so that the Tux can drag a gif onto the user screen
3. Decide how to implement sound capabilities & start working on them
4. ~investigaste multiple screen width/height issue (on multiple monitors the physical max height _seems_ to follow the max height of the screen from which the program is started and I haven't found a way to distinguish between single _physical_ monitors)~ (issue fixed but not committed, since the rendering will be rewritten I'm going to incorporate this in #1)  

Current overall "plan":
* Establish a solid foundation of structures and methods that are _working_ , to build up on
* Customizable characters/animations (both in _at least_ the _.xpm_ and _.png_ file formats)
* Customizable sound effects in both _.mp3_ and _.ogg_
* Different behaviors that are independent from one another so that you can toggle whatever you want
* Customizable typed messages
