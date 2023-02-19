/*!***************************************************************************************
\file			game-object.h
\project        
\author			Zacharie Hong (50%)
\co-authors     Matthew Lau (50%)

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Declares GameObject Class and member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

//INCLUDES

#include "Math/math-library.h"
#include <Utilities/sparse-set.h>
#include <config.h>
#include <string>
#include <GameObject/components.h>
#include "CopiumCore/uuid.h"

//USING

namespace Copium
{
    class UUID;
    class GameObject final
    {
    private:
        //friend class GameObjectFactory;
        ComponentsPtrArrays componentPtrArrays{};

    public:           
        //Global ID for gameObjects
        UUID uuid;
        Transform transform;
        std::string name;                   //Name of gameObject
        bool active;

        /***************************************************************************/
        /*!
        \brief
            Constructor that initializes Transform with given values
        \param _position
            Position of transform to initialize with
        \param _rotation
            Rotation of transform to initialize with
        \param _scale
            Scale of transform to initialize with
        */
        /**************************************************************************/
        GameObject(UUID _uuid = UUID(),
            Math::Vec3 _position = { 0,0,0 },
            Math::Vec3 _rotation = { 0,0,0 },
            Math::Vec3 _scale = { 1,1,1 });

        GameObject& operator=(const GameObject& rhs)
        {
            name = rhs.name;
            active = rhs.active;
            transform = rhs.transform;
            //Find Components?
            return *this;
        }

        GameObject(const GameObject& rhs, UUID _uuid = UUID()) : uuid{_uuid}, transform{rhs.transform}
        {
            name = rhs.name;
            active = rhs.active;
            //Find Components?
        }

        template <typename T>
        bool HasComponent()
        {
            ComponentsPtrArray<T>& components{ componentPtrArrays.GetArray<T>() };
            return !components.empty();
        }

        bool HasComponent(ComponentType componentType);

        /*******************************************************************************
        /*!
            \brief
                Gets all components of type from components list
            \param T
                Type of component to get
            \return
                Vector of components gotten
        */
        /*******************************************************************************/
        template <typename T>
        const ComponentsPtrArray<T>& GetComponents()
        {
            return componentPtrArrays.GetArray<T>();
        }

        /*******************************************************************************
        /*!
        *
        \brief
            Template function to add a component to components list
        \param T
            Type of component to add
        \return
            Pointer to component added
        */
        /*******************************************************************************/
        template <typename T>
        void AddComponent(T* component)
        {
            componentPtrArrays.GetArray<T>().push_back(component);
        }

        /*******************************************************************************
        /*!
        *
        \brief
            Template function to get a component from components list
        \param T
            Type of component to get
        \return
            Pointer to component gotten
        */
        /*******************************************************************************/
        template <typename T>
        T* GetComponent()
        {
            ComponentsPtrArray<T>& components{componentPtrArrays.GetArray<T>()};
            if (!components.empty())
                return components[0];
            return nullptr;
        }

        /*******************************************************************************
        /*!
        *
        \brief
            Deletes a component from components list
        \param UUID
            ID of component to match in order to delete it from the components list
        */
        /*******************************************************************************/
        //void removeComponent(UUID UUID);

        /*******************************************************************************
        /*!
        *
        \brief
            Template function to delete a component from components list
        \param T
            Type of component to delete
        */
        /*******************************************************************************/
        //template <typename T>
        //void removeComponent()
        //{
        //    static_assert(std::is_base_of<Component, T>::value);
        //    std::string tName = typeid(T).name() + std::string("class Copium::").length();
        //    ComponentType componentType = Component::nameToType(tName); 
        //    removeComponent(componentType);
        //}

        /***************************************************************************/
        /*!
        \brief
            Destructor, does nothing
        */
        /**************************************************************************/
        ~GameObject();
        bool IsActive();
        void SetActive(bool _active);
    };

    std::ostream& operator<<(std::ostream& _os, const GameObject& _go);

    using GameObjectsArray = SparseSet<GameObject, MAX_GAMEOBJECTS>;
    using GameObjectsPtrArray = std::vector<GameObject*>;

    template <typename T, typename... Ts>
    struct HasComponentStruct
    {
        GameObject& gameObject;
        HasComponentStruct(TemplatePack<T, Ts...> pack) {}
        HasComponentStruct(GameObject& _gameObject) : gameObject{ _gameObject } {}
        bool HasComponentRecurse(ComponentType componentType) { return false; }
        template <typename T1, typename... T1s>
        bool HasComponentRecurse(ComponentType componentType)
        {
            if ((int)GetComponentType<T1>::e == (int)componentType)
                return gameObject.HasComponent<T1>();
            if constexpr (sizeof...(T1s) != 0)
                return HasComponentRecurse<T1s...>(componentType);
            return false;
        }
        bool HasComponent(ComponentType componentType)
        {
            return HasComponentRecurse<T, Ts...>(componentType);
        }
    };

    using HasComponentType = decltype(HasComponentStruct(ComponentTypes()));
}


#endif // !GAME_OBJECT_H
