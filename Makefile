FLAGS= -Wall -Wextra -pedantic

wtf: wtf.cpp
	mkdir -p build
	$(CXX) ${FLAGS} -o build/$@ $^

clean:
	rm -rf build
