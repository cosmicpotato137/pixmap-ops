# canvas-drawer

Implements a simple drawing api

TODO: Add a representative image for your project here

## How to build

*Windows*

Open git bash to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake -G "Visual Studio 16 2019" ..
canvas-drawer/build $ start Draw-2D.sln
```

Your solution file should contain two projects: `pixmap_art` and `pixmap_test`.
To run from the git bash command shell, 

```
canvas-drawer/build $ ../bin/Debug/draw_test
canvas-drawer/build $ ../bin/Debug/draw_art
```

*macOS*

Open terminal to the directory containing this repository.

```
canvas-drawer $ mkdir build
canvas-drawer $ cd build
canvas-drawer/build $ cmake ..
canvas-drawer/build $ make
```

To run each program from build, you would type

```
canvas-drawer/build $ ../bin/draw_test
canvas-drawer/build $ ../bin/draw_art
```

# pixmap-ops

I implemented a sobel and a general kernel operation that let me play with filter matrices of different sizes. 

- Emboss Filter
 
![alt text](https://github.com/msunde137/pixmap-ops/blob/master/building(emboss).png)

- Sharpen Filter

![alt text](https://github.com/msunde137/pixmap-ops/blob/master/building(sharpen).png)

- Outline filter + blur + sobel

![alt text](https://github.com/msunde137/pixmap-ops/blob/master/building(ourline%2Bedge_detect).png)

I also implemented vertical flip, and channel filters.

![alt text](https://github.com/msunde137/pixmap-ops/blob/master/building_red.png)
