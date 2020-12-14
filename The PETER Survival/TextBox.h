#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
using namespace sf;
using namespace std;

class TextBox
{
public:
	TextBox(int textSize, Color color, Font* font, Vector2f position, bool isSelected, bool hasLimit, int limit);

	void setSelected(bool sel);
	void update(Event input);
	void draw(RenderWindow& window);
	void clear();
	string getInput() { return text.str(); }

private:
	Text input;
	Font* font;
	string t;
	string newT;
	ostringstream text;
	bool isSelected = false;
	bool hasLimit = false;
	int limit;

	void inputLogic(int charType);
	void deleteLastChar();
};