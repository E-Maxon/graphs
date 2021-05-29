#include <SFML/Graphics.hpp>
#include "form.h"
#include "graph.h"
#include "plot.h"
#include <iostream>
#include <ctime>

using namespace sf;
using namespace form;

const double height = 900;
const double width = 1800;
const double outline = 30;

void update_graph(int& n, vector<Edge>& edges, vector<vector<int> >& graph) {
    graph.clear();

    n = 0;

    for (auto p : edges) {
        n = max(n, max(p.v, p.u));
    }

    graph.resize(n + 1);
    
    for (auto edge : edges) {
        graph[edge.v].push_back(edge.u);
    }
}

void update_edges(vector<Edge>& edges, vector<string> text) {
    edges.clear();
    for (auto s : text) {
        if (s == "|") continue;
        vector<int> data(3);
        int mode = 0;
        char prev = '-';
        for (auto c : s) {
            if (isdigit(c)) {
                data[mode] = data[mode] * 10 + (c - '0');
            }
            else if (isspace(c) && !isspace(prev)) {
                if (mode == 2) break;
                ++mode;
            }
            prev = c;
        }
        edges.push_back({ data[0] - 1, data[1] - 1, data[2] });
    }
}

bool input_mode(RenderWindow& window, Button& start, bool& edge_closed, vector<Edge>& edges, Input& input,
    vector<pair<double, double> >& poly, int& vertex_focus, Vector2i& pos0, RectangleShape& graph_space,
    vector<vector<int> >& graph, Vector2i& shift, int& start_edge, int& n, RectangleShape& background,
    Graph<double>& g) {

    while (window.isOpen()) {
        window.clear();
        window.draw(background);

        sf::Event event;
        while (window.pollEvent(event)) {

            start.update(Mouse::getPosition(window));

            if (edge_closed) {
                if (event.type == sf::Event::TextEntered) {
                    char digit = event.text.unicode;
                    if (isdigit(digit)) {
                        edges.back().w = edges.back().w * 10 + (digit - '0');
                    }

                }
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    if (event.key.code == sf::Keyboard::Enter) {
                        edge_closed = false;
                        input.text.back() += to_string(edges.back().v + 1) + " " +
                            to_string(edges.back().u + 1) + " " +
                            to_string(edges.back().w);
                        input.text.push_back("");
                    }
                    if (event.key.code == sf::Keyboard::BackSpace) {
                        edges.back().w /= 10;
                    }
                }
                continue;
            }

            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                Vector2i mouse = Mouse::getPosition(window); // Считываем координаты мыши(если че обратиться можно будет mouse.x mouse.y)
                input.select(mouse);			//поле ввода
                if (start.select(mouse)) {
                    start.press();
                    g = Graph<double>(graph.size(), edges.size(), 0);
                    for (int i = 0; i < edges.size(); i++) {
                        g.addEdge(edges[i].v, edges[i].u, edges[i].w, i);
                    }
                    g.precalcPoints(100);
                    return true;
                }
                for (int i = (int)poly.size() - 1; i >= 0; --i) {
                    if (intersect(poly[i], mouse.x, mouse.y)) {
                        vertex_focus = i;
                        pos0 = mouse;
                        break;
                    }
                }
                if (vertex_focus == -1 &&
                    !(mouse.x < graph_space.getPosition().x + radius ||
                        mouse.x > graph_space.getPosition().x + graph_space.getSize().x - radius ||
                        mouse.y < graph_space.getPosition().y + radius ||
                        mouse.y > graph_space.getPosition().y + graph_space.getSize().y - radius)) {
                    poly.push_back({ mouse.x, mouse.y });
                    graph.resize(graph.size() + 1);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (vertex_focus != -1 && shift == Vector2i({ 0, 0 }) && start_edge == -1) {
                    start_edge = vertex_focus;
                }
                else if (start_edge != -1) {
                    if (vertex_focus != -1) {
                        edges.push_back({ start_edge, vertex_focus, 0 });
                    }
                    else {
                        edges.push_back({ start_edge, (int)graph.size() - 1, 0 });
                    }
                    start_edge = -1;
                    edge_closed = true;
                }

                vertex_focus = -1;
                shift = Vector2i({ 0, 0 });
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                if (start_edge != -1) {
                    start_edge = -1;
                }
            }
            if (event.type == sf::Event::TextEntered) {
                if (input.select()) {
                    int sz = input.text.size();
                    input.reText(event.text.unicode);
                    if (input.text.size() != sz || (input.text.size() == 1 && input.text[0].size() == 1)) {
                        update_edges(edges, input.text);
                        update_graph(n, edges, graph);
                        build_graph(graph_space.getPosition().x + (graph_space.getSize().x - graph_space.getSize().y) / 2, graph_space.getPosition().y, graph_space.getSize().y, graph_space.getSize().y, graph, poly);
                    }
                }

            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return false;
                }
                if (event.key.code == sf::Keyboard::Enter && input.select()) {
                    input.text[input.text.size() - 1].pop_back();
                    input.text.push_back("|");
                    update_edges(edges, input.text);
                    update_graph(n, edges, graph);
                    build_graph(graph_space.getPosition().x + (graph_space.getSize().x - graph_space.getSize().y) / 2, graph_space.getPosition().y, graph_space.getSize().y, graph_space.getSize().y, graph, poly);
                }
            }
        }

        if (vertex_focus != -1) {
            Vector2i mouse = Mouse::getPosition(window);
            if (mouse.x < graph_space.getPosition().x + radius ||
                mouse.x > graph_space.getPosition().x + graph_space.getSize().x - radius ||
                mouse.y < graph_space.getPosition().y + radius ||
                mouse.y > graph_space.getPosition().y + graph_space.getSize().y - radius) {
                return false;
            }
            poly[vertex_focus].first += mouse.x - pos0.x;
            poly[vertex_focus].second += mouse.y - pos0.y;
            shift.x += mouse.x - pos0.x;
            shift.y += mouse.y - pos0.y;
            pos0 = mouse;
        }

        window.draw(input.displayButton());
        input.displayText(window);
        window.draw(start.displayButton());
        start.displayText(window);
        window.draw(graph_space);

        if (start_edge != -1) {
            Vector2i mouse = Mouse::getPosition(window);
            draw_edge(poly[start_edge], { mouse.x, mouse.y }, 0, 0, 0, window);
        }
        draw_graph(poly, graph, edges, edge_closed, window, 0);

        window.display();
    }

    return false;
}

int moving_points_mode(RenderWindow& window, Button& start, vector<Edge>& edges,
    vector<pair<double, double> >& poly, int& vertex_focus, Vector2i& pos0, RectangleShape& graph_space,
    vector<vector<int> >& graph, Vector2i& shift, int& n, RectangleShape& background,
    Graph<double>& g) {
    
    Button stop(2.0 * width / 3.0 + outline, 8.0 * height / 10.0, width / 3.0 - 2.0 * outline, height / 10.0, "Stop", Color(176, 195, 234), Color::Black, Color(124, 124, 124), Color(102, 235, 85), Color(113, 176, 240), 30);

    bool moving_points = false;
    int t = 0;
    int prep = 100;
    Button view_plot(2.0 * width / 3.0 + outline, 6.0 * height / 10.0, width / 3.0 - 2.0 * outline, height / 10.0, "View plot", Color(176, 195, 234), Color::Black, Color(124, 124, 124), Color(102, 235, 85), Color(113, 176, 240), 30);

    while (window.isOpen()) {
        stop.update(Mouse::getPosition(window));
        view_plot.update(Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 1;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                Vector2i mouse = Mouse::getPosition(window); 
                if (stop.select(mouse)) {
                    stop.press();
                    return 0;
                }
                if (view_plot.select(mouse)) {
                    view_plot.press();
                    return 2;
                }
                for (int i = (int)poly.size() - 1; i >= 0; --i) {
                    if (intersect(poly[i], mouse.x, mouse.y)) {
                        vertex_focus = i;
                        pos0 = mouse;
                        break;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                vertex_focus = -1;
                shift = Vector2i({ 0, 0 });
            }
            if (event.type == sf::Event::KeyPressed && !moving_points) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                    return 1;
                }
            }
        }

        if (vertex_focus != -1) {
            Vector2i mouse = Mouse::getPosition(window);
            if (mouse.x < graph_space.getPosition().x + radius ||
                mouse.x > graph_space.getPosition().x + graph_space.getSize().x - radius ||
                mouse.y < graph_space.getPosition().y + radius ||
                mouse.y > graph_space.getPosition().y + graph_space.getSize().y - radius) {
                continue;
            }
            poly[vertex_focus].first += mouse.x - pos0.x;
            poly[vertex_focus].second += mouse.y - pos0.y;
            shift.x += mouse.x - pos0.x;
            shift.y += mouse.y - pos0.y;
            pos0 = mouse;
        }

        window.clear();
        window.draw(background);
        window.draw(stop.displayButton());
        window.draw(view_plot.displayButton());
        view_plot.displayText(window);
        stop.displayText(window);
        window.draw(graph_space);

        if (t == prep) {
            prep += 100;
            g.precalcPoints(prep);
        }
        PointsSet points = g.getPoints(t);

        draw_graph(poly, graph, edges, 0, window, 1, points, t);
        
        t++;

        sleep(milliseconds(50));

        window.display();
    }
    return 1;
}

bool plot_mode(RenderWindow& window, RectangleShape& background, Graph<double>& graph_points,
    vector<Edge>& edges, vector<vector<int> >& graph) {
    RectangleShape plot_space;
    plot_space.setSize(Vector2f(width - 2 * outline, height - 2 * outline));
    plot_space.setPosition(Vector2f(outline, outline));
    plot_space.setFillColor(Color::White/*Color(204, 204, 204)*/);

    auto time0 = clock();

    graph_points.precalc(300);

    while (window.isOpen()) {
        window.clear();
        window.draw(background);

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    return true;
                }
            }
        }

        window.draw(plot_space);
        //draw_edge({ outline * 2, height - 2 * outline }, { outline * 2, outline * 2 }, 0, true, false, window);
        //draw_edge({ outline * 2, height - 2 * outline }, { width - outline * 2, height - outline * 2 }, 0, true, false, window);
        draw_plot((clock() - time0) / CLK_TCK, graph_points, outline * 2,
            height - 3 * outline, width - 4 * outline, height - 5 * outline, window, edges.size() - graph.size());
        window.display();
    }
    return false;
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;
    sf::RenderWindow window(sf::VideoMode(width, height), "graph editor", sf::Style::Default, settings);
    RectangleShape background;
    background.setSize(Vector2f(width, height));
    background.setFillColor(Color(33, 33, 33));
    Input input(2.0 * width / 3.0 + outline, height / 10.0, width / 3.0 - 2.0 * outline, 6.0 * height / 10.0, Color::White, Color::Black, Color(124, 124, 124));
    Button start(2.0 * width / 3.0 + outline, 8.0 * height / 10.0, width / 3.0 - 2.0 * outline, height / 10.0, "Go!", Color(176, 195, 234), Color::Black, Color(124, 124, 124), Color(102, 235, 85), Color(113, 176, 240), 30);
    RectangleShape graph_space;
    graph_space.setSize(Vector2f(2.0 * width / 3.0 - outline, 8.0 * height / 10.0));
    graph_space.setPosition(Vector2f(outline, height / 10.0));
    graph_space.setFillColor(Color(204, 204, 204));

    vector<vector<int> > graph;
    vector<Edge> edges;

    int n;

    vector<pair<double, double> > poly;
    //build_graph(graph_space.getPosition().x + (graph_space.getSize().x - graph_space.getSize().y) / 2, graph_space.getPosition().y, graph_space.getSize().y, graph_space.getSize().y, graph, poly);

    int vertex_focus = -1;
    Vector2i pos0;
    Vector2i shift = Vector2i({ 0, 0 });
    int start_edge = -1;

    bool edge_closed = false;

    int mode = 0;

    Graph<double> g;
    // (u, v, edgeWeight^2 (prime number), edgeNum \in \{0, \cdots, m - 1\})

    while (window.isOpen()) {
        if (mode == 0) {
            if (input_mode(window, start, edge_closed, edges, input, poly,
                vertex_focus, pos0, graph_space, graph, shift, start_edge, n, background, g)) {
                mode = 1;
            }
        }
        else if (mode == 1) {
            mode = moving_points_mode(window, start, edges, poly, vertex_focus, pos0, graph_space, graph, shift, n, background, g);
        }
        else {
            if (plot_mode(window, background, g, edges, graph)) {
                mode = 1;
            }
        }
        
        window.display();

        //sleep(milliseconds(1000 / 60));
    }
}