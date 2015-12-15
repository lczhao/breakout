#include <iostream>
#include <list>
#include "Displayable.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

//static variables
int max_level = 5;
int Game::score = 0;
int Game::entities_speed = 0;
int Game::FPS = 0;
int Game::live = 3;

Game::Game(): is_game_ended(true), is_game_over(true), highscore(0), level(1)
{
  this->bricks = new list<Brick*>();
  this->ball = new Ball(0, 0, 0);
  this->ball->set_game(this);
  this->padel = new Padel(0, 0);
}

void Game::placePadel(XInfo& xinfo) {
  
  //set the padel size & position
  int width = xinfo.width, height = xinfo.height;
  int padelWidth = width * 0.25;    //20% of the window width;
  int padelX = xinfo.width / 2 - padelWidth / 2;
  int padelHeight = height * 0.01; //1% of the window height;
  int padelY = xinfo.height - 10 * padelHeight;   //leaves three padel space
  padel->set_x(padelX);
  padel->set_y(padelY);
  padel->set_width(padelWidth);
  padel->set_height(padelHeight);

  //set the ball size & position
  int ballDiameter = padelWidth * 0.15;
  int ballX = padelX + padelWidth / 2 - ballDiameter / 2;
  int ballY = padelY - ballDiameter;
  ball->is_launch = false;
  ball->set_diameter(ballDiameter);
  ball->set_x(ballX);
  ball->set_y(ballY);
  ball->set_delt_x(0);
  ball->set_delt_y(0);

  //set the brick's global width/height
  Brick::width = xinfo.width * 0.10;    
  Brick::height = xinfo.height * 0.03;    
}

void Game::placeBricks(XInfo& xinfo) {
  //add additional block for upper level
  switch(level) {
    case 2:
      bricks->push_front(new Brick(0, 8, 3));
      bricks->push_front(new Brick(9, 8, 3));
      for(int i = 0; i < 10; i ++) {
        bricks->push_front(new Brick(i, 9, 2));
      }
    case 1:
      for(int i = 0; i < 8; i ++) {
        for(int j = 0; j < 5; j ++) {
           //assign the off set to new brick
           bricks->push_front(new Brick(i + 1, j + 4, 2));
        }
      }
      break;
    case 5:
      for(int i = 0; i < 10; i ++) {
        bricks->push_front(new Brick(i, 13, 4));
      }
    case 4:
      for(int i = 0; i < 8; i ++) {
        bricks->push_front(new Brick(i + 1, 12, 4));
      }
      bricks->push_front(new Brick(0, 12, 3));
      bricks->push_front(new Brick(9, 12, 3));
    case 3:
      for(int i = 0; i < 8; i ++) {
        for(int j = 0; j < 3; j ++) {
          if(i % 2 == j % 2) {
            bricks->push_front(new Brick(i + 1, j + 8, 2));  
          } else {
            bricks->push_front(new Brick(i + 1, j + 8, 3));
          }
        }
      }
      for (int i = 0; i < 3; i ++) {
        bricks->push_front(new Brick(i + 1, 4, 2));
        bricks->push_front(new Brick(i + 1, 5, 3));
        bricks->push_front(new Brick(i + 6, 4, 2));
        bricks->push_front(new Brick(i + 6, 5, 3));
      }
      break;

  }
}

void Game::restart() {
  is_game_ended = true;
  is_game_over = true;
}

void Game::start_game(XInfo& xinfo) {
  Game::score = 0;
  Game::live = 3;
  level = 1;
  is_game_ended = false;
  is_game_over = false;
  //clean up bricks
  bricks = new list<Brick*>();
  Game::placePadel(xinfo);
  Game::placeBricks(xinfo);
}

bool Game::game_over() {
  return is_game_over;
}

void Game::end_game(XInfo& xinfo) {
  live -= 1;
  if(live < 0) {
    is_game_over = true;
    if(Game::score > highscore) {
      highscore = Game::score;
    } 
  } else {
    Game::placePadel(xinfo);
  }
  
}

void Game::move(XInfo& xinfo) {
  ball->move(xinfo, padel, bricks);
}

void Game::move_padel(int x, XInfo &xinfo) {
  int origin_x = padel->get_x();
  padel->move_to(x - padel->get_width() / 2, xinfo, ball);

}

void Game::paint(XInfo& xinfo) {
  //paint the singleton objects
  padel->paint(xinfo);
  ball->paint(xinfo);
  
  //draw bricks
  list<Brick *>::const_iterator begin = bricks->begin();
  list<Brick *>::const_iterator end = bricks->end();
  while( begin != end ) {
    Brick *b = *begin;
    b->paint(xinfo);
    begin++;
  }

  //paint little ball represents live
  int x = xinfo.width * 0.1;
  int y = xinfo.height * 0.95;
  int diameter = ball->get_width()*0.6;
  for(int i = 0; i < Game::live; i ++) {
    XFillArc(
     xinfo.display, 
     xinfo.pixmap, 
     xinfo.gc[3], 
     x + i *(diameter *1.5), y, 
     diameter, diameter, 0, 360*64);
  }
}

bool Game::game_ended() {
  return is_game_ended;
}

void Game::handle_key_press(char key,XInfo& xinfo) {
  switch(key) {
    case '1':
      level_down(xinfo);
      break;
    case '2':
      level_up(xinfo);
      break;
    case '3':
      score += 100;
      break;
    case 'A':
    case 'a':
      padel->set_delt_x(- Game::entities_speed);
      padel->move(xinfo, ball);
      break;
    case 'D':
    case 'd':
      padel->set_delt_x(Game::entities_speed);
      padel->move(xinfo, ball);
      break;
    case 'Q':
    case 'q':
      XCloseDisplay(xinfo.display);
      break;
    case ' ':
      ball->launch();
      break;
  }
}

void Game::launch() {
  ball->launch();
}

void Game::level_up(XInfo& xinfo) {
  if(level < max_level) {
    level += 1;
    bricks = new list<Brick*>();
    Game::placePadel(xinfo);
    Game::placeBricks(xinfo);
  } else {
    Game::is_game_over = true;
    if(Game::score > highscore) {
      highscore = Game::score;
    }
  }
  
}
void Game::level_down(XInfo& xinfo) {
  if(level > 1) {
    level -= 1;
    bricks = new list<Brick*>();
    Game::placePadel(xinfo);
    Game::placeBricks(xinfo);
  }
  
}
int Game::get_highscore() {
  return highscore;
}