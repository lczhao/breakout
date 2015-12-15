#ifndef __GAME_H__
#define __GAME_H__
#include <iostream>
#include <list>
#include "Displayable.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

class Ball;
class Brick;
class Padel;
struct XInfo;

class Game {
    private:
      //game entities
      Ball* ball;
      list<Brick*>* bricks;
      Padel* padel;

      //game state:
      bool is_game_ended;                 //we are in welcome screen
      bool is_game_over;                  //we are in gameover screen

      //highscore
      int highscore;

      //game initializer:
      //those two methods places game entities depends on the size
      //of the window
      void placePadel(XInfo&);            //places padel & ball
      void placeBricks(XInfo&);           //places brick depends on the level
    public:
      static int score;                   //the global score
      static int entities_speed;          //speed at which all things moves at
      static int FPS;                     //FPS (unused)
      static int live;

      int level;                          //current level

      //game state getter
      bool game_ended();                  //return is game ended
      bool game_over();                   //return is game over

      //game state changer
      void restart();                     //go to welcome screen
      void end_game(XInfo& xinfo);                    //go to welcome screen
      void start_game(XInfo&);            //starts the game

      //level changer
      void level_up(XInfo&);              //up 1 level
      void level_down(XInfo&);            //down 1 level

      //game machnism
      int get_highscore();                //retrieve the highscore
      void paint(XInfo&);                 //paints game entities
      void handle_key_press(char, XInfo&);//handels all key presses

      //game movement / event cycle
      void move_padel(int, XInfo&);       //moves padel quickly
      void launch();                      //set the ball going
      void move(XInfo&);                  //movement per frame
      
      //constructors
      Game();
};
#endif
