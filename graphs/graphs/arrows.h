#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

void draw_arrow(pair<double, double> v1, pair<double, double> v2, Color clr, RenderWindow& window) {
    VertexArray line(Lines, 2);
    line[0].position = Vector2f(v1.first, v1.second);
    line[0].color = clr;
    line[1].position = Vector2f(v2.first, v2.second);
    line[1].color = clr;
    window.draw(line);
}