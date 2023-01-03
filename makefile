all: main test

.outputFolder:
	mkdir -p out

FLAGS = -std=c++17 -lstdc++ -Iinclude/ -lm -ltbb

main: .outputFolder
	clang++ ${FLAGS} src/main.cpp -Wall -Wextra -Werror -o out/main

test: .outputFolder
	clang++ ${FLAGS} test/test.cpp -Wall -Wextra -Werror -o out/test
	./out/test
