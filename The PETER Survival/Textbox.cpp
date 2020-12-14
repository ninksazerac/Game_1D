#include "TextBox.h"

TextBox::TextBox(int textSize, Color color, Font* font, Vector2f position, bool isSelected, bool hasLimit, int limit)
{
	this->isSelected = isSelected;
	this->hasLimit = hasLimit;
	this->font = font;
	this->limit = limit - 1;

	this->input.setCharacterSize(textSize);
	this->input.setFillColor(color);
	this->input.setFont(*this->font);
	this->input.setPosition(position);

	if (isSelected) input.setString("");
	else input.setString("");
}

void TextBox::setSelected(bool sel)
{
	isSelected = sel;
	if (isSelected)
	{
		t = text.str();
		newT = "";

		for (int i = 0; i < t.length(); i++) newT += t[i];
		input.setString(newT);
	}
}

void TextBox::update(Event input)
{
	if (this->isSelected)
	{
		int charType = input.text.unicode;
		if (charType < 128)
		{
			if (hasLimit)
			{
				if (text.str().length() <= limit) inputLogic(charType);
				else if (text.str().length() > limit && charType == DELETE_KEY) deleteLastChar();
			}
			else inputLogic(charType);
		}
		// ตำแหน่งใส่ชื่อ
		//this->input.setOrigin(this->input.getGlobalBounds().width / 2.f, 0.f);
	}
}

void TextBox::draw(RenderWindow& window)
{
	window.draw(input);
}

void TextBox::clear()
{
	text.str("");
	input.setString(text.str());
}

void TextBox::inputLogic(int charType)
{
	if (charType != DELETE_KEY && charType != ENTER_KEY && charType != ESCAPE_KEY) text << static_cast<char>(charType);
	else if (charType == DELETE_KEY) if (text.str().length() > 0) deleteLastChar();
	input.setString(text.str());
}

void TextBox::deleteLastChar()
{
	t = text.str();
	newT = "";

	for (int i = 0; i < text.str().length() - 1; i++) newT += t[i];
	text.str("");
	text << newT;

	input.setString(text.str());
}