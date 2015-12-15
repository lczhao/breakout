#ifndef __DISPLAYABLE_H__
#define __DISPLAYABLE_H__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include "Game.h"

using namespace std;
class Game;
struct XInfo {
	Display* display;
	int screen;
	int width;
	int height;
	Pixmap	pixmap;			//double buffering
	Window window;

	GC gc[6];
	XFontStruct * font[6];
};

class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
		
		//property getter for all displayable
		virtual int get_x() = 0;
		virtual int get_y() = 0;
		virtual int get_width() = 0;
		virtual int get_height() = 0;

};
class Padel;
class Brick : public Displayable {
   public:
      //parent methods
      virtual void paint(XInfo &);
     	
     	//global width & height;
     	//all bricks use this to paint
     	static int width;
     	static int height;

      //property getter
      int get_x();
      int get_y();
      int get_width();
      int get_height();

      //movement:
      bool hit();			//return true if brick will break;
      
      //constructor
      Brick(int, int, int);
   private:
   		//brick's cood is constructed using offset * size
      int x_offset;
      int y_offset;
      int gc_num;		//type of the brick(color)
};

class Ball : public Displayable {
	public:
		bool is_launch;
		//inheritive methods
		virtual void paint(XInfo&);
		int collide(Displayable*);
		
		//property getter
		int get_x();
		int get_y();
		int get_width();
		int get_height();
		
		//property setter
		void set_x(int);
		void set_y(int);
		void set_diameter(int);
		void set_delt_x(int);
		void set_delt_y(int);
		void set_game(Game *);
		
		//movement
		void launch();
		void move(XInfo&, Padel*, list<Brick*>*);
		
		//constructor
		Ball(int, int, int);
	private:
		//need to add score to the game, and notify when loses
		Game* game;			
		int x;
		int y;
		int delt_x;
		int delt_y;
		int diameter;
};
class Padel : public Displayable {
	public:
		virtual void paint(XInfo&);
		
		//property getter
		int get_x();
		int get_y();
		int get_width();
		int get_height();
		
		//property setter;
		void set_x(int);
		void set_y(int);
		void set_width(int);
		void set_height(int);
		void set_delt_x(int);
		
		//movement
		void move(XInfo&, Ball*);
		void move_to(int, XInfo&, Ball*);
		//constructor
		Padel(int, int);
	private:
		int x;
		int y;
		int width;
		int height;
		int delt_x;
};



#endif
