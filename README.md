# Shape-Reconfiguration-Distributed-Algorithm
Shape-Reconfiguration-Distributed-Algorithm

authors:
  * [Mohammad SHREM](https://www.linkedin.com/in/mohammadbshreem/) <mohammad.shrem@edu.univ-fcomte.fr>
  * [Mariam AL KHALAF]() <mariam.al_khalaf@edu.univ-fcomte.fr>
  * [Idrissa MASSALY]() <idrissa.massaly@edu.univ-fcomte.fr>

supervisor:
  * [Prof. Benoit PIRANDA](https://www.femto-st.fr/fr/personnel-femto/bpiranda) <benoit.piranda@univ-fcomte.fr>

project relaized in [VisibleSim](https://github.com/VisibleSim/VisibleSim)

---
# HexaHomeworkCode

HexaHomeworkCode is a C++ implementation designed to control and coordinate the behavior of hexagonal modular blocks within a distributed system. This code enables communication, movement, and decision-making among the modules in a simulated environment. The implementation showcases distributed algorithms, event-driven programming, and dynamic motion coordination.

## Features

1. **Distributed Messaging**:

   - Utilizes "Go," "Back," and "Move" messages for coordination.
   - Ensures seamless communication between neighboring modules.

2. **Leader Election**:

   - Identifies and assigns leader responsibilities using the `isLeader` flag.

3. **Motion Scheduling**:

   - Schedules motion events to reposition modules dynamically.

4. **Target Detection**:

   - Implements functions to check if a module or position is within the target region.

5. **Dynamic Behavior**:

   - Callback functions handle various message types and adapt module behavior based on the system's state.

## Code Structure

### Constructor

The constructor initializes the `HexaHomeworkCode` class, registering callbacks for message handling and setting up initial conditions.

### Methods

#### 1. **startup()**

Initializes the module, setting its color and initiating message propagation or motion based on its role.

#### 2. **myGoFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface* sender)**

Handles "Go" messages to propagate distance values and establish parent-child relationships among modules.

#### 3. **myBackFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface* sender)**

Processes "Back" messages to aggregate information and determine the farthest node.

#### 4. **myMoveFunc(std::shared_ptr<Message> _msg, P2PNetworkInterface* sender)**

Handles "Move" messages, coordinating module movements toward the designated target.

#### 5. **parseUserBlockElements(TiXmlElement *config)**

Parses configuration data to initialize leader status from an XML configuration file.

#### 6. **onMotionEnd()**

Handles logic after a motion ends, determining whether further actions or motions are needed.

#### 7. **Helper Functions**

- **isInTarget()**: Checks if the module is in the target region.
- **isPositionInTarget(Cell3DPosition position)**: Verifies if a specific position is within the target.
- **isConnector()**: Identifies if the module can connect to an available position in the target.
- **onInterfaceDraw()**: Returns a string indicating the number of motions performed.

## Pseudo Code

### Methods

#### startup()
```
Function startup():
    Print module ID
    If module is in target:
        Set color to ORANGE
    If module is leader:
        Set color to BLUE
        Set distance to 0
        If in target:
            Send "Go" message to all neighbors
        Else:
            Identify clockwise motion direction
            Schedule motion event
    Else:
        Set distance to -1
```

#### myGoFunc()
```
Function myGoFunc(message, sender):
    If parent is NULL:
        Update distance
        Set sender as parent
        Send "Go" message to neighbors
        If no waiting messages:
            If module is not in target:
                Send "Back" message with module ID and distance
            Else:
                Send "Back" message with -1, -1
    Else:
        Send "Back" message with -1, -1
```

#### myBackFunc()
```
Function myBackFunc(message, sender):
    Decrement waitingMessage
    Update farthestNode if applicable
    If no waiting messages:
        If leader:
            Send "Move" message with farthestNode ID
            Set leader to False
            Set color to ORANGE
        Else:
            Send "Back" message to parent with appropriate data
```

#### myMoveFunc()
```
Function myMoveFunc(message, sender):
    If message already received:
        Return
    Set color based on position
    If module ID matches message ID:
        Schedule motion event to target
    Propagate "Move" message to neighbors
```

#### onMotionEnd()
```
Function onMotionEnd():
    Increment motionCounter
    If not clockwise:
        Set leader to True
        Set color to ORANGE
        Send "Go" message to neighbors
        Return
    If module is in target and neighbor count decreases:
        Identify counterclockwise motion
        Schedule motion event
        Return
    Else:
        Identify clockwise motion
        Schedule motion event
        If returned to initial position:
            Return
```

### Helper Functions

#### isInTarget()
```
Function isInTarget():
    Return True if module position is within target
```

#### isPositionInTarget()
```
Function isPositionInTarget(position):
    Return True if given position is within target
```

#### isConnector()
```
Function isConnector():
    Iterate over neighbors:
        If neighbor is free and in target:
            Return True, neighbor position
    Return False, (0,0,0)
```

#### onInterfaceDraw()
```
Function onInterfaceDraw():
    Return "Number of motions: " + motionCounter
```

## Block Colors

| **Color**   | **Meaning**                           |
|-------------|---------------------------------------|
| ORANGE      | Module is in the target region.       |
| BLUE        | Module is a leader.                  |
| YELLOW      | Module is not in target and exploring.|
| WHITE       | Module is not in target or leader.    |
| GREEN       | Module reached the destination block. |


## Result for "Shape-Reconfiguration-Distributed-Algorithm"

<div align="center">
<img src="https://github.com/user-attachments/assets/fbca9405-bd43-493e-a957-4752523dc197" alt="Result of the Program" width="600">
<br>
<i>Figure 1: L1 Confugration</i>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/4bc95a37-b104-48f2-900c-f47286469eef" alt="Result of the Program" width="600">
<br>
<i>Figure 2: L2 Confugration</i>
</div>

<div align="center">
<a href="https://github.com/user-attachments/assets/f6ddb675-a8a5-4fdf-b5d9-504c219f5234">
<img src="https://github.com/user-attachments/assets/81f07e91-d9f1-4507-a270-3c04f7a5728a" alt="Result on Second Configuration" width="600">
</a>
<br>
<i>Figure 3: Result of the Program on L1/L2 Configuration</i>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/694bd5f5-3868-47dc-8480-7b4f41a61772" alt="Result of the Program" width="600">
<br>
<i>Figure 4: Cylinder Confugration</i>
</div>

<div align="center">
<img src="https://github.com/user-attachments/assets/4d381a1a-da59-442d-89a5-46ea54293d2b" alt="Result of the Program" width="600">
<br>
<i>Figure 5: Cylinder Hole Confugration</i>
</div>

<div align="center">
<a href="https://github.com/user-attachments/assets/1ecfb2b4-de8b-4f7e-8c14-705ca448020c">
<img src="https://github.com/user-attachments/assets/2d15acac-d92a-43fc-9e5d-eb8410ced2d3" alt="Result on Second Configuration" width="600">
</a>
<br>
<i>Figure 6: Result of the Program on L1/L2 Configuration</i>
</div>







