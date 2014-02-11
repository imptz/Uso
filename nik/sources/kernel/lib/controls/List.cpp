#include "List.h" 

#include "../string.h"

List::List()
{

}

List::List(unsigned int _stringCount, unsigned _stringLength, unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, _stringLength * 8, _stringCount * 16, _messageReceiver), stringCount(_stringCount), stringLength(_stringLength), addCount(0)
{
	strings = new char[stringCount * (stringLength + 1)];
	clear();
}

List::~List()
{
	delete[] strings;
}

void List::draw()
{
	for (unsigned int i = 0; i < stringCount; i++)
		Display::getSingleton().print((strings + i * (stringLength + 1)) , positionX, positionY + i);

	drawChildControls();
}

void List::onMessage(Message message)
{

}

void List::clear(bool redraw)
{
	memset(strings, ' ', (stringLength + 1) * stringCount);
	for (unsigned int i = 0; i < stringCount; i++)
		strings[stringLength * (i + 1) + i] = 0;

	addCount = 0;

	if (redraw)
		draw();
}

void List::insert(char* str, unsigned int index, bool redraw)
{
	if (index >= stringCount)
		return;

	if (index == stringCount - 1)
	{
		addToEnd(str, redraw);
		return;
	}

	for (unsigned int i = stringCount; i > index + 1; i--)
		memcpy(strings + (i - 1) * (stringLength + 1), strings + (i - 2) * (stringLength + 1), stringLength + 1);

	add(str, index, redraw);
}

void List::add(char* str, unsigned int index, bool redraw)
{
	if (index >= stringCount)
		return;

	if (index == addCount)
	{
		addToEnd(str, redraw);
		return;
	}

	if (static_cast<unsigned int>(strlen(str)) > stringLength)
		str[stringLength] = 0;

	memset(strings + index * (stringLength + 1), ' ', stringLength);
	strings[(index + 1) * stringLength] = 0;
	memcpy(strings + index * (stringLength + 1), str, strlen(str)); 
	
	if (addCount != stringCount)
		addCount++;

	if (redraw)
		draw();
}

void List::addToTop(char* str, bool redraw)
{
	insert(str, 0, redraw);
}

void List::addToEnd(char* str, bool redraw)
{
	if (addCount == stringCount)
	{
		memcpy(strings, (strings + (stringLength + 1)), (stringLength + 1) * (stringCount - 1));
		addCount--;
	}

	if (static_cast<unsigned int>(strlen(str)) > stringLength)
		str[stringLength] = 0;

	memset(strings + addCount * (stringLength + 1), ' ', stringLength);
	strings[addCount * (stringLength + 1) + stringLength] = 0;
	memcpy((strings + addCount * (stringLength + 1)), str, strlen(str)); 
	addCount++;

	if (redraw)
		draw();
}
