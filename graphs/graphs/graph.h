#include <cmath>
#include <unordered_map>
#include <string>
#include "condense.h"
#include "topsort.h"
#include "arrows.h"

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
    draw_arrow(v1, v2, Color::Black, window);
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

void build_component(int x, int y, int width, int height, vector<int>& vertex, vector<pair<double, double> >& poly) {
    x = (x + width / 2 / 3);
    y = (y + height / 2 / 3);
    width = width * 4 / 6;
    height = height * 4 / 6;
    int k = vertex.size();

    unordered_map<int, int> num;
    for (int i = 0; i < k; ++i) {
        num[vertex[i]] = i;
    }

    double x0 = (double)x + (double)width / 2.0;
    double y0 = (double)y + (double)height / 2.0;

    double alpha = 360.0 / (double)k;
    double x1 = x0;
    double y1 = y0 - (double)height / 2.0;
    poly[vertex[0]] = { x1, y1 };
    pair<double, double> v = { x1 - x0, y1 - y0 };

    for (int i = 1; i < k; ++i) {
        v = rotate(v, alpha);
        poly[vertex[i]] = { x0 + v.first, y0 + v.second };
    }

    return;
}

void draw_graph(vector<pair<double, double> >& poly, vector<vector<int> >& graph, RenderWindow& window) {
    for (int i = 0; i < graph.size(); ++i) {
        for (auto v : graph[i]) {
            draw_edge(poly[i], poly[v], window);
        }
    }

    for (int i = 0; i < graph.size(); ++i) {
        draw_vertex(poly[i], i, window);
    }
}

void build_graph(int x, int y, int width, int height, vector<vector<int> >& graph, vector<pair<double, double> >& poly) {
    vector<vector<int> > comp = condense(graph);

    for (auto i : comp) {
        for (auto j : i) {
            cout << j + 1 << " ";
        }
        cout << '\n';
    }

    vector<int> num = comps;
    vector<vector<int> > levels = topsort(comp, graph, num);

    int w = width / maxcnt;
    int h = height / (int)levels.size();

    for (int i = 0; i < levels.size(); ++i) {
        int x0 = x + w * (maxcnt - levels[i].size()) / 2;
        for (auto j : levels[i]) {
            build_component(x0, y, w, h, comp[j], poly);
            x0 += w;
        }
        y += h;
    }
}