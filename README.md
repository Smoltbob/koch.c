# koch.c
My solution for a school assignement, writing a Koch fractal generator in C.

![triangle](https://raw.github.com/Smoltbob/koch.c/master/examples/carre.gif)
![carre](https://raw.github.com/Smoltbob/koch.c/master/examples/triangle.gif)
![etoile](https://raw.github.com/Smoltbob/koch.c/master/examples/circle.gif)

This code generates images in the ppm format. You can choose the number of sides of the figure, as well as generate enough images to make gifs !
## Getting started

These instructions will let you run the code and generate as many Koch snowflakes as you'd like.

### Prerequisites

Installed on your computer :
```
gcc or clang
```
As well as a way to display ppm images (a web browser or most image viewers will work).

## Compilation

Just type ```make``` in your terminal.

## Running the code

The syntax is the following :
```
./koch <size> <number of sides> <number of iterations> <foreground color> <background color> <file name> [all]
```
* <size> is the size in pixels of the first segment. We deduce the size of the image from it.
* <number of sides> is the number of segments in the polygon (3 = triangle, etc).
* <number of iterations> is the number of times the algorithm will be ran ("depth" of the fractal).
* <foreground color> is the color used for the lines, writen in 0x**RRGGBB hex format.
* <background color> is the color of the background, written in the same format.
* <file name> is the name of the output image.
* [all] : when added, every step of the fractal generation will be saved.

## Authors

* **Jules Simon** - *Initial work* - [Smoltbob](https://github.com/Smoltbob)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Ensimag school for giving the assignement.

