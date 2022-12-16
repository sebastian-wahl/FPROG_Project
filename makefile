all: main test

.outputFolder:
	mkdir -p out

main: .outputFolder
	clang++ -std=c++20 -lstdc++ -lm src/main.cpp -Wall -Wextra -Werror -o out/main
	./out/main

test: .outputFolder
	clang++ -std=c++20 -lstdc++ -lm test/test.cpp -Wall -Wextra -Werror -o out/test
	./out/test
