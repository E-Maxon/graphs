#include <SFML/Graphics.hpp>
#include "form.h"
#include "graph.h"
#include <iostream>

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

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;
    sf::RenderWindow window(sf::VideoMode(width, height), "graph editor", sf::Style::Default, settings);
    RectangleShape background;
    background.setSize(Vector2f(width, height));
    background.setFillColor(Color(33, 33, 33));
    Input input(2.0 * width / 3.0 + outline, height / 10.0, width / 3.0 - 2.0 * outline, 6.0 * height / 10.0, Color::White, Color::Black, Color(124, 124, 124));
    Button start(2.0 * width / 3.0 + outline, 8.0 * height / 10.0, width / 3.0 - 2.0 * outline, height / 10.0, "Go!", Color(176, 195, 234), Color::Black, Color(124, 124, 124), Color(68, 193, 94), Color(126, 197, 215), 30);
    RectangleShape graph_space;
    graph_space.setSize(Vector2f(2.0 * width / 3.0 - outline, 8.0 * height / 10.0));
    graph_space.setPosition(Vector2f(outline, height / 10.0));
    graph_space.setFillColor(Color(204, 204, 204));

    vector<vector<int> > graph;
    vector<Edge> edges;

    int n;

    int cnt = 0;

    vector<pair<double, double> > poly;
    //build_graph(graph_space.getPosition().x + (graph_space.getSize().x - graph_space.getSize().y) / 2, graph_space.getPosition().y, graph_space.getSize().y, graph_space.getSize().y, graph, poly);

    int vertex_focus = -1;
    Vector2i pos0;
    Vector2i shift = Vector2i({ 0, 0 });
    int start_edge = -1;

    bool edge_closed = false;

    bool moving_points = false;
    int t = 0;
    int prep = 100;
    Graph<double> g;

    while (window.isOpen()) {
        start.update(Mouse::getPosition(window));
        sf::Event event;
        while (window.pollEvent(event)) {   

            if (edge_closed && !moving_points) {
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
                return 0;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                Vector2i mouse = Mouse::getPosition(window); // —читываем координаты мыши(если че обратитьс€ можно будет mouse.x mouse.y)
                input.select(mouse);			//поле ввода
                if (start.select(mouse)) {
                    start.press();
                    if (!moving_points) {
                        moving_points = true;
                        g = Graph<double>(graph.size(), edges.size(), 0);
                        for (int i = 0; i < edges.size(); i++)
                            g.addEdge(edges[i].v, edges[i].u, edges[i].w, i);
                        g.precalcPoints(prep);
                    }
                    else {
                        moving_points = false;
                    }
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
                    mouse.y > graph_space.getPosition().y + graph_space.getSize().y - radius) && !moving_points) {
                    poly.push_back({ mouse.x, mouse.y });
                    graph.resize(graph.size() + 1);
                }
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                if (vertex_focus != -1 &&  shift == Vector2i({ 0, 0 }) && start_edge == -1) {
                    start_edge = vertex_focus;
                }
                else if (start_edge != -1 && !moving_points) {
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
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && !moving_points) {
                if (start_edge != -1) {
                    start_edge = -1;
                }
            }
            if (event.type == sf::Event::TextEntered && !moving_points) {
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
            if (event.type == sf::Event::KeyPressed && !moving_points) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Enter && input.select() && !moving_points) {
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
        window.draw(input.displayButton());
        input.displayText(window);
        window.draw(start.displayButton());
        start.displayText(window);
        window.draw(graph_space);

        if (start_edge != -1 && !moving_points) {
            Vector2i mouse = Mouse::getPosition(window);
            draw_edge(poly[start_edge], { mouse.x, mouse.y }, 0, 0, 0, window);
        }
        draw_graph(poly, graph, edges, edge_closed, window);

        if (moving_points) {
            if (t == prep) {
                prep += 100;
                g.precalcPoints(prep);
            }
            PointsSet points = g.getPoints(t);
            draw_all_points(poly, graph, edges, points, t, window);
            t++;
            sleep(milliseconds(50));
        }
        window.display();
    }
}