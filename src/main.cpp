#include "tgaimage.h"

#include <iostream>
#include "point2i.h"

int main()
{
	Point2i a, b(3, 4);

	auto print = [&]()
	{
		std::cout << "a: " << a.x << ' ' << a.y << '\n';
		std::cout << "b: " << b.x << ' ' << b.y << '\n';
	};

	print();

	a = a + b;
	print();

	b = b * 2;
	print();

	std::cout << (a == b) << '\n';

	return 0;
}