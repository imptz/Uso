#include "InfoPanel.h"
#include "../version.h"

#pragma warning (disable : 4355)

InfoPanel::InfoPanel(unsigned int _positionX, unsigned int _positionY, MessageReceiver* _messageReceiver)
	:	Control(_positionX, _positionY, PANEL_WIDTH, PANEL_HEIGHT, _messageReceiver), 
	panel(new Panel(_positionX, _positionY, PANEL_WIDTH, PANEL_HEIGHT, PANEL_BORDER_STYLE)),
	usoModeControl(new UsoModeControl(_positionX + 1, _positionY, this)),
	clockControl(new ClockControl(_positionX + DATE_TIME_CONTROL_POSITION_X, _positionY + DATE_TIME_CONTROL_POSITION_Y)),
	version(new Label(VERSION_POSITION_X, _positionY + 1, 15, 1))
{ 
	addChildControl(panel);	
	panel->addChildControl(usoModeControl);	
	panel->addChildControl(clockControl);	
	panel->addChildControl(version);	

	char str[20];	
	memset(str, 0, 20);

	Display::getSingleton().toStringWithoutNull(VERSION, str);
	int index = 0;
	for (index; index < 20; index++)
		if (str[index] == 0)
			break;

	if (index < 20)
	{
		str[index++] = '.';
		Display::getSingleton().toStringWithoutNull(SUB_VERSION, &str[index]);

		for (index; index < 20; index++)
			if (str[index] == 0)
				break;

		if (index < 20)
		{
			str[index++] = '.';
			Display::getSingleton().toStringWithoutNull(BUILD, &str[index]);
		}
	}

	version->setText(str, true);
}

InfoPanel::~InfoPanel()
{

}

void InfoPanel::draw()
{
	panel->setBlinking(blinking);
	drawChildControls();
}

void InfoPanel::onMessage(Message message)
{
	if ((message.from == usoModeControl->getId()) && (message.msg == UsoModeControl::USO_MODE_CONTROL_MESSAGE_NEW_MODE))
		sendMessage(Message(id, MESSAGE_FROM_OFFSET_CONTROLS + INFO_PANEL_MESSAGE_NEW_USO_MODE, message.par1, message.par2));
}

UsoModeControl* InfoPanel::getUsoModeControl()
{
	return usoModeControl;
}