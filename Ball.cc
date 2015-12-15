#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <list>
#include <math.h>   //cos

#include "Displayable.h"
#include "Game.h"

using namespace std;

Ball::Ball(int x, int y, int diameter): 
  x(x), y(y), diameter(diameter), game(NULL)
{
	delt_x = 0;
	delt_y = 0;
	is_launch = false;
}

/*
   Painter
*/
void Ball::paint(XInfo &xinfo) {
	//paint a ball
	XFillArc(
	   xinfo.display, 
	   xinfo.pixmap, 
	   xinfo.gc[3], 
	   x, y, 
	   diameter, diameter, 0, 360*64);
}
/* 
   detects if collision happen 
   0 - no collision, 1 - top, 2 - bottom, 3 - left, 4 - right
*/
int Ball::collide(Displayable *dp) {
   //var for coor after movement
   int after_x = this->x + this->delt_x, after_y = this->y + this->delt_y;
   //var for coor before movement
   int before_x = this->x, before_y = this->y;
   //target properties
   int dp_y = dp->get_y(), dp_height = dp->get_height();
   int dp_x = dp->get_x(), dp_width = dp->get_width();
   
   //check x boundary for horizontal alignment
   if(after_x >= dp_x - this->diameter &&
      after_x <= dp_x + dp_width)
   {
      //cout << "in x range: " << before_y << ", " << after_y << ", " << dp_y << endl;
      if(this->delt_y > 0 &&                          //moving downward
         before_y + this->diameter < dp_y &&          //not touching it
         after_y + this->diameter >= dp_y)             //touches it if move
      {         
         //cout << "top collision" << endl;
         return 1;   //top collision
      }
      if(this->delt_y < 0 &&                          //moving upward
         before_y > dp_y + dp_height &&               //was not touching
         after_y <= dp_y + dp_height) {                //touches if it move
         //cout << "bottom collision" << endl;
         return 2;   //bottom collision
      }
   }
   
   //check y boundary for vertial alignment
   if(after_y >= dp_y - this->diameter &&
      after_y <= dp_y + dp_height) 
   {
      if(this->delt_x > 0 &&                          //moving right
         before_x + this->diameter < dp_x &&          //was not touching
         after_x + this->diameter >= dp_x)             //touches if move
      {
         //cout << "left collision" << endl;
         return 3;   //left collision
      }
      if(this->delt_x < 0 &&                          //moving left
         before_x > dp_x + dp_width &&                //was not touching
         after_x <= dp_x + dp_width)                   //touches if move
      {
         //cout << "right collision" << endl;
         return 4;  //right collision
      }
   } 
   
   return 0;   //no collision
}

/*
   Property getter
*/
int Ball::get_width() {return this->diameter;}
int Ball::get_height() {return this->diameter;}
int Ball::get_x() {return this->x;}
int Ball::get_y() {return this->y;}

/*
   Property setter
*/
void Ball::set_x(int x) {this->x = x;}
void Ball::set_y(int y) {this->y = y;}
void Ball::set_diameter(int diameter) {this->diameter = diameter;}
void Ball::set_delt_x(int delt_x) {
	this->delt_x = delt_x;
}
void Ball::set_delt_y(int delt_y) {
	this->delt_y = delt_y;
}
void Ball::set_game(Game * game) {
  this->game = game;
}

/* 
   Movements
*/

//moves the ball
void Ball::move(XInfo &xinfo, Padel *padel, list<Brick*>* bricks) {
	//check if ball is dead
  if(this->y > xinfo.height) {
    this->game->end_game(xinfo);
    this->is_launch = false;
    return;
  }
	
	//border detection
	if(this->y <= 0) {
		this->delt_y = - this->delt_y;
	}
	if(this->x <= 0 || this->x + this->diameter >= xinfo.width) {
		this->delt_x = - this->delt_x;
	}
  int collision = this->collide(padel);
  //padel collision
  if(collision) {
    switch(this->collide(padel)) {
     //top/bottom collision
     case 1:
     case 2:
        this->delt_y = - this->delt_y;
        break;
     //left/right collision
     case 3:
     case 4:
        this->delt_x = - this->delt_x;
        break;
    }
    //depends on the place collide, change delt_x
    //Get where the ball is relative to padel
    float indent = this->x + this->diameter / 2 - padel->get_x();  
    //scale from -1 to 1
    //f(x) = -cos(x/width * pi) -> assolate between x = 0 ~ width
    double factor = - cos(indent / (float) padel->get_width() * 3.14);  
    this->delt_x = Game::entities_speed * factor;
    return; //brick collision is not possible
  }
	
	
	//bricks detection
	list<Brick *>::iterator begin = bricks->begin();
	list<Brick *>::iterator end = bricks->end();
   while( begin != end ) {
		Brick *b = *begin;
		collision = this->collide(b);
		if(collision) {
		   switch(this->collide(b)) {
	         //top/bottom collision
	         case 1:
	         case 2:
	            this->delt_y = - this->delt_y;
	            break;
	         //left/right collision
	         case 3:
	         case 4:
	            this->delt_x = - this->delt_x;
	            break;
	      }
        Game::score += 20;
        if(b->hit()) {
          begin = bricks->erase(begin);  
          //cout << "removed brick" << endl;
        }
		} else {
		   begin++;
		}
	}
  if(bricks->empty()) {
    game->level_up(xinfo);
  }
  this->x += delt_x;
	this->y += delt_y;

}

void Ball::launch() {
	if(is_launch) return;
	this->delt_y = - Game::entities_speed;
	this->delt_x = - Game::entities_speed / 2;
	is_launch = true;
}
