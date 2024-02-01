#!/bin/bash

EXEC="mushroom"
BLUE="\033[1;34m"
RESET="\033[1;0m"

OPT="$1"
FILE="$2"

function color() {
	echo -e "${BLUE} ==== $1 ==== ${RESET}"
}

color "COMPILING"


if [ $OPT == "-c" ]; then
	cmake --build build/
	EXEC="build/mushroom"

elif [ $OPT == "-d" ]; then
	cmake --build build/

	gdb "build/$EXEC"
	exit

elif [ $OPT == "-v" ]; then
	cmake --build build/

	valgrind --leak-check=full  "build/$EXEC" $FILE
	exit

else
	echo "Invalid option!"
	exit
fi


color "RUNNING"

# Command line or file
./$EXEC $FILE

if [ $OPT != "-c" ]; then
	make clean
fi
