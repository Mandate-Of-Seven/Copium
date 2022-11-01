#pragma once

#include <stack>
#include "GameObject/game-object.h"

namespace Copium
{
	namespace UndoRedo
	{
		class Command
		{
			public:
				virtual void Undo(std::stack<Command*>* stackPointer) = 0;
				virtual void Redo(std::stack<Command*>* stackPointer) = 0;
				virtual void printCommand() = 0;
		};

		class TransformCommand : public Command
		{
			public:
				TransformCommand(float* _pointer, float _value);
				~TransformCommand();
				void Undo(std::stack<Command*>* stackPointer) override;
				void Redo(std::stack<Command*>* stackPointer) override;
				void printCommand()override;

				float* pointer;//pointer to what was changed
				float value;//original value
		};

		class CommandManager
		{
			public:
				std::stack<Command*> undoStack;
				std::stack<Command*> redoStack;
		};
	}
	
}
