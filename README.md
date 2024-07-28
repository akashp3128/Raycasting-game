# RayCasting Game

simple game engine to render a "3d" world and player.

## Installation

clone the repo and open with vscode studio.

## Usage

to compile run 

```g++ -std=c++11 -o raycast main.cpp utility.cpp framebuffer.cpp map.cpp texture.cpp -lm```

```./raycast```

once ppm images are created use ffmpeg to create a video of the ppm images in the folder they are stored in.

```ffmpeg -framerate 60 -start_number 0 -i output/%05d.ppm -c:v libx264 -r 60 -pix_fmt yuv420p output/video.mp4```

## Roadmap

The goal is to create a functional game in which the character can move within the map and interact with walls, doors, and sprites. The project is mainly to understand C++ programming techniques and applying high level concepts to develop a strong foundation in C++

## License

[MIT](https://choosealicense.com/licenses/mit/)
