#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>

#include "Displayable.h"
#include "Game.h"

using namespace std;

Padel::Padel(int x, int y): x(x), y(y), width(0), height(0) {}

/*
   Painter
*/
void Padel::paint(XInfo &xinfo) {
  int widget_width = width * 0.2;
  int bar_width = width * 0.6;
  //blue block at each side
  XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[5], 
                  x, y, 
                  widget_width, widget_width, 0, 180*64);
  XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[5], 
                  x + widget_width + bar_width, y, 
                  widget_width, widget_width, 0, 180*64);

  //main bar
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], 
                  x + widget_width / 2, y, 
                  bar_width + widget_width, widget_width / 2);
  
}

/*
	Property getter
*/
int Padel::get_x() {return this->x;}
int Padel::get_y() {return this->y;}
int Padel::get_width() {return this->width;}
int Padel::get_height() {return this->height;}

/*
	Property setter
*/
void Padel::set_y(int y) {this->y = y;}
void Padel::set_x(int x) {this->x = x;}
void Padel::set_width(int width) {this->width = width;}
void Padel::set_height(int height) {this->height = height;}
void Padel::set_delt_x(int delt_x) {this->delt_x = delt_x;}

/*
   Movements
*/
void Padel::move(XInfo &xinfo, Ball *ball) {
   //moves the padel using delt_x
	if(x + delt_x > 0 && x + delt_x < xinfo.width - width) {
		this->x += this->delt_x;
    //moves the ball around if it's not lunched
    if(! ball->is_launch) {
      ball->set_x(ball->get_x() + this->delt_x);
    }
	}
}

void Padel::move_to(int x, XInfo &xinfo, Ball* ball) {
  if(x > 0 && x < xinfo.width - width) {
    set_x(x);
    if(! ball->is_launch) {
      ball->set_x(this->x + this->width / 2 - ball->get_width() / 2);
    }
  }
}
