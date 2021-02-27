#include "tgaimage.h"

#include <iostream>
#include "point3d.h"

int main()
{
	Point3d a, b(3, 4, 5.5);

	auto print = [&]()
	{
		std::cout << "a: " << a.x << ' ' << a.y << ' ' << a.z << '\n';
		std::cout << "b: " << b.x << ' ' << b.y << ' ' << b.z << '\n';
	};

	print();

	a = a + b;
	print();

	b = b * 2;
	print();

	std::cout << (a == b) << '\n';

	return 0;
}