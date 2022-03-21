FLAGS= -Wall -Wextra -pedantic
wtf: wtf.c
	mkdir -p build
	$(CC) ${FLAGS} -o build/$@ $^

wtfcpp: wtf.cpp
	mkdir -p build
	$(CXX) ${FLAGS} -o build/$@ $^

clean:
	rm -rf build
