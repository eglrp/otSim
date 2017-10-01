/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Header:       CommandManager.h
Author:       Cory Parks
Date started: 09/2017

See LICENSE file for copyright and license information

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
SENTRY
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef CommandManager_H
#define CommandManager_H

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
INCLUDES
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "Singleton.h"
#include <vector>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
FORWARD DECLARATIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

namespace otInput {
class Command;
class CommandObserver;

enum ActionKeyCommands : int;
enum ActionAxisCommands : int;
}
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
DEFINITIONS
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifdef INPUT_EXPORTS
#define INPUT_API __declspec(dllexport)
#else
#define INPUT_API __declspec(dllimport)
#endif

namespace otInput {

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DOCUMENTATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/** Command manager class.  Singleton in nature.

@author Cory Parks
*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
CLASS DECLARATION
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class INPUT_API CommandManager : public otCore::Singleton<CommandManager>
{
	friend class otCore::Singleton < CommandManager >;
public:

	///Register single commands by passing the observer pointer and the command ID
	void listenCommand(CommandObserver * viewer, int command);

	///Register single axis commands by passing the observer pointer and the axis command ID
	void listenAxisCommand(CommandObserver * viewer, int axisCommand);

	///Register multiple commands by range by passing the observer pointer and the start and end command ID
	void listenCommandRange(CommandObserver * viewer, int startCommand, int endCommand);

	///Register multiple axis commands by range by passing the observer pointer and the start and end axis command ID
	void listenAxisCommandRange(CommandObserver * viewer, int startAxisCommand, int endAxisCommand);

	///Returns a pointer to the actual command object by passing the ActionKeyCommands ID
	Command* getCommandByID(ActionKeyCommands command) const;

	///Returns a pointer to the actual command object by passing the ActionAxisCommands ID
	Command* getAxisCommandByID(ActionAxisCommands axisCommand) const;


private:
	/// Constructor
	CommandManager();
	/// Destructor
	~CommandManager();

	std::vector<Command*> commandList;
	std::vector<Command*> axisCommandList;
};


} //namespace otInput

#endif //CommandManager_H