/*!***************************************************************************************
\file			editor-undoredo.cpp
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			25/11/2022

\brief
	This file holds functions to undo and redo changes made in gameobjects as well as
	transform components.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Editor/editor-undoredo.h"
#include "GameObject/game-object-factory.h"
#include "SceneManager/sm.h"

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	UndoRedo::GameObjectCommand::GameObjectCommand(GameObject* _value,bool _deleting)
	{
		std::cout << "gameobj cmd\n";
		this->value = GOF.clone(*_value,nullptr);
		this->pointer = _value;
		this->deleting = _deleting;
	}

	UndoRedo::GameObjectCommand::~GameObjectCommand()
	{
		std::cout<<"Undo Redo being destroyed: " << this->value->get_name()<<" | "<<this->value << "\n\n";
		//GOF.destroy(this->value);
		delete this->value;
	}

	void UndoRedo::GameObjectCommand::Undo(std::stack<Command*>* stackPointer)
	{
		if (!stackPointer)
		{
			PRINT("Invalid stack pointer");
			return;
		}

		bool isDeleting = this->deleting;

		if (isDeleting)
		{
			if (!value->get_name().empty())
			{
				std::cout << "Delete" << std::endl;
				GOF.destroy(this->pointer);
				Command* temp = new GameObjectCommand(this->value, false);
				stackPointer->push(temp);
			}
			else
			{
				std::cout << "The pointer is invalid, skipping undo command";
			}
		}
		else
		{
			if (!value->get_name().empty())
			{
				std::cout << "Create" << std::endl;
				GameObject* newObj = GOF.instantiate(*this->value);
				Command* temp = new GameObjectCommand(newObj, true);
				stackPointer->push(temp);
			}
			else
			{
				std::cout << "The pointer is invalid, skipping undo command";
			}
		}
	}

	void UndoRedo::GameObjectCommand::Redo(std::stack<Command*>* stackPointer)
	{
		if (!stackPointer)
		{
			PRINT("Invalid stack pointer");
			return;
		}

		bool isDeleting = this->deleting;

		if (isDeleting)
		{

			if (!value->get_name().empty())
			{
				std::cout << "Delete" << std::endl;
				GOF.destroy(this->pointer);
				Command* temp = new GameObjectCommand(this->value, false);
				stackPointer->push(temp);
			}
			else
			{
				std::cout << "The pointer is invalid, skipping undo command";
			}
		}
		else
		{
			if (!value->get_name().empty())
			{
				std::cout << "Create" << std::endl;
				GameObject* newObj = GOF.instantiate(*this->value);
				Command* temp = new GameObjectCommand(newObj, true);
				stackPointer->push(temp);
			}
			else
			{
				std::cout << "The pointer is invalid, skipping undo command";
			}
		}
	}

	void UndoRedo::GameObjectCommand::printCommand()
	{
		if (!value->get_name().empty())
		{
			std::cout << value->get_name() << " - ";
		}
	}

	bool UndoRedo::GameObjectCommand::getDeleting()
	{
		return deleting;
	}
}
