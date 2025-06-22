Bowling-Game
This is a simple 2D bowling game simulation written in the C programming language. It allows up to four players to play a virtual bowling game by entering the angle of their bowling throw. The goal is to make the game feel interactive, track scores across frames.
* Players take turns to bowl across 10 frames.
* Each player gets two chances (balls) per frame, unless they get a strike.
* The number of pins knocked down depends on the angle they enter (using trigonometric calculation).
* The game shows a visual of the pins using ASCII art after each roll.
* The program keeps track of each roll and calculates the score, including strikes and spares with bonus points.
* At the end, a scoreboard is displayed showing each player’s frame-by-frame results and total score.

Key Features

* Uses structures to manage data for players, frames, and scores.
* Includes a strike and spare logic similar to real bowling rules.
* Calculates scores dynamically using a bonus score function.
* Adds a bit of realism by using the sine of the angle to decide how many pins fall.
* Displays a nice ASCII bowling lane and pins to make it fun to watch.
