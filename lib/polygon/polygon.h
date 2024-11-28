#include <vector>
#include <object.h>

struct Polygon : Object
{
    std::vector<std::pair<size_t, size_t>> vertices;

public:
    Polygon(std::initializer_list<std::pair<size_t, size_t>> vertices) : vertices(vertices) {};
    void draw(int *frame, short f_height, short f_width, short pos_x, short pos_y) override
    {

    }
};