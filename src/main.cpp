#include "tgaimage.h"

#include <iostream>
#include "vector.h"
#include "point3d.h"

int main()
{
	Vector2i a(1, 2);

	auto print = [](auto v)
	{
		for (size_t i=0; i < v.size(); ++i)
			std::cout << v[i] << ' ';
		std::cout << '\n';
	};

	print(a);
	Vector2d c = a * 2.5;
	print(c);
	Vector2i b(a);
	print(b);
	std::cout << a * b << '\n';

	return 0;
}