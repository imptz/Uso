#ifndef UI_H 
#define UI_H

#include "..\Singleton.h"
#include "..\message\Messages.h"
#include "Desktop.h"
#include "TabControl.h"
#include "LogControl.h"
#include "InfoPanel.h"
#include "MainTabControl.h"
#include "ToolsTabControl.h"

class UI : public Singleton<UI>, public MessageReceiver
{
	private:
		Desktop* desktop;
		InfoPanel* infoPanel;
		LogControl* logControl; 
		TabControl* tabControl; 
		MainTabControl* mainTabControl;
		ToolsTabControl* toolsTabControl;

	public:
		UI()
		{
			desktop = new Desktop(0, 0, 80, 25, this);

			infoPanel = new InfoPanel(0, 0, this);
			logControl = new LogControl(0, 14); 
			tabControl = new TabControl(0, 3, 80, 11, 2);
			mainTabControl = new MainTabControl(0, 3, 80, 11);
			toolsTabControl = new ToolsTabControl(0, 3, 80, 11, this);
			
			desktop->addChildControl(infoPanel);
			desktop->addChildControl(logControl); 
			desktop->addChildControl(tabControl); 
			tabControl->getTabPanel(0)->addChildControl(mainTabControl);
			tabControl->getTabPanel(1)->addChildControl(toolsTabControl);
			tabControl->setActiveTab(0);
			desktop->draw();
		}

		~UI()
		{

		}

		virtual void onMessage(Message message){
			if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + infoPanel->getId()) && (message.msg == MESSAGE_USO_MODE_CONTROL_TO_TOOLS)){
				tabControl->setActiveTab(1);
			}

			if ((message.from == MESSAGE_FROM_OFFSET_CONTROLS + toolsTabControl->getId()) && (message.msg == ToolsTabControl::TOOLS_TAB_CONTROL_MESSAGE_EXIT)){
				infoPanel->getUsoModeControl()->change_tools();
				tabControl->setActiveTab(0);
			}
		}

		UsoModeControl* getUsoModeControl()
		{
			return infoPanel->getUsoModeControl();
		}

		MainTabControl* getMainTabControl()
		{
			return mainTabControl;
		}
};

#endif