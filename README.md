Graphics Assignment 1
=====================

Pacman Killer 2D game in OpenGL 3. More details in the statement [PDF](Assign1.pdf).

## 1. How to run the game

### First compile:

```
cmake CMakeLists.txt
make
```

### Then run the executable:

```2
 ./graphics_assign1
```

## 2. Controls :
* The user can play by keyboard.
* Zooming and done using the mouse.

##### Keyboard Controls

|         KEY |     ACTION |
| ----------: | ---------: |
|    Up arrow |       Jump |
|  Down arrow |    Nothing |
|  Left arrow |  Move left |
| Right arrow | Move right |
|      q or Q |       Quit |
|           w |     Pan up |
|           a |   Pan left |
|           s |  Pand down |
|           d |  Pan right |


##### Mouse Controls
* Scroll up to Zoom in.
* Scroll down to Zoom out.
* Mouse click and drag for panning when screen is zoomed.

## 3. Scoring
-------
- Each enemy ball is given random points on the basis of its color.
- Balls with reflection slab has no points.
- Destroy white balls for bonus points and level boost.

## 4. Rules

* You have a total of 3 lives to compete in the game before the time runs out.
* The game runs for 3 minutes of time and then ends.
* You move on to the next level after scoring 40 points.
* There is a porcupine which appears on level 2 onwards. Touching this porcupine would reduce your life by 1.
* Jumping on crimson colored balls reduce life by 1.
* A magnet randomly arrives in either side of the board level 3 onwards and attracts the ball towards itself when it is in its field of view.
* The trampoline can be used to jump higher and kill the higher placed enemies.
* The player gets reflected by the plank according to the laws of reflection.
* Pond follows the physics of fluid mechanics. Basically movement inside pond is much slower.

## 5. Additional Features

* The game has background audio implemented using threading.
* Zooming and Panning were implemented.
* Scoring scheme was implemented segregating enemies by color scheme.
* Lives were implemented.
* Time remaining is shown along with the lives left.
* Crimson color balls kill life.
* White color balls kill all the enemies present on the screen.

License
-------
The MIT License

Copyright &copy; 2017 Utkarsh <utkarshazad98@gmail.com>

Music Credits: Hans Zimmer (www.hans-zimmer.com/)
