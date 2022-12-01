 #!/bin/bash
if gcc ./source/main.c\
 ./source/sphere.c\
 ./source/interpolation.c\
 ./source/media.c\
 ./source/window.c\
 ./source/viewport.c\
 ./source/geometry.c\
 ./source/groundsurface.c\
 ./source/drawables.c\
 ./source/drawfunctions.c\
  -lcsfml-system -lcsfml-graphics -lcsfml-window -g -lm;  then
./a.out
fi
