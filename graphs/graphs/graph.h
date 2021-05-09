#include <cmath>
#include <unordered_map>
#include <string>
#include "condense.h"
#include "topsort.h"
#include "hamilton.h"

using namespace std;
using namespace sf;

const double PI = 3.14159265;
const double radius = 30;
const double size_of_arrows = 50;
const double angle_of_rot_arrow = 10;

struct Edge {
    int v, u;
    int w;
    Edge() {}
    Edge(int v, int u, int w) : v(v), u(u), w(w) {}
};

pair<double, double> rotate(pair<double, double> v, double alpha) {
    alpha = alpha * PI / 180.0; //переводим в радианы
    double x = v.first * cos(alpha) - v.second * sin(alpha);
    double y = v.first * sin(alpha) + v.second * cos(alpha);
    return { x, y };
}

pair<double, double> plus_pair(pair<double, double> v1, pair<double, double> v2) {
    double x = v1.first + v2.first;
    double y = v1.second + v2.second;
    return { x, y };
}

pair<double, double> minus_pair(pair<double, double> v1, pair<double, double> v2) {
    double x = v1.first - v2.first;
    double y = v1.second - v2.second;
    return { x, y };
}

pair<double, double> norm_for_arrow(pair<double, double> v) {
    double k = sqrt(v.first * v.first + v.second * v.second);
    k = size_of_arrows / k;
    double x = v.first*k;
    double y = v.second*k;
    return { x, y };
}

void draw_line(pair<double, double> v1, pair<double, double> v2, Color clr, RenderWindow& window) {
    VertexArray line(Lines, 2);
    line[0].position = Vector2f(v1.first, v1.second);
    line[0].color = clr;
    line[1].position = Vector2f(v2.first, v2.second);
    line[1].color = clr;
    window.draw(line);
}

template<typename T>
void draw_text(pair<double, double> v, T w, RenderWindow& window) {
    Font font;
    font.loadFromFile("Times-New-Roman.ttf");
    Text TXT;
    TXT.setFont(font);
    TXT.setCharacterSize(20);
    TXT.setFillColor(Color::Black);
    TXT.setPosition(v.first, v.second);
    wstring str = L"√" + to_wstring(w);
    TXT.setString(str);
    window.draw(TXT);
}

void draw_edge(pair<double, double> v1, pair<double, double> v2, int w, RenderWindow& window) {
    draw_line(v1, v2, Color::Black, window);
    pair<double, double> d = minus_pair(v1, v2);
    pair<double, double> arrow1 = plus_pair(norm_for_arrow(rotate(d, angle_of_rot_arrow)), v2);
    pair<double, double> arrow2 = plus_pair(norm_for_arrow(rotate(d, -angle_of_rot_arrow)), v2);
    double k = sqrt(d.first * d.first + d.second * d.second);
    k = radius / k;
    d.first *= k;
    d.second *= k;
    draw_line(plus_pair(v2, d), arrow1, Color::Black, window);
    draw_line(plus_pair(v2, d), arrow2, Color::Black, window);

    pair<double, double> v = { v2.first - v1.first, v2.second - v1.second };
    auto norm = rotate(v, 90);
    norm = norm_for_arrow(norm);
    norm.first /= 2.0;
    norm.second /= 2.0;
    v.first /= 2.0;
    v.second /= 2.0;
    norm.first += v.first;
    norm.second += v.second;
    norm.first += v1.first;
    norm.second += v1.second;
    draw_text(norm, w, window);

    return;
}

void draw_vertex(pair<double, double> v, int num, RenderWindow& window) {
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

bool intersect(pair<double, double> p, double x, double y) {
    double sqdist = (x - p.first) * (x - p.first) + (y - p.second) * (y - p.second);
    return sqdist <= radius * radius;
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

    if (k == 1) {
        poly[vertex[0]] = { x0, y0 };
        return;
    }

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

void draw_graph(vector<pair<double, double> >& poly, vector<vector<int> >& graph, vector<Edge>& edges, RenderWindow& window) {
    for (auto e : edges) {
        draw_edge(poly[e.v], poly[e.u], e.w, window);
    }
    
    for (int i = 0; i < graph.size(); ++i) {
        draw_vertex(poly[i], i, window);
    }
}

void build_graph(int x, int y, int width, int height, vector<vector<int> >& graph, vector<pair<double, double> >& poly) {
    vector<vector<int> > comp = condense(graph);
    sort_by_hamilthonian_path(comp, graph);

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