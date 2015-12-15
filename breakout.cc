/*
 * NAME: 
 * USERID: 
 *
 * Fall 2015 CS349 Assignment 1:  An implementation of Breakout in C/C++ and Xlib.
 * 
 * 
 *
 * Commands to compile and run:
 *
 *  g++ -o a1 a1.cpp -L/usr/X11R6/lib -lX11 -lstdc++
 *  ./a1
 *
 * Note: the -L option and -lstdc++ may not be needed on some machines.
 */


#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <unistd.h>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

//user defined library
#include "Displayable.h"
#include "Game.h"

using namespace std;

int FPS = 30;
int Speed = 300;
const int BufferSize = 10;		//size of a char

/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
    cerr << str << endl;
    exit(0);
}

/* 
 * Singleton objects
 */
Game game;
/*
 * Event functions
 */


void paint_text(string s, int offset, XInfo &xinfo, int gc_num = 2) {
  int textWidth, textx, texty;
  textWidth = XTextWidth(xinfo.font[gc_num], s.c_str(), s.length());
  textx = (xinfo.width - textWidth) / 2; 
  texty = xinfo.height / 2 + xinfo.font[gc_num]->ascent * offset;
  XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc[gc_num], textx, texty, s.c_str(), s.length());
}

//draws the welcome page
void paint_welcome(XInfo &xinfo) {
  XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 0, 0, xinfo.width, xinfo.height);
  string s = "Welcome To:";
  string s2 = "BREAKOUT";
  string s3 = "Press Space to Start...";
  string s5 = "Made by: LiangChao Zhao";
  string s6 = "Id: 20464479";
  stringstream ss;
  ss << "highscore: " << game.get_highscore();
  string s4 = ss.str();

  int textx, texty, textWidth;
  //Welcome To:
  textWidth = XTextWidth(xinfo.font[0], s.c_str(), s.length());
  textx = xinfo.width / 2 - textWidth / 2 - 10; 
  texty = xinfo.height / 2 - xinfo.font[0]->ascent;
  XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc[0], textx, texty, s.c_str(), s.length());
  
  //BREAKOUT:
  textWidth = XTextWidth(xinfo.font[0], s2.c_str(), s2.length());
  textx = xinfo.width / 2 - textWidth / 2 + 10; 
  texty = xinfo.height / 2;
  XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc[0], textx, texty, s2.c_str(), s2.length());
  
  //made by:
  paint_text(s5, 2, xinfo);
  //id:
  paint_text(s6, 3, xinfo);
  //highscore:
  paint_text(s4, 5, xinfo);
  //press space to start
  paint_text(s3, 7, xinfo);

  XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
    0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
    0, 0); // position to put top left corner of pixmap in window

}

void paint_gameover(XInfo &xinfo) {
  string s1 = "Game Over";
  stringstream ss;
  ss << "Highscore: " << game.get_highscore();
  string s2 = ss.str();
  stringstream ss2;
  ss2 << "Score: " << Game::score;
  string s3 = ss2.str();
  string s4 = "Press space to restart..";
  paint_text(s1, -2, xinfo, 0);
  paint_text(s3, -1, xinfo, 0);
  paint_text(s2, 1, xinfo);
  paint_text(s4, 3, xinfo);

  XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
    0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
    0, 0); // position to put top left corner of pixmap in window
}

void repaint(XInfo &xinfo) {
  //clearout the background
  XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 0, 0, xinfo.width, xinfo.height);
  
  //game paint entities
  game.paint(xinfo);

  //paint the score
  stringstream ss;
  ss << " Score: " << game.score;
  string s = ss.str();
  int textx, texty;
  textx = 0; 
  texty = xinfo.font[0]->ascent + xinfo.height * 0.01;
  XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc[0], textx,texty, s.c_str(), s.length());
  
  //paint the level
  stringstream ss2;
  ss2 << "FPS: " << FPS << " ";
  s = ss2.str();
  int textWidth = XTextWidth(xinfo.font[0], s.c_str(), s.length());
  textx = xinfo.width  - textWidth;
  XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc[0], textx,texty, s.c_str(), s.length());

  //paint the FPS
  stringstream ss3;
  ss3  << "Level: " << game.level;
  s = ss3.str();
  textWidth = XTextWidth(xinfo.font[0], s.c_str(), s.length());
  textx = xinfo.width / 2  - textWidth / 2;
  XDrawImageString(xinfo.display, xinfo.pixmap, xinfo.gc[0], textx,texty, s.c_str(), s.length());

  XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
    0, 0, xinfo.width, xinfo.height,  // region of pixmap to copy
    0, 0); // position to put top left corner of pixmap in window

  XFlush(xinfo.display);
}

/*
 * Valid keys: A, D, Space
 *  - A: Moves the ball to the left
 *  - D: Moves the ball to the right
 *  - Space: Lunch the ball
 */
void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	//cout << "keycode: " << event.xkey.keycode << endl;
	int i = XLookupString(
			(XKeyEvent *)&event,			//the keyboard event
			text,											//buffer to the text
			BufferSize,								//size
			&key,
			NULL);
	if(i == 1) {
    if(game.game_ended()) {
      switch(text[0]) {
        case 'Q':
        case 'q':
          XCloseDisplay(xinfo.display);
          break;
        case ' ':
          game.start_game(xinfo);
      }
    } 
    else if(game.game_over()) {
      switch(text[0]) {
        case 'Q':
        case 'q':
          XCloseDisplay(xinfo.display);
          break;
        case ' ':
          game.restart();
      }
    }
    else {
      game.handle_key_press(text[0], xinfo);
    }
		
	}
}

void handelMousePress(XInfo &xinfo, XEvent &event) {
  if(game.game_ended()) {
    game.start_game(xinfo);
  } else if(game.game_over()) {
    game.restart();
  } else {
    game.launch();
  }
}

// update width and height when window is resized
void handleResize(XInfo &xinfo, XEvent &event) {
  
  XConfigureEvent xce = event.xconfigure;
  if (xce.width != xinfo.width || xce.height != xinfo.height) {
    xinfo.width = xce.width;
    xinfo.height = xce.height;
    int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
    xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, xinfo.width, xinfo.height, depth);
  }
}

void handleMouseMove(XInfo& xinfo,XEvent& event,bool inside) {
  game.move_padel(event.xbutton.x, xinfo);
}
/*
 the eventloop
 */
void eventloop(XInfo& xinfo) {
	XEvent event;
	
  bool inside = false;
	while(true) {
		while(XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			//handles the event
			switch(event.type) {
				case KeyPress:
					handleKeyPress(xinfo, event);
          break;
        case ConfigureNotify:
          handleResize(xinfo, event);
          break;
        case MotionNotify:
          handleMouseMove(xinfo, event, inside);
          break;
        case ButtonPress:
          handelMousePress(xinfo, event);
          break;
        case EnterNotify:
          inside = true;
          break;
        case LeaveNotify:
          inside = false;
          break;
			}
			
		}
		
    //check the current game status after event handling
    //welcome screen:
    if(game.game_ended()) {
      paint_welcome(xinfo);  
    } 
    //game over (die / finished all level)
    else if(game.game_over()) {
      paint_gameover(xinfo);
    }
    //in game
    else {
      game.move(xinfo);
      repaint(xinfo);
    }
    
		usleep(1000000/FPS);
	}
}

/*
 * Create a window
 */
void initX(int argc, char* argv[], XInfo& xInfo) {


    /*
    * Display opening uses the DISPLAY	environment variable.
    * It can go wrong if DISPLAY isn't set, or you don't have permission.
    */
    xInfo.display = XOpenDisplay( "" );
    if ( !xInfo.display )	{
        error( "Can't open display." );
    }

    /*
    * Find out some things about the display you're using.
    */
    xInfo.screen = DefaultScreen( xInfo.display ); // macro to get default screen index
		xInfo.width = 600;
		xInfo.height = 800;
    unsigned long white, black, grey;
    white = XWhitePixel( xInfo.display, xInfo.screen ); 
    black = XBlackPixel( xInfo.display, xInfo.screen );

    xInfo.window = XCreateSimpleWindow(
       xInfo.display,				// display where window appears
       DefaultRootWindow( xInfo.display ), // window's parent in window tree
       10, 10,			           // upper left corner location
       xInfo.width, xInfo.height,	               // size of the window
       5,						     // width of window's border
       white,						// window border colour
       black );					    // window background colour
		
    //================================
		//creates a graphics context
    //================================
		int i = 0;
    XFontStruct * font;
    Colormap screen_colormap = 
      DefaultColormap(xInfo.display, DefaultScreen(xInfo.display));
		
    xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
    font = XLoadQueryFont(xInfo.display, "*x24");
    xInfo.font[i] = font;
    XSetFont(xInfo.display, xInfo.gc[i], font->fid);
		XSetForeground(xInfo.display, xInfo.gc[i], white);
		XSetBackground(xInfo.display, xInfo.gc[i], black);
		XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
		
		i = 1;
		xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
    xInfo.font[i] = font;
    XSetForeground(xInfo.display, xInfo.gc[i], black);
		XSetBackground(xInfo.display, xInfo.gc[i], white);
		XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
		
		XColor xcolor, exact_color;
		
		i = 2;
		XAllocNamedColor(xInfo.display, screen_colormap, "red", &xcolor, &exact_color);
		xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
    font = XLoadQueryFont(xInfo.display, "fixed");
    xInfo.font[i] = font;
		
    XSetForeground(xInfo.display, xInfo.gc[i], xcolor.pixel);
		XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);

    i = 3;
    XAllocNamedColor(xInfo.display, screen_colormap, "gray", &xcolor, &exact_color);
    xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
    XSetForeground(xInfo.display, xInfo.gc[i], xcolor.pixel);
    XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);

    i = 4;
    XAllocNamedColor(xInfo.display, screen_colormap, "yellow", &xcolor, &exact_color);
    xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
    XSetForeground(xInfo.display, xInfo.gc[i], xcolor.pixel);
    XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);

    i = 5;
    XAllocNamedColor(xInfo.display, screen_colormap, "blue", &xcolor, &exact_color);
    xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
    XSetForeground(xInfo.display, xInfo.gc[i], xcolor.pixel);
    XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
    //================================
    //end of drawing pens.......
    //================================
		
    XSizeHints hints;
    hints.x = 100;
    hints.y = 100;
    hints.width = 400;
    hints.height = 500;
    hints.flags = PPosition | PSize;
    // extra window properties like a window title
    XSetStandardProperties(
        xInfo.display,		// display containing the window
        xInfo.window,		// window whose properties are set
        "x1_openWindow",	// window's title
        "OW",				// icon's title
        None,				// pixmap for the icon
        argv, argc,			// applications command line args
        &hints);			// size hints for the window
    int depth = DefaultDepth(xInfo.display, DefaultScreen(xInfo.display));
    xInfo.pixmap = XCreatePixmap(xInfo.display, xInfo.window, xInfo.width, xInfo.height, depth);
    //xInfo.width = hints.width;
    //xInfo.height = hints.height;

		// selects inputs
		XSelectInput(xInfo.display, xInfo.window,
          KeyPressMask | 
          StructureNotifyMask | 
          ButtonPressMask |
          EnterWindowMask | LeaveWindowMask |
          PointerMotionMask);
    //don't paint the background
    XSetWindowBackgroundPixmap(xInfo.display, xInfo.window, None);
    /*
     * Put the window on the screen.
     */
    XMapRaised( xInfo.display, xInfo.window );

    XFlush(xInfo.display);
    sleep(2);	// let server get set up before sending drawing commands
                // this is a hack until we get to events
}



/*
 *   Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char* argv[] ) {

    XInfo xInfo;

    //reading FPS & speed information
    switch(argc) {
      case 3:
        Speed = atoi(argv[2]);
      case 2:
        FPS = atoi(argv[1]);
        break;
    }
    cout << "FPS: " << FPS << ", speed: " << Speed << endl;
    if(Speed < FPS) {
      cout << "Error: Speed has to be greater than FPS" << endl;
      return 0;
    }
    
    //calculate speed / pixel
    Game::entities_speed = Speed / FPS;

    //start the window
    initX(argc, argv, xInfo);

    //trigger the event loop
		eventloop(xInfo);

    //we should not reach here..
    XCloseDisplay(xInfo.display);
}
