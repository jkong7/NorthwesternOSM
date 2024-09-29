build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp node.cpp nodes.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp node.cpp nodes.cpp osm.cpp tinyxml2.cpp -Wno-unused-variable -Wno-unused-function
	valgrind --tool=memcheck --leak-check=full ./a.out

clean:
	rm -f ./a.out

submit:
	/home/cs211/f2024/tools/project01 submit *.cpp *.h

commit:
	git add .
	git commit -m "committed changes"
