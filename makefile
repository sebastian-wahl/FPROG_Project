all: main test

test: util wordcount

.outputFolder:
	mkdir -p out

.testOutputFolder:
	mkdir -p out/test

FLAGS = -std=c++17 -lstdc++ -Iinclude/ -lm -ltbb

main: .outputFolder
	clang++ ${FLAGS} src/main.cpp -Wall -Wextra -Werror -o out/main

util: .testOutputFolder
	clang++ ${FLAGS} test/util.cpp -Wall -Wextra -Werror -o out/test/util
	./out/test/util

wordcount: .testOutputFolder
	clang++ ${FLAGS} test/wordcount.cpp -Wall -Wextra -Werror -o out/test/wordcount
	./out/test/wordcount
