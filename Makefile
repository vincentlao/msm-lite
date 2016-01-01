.PHONY: clean test example
CXX=clang++

all:
	time $(CXX) test/pt.cpp -I include -I. -std=c++1y -include test/common/test.hpp && ./a.out

test:
	#$(CXX) test/ft.cpp -I include -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors -include test/common/test.hpp && ./a.out
	g++ test/ft.cpp -I include -I. -std=c++1y -include test/common/test.hpp && ./a.out

example:
	$(CXX) example/msm.cpp -I include -I ../boost/libs/di/include/ -I. -std=c++1y -Wall -Wextra -pedantic -fno-exceptions -Werror -pedantic-errors && ./a.out

clean:
	rm -f a.out