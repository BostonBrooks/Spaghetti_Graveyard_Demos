 #!/bin/bash
 
while [ 1 == 1 ]
do
 if gcc ./source/main.c\
  ./source/prompt.c\
  ./source/input.c\
  -lcsfml-system -lcsfml-graphics -lcsfml-window -g -lm; 
  then
    echo "Compile Succeeded"
    ./a.out
  else 
    echo "Compile Failed"
    exit
  fi
  
done  
