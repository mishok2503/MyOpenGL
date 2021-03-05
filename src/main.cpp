#include "tgaimage.h"

#include <iostream>
#include "matrix.h"

int main()
{
	Matrix<3, 3, double> a, b({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

	auto print = [](const auto& m) {
		for (size_t i=0; i < m.size().x; ++i)
		{
			for (size_t j=0; j < m.size()[1]; ++j)
				std::cout << m[i][j] << ' ';
			std::cout << '\n';
		}
	};

	print(b);

	a = b * 2;
	print(a);

	std::cout << a.det() << '\n';

	b = a + b;
	print(b);
	
	a = a * b;
	print(a);

	return 0;
}