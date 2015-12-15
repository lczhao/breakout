BREAKOUT
================================

This game is a classic game reproduced under the X11 lib
Player controls a padel using either keyboard or mouse to bounce off
the ball to hit the brick. 
Each brick hit will award player with 20 score points.
If the ball goes out of the bottom border, player lose a live.
The game ends when the player ran out of live (3 lives in total)

		Compile the game:
		$ make                              # makes the executable
		$ ./breakout [frame_rate] [speed]   # plays the game with specified frame rate
                                            # and speed (default 30, 300 is set)

**Note**: speed > frame rate is required, and it works better when speed is divisable by frame_rate (no rounding required for the program) frame_rate can ranged between 30 ~ 60  

Controls:
-----------
  This game supports both mouse and keyboard  
<table>
  <tr>
    <th>Keyboard</th>
    <th>Mouse</th>
    <th>Meaning</th>
  </tr>
  <tr>
    <td>A</td>
    <td>Move left</td>
    <td>Move Padel left</td>
  </tr>
  <tr>
    <td>D</td>
    <td>Move right</td>
    <td>Move padel right</td>
  </tr>
  <tr>
    <td>Space</td>
    <td>Left click</td>
    <td>Lunch ball</td>
  </tr>
  <tr>
    <td>Q</td>
    <td> - </td>
    <td>Quit Game</td>
  </tr>
</table>

Cheats:
-----------
  1       - level down  
  2       - level up  
  3       - give yourself 100 score  

Features
-----------
  - 5 levels available. Each level is more difficult than the previous
    one. (in terms of block type and layout)
  - 3 Lives in total, represented by 3 little ball on the bottom left of
    the screen
  - 3 block types available
    - Clay brick, easiest to break, takes 1 hit, introduced at level 1
    - Stone brick, harder to break, takes 2 hits, introduced at level 2
    - Gold brick, hardest to break, takes 3 hits, introduced at level 4
  - Depend on where the ball lands on the padel, it changes the direction
    of the ball.
  - Game over panel, if game is ended, a game over panel is displayed with
    the highscore and your score, press space will take you back to the 
    start screen

**Pro tip:**  
  Levels are designed in a way such that if you get your ball to the top of all bricks, it would be easier (especially in level 4 and 5) Use the level design and controllable ball direction to your advantage!  
