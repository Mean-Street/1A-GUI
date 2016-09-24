
C Project - Graphics Library
============================

Documentation
-------------
* Generate the documentation :
	make doc
* Read the documentation :
Latex
    cd docs/latex/
    make
    evince refman.pdf 
Internet
    firefox docs/index.html


Notes
-----
	===== WARNING =====
When a toplevel is declared, you have to call toplevel-configure BEFORE adding sons to this toplevel.
Because the toplevel-configure function is charged to create the close-button, the resize-button and the central frame.
A widget in a toplevel will actually be a son of the central frame : so we cannot declare a son of the toplevel before having created the central frame.

Note : That's why we brought a little modification to the test file "hello-world.c". We call widget-create for the button
after the frame-configure call.

	===== WARNING =====
Invalid rects doesn't work. I tried to create a list of ei-rect-t, and to redraw only in these rectangles.
The code is still in ei-global.c, but not called. It seems that the computations are more time-expensive than the drawing, 
so it slows the programm a lot. In this version, we always redraw the whole root-window (clipper is always NULL).


Known problems
--------------
* when an image is in a button, which is in a toplevel
When we move the toplevel out of bounds, there's a lag and a Warning : "Attempt to copy surfaces with different ROI sizes."
It's probably because we don't clip the image.

* the two048 test doesn't work well, but we hadn't time to work on it.


Add a widgetclass or a geometry-manager
---------------------------------------
*TODO List* => add your widgetclass to the project :
* Create the .c and .h files
* Declare a structure for this widgetclass, which contains an ei-widget-t as first element (for polymorphism)
* Implement the different functions (Required : alloc, release, draw, set-defaults, geomnotify)
* Link them to the structure in a register-class function (write it in widgetclass.c)
* Call this function in the register-elements function (in ei-application.c)

*TODO List* => add your geometry-manager to the project :
* Create the .c and .h files
* Declare a structure for this geometry-manager, which contains a manager's pointer as first element (for polymorphism)
* Implement the different functions (Required : run, release)
* In geometrymanager.c, write the function that will be called by the user of the library
*						write a register function
* Call this function in the register-elements function (in ei-application.c)
