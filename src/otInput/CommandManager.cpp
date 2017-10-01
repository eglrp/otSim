/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Module:       CommandManager.cpp
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

FUNCTIONAL DESCRIPTION
--------------------------------------------------------------------------------
Command handler

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
NOTES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "CommandManager.h"
#include "Command.h"
#include "CommandObserver.h"
#include "ActionCommands.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS IMPLEMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otInput {

CommandManager::CommandManager()
{
	//Create commands
	for (int i = 0; i<COMMAND_MAXIMUM; i++)
	{
		commandList.push_back(new Command(i, false));
	}

	for (int i = 0; i<COMMAND_AXIS_MAXIMUM; i++)
	{
		axisCommandList.push_back(new Command(i, true));
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

CommandManager::~CommandManager()
{
	//Delete commands
	for (int i = 0; i<COMMAND_MAXIMUM; i++)
	{
		delete commandList[i];
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Command* CommandManager::getCommandByID(ActionKeyCommands command) const
{
	return commandList[command];
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Command* CommandManager::getAxisCommandByID(ActionAxisCommands axisCommand) const
{
	return axisCommandList[axisCommand];
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CommandManager::listenCommand(CommandObserver * viewer, int command)
{
	if (command >= 0 && command < COMMAND_MAXIMUM)
		commandList[command]->addListener(viewer);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CommandManager::listenAxisCommand(CommandObserver * viewer, int axisCommand)
{
	if (axisCommand >= 0 && axisCommand < COMMAND_AXIS_MAXIMUM)
		axisCommandList[axisCommand]->addListener(viewer);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CommandManager::listenCommandRange(CommandObserver * viewer, int startCommand, int endCommand)
{
	if (startCommand >= 0 && endCommand >= startCommand)
	{
		if (endCommand < COMMAND_MAXIMUM)
		{
			for (int i = startCommand; i <= endCommand; i++)
				commandList[i]->addListener(viewer);
		}
		else
		{
			for (int i = startCommand; i<COMMAND_MAXIMUM; i++)
				commandList[i]->addListener(viewer);
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void CommandManager::listenAxisCommandRange(CommandObserver * viewer, int startAxisCommand, int endAxisCommand)
{
	if (startAxisCommand >= 0 && endAxisCommand >= startAxisCommand)
	{
		if (endAxisCommand < COMMAND_AXIS_MAXIMUM)
		{
			for (int i = startAxisCommand; i <= endAxisCommand; i++)
				axisCommandList[i]->addListener(viewer);
		}
		else
		{
			for (int i = startAxisCommand; i<COMMAND_AXIS_MAXIMUM; i++)
				axisCommandList[i]->addListener(viewer);
		}
	}
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


} //namespace otInput

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%