#include <string>
#include <vector>
#include <iostream>
#include <ctype.h>

using namespace std;
using namespace sf;

namespace form {
    Font None;				// ����� ��������� �������������

    class Label {
    public:
        vector<string> text;	//�����
    protected:
        double width;		// ������ ������
        double height;		// ������ ������
        double x;		//���������� x
        double y;		//���������� y
        int sz = 20;
        Text TXT;		//������������ �����

    public:
        Label(double _x = 0, double _y = 0/*, string _text = "�����"*/)
        {
            None.loadFromFile("Roboto-Regular.ttf");			//�������� ������ ������ ���� ������

            x = _x;
            y = _y;
            //text = _text;								// ��������� �����
            text.resize(1);
            TXT.setFont(None);							// ��������� �����
            TXT.setCharacterSize(sz); 					// � ��������, � �� ������!
            TXT.setFillColor(Color::White);					// ������������� ���� ���������� ������
            TXT.setPosition(x, y);						// ������� ������
        }
        Text displayText(RenderWindow& window)
        {
            for (int i = 0; i < text.size(); ++i) {
                TXT.setPosition(x, y + i * sz);
                TXT.setString(text[i]);						// ��������� � ��������� ����� ����������� �����
                window.draw(TXT);
            }
            return TXT;
        }
    };

    class Button : public Label {
    protected:
        int cnt;
        RectangleShape BOX;	// ������� ������������� � �������� width, height
        Color clr;
        Color clr_pressed;
        Color clr_selected;
    public:
        Button(double _x = 0, double _y = 0, double _width = 150, double _height = 30, string _text = "", Color _bcolor = Color::White, Color _tcolor = Color::Black, Color _outcolor = Color::Black, Color _color_pressed = Color::Green, Color _color_selected = Color::Blue, int _sz = 20)
        {
            None.loadFromFile("Roboto-Regular.ttf");			//�������� ������ ������ ���� ������

            cnt = 0;
            sz = _sz;
            x = _x;
            y = _y;
            width = _width;
            height = _height;
            text.push_back(_text);								// ��������� �����
            TXT.setFont(None);							// ��������� �����
            TXT.setCharacterSize(sz); 					// � ��������, � �� ������!
            TXT.setFillColor(_tcolor);					// ������������� ���� ���������� ������
            TXT.setPosition(x + width / 2.0 - sz * _text.size() / 4, y + height / 2.0 - sz);						// ������� ������
            TXT.setString(_text);
            
            clr = _bcolor;
            clr_pressed = _color_pressed;
            clr_selected = _color_selected;

            BOX.setSize(Vector2f(width, height));		// ������ ������
            BOX.setPosition(x, y);						// ���������� ������
            BOX.setFillColor(_bcolor);	// ���� ������
            BOX.setOutlineThickness(2);					// ������ X ��������
            BOX.setOutlineColor(_outcolor);		// ���� �������
        }
        //�������� ������� �� ������
        bool select(Vector2i _mouse)
        {
            if ((_mouse.x > x && _mouse.x < x + width) && (_mouse.y > y && _mouse.y < y + height)) {
                return  true;
            }
            else {
                return false;
            }
        }
        //����� ������
        Text displayText(RenderWindow& window)
        {
            window.draw(TXT);
            return TXT;
        }
        //����� ������
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
            //text = _text;								// ��������� �����
            text.resize(1);
            focus = false;								// ����� ���� �����

            TXT.setFont(None);							// ��������� �����
            TXT.setCharacterSize(20); 					// � ��������, � �� ������!
            TXT.setFillColor(_tcolor);					// ������������� ���� ���������� ������
            TXT.setPosition(x, y);						// ������� ������

            BOX.setSize(Vector2f(width, height));		// ������ ���� �����
            BOX.setPosition(x, y);						// ���������� ���� �����
            BOX.setFillColor(_bcolor);	// ���� ������
            BOX.setOutlineColor(_outcolor);
        }
        //����� .reText( char ) �������� ����� ������ ������ ���� ������ Input
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

        // �������� ������ Input
        bool select()
        {
            return focus;
        }

        // ��������� ������ Input
        bool select(Vector2i _mouse)
        {
            int last = text.size() - 1;
            if ((_mouse.x > x && _mouse.x < x + width) && (_mouse.y > y && _mouse.y < y + height)) { 	//���� ����� ������ ��� �������� Input...
                if (!focus) {
                    text[last] += "|";
                }
                focus = true;																	   	// ����� true
                																		// � ����� ������ �������� | (��� �� �������� ��� input � ������)
            }
            else {																				//...����� ���� ������� ��������� �� ��� ��������, ��...
                if (text[last].size() > 0) {																// �������� ���������� �������(����� ��������)
                    if (text[last][text[last].size() - 1] == 124) {													// ���� ������ | ...
                        text[last].erase(text[last].size() - 1);														// ... �� ������� ���
                    }
                }
                focus = false;																		// ������������� ����� false
            }
            return focus;
        }

        //����� ������
        Text displayText(RenderWindow& window)
        {
            for (int i = 0; i < text.size(); ++i) {
                TXT.setPosition(x, y + i * sz);
                TXT.setString(text[i]);						// ��������� � ��������� ����� ����������� �����
                window.draw(TXT);
            }
            return TXT;
        }

        //����� ���� Input
        RectangleShape displayBOX()
        {
            return BOX;
        }

        //��� ����, ��� �� ������� ����� � ���� Ipnut ���������� ���������  readText() ������� ���������� string
        vector<string> readText()
        {
            int last = text.size() - 1;
            if (text[last][text[last].size() - 1] == 124) { 	// ������� (���� ����) ������ |
                text[last].erase(text[last].size() - 1);
            }
            return text;
        }
    };

} /* namespace form */