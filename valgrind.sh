EXEC="build/mushroom"

if [ "$1" == "-s"  ]; then
	echo -e "== FULL LOG ==\n"
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes ./$EXEC $2

elif [ "$1" == "-ss"  ]; then
	valgrind --leak-check=full ./build/mushroom 2>&1 | grep -v -e "Memcheck" -e "Command:" -e "Copyright" -e "Using" -e "total heap" -e "ERROR SUMMARY" -e "For lists"

else
	valgrind --leak-check=full ./$EXEC $1
fi
