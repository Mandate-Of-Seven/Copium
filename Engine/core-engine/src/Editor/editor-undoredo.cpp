/*!***************************************************************************************
\file			editor-undoredo.cpp
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			28/10/2022

\brief
	This file holds functions to undo and redo changes made in gameobjects as well as
	transform components.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

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





	//GameObject UndoRedo not ready
	UndoRedo::GameObjectCommand::GameObjectCommand(GameObject* _pointer, GameObject& _value)
	{
		//this->pointer = _pointer;
		//this->value(_value);//no assignment operator yet
	}

	UndoRedo::GameObjectCommand::~GameObjectCommand()
	{

	}

	void UndoRedo::GameObjectCommand::Undo(std::stack<Command*>* stackPointer)
	{
		if (!stackPointer)
		{
			PRINT("Invalid stack pointer");
			return;
		}
		Command* temp = new GameObjectCommand(this->pointer, *this->pointer);
		stackPointer->push(temp);
		if (this->pointer != nullptr)
		{
			//*this->pointer = this->value;//no assignment operator
		}
		else
		{
			std::cout << "The pointer is invalid, skipping undo command";
		}
	}

	void UndoRedo::GameObjectCommand::Redo(std::stack<Command*>* stackPointer)
	{
		if (!stackPointer)
		{
			PRINT("Invalid stack pointer");
			return;
		}
		Command* temp = new GameObjectCommand(this->pointer, *this->pointer);
		stackPointer->push(temp);

		if (this->pointer)
		{
			//*this->pointer = this->value;//no assignment operator
		}
		else
		{
			std::cout << "The pointer is invalid, skipping redo command";
		}
	}

	void UndoRedo::GameObjectCommand::printCommand()
	{
		if (this->pointer)
		{
			std::cout << "The command's pointer is valid";
		}
		else
		{
			std::cout << "The command's pointer is valid";
		}

		//print GameObject data
	}
}
