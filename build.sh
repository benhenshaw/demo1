cc main.c -o demo1 -framework SDL2
if [[ $? -eq 0 ]]
then
    ./demo1
fi
