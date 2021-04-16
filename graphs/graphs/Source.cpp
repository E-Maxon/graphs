#include <SFML/Graphics.hpp>
#include "form.h"
#include "graph.h"
#include <iostream>

using namespace sf;
using namespace form;

const double height = 900;
const double width = 1800;
const double outline = 30;

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "graph editor", sf::Style::Default, settings);
    RectangleShape background;
    background.setSize(Vector2f(width, height));
    background.setFillColor(Color(33, 33, 33));
    Input input(2.0 * width / 3.0 + outline, height / 10.0, width / 3.0 - 2.0 * outline, 6.0 * height / 10.0, Color::White, Color::Black, Color(124, 124, 124));
    Button start(2.0 * width / 3.0 + outline, 8.0 * height / 10.0, width / 3.0 - 2.0 * outline, height / 10.0, "Go!", Color(176, 195, 234), Color::Black, Color(124, 124, 124), Color::Green, 30);
    RectangleShape graph_space;
    graph_space.setSize(Vector2f(2.0 * width / 3.0 - outline, 8.0 * height / 10.0));
    graph_space.setPosition(Vector2f(outline, height / 10.0));
    graph_space.setFillColor(Color(204, 204, 204));

    vector<vector<int> > graph;
    vector<int> vertex;

    int n, m;
    cin >> n >> m;

    graph.resize(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        graph[--a].push_back(--b);
    }
    for (int i = 0; i < n; ++i) {
        vertex.push_back(i);
    }

    int cnt = 0;

    vector<pair<double, double> > poly(n);
    build_graph(graph_space.getPosition().x + (graph_space.getSize().x - graph_space.getSize().y) / 2, graph_space.getPosition().y, graph_space.getSize().y, graph_space.getSize().y, graph, poly);

    int vertex_focus = -1;
    Vector2i pos0;
    while (window.isOpen()) {
        if (cnt == 0) {
            start.update();
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                Vector2i mouse = Mouse::getPosition(window); // —читываем координаты мыши(если че обратитьс€ можно будет mouse.x mouse.y)
                input.select(mouse);			//поле ввода
                if (start.select(mouse)) {
                    start.press();
                }
                for (int i = (int)poly.size() - 1; i >= 0; --i) {
                    if (intersect(poly[i], mouse.x, mouse.y)) {
                        vertex_focus = i;
                        pos0 = mouse;
                        break;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                vertex_focus = -1;
            }
            if (event.type == sf::Event::TextEntered) {
                if (input.select()) {
                    input.reText(event.text.unicode);
                }

            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Enter && input.select()) {
                    input.text[input.text.size() - 1].pop_back();
                    input.text.push_back("|");
                    cout << "hello\n";
                }
            }
        }

        if (vertex_focus != -1) {
            Vector2i mouse = Mouse::getPosition(window);
            poly[vertex_focus].first += mouse.x - pos0.x;
            poly[vertex_focus].second += mouse.y - pos0.y;
            pos0 = mouse;
        }

        window.clear();
        window.draw(background);
        window.draw(input.displayButton());
        input.displayText(window);
        window.draw(start.displayButton());
        start.displayText(window);
        window.draw(graph_space);

        draw_graph(poly, graph, window);
        window.display();

        sleep(milliseconds(1000 / 60));
        cnt = (cnt + 1) % 10;
    }
}