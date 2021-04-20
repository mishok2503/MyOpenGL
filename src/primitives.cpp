#include "primitives.h"

#include <cmath>

#include "tgaimage.h"
#include "vector.h"

void line(TGAImage& image, Vector2i p1, Vector2i p2, const TGAColor& color)
{
    bool swp_xy = std::abs(p1.x - p2.x) < std::abs(p1.y - p2.y);
    if (swp_xy)
    {
        std::swap(p1.x, p1.y);
        std::swap(p2.x, p2.y);
    }

    if (p2.x < p1.x)
        std::swap(p1, p2);

    int dx = p2.x - p1.x;
    int y = p1.y;
    int de = 2 * std::abs(p1.y - p2.y);
    int e = 0;
    for (int x = p1.x; x <= p2.x; ++x)
    {
        if (swp_xy)
            image.set(y, x, color);
        else
            image.set(x, y, color);

        e += de;

        if (e > dx)
        {
            e -= dx * 2;
            y += p1.y < p2.y ? 1 : -1;
        }
    }
}
