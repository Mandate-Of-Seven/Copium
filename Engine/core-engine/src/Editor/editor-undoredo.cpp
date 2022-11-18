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


	//GameObject UndoRedo not ready
	//UndoRedo::GameObjectCommand::GameObjectCommand(GameObject& _value)
	//{
	//	//std::cout << "undo\n";
	//	//std::cout << _value.get_name() << std::endl;
	//	this->value = _value;
	//	this->deleting = false;
	//	std::cout << "Hello2";
	//}

	UndoRedo::GameObjectCommand::GameObjectCommand(GameObject& _value,bool _deleting)
	{
		std::cout << "gameobj cmd\n";
		this->value = _value;
		this->pointer = &_value;
		this->deleting = _deleting;
	}

	UndoRedo::GameObjectCommand::~GameObjectCommand()
	{
		std::cout << this->pointer << std::endl;
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
			
			if (!this->value.get_name().empty())
			{
				std::cout << "Delete" << std::endl;
				NewSceneManager::Instance()->get_gof().delete_gameobject(this->pointer);
				//Command* temp = new GameObjectCommand(this->value, false);
				//stackPointer->push(temp);
			}
			else
			{
				std::cout << "The pointer is invalid, skipping undo command";
			}
		}
		else
		{
			if (!this->value.get_name().empty())
			{
				std::cout << "Create" << std::endl;
				GameObject* newObj = NewSceneManager::Instance()->get_gof().build_gameobject(this->value);
				Command* temp = new GameObjectCommand(*newObj, true);
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

			if (!this->value.get_name().empty())
			{
				std::cout << "Delete" << std::endl;
				NewSceneManager::Instance()->get_gof().delete_gameobject(this->pointer);
				//Command* temp = new GameObjectCommand(this->value, false);
				//stackPointer->push(temp);
			}
			else
			{
				std::cout << "The pointer is invalid, skipping undo command";
			}
		}
		else
		{
			if (!this->value.get_name().empty())
			{
				std::cout << "Create" << std::endl;
				GameObject* newObj = NewSceneManager::Instance()->get_gof().build_gameobject(this->value);
				Command* temp = new GameObjectCommand(*newObj, true);
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
		if (!this->value.get_name().empty())
		{
			std::cout << this->value.get_name() << " - ";
			std::cout << this->value.get_ppid() << std::endl;
		}
	}

	bool UndoRedo::GameObjectCommand::getDeleting()
	{
		return deleting;
	}
}
