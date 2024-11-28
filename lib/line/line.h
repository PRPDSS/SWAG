#include <vector>
#include <object.h>

class Line : Object
{
    std::vector<std::pair<size_t, size_t>> vertices;

public:
    Line(std::initializer_list<std::pair<size_t, size_t>> vertices) : vertices(vertices) {}
    void draw(int *frame, short f_height, short f_width, short pos_x, short pos_y) override
    {
        for (size_t i = 0; i < vertices.size() - 1; i)
        {
            int x1 = vertices[i].first, y1 = vertices[i].second;
            int x2 = vertices[++i].first, y2 = vertices[i].second;
            int dx = x2 - x1, dy = y2 - y1;
            float slope = dx != 0 ? dy / dx : 1;
            for (int i = 0; i <= dx; i++)
            {
                int x = x1 + i;
                float y = y1 + i * slope;
                float dist = y - int(y);
            }
        }
    }
};
