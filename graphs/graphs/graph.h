#include <cmath>
#include <unordered_map>
#include <string>

using namespace std;
using namespace sf;

const double PI = 3.14159265;

pair<double, double> rotate(pair<double, double> v, double alpha) {
    alpha = alpha * PI / 180.0; //переводим в радианы
    double x = v.first * cos(alpha) - v.second * sin(alpha);
    double y = v.first * sin(alpha) + v.second * cos(alpha);
    return { x, y };
}

void draw_edge(pair<double, double> v1, pair<double, double> v2, RenderWindow& window) {
    VertexArray line(Lines, 2);
    line[0].position = Vector2f(v1.first, v1.second);
    line[0].color = Color::Black;
    line[1].position = Vector2f(v2.first, v2.second);
    line[1].color = Color::Black;
    window.draw(line);
    return;
}

void draw_vertex(pair<double, double> v, int num, RenderWindow& window) {
    const double radius = 30;
    CircleShape circle(radius);
    circle.setPosition(v.first - radius, v.second - radius);
    circle.setFillColor(Color::White);
    circle.setOutlineColor(Color::Black);
    circle.setOutlineThickness(2);
    window.draw(circle);

    Font font;
    font.loadFromFile("Roboto-Regular.ttf");
    Text text;
    text.setFont(font);
    const int sz = 20;
    text.setCharacterSize(sz);
    text.setFillColor(Color::Black);
    string s = to_string(num + 1);
    text.setString(s);
    text.setPosition(v.first - sz * s.size() / 4.0, v.second - sz / 2.0);
    window.draw(text);

    return;
}

void build_component(int x, int y, int width, int height, RenderWindow& window, vector<int>& vertex, vector<vector<int> >& graph) {
    x = (x + width / 2 / 3);
    y = (y + height / 2 / 3);
    width = width * 4 / 6;
    height = height * 4 / 6;
    int k = vertex.size();

    unordered_map<int, int> num;
    for (int i = 0; i < k; ++i) {
        num[vertex[i]] = i;
    }
    
    vector<pair<double, double> > poly;
    double x0 = (double)x + (double)width / 2.0;
    double y0 = (double)y + (double)height / 2.0;

    double alpha = 360.0 / (double)k;
    double x1 = x0;
    double y1 = y0 - (double)height / 2.0;
    poly.push_back({ x1, y1 });
    pair<double, double> v = { x1 - x0, y1 - y0 };

    for (int i = 1; i < k; ++i) {
        v = rotate(v, alpha);
        poly.push_back({ x0 + v.first, y0 + v.second });
    }

    for (int i = 0; i < k; ++i) {
        for (auto v : graph[vertex[i]]) {
            if (num.find(v) != num.end()) {
                draw_edge(poly[i], poly[num[v]], window);
            }
        }
    }

    for (int i = 0; i < k; ++i) {
        draw_vertex(poly[i], vertex[i], window);
    }

    return;
}