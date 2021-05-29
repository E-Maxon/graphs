//#include "points_on_edges.h"

void draw_plot_point(double x, double y, RenderWindow& window) {
    CircleShape p;
    p.setPosition(x - 4, y - 4);
    p.setRadius(4);
    p.setFillColor(Color::Blue);
    window.draw(p);
}

void draw_description(pair<double, double> v, double w, bool OX, RenderWindow& window) {
    Font font;
    font.loadFromFile("Roboto-Regular.ttf");
    Text TXT;
    TXT.setFont(font);
    TXT.setCharacterSize(20);
    TXT.setPosition(v.first, v.second);
    TXT.setFillColor(Color::Black);
    wstring str = to_wstring(w);
    if (OX) {
        str = to_wstring((int)w);
    }
    TXT.setString(str);
    window.draw(TXT);
}

void draw_axis(pair<double, double> v1, pair<double, double> v2, int cnt, double start, double finish, bool OX, RenderWindow& window) {
    draw_line(v1, v2, Color::Black, window);
    pair<double, double> d = minus_pair(v1, v2);
    pair<double, double> arrow1 = plus_pair(norm_for_arrow(rotate(d, angle_of_rot_arrow)), v2);
    pair<double, double> arrow2 = plus_pair(norm_for_arrow(rotate(d, -angle_of_rot_arrow)), v2);
    double k = sqrt(d.first * d.first + d.second * d.second);
    k = 1 / k;
    d.first *= k;
    d.second *= k;
    draw_line(plus_pair(v2, d), arrow1, Color::Black, window);
    draw_line(plus_pair(v2, d), arrow2, Color::Black, window);

    double step = (finish - start) / cnt;
    double step_koord = (v2.first - v1.first) / (double)cnt;
    double q = start;
    double koord = v1.first;

    if (!OX) {
        koord = v1.second;
        step_koord = (v2.second - v1.second) / (double)cnt;
    }

    for (int i = 0; i <= cnt; ++i) {
        pair<double, double> v = { koord - v1.first, 0};

        if (!OX) {
            v = { 0, koord - v1.second };
        }

        auto norm = rotate(v, 90);
        norm = norm_for_arrow(norm);
        norm.first /= 2.0;
        norm.second /= 2.0;
        //v.first /= 2.0;
        //v.second /= 2.0;
        norm.first += v.first;
        norm.second += v.second;
        norm.first += v1.first;
        norm.second += v1.second;
        draw_description(norm, q, OX, window);
        q += step;
        koord += step_koord;
    }
}

void draw_plot(clock_t tm, Graph<double>& graph, double x0, double y0, double w, double h, RenderWindow& window, int alpha) {

    const double height = 900;
    const double width = 1800;
    const double outline = 30;

    //int cnt = min((int)(tm) * 20, 1000);
    int cnt = 300;

    vector<double> points(cnt + 1);
    double mx = 0;

    for (int i = 11; i <= cnt; ++i) {
        points[i] = graph.getN(i) / pow(i, alpha);
        mx = max(mx, points[i]);
    }

    draw_axis({ outline * 2, height - 3 * outline }, { outline * 2, outline * 2 }, 10, 0, mx, false, window);
    draw_axis({ outline * 2, height - 3 * outline }, { width - outline * 2, height - outline * 3 }, 10, 0, cnt, true, window);

    double prevx;
    double prevy;
    for (int i = 11; i <= cnt; ++i) {
        double x = x0 + (w / (cnt - 10) * (i - 10));
        double y = y0 - (h / mx * points[i]);
        draw_plot_point(x, y, window);
        if (i != 1) {
            //draw_line({ prevx, prevy }, { x, y }, Color::Black, window);
        }
        prevx = x;
        prevy = y;
    }

    cout << points[cnt] << '\n';

    return;
}