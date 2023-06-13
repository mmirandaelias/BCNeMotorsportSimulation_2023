# BCNeMotorsportSimulation_2023
Aquest programa préten ser un simulador per a cotxes que segueixen un model dinàmic de 4 rodes, el qual és controlat per un controlador amb les següents sortides:
- Angle de gir del volant.
- Acceleració al padal. 
## Estructura:
- model_cat: package principal
  - launch: conté els arxius python per executar la visualització del simulador.
    - visualitza_moelcibicletaCAT15x.py: el visualitza amb rviz2.
    - visualitza_modelbicicletaCAT15x_gazebo.py: el visualitza amb gazebo (actualment conté un bug i no està disponible).
  - meshes: conté els elements sdf per visualitza el cotxe.
    - wheelFL/FR/RL/RR: elements de cad aun dels pneumàtics.
    - steering_hinge: conjunt roda.
    - whole_car: monocasc i aerodinàmica.
  - Parameters: conté el document YALM per definir els diferents paràmetres del model i de la simulació.
  - plugins: conté el plugin amb el model del cotxe i el càlcul de forces.
    - include:
      - CalculEstat.hh
      - CalculForces.hh
      - Parametres.hh
      - Pluggin.hh
      - Update.hh
    - src
      - CalculEstat.cpp: conté els mètodes amb les equacions d'estat del vehicle.
      - CalculForces.cpp: conté els mètodes pel càlcul de forces (necesàries per calcular l'estat) del vehicle.
      - Parametres.cpp: conté el mètode que transforma els paràmetres escrits a l'arxiu YALM per ser cridats en l'execució del codi.
      - Pluggin.cpp: conté el pluggin del vehicle (amb les interfícies necessàries) que s'dajuntarà al robot. 
      - Update.cpp: conté el mètode que s'actualitzarà a cada iteració del càlcul d'estats.
    -urdf: conté els diferents robots que simulen el vehicle:
      - model4WCAT15x.urdf: conté la definició del robot amb el pluggin inclòs.
## Llibreries utilitzades i execució:
- Per executar el codi és necesari disposar de:
  - ROS2 humble.
  - Ignition Gazebo (Fortress).
- Per compilar i executar la simulació es recomana:
  - Compilació: des del ws: 
   "source /opt/ros/humble/setup.bash
   "colcon build"
   "source install/setup.bash"
  - Execució des de la carpeta launch:
   "ros2 launch visualitza_moelcibicletaCAT15x.py"
   
  
