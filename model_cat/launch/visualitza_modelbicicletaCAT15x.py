import os
import launch
# from launch.substitution import LaunchConfiguration
import launch_ros
from sensor_msgs.msg import JointState
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    ## CÀRREGA DEL TEU ARXIU URDF ##
    urdf_file_name = 'modelbicicletaCAT15x.urdf'
    urdf = os.path.join( #Comanda necessària per carregar el teu urdf
        get_package_share_directory('model_cat'),
        'urdf',
        urdf_file_name) #Li dones la direcció d'on es troba el teu urdf
    with open(urdf,'r') as infp: #Llegeix el teu arxiu urdf i s'assegura que es tanca automàticament i sense errors després de llegirlo. La r indica que l'obra en mode lectura. 
        robot_desc = infp.read() #Emmagatzema el contingut de l'arxiu a la variable robot_desc.
    
    ## PUBLICACIÓ DEL TEU MODEL A TRAVÉS DELS NODES INCORPORATS A ROS QUE PERMETEN LA CONNEXIÓ ENTRE URDF, ROS I EL VISUALITZADOR DE ROS RVIZ ##
    
    #CREACIÓ DELS NODES:
    #Utilitza la informació proporcionada en l'urdf per calcular i publicar les transformacions que descriuen
    #la orientació i la posició de les diferents articulacions del robot que s'han descrit amb diferents sistemes de coordenades.
    robot_state_publisher_node = launch_ros.actions.Node(
        package='robot_state_publisher', #Paquet que ja ve amb ROS
        executable='robot_state_publisher', #Executable que ja ve amb ROS
        name='robot_state_publisher', #Node que ja ve amb ROS
        output='screen', #On redigirà l'output del node (en aquest cas la terminal).
        parameters=[{'robot_description':robot_desc}],#Li dones larxiu 'urdf'
        arguments=[urdf] #Ubicació de l'arxiu (el path).
    )

    #Aquest Node et publica les dades d'estat dels joints d'un robot. EL node rep les dades de posició, velocitat, acceleració,... 
    joint_state_publisher_node = launch_ros.actions.Node(
        package='joint_state_publisher', #Paquet que ja ve amb ROS
        executable='joint_state_publisher', #Executable que ja ve amb ROS
        name='joint_state_publisher', #Node que ja ve amb ROS
       # condition=launch.conditions.UnlessConditions(LaunchConfiguration('gui')) #Et comprova si es dona aquesta condició per llençar el node. En aquest cas, la condició és que a gui no hi hagi un arxiu de preconfirguració. Gui es refereix a la interfície gràfica.
    )

    #El node joint_state_publisher_gui és una interfície gràfica d'usuari que permet als usuaris editar les dades d'estat dels joints del robot a temps real.
    joint_state_publisher_gui_node = launch_ros.actions.Node(
        package='joint_state_publisher_gui', #Paquet que ja ve amb ROS
        executable='joint_state_publisher_gui', #Executable que ja ve amb ROS
        name='joint_state_publisher_gui', #Node que ja ve amb ROS
        #condition =launch.conditions.IfConditions(LaunchConfiguration('gui')) #Comprova que tinguis la gui oberta. EN el cas de ROS2 -> gui = rviz
    )

    #Crea el node rviz2
    rviz_node = launch_ros.actions.Node(
        package = 'rviz2',#Paquet que ja ve amb ROS
        executable='rviz2',#Executable que ja ve amb ROS
        name='rviz2',#Node que ja ve amb ROS
        output='screen'
    )

    #LLANÇAMENT DELS NODES
    return launch.LaunchDescription(
        [
        launch.actions.DeclareLaunchArgument(name='gui', default_value='True',description='Flag to enable joint_state_publisher_gui'), #Et dona el valor per defecte de gui com a true i és una argument que es donarà a tots els nodes.
        #LLANÇA ELS NODES QUE HEM CREAT:
        joint_state_publisher_node, 
        joint_state_publisher_gui_node,
        robot_state_publisher_node,
        rviz_node,
        ]
    )

