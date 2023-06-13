#include "ignition/gazebo/EntityComponentManager.hh"
#include "ignition/gazebo/System.hh"
#include "ignition/sdformat12/sdf/Element.hh"
#include "CalculForces.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/Float64.hpp"

// El tipus de plugin que estem fent és de tipus System. A més a més, hem d'afegir interfícies perquè interactui amb l'sdf i vagi modificant els components que necessitem.
// Interfícies:
// 1 - ISystemConfigure:
//    Has read-write access to world entities and components.
//    Executed once the moment the plugin is loaded.
//    Can be used to get custom configuration from the SDF file, register events with the event manager, as well as modifying entities and components.
// 2 - ISystemUpdate:
//    Has read-write access to world entities and components.
//    Used for physics simulation step (i.e., simulates what happens at time ignition::gazebo::UpdateInfo::simTime).


class Plugin
// Classes Heredades i Necessàries.
//      : public System,
//        public ISystemConfigure,
//        public ISystemPostUpdate
{
// UTILITZARÉ EL CONFIGURE PER EMMAGATZEMAR EL QUE NECESSITO DEL MODEL I CREAR LES VARIABLES QUE NECESSITO: VARIABLE D'ESTAT, VARIABLE 
  void Plugin::Configure(const ignition::gazebo::Entity &_entity,const std::shared_ptr<const sdf::Element> &_sdf, ignition::gazebo::EntityComponentManager &_ecm, ignitnion::gazebo::EventManager &/*_eventMgr*/)
  {
    auto model = _ecm.EntityByComponents(ignition::gazebo::components::Name("CAT15x_Bicicleta"), ignition::gazebo::components::Model());
    CalculForces::State state;
    CalculForces::Input input_nou;
    CalculForces::Input input;
    rclcpp::Node SubsJointController = rclcpp::Node node->create_subscriber<sensor_msgs::msg::Joy>("keyboard_arrow_keys", 1, std::bind(Plugin::EmmagatzemarInput()));
    int  argc  = 0; //argc i argv són els paràmetres que s'han de passar a ros::init
    char *argv = nullptr;
    rclcpp::init(argc, argv);
    auto node_SubsJointController = std::make_shared<SubsJointController>();
  }

  CalculForces::Input Plugin::EmmagatzemarInput(const sensor_msgs::msg::Joy::SharedPtr msg)
  {
    if (msg->axes[0] > 0.0){input_nou.steering_angle = 3.0}
    else if (msg->axes[0] < 0.0){input_nou.steering_angle = -3.0}
    if (msg->buttons[0] == 1){input_nou.dc= 10.0}
    else{input_nou.dc = -5.0}
    return input_nou;
  }

//UTILITZARÉ EL SYSTEMUPDATE PER LLEGIR L'ESTAT ACTUAL DE LES MEVES VARIABLES I DEL CONTROLADOR I EXECUTAR LA FUNCIÓ UPDATE QUE HE CREAT.
  void Plugin::Update(const ignition::gazebo::UpdateInfo &_info,ignition::gazebo::EntityComponentManager &_ecm) override;
  {
    //LLEGIR L'ESTAT ACTUAL DEL MODEL
    const ignition::gazebo::components::Pose* posicio_model =_ecm.Component<ignition::gazebo::components::Pose>(model)->Data();
    ignition::math::Vector3d posicio = posicio_model.Pos();
    double state.x = position.X();
    double state.y = position.Y();
    ignition::math::Quaterniond orientacio = posicio_model.Rot();
    double state.yaw = orientacio.Yaw();

    const ignition::gazebo::components::LinearVelocity* velocitatlineal_model =_ecm.Component<ignition::gazebo::components::LinearVelocity>(model)->Data();
    double state.v_x = velocitatlineal_model.X();
    double state.v_y = velocitatlineal_model.Y();
    const ignition::gazebo::components::AngularVelocity* velocitatangular_model =_ecm.Component<ignition::gazebo::components::AngularVelocity>(model)->Data();
    double state.r = velocitatangular_model.Z();

    const ignition::gazebo::components::LinearAcceleration* acceleraciolineal_model =_ecm.Component<ignition::gazebo::components::LinearAcceleration>(model)->Data();
    double state.a_x = acceleraciolineal_model.X();
    double state.a_y = acceleraciolineal_model.Y();

    //LLEGIR L'INPUT ACTUAL DEL CONTROLADOR
    rclcpp::spin(node_SubsJointController);
    input.steering_angle = input_nou.steering_angle+input.steering_angle;
    input.dc = input_nou.dc+input.dc;

    //CÀLCUL DEL NOU ESTAT DEL MODEL
    Update::Update(CalculForces::State& state, CalculForce::Input& input, const Parametres& parametres);
    
    //ACTUALITZACIÓ DE L'ESTAT DEL MODEL
    posicio = math.Pose3d();
    posicio.Pos(state.x, state.y, 0);
    posicio.Rot.SetFromYaw(state.yaw);
    pose_component = gazebo.components.Pose(posicio);

    linear = math.Vector3d(state.v_x, state.v_y,0);
    angular = math.Vector3d(0,0,state.r);
    linear_velocity_component = gazebo.components.LinearVelocity(linear);
    angular_velocity_component = gazebo.components.AngularVelocity(angular);

    linear_acc = math.Vector3d(state.a_x, state.a_y, 0);
    linear_acceleration_component = gazebo.components.LinearAcceleration(linear_acc);

    ecm.CreateComponent(model, pose_component)
    ecm.CreateComponent(model, linear_velocity_component)
    ecm.CreateComponent(model, angular_velocity_component)
    ecm.CreateComponent(model, linear_acceleration_component)

  }
};

IGNITION_ADD_PLUGIN(MyPlugin,
                    ignition::gazebo::System,
                    MyPlugin::ISystemConfigure,
                    MyPlugin::ISystemPostUpdate)
IGNITION_ADD_PLUGIN_ALIAS(MyPlugin,"ignition::gazebo::systems::Plugin")