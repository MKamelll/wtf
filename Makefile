FLAGS= -Wall -Wextra -pedantic

wtf: wtf.cpp
	mkdir -p build
	$(CXX) ${FLAGS} -o build/$@ $^

release: wtf.cpp
	mkdir -p build
	$(CXX) -o2 ${FLAGS} -o build/$@ $^

clean:
	rm -rf build
