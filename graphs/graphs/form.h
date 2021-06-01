#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>

using namespace std;
using namespace sf;

namespace form {
    Font None;				// Шрифт первичная инициализация

    class Label {
    public:
        vector<string> text;	//Текст
    protected:
        double width;		// Ширина Кнопки
        double height;		// Высота Кнопки
        double x;		//Координата x
        double y;		//Координата y
        int sz = 20;
        Text TXT;		//Отображаемый текст

    public:
        Label(double _x = 0, double _y = 0/*, string _text = "Текст"*/)
        {
            None.loadFromFile("Roboto-Regular.ttf");			//передаем нашему шрифту файл шрифта

            x = _x;
            y = _y;
            //text = _text;								// загружает текст
            text.resize(1);
            TXT.setFont(None);							// загружаем фрифт
            TXT.setCharacterSize(sz); 					// в пикселях, а не точках!
            TXT.setFillColor(Color::White);					// устанавливаем цвет выводимого текста
            TXT.setPosition(x, y);						// позиция текста
        }
        Text displayText(RenderWindow& window)
        {
            for (int i = 0; i < text.size(); ++i) {
                TXT.setPosition(x, y + i * sz);
                TXT.setString(text[i]);						// загружаем в выводимый текст загружаемый текст
                window.draw(TXT);
            }
            return TXT;
        }
    };

    class Button : public Label {
    protected:
        int cnt;
        RectangleShape BOX;	// Создаем прямоугольник с размером width, height
        Color clr;
        Color clr_pressed;
        Color clr_selected;
    public:
        Button(double _x = 0, double _y = 0, double _width = 150, double _height = 30, string _text = "", Color _bcolor = Color::White, Color _tcolor = Color::Black, Color _outcolor = Color::Black, Color _color_pressed = Color::Green, Color _color_selected = Color::Blue, int _sz = 20)
        {
            None.loadFromFile("Roboto-Regular.ttf");			//передаем нашему шрифту файл шрифта

            cnt = 0;
            sz = _sz;
            x = _x;
            y = _y;
            width = _width;
            height = _height;
            text.push_back(_text);								// загружает текст
            TXT.setFont(None);							// загружаем фрифт
            TXT.setCharacterSize(sz); 					// в пикселях, а не точках!
            TXT.setFillColor(_tcolor);					// устанавливаем цвет выводимого текста
            TXT.setPosition(x + width / 2.0 - sz * _text.size() / 4, y + height / 2.0 - sz);						// позиция текста
            TXT.setString(_text);
            
            clr = _bcolor;
            clr_pressed = _color_pressed;
            clr_selected = _color_selected;

            BOX.setSize(Vector2f(width, height));		// размер кнопки
            BOX.setPosition(x, y);						// координаты кнопки
            BOX.setFillColor(_bcolor);	// цвет кнопки
            BOX.setOutlineThickness(2);					// Контур X пикселей
            BOX.setOutlineColor(_outcolor);		// Цвет контура
        }
        //Проверка нажатия на кнопку
        bool select(Vector2i _mouse)
        {
            if ((_mouse.x > x && _mouse.x < x + width) && (_mouse.y > y && _mouse.y < y + height)) {
                return  true;
            }
            else {
                return false;
            }
        }
        //Вывод Текста
        Text displayText(RenderWindow& window)
        {
            window.draw(TXT);
            return TXT;
        }
        //Вывод кнопки
        RectangleShape displayButton()
        {
            return BOX;
        }

        void selected() {
            BOX.setFillColor(clr_selected);
        }

        void update(Vector2i mouse) {
            if (select(mouse))
                selected();
            else 
                BOX.setFillColor(clr);
        }

    };

    class Input : public Button {
    protected:
        bool focus;
    public:
        Input(double _x = 0, double _y = 0, double _width = 150, double _height = 30, Color _bcolor = Color::White, Color _tcolor = Color::Black, Color _outcolor = Color::Black, int _sz = 20)
        {
            sz = _sz;
            x = _x;
            y = _y;
            width = _width;
            height = _height;
            //text = _text;								// загружает текст
            text.resize(1);
            focus = false;								// фокус поля ввода

            TXT.setFont(None);							// загружаем фрифт
            TXT.setCharacterSize(20); 					// в пикселях, а не точках!
            TXT.setFillColor(_tcolor);					// устанавливаем цвет выводимого текста
            TXT.setPosition(x, y);						// позиция текста

            BOX.setSize(Vector2f(width, height));		// размер поля ввода
            BOX.setPosition(x, y);						// координаты поля ввода
            BOX.setFillColor(_bcolor);	// цвет кнопки
            BOX.setOutlineColor(_outcolor);
        }
        //метод .reText( char ) изменяет текст внутри класса Типа данных Input
        void reText(char _tmp)
        {
            if (_tmp == '\n' + 3) {
                return;
            }

            if (!isdigit(_tmp) && !isspace(_tmp) && _tmp != 8) {
                return;
            }

            int last = text.size() - 1;
            text[last].erase(text[last].size() - 1);

            if (_tmp != 8) {
                text[last] += _tmp;
            }
            else {
                if (text[last].size() > 0) {
                    text[last].erase(text[last].size() - 1);
                }
                else if (last != 0) {
                    text.pop_back();
                    --last;
                }

            }
            text[last] += "|";
            //TXT.setString(text[last]);
        }

        // Проверка Фокуса Input
        bool select()
        {
            return focus;
        }

        // Установка Фокуса Input
        bool select(Vector2i _mouse)
        {
            int last = text.size() - 1;
            if ((_mouse.x > x && _mouse.x < x + width) && (_mouse.y > y && _mouse.y < y + height)) { 	//Если нажат клаиша над объектом Input...
                if (!focus) {
                    text[last] += "|";
                }
                focus = true;																	   	// Фокус true
                																		// В конец строки добаляем | (что бы понимать что input в фокусе)
            }
            else {																				//...Иначе если нажатие произошло не над объектом, то...
                if (text[last].size() > 0) {																// проверка последнего символа(иначе вылетает)
                    if (text[last][text[last].size() - 1] == 124) {													// если символ | ...
                        text[last].erase(text[last].size() - 1);														// ... то удаляем его
                    }
                }
                focus = false;																		// устанавливаем фокус false
            }
            return focus;
        }

        //Вывод Текста
        Text displayText(RenderWindow& window)
        {
            for (int i = 0; i < text.size(); ++i) {
                TXT.setPosition(x, y + i * sz);
                TXT.setString(text[i]);						// загружаем в выводимый текст загружаемый текст
                window.draw(TXT);
            }
            return TXT;
        }

        //Вывод фона Input
        RectangleShape displayBOX()
        {
            return BOX;
        }

        //Для того, что бы считать текст с поля Ipnut необходимо пременить  readText() который возвращает string
        vector<string> readText()
        {
            int last = text.size() - 1;
            if (text[last][text[last].size() - 1] == 124) { 	// удаляем (если есть) символ |
                text[last].erase(text[last].size() - 1);
            }
            return text;
        }
    };

} /* namespace form */