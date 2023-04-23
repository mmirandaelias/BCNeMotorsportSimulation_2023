import os

from ament_index_python.packages import get_package_share_directory
from launch_ros.parameter_descriptions import ParameterValue
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, ExecuteProcess
from launch.conditions import IfCondition
from launch.substitutions import Command, FindExecutable, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch.actions import RegisterEventHandler
from launch.event_handlers import OnProcessExit

def generate_launch_description():

    model_arg = DeclareLaunchArgument(name='CAT15x_bicicleta') # Et declara un argument tipu launch (anomenat CAT15x_bicicleta) per poder-lo utilitzar més tard.
    use_sim_time = LaunchConfiguration('use_sim_time') # Et declara un element tipu LaunchCOnfiguration (anomenat use_sim_time) que et permetrà obtenir la variable de configuració 'use_sim_time' d'un arxiu launch de gazebo.
    package_name = 'model_cat' #Defineix el nom del paquet que conté el model del robot per poder-lo utilitzar més tard.
    pkg_share = FindPackageShare(package=package_name).find(package_name) #Et busca el path fins la carpeta que conté el package_name, és el dir el path fins la carpeta de model_cat.
    pkg_gazebo_ros = FindPackageShare(package='gazebo_ros').find('gazebo_ros') # Et gurda el path fins el paquest de gazebo_ros, que ja ve amb ROS
    

    #world_file_path = 'world.world' #Defineix el nom de l'arxiu que conté el món de gazebo on simularem el cotxe.
    #world = LaunchConfiguration('world') #Et crea un arxiu de configuració anomenat world. 
    #world_path = os.path.join(pkg_share, 'worlds', world_file_path) #Defineix el path fins l'arxiu on tens el teu world. És important que estigui dins una carpeta anomenada worlds que es troba en el teu workspace. 

    #Poses la variable de use_sim_time a true per la configuració de llançament de gazebo.
    declare_use_sim_time_cmd = DeclareLaunchArgument(
        name ='use_sim_time',
        default_value='true' 
    )

    robot_name_in_model = 'CAT15x_Bicicleta' #Assignes CAT15x_Bicicleta (nom del robot dins l'arxiu urdf) a la variable robot_name_in_model per poder utilitzar-ho més tard

    urdf_file_name ='modelbicicletaCAT15x.sdf' #Assignes modelbicicletaCAT15x (nom de l'arxiu que conté el model urdf del robot) a la variable urdf_file_name per poder utilitzar-ho més tard
    urdf = os.path.join(
        get_package_share_directory('model_cat'),
        'urdf',
        urdf_file_name
    ) #Et don el path fins on tens el model urdf del robot (l'ha de tenir a una carpeta anomenada urdf dins la carpeta model_cat que està al teu workspace)
    with open(urdf, 'r') as infp: #Obra l'arxiu urdf en mode lectura 
        robot_desc = infp.read() #Llegeix l'arxiu i el guarda a la variable robot_desc
    
    #Declaració del node rviz2 (ja ve amb ROS2)
    rviz2 = Node(
        package = 'rviz2', #Nom del paquet que conté el node rviz2
        executable='rviz2', #Nom de l'executable de node rviz2
        name = 'rviz2', #Nom del node
        output = 'log', #Nom de l'output, en aquest cas és la consola.
        parameters = [{'use_sim_time':use_sim_time}], #Agafa la variable de use_sim_time que has declarat amb el LaunchConfiguration i amb el valor que li has assignat amb el DeclareLaunchArgument
    )

    #Declaració del node robot_state_publisher_node (ja ve amb ROS2) -> Publica NOMÉS la posició i orientació del teu robot el global a mesura que el robot es mou.
    robot_state_publisher_node = Node(
        package='robot_state_publisher', #Nom del paquet on es troba el node robot_state_publisher
        executable='robot_state_publisher', #Nom de l'executable del node robot_state_publisher
        name='robot_state_publisher',
        output='screen',
        parameters= [{'use_sim_time':use_sim_time, 'robot_description':robot_desc}] #Li passa la configuració use_sim_time i la variable on tenim l'arxiu urdf del robot
    )

    pkg_ros_gz_sim = get_package_share_directory('ros_gz_sim')
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_ros_gz_sim, 'launch', 'gz_sim.launch.py')),
        launch_arguments={'gz_args': '-r empty.sdf'}.items(),
    )
    #Declaració del node robot_state_publisher_node (ja ve amb ROS2) -> Publica la posició, orientació, velocitat i acceleració (AQUESTES ÚLTIMES HAS DE CONFIGURAR EL NODE PERQUÈ HO FACI) de les articulacions del teu robot
    start_joint_state_publisher_cmd = Node(
        package='joint_state_publisher', #paquet on es troba el node
        executable='joint_state_publisher', #executable de node
        parameters=[{'use_sim_time': use_sim_time}], #configuració del node
        name='joint_state_publisher' #nom del node
    )
    #Declaració del node que visualitza el model del teu robot a gazebo
    spawn = Node(
        package='ros_gz_sim', #Nom del paquet
        executable='create', #Nom de l'executable que conté el node
        arguments=["-topic","/robot_description", #Li passes el nom del topic del qual llegeix el robot.
                   "-entity",robot_name_in_model, #Li passes el nom del robobt dins de l'urdf que s'està publicant al topic /robot_description
                   "-x",'0.0', #Li passes les coordenades on vols que col·loqui el teu robot.
                   "-y",'0.0',
                   "-z", '0.05',
                   "-Y",'0.0'] 
    )

    # Aquesta part de codi executa el procés de Gazebo per carregar el món i executar la simulació del robot. 
    # En concret, fa servir la funció ExecuteProcess per executar la comanda gazebo, passant-li com a arguments el path fins al fitxer del món (world_path) i diverses opcions com ara --verbose, que fa que s'imprimeixi informació addicional per pantalla, i les llibreries libgazebo_ros_factory.so i libgazebo_ros_init.so, que permeten la integració de ROS amb Gazebo.
    # El paràmetre output indica com s'ha de gestionar la sortida del procés, en aquest cas es mostra per pantalla en temps real (output='screen').

    #Executa els nodes.
    return LaunchDescription([
        declare_use_sim_time_cmd,
        spawn,
        start_joint_state_publisher_cmd, # (!!!!Aquesta linia l'has de comentar quan utilitzis el plugin de Gazebo!!!)
        robot_state_publisher_node,
        gazebo
    ])




























