all: main test

.outputFolder:
	mkdir -p out

main: .outputFolder
	clang++ -std=c++17 -lstdc++ -Iinclude/ -lm src/main.cpp -Wall -Wextra -Werror -o out/main

test: .outputFolder
	clang++ -std=c++17 -lstdc++ -Iinclude/ -lm test/test.cpp -Wall -Wextra -Werror -o out/test
	./out/test
