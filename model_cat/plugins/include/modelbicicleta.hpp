#include <ignition/gazebo/EntityComponentManager.hh>
#include <ignition/gazebo/System.hh>
#include <ignition/gazebo/components/Link.hh>
#include <ignition/gazebo/components/Name.hh>
#include <ignition/gazebo/components/ParentEntity.hh>
#include <ignition/gazebo/components/Pose.hh>
#include <ignition/plugin/Register.hh>

//Els namespace serveixen per no haver de escriure les classes que venen per defecte amb gazebo de forma llarga. 
//Sense ells a les classes de System, ISystemConfigure i ISystemPostUpdate els hi hauria de dir: ignition::gazebo::systems::Systems, ignition::gazebo::systems::ISystemConfigure i ignition::gazebo::systems::ISystemPostUpdate
using namespace ignition;
using namespace gazebo;
using namespace systems;

//Defineix la classe ModelbicicletaPlugin que hereta de les classes System, ISystemConfigure i ISystemPostUpdate proporcionades per Ignition Gazebo. 
//Aquestes classes proporcionen interfícies per configurar i actualitzar plugins en el cicle de vida del món de simulació d'Ignition Gazebo.
class ModelbicicletaPlugin
      : public System, //És una classe que et dona accés a les interfícies ISystemConfigure i ISystemPorUpdate
        public ISystemConfigure, //Em donarà accés a la funció Configure. Aquesta interfície és important peruqè sobreescriure la funció de realimentació Configure, que dóna accés a diverses coses, com ara el punter SDF (que apuntarà al nostra robot). Abans (amb gazebo-11) això es feia a través de la funció de càrrega (Load) del plugin.
        public ISystemPostUpdate //Em donarà accés a la funció PostUpdate. 
    {

    public: 
        ModelbicicletaPlugin();
        ~ModelbicicletaPlugin() override ;
        void Reset() override;
        virtual void Configure (Entity &_entity,
                         const std::shared_ptr<const sdf::Element> &_sdf,
                         EntityComponentManager &_ecm,
                         EventManager &/*_eventMgr*/) override
        virtual void PostUpdate(const UpdateInfo &/*_info*/,
                          const EntityComponentManager &_ecm) override
    
    private: Entity linkEntity;

    IGNITION_ADD_PLUGIN(ModelbicicletaPlugin,
                    ignition::gazebo::System,
                    ModelbicicletaPlugin::ISystemConfigure,
                    ModelbicicletaPlugin:ISystemPostUpdate)
    
    IGNITION_ADD_PLUGIN_ALIAS(ModelbicicletaPlugin, "ignition::gazebo::systems::ModelbicicletaPlugin")
    }
