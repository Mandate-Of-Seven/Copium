#include "pch.h"
#include "Editor/editor-undoredo.h"


namespace Copium
{
	UndoRedo::TransformCommand::TransformCommand(float* _pointer, float _value)
	{
		this->pointer = _pointer;
		this->value =  _value;
		std::cout << "New Undo created with value: " << this->value<<"\n";
	}

	UndoRedo::TransformCommand::~TransformCommand()
	{
		
	}

	void UndoRedo::TransformCommand::Undo(std::stack<Command*>* stackPointer)
	{
		if (!stackPointer)
		{
			PRINT("Invalid stack pointer");
			return;
		}
		Command* temp = new TransformCommand(this->pointer, *this->pointer);
		stackPointer->push(temp);
		if (this->pointer != nullptr)
		{
			*this->pointer = this->value;
		}
		else
		{
			std::cout << "The pointer is invalid, skipping undo command";
		}
	}

	void UndoRedo::TransformCommand::Redo(std::stack<Command*>* stackPointer)
	{
		if (!stackPointer)
		{
			PRINT("Invalid stack pointer");
			return;
		}
		Command* temp = new TransformCommand(this->pointer, *this->pointer);
		stackPointer->push(temp);

		if (this->pointer)
		{
			*this->pointer = this->value;
		}
		else
		{
			std::cout << "The pointer is invalid, skipping redo command";
		}
	}

	void UndoRedo::TransformCommand::printCommand()
	{
		if (this->pointer)
		{
			std::cout << "The command's pointer is valid";
		}
		else
		{
			std::cout << "The command's pointer is valid";
		}
		std::cout << " & the value to restore is "<<this->value;
	}

}
