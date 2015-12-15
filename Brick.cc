#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>

#include "Displayable.h"

int Brick::width = 0;
int Brick::height = 0;

Brick::Brick(int x_offset, int y_offset, int gc_num):
	x_offset(x_offset), 
	y_offset(y_offset),
	gc_num(gc_num)
{}

void Brick::paint(XInfo &xinfo) {
   int x = x_offset * width;
   int y = y_offset * height;
   int diameter = width / 7;
   
   XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[gc_num], x, y, width, height);
   XDrawRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], x, y, width, height);
   if(gc_num < 4) {
      for(int i = 0; i < 3; i ++) {
      XDrawArc(xinfo.display, xinfo.pixmap, xinfo.gc[0], 
                  x + 2 * i * diameter + diameter, y + height / 2 - diameter / 2, 
                  diameter, diameter, 0, 360*64);
      XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[1], 
                  x + 2 * i * diameter + diameter, y + height / 2 - diameter / 2, 
                  diameter, diameter, 0, 360*64);
   }  
   }
   
}

bool Brick::hit() {
   this->gc_num -= 1;
   if(this->gc_num < 2) {
      return true;
   }
   return false;
}

int Brick::get_x() {
   return x_offset * width;
}

int Brick::get_y() {
   return y_offset * height;
}

int Brick::get_width() {
   return width;
}

int Brick::get_height() {
   return height;
}
