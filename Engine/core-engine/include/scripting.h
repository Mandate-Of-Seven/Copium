#include "component.h"
#include <mono/metadata/assembly.h>

class ScriptComponent;

class ScriptComponent final : public Component
{
public:
    /***************************************************************************/
    /*!
    \brief
        Default constructor for script Components
    */
    /**************************************************************************/
    ScriptComponent() = delete;

    ScriptComponent(const std::string& _name);

    const std::string& Name();

    void Name(const std::string&);

    void Awake();

    void Start();

    void Update();

    void LateUpdate();

    void OnCollisionEnter();

    void refreshMethods();

private:

    std::string name;
    MonoClass* monoClass;
    MonoObject* instance;
    MonoMethod* csAwake;
    MonoMethod* csStart;
    MonoMethod* csUpdate;
    MonoMethod* csLateUpdate;
    MonoMethod* csOnCollisionEnter;

};