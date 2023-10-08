 #!/bin/bash
 
BGreen='\033[1;32m' 
Color_Off='\033[0m'
 
#while [ 1 == 1 ]
#do

 echo -e "Look, a Tortoise:\n${BGreen}\n       _____     ____\n     /      \  |  o |\n    |        |/ ___\| \n    |_________/     \n    |_|_| |_|_|\n//////////////////////////////////////////////////////////////${Color_Off}\n\nAttempting to compile:\n"

 if gcc\
  -D _WIN32\
  -I /mingw64/include\
  -L /mingw64/lib\
  10_Main.c\
  -lcsfml-system -lcsfml-graphics -lcsfml-window -g -lm;
  then
    echo "Compile Succeeded:"
    ./a.exe
  else 
    echo "Compile Failed!"
 #   exit
  fi
  
#done
