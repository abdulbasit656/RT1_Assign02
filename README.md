# Research Track 01 Assignment 02
-----------------------------------

ROS Based F1 Track Car Drive
-------------------------------
This ROS based assignment aims to control a simple robot that runs along a Track, equipped with a laser scanner, and with the possibility to increase or decrease its velocity and reset to the initial position.

Installing and Running
------------------------------
Clone /rt1_assignment2 repository in the src folder of your ROS workspace, compile it with catkin_make and run all commands in seperate terminals:

```bash
$ roscore
$ roslaunch rt1_assignment2 run.launch
$ rosrun rt1_assignment2 user_control
```
![robot](https://user-images.githubusercontent.com/17598805/172030308-5eaae345-90cd-4a48-8479-cac3c0bec09e.png)

Implementation of the ROS Package
-----------------
In this assignment I have used Two ROSNodes (i.e. robot_control & user_control) along with ROS Service (i.e. Teleop_Robot.srv)and ROS Message(i.e. Vel.msg) for the communication between Nodes and a ROS Launch for running multiple nodes at the same time.

ROS Nodes
---------------
![assig02_rosgraph](https://user-images.githubusercontent.com/17598805/172030288-1b9c6d78-94b9-4a53-87e8-ccef2ba9d5ca.png)

### robot_control
The aim to implement robot_control node is to drive the robot autonomously in the given track without hitting the boundary, with a constant speed and to turn to avoid collision. for achieving above goal, it makes a 
Subscribe:
    
    /base_scan(sensor_msgs/LaserScan)       //Published by stage_ros node

/base_scan topic is published by stage_ros node, whose message type is sensor_msgs/LaserScan.msg. In our case, we're interested to get the ranges[] Vector that contains, in the i-th position, the distance from the wall of the i-th laser (721 elements) in a range of 180 degrees. from these ranges i took only three range elements, which are: 
    
    float range_right = msg->ranges[0];
	  float range_center = msg->ranges[360];
	  float range_left = msg->ranges[720];

Publisher:

    cmd_vel (geometry_msgs/Twist)       //to express the velocity of the robot.

cmd_vel topic is used to publish the velocity to the robot using the message type geometry_msgs/Twist.

Service:
    
    teleop_robot(rt1_assignment2/Teleop_Robot)
    
teleop_vel service is basically used to communicate between robot_control and user_control node to increase/decrease velocity and reset position of the robot.

### Stage_ros node
The stage_ros node wraps the Stage 2-D multi-robot simulator via libstage and simulates a world as define in .wold file.

Subscriber:

    cmd_vel (geometry_msgs/Twist): to express the velocity of the robot.

Publisher:

    base_scan (sensor_msgs/LaserScan): scans from the laser model.
    odom (na_msgs/Odometry): odometry data from the position model.
    base_pose_ground_truth (nav_msgs/Odometry): ground truth pose.
    image (sensor_msgs/Image): visual camera image.
    depth (sensor_msgs/Image): depth camera image.
    camera_info(sensor_msgs/CameraInfo): camera calibration info.
![track](https://user-images.githubusercontent.com/17598805/172030298-8bd16187-ee1a-408e-ab14-3458078443e7.png)

### user_control
The user_control node waits until a key is pressed by the user, then asks the racer_node with the Command service to perform the command received.
Here's a list of commands (e.g. keyboard keys) the user can use to control the robot:

    i: increases the speed of the robot, every time 'i' key is pressed the velocity is increased by current_vel*0.1;
    k: decreases the speed of the robot, every time 'k' key is pressed the velocity is decreased by current_vel*0.1;
    s: resets the position of the robot.

### Flowchart
![Assign02](https://user-images.githubusercontent.com/17598805/172031603-65050cda-b402-48d1-b3e0-bc5262c1fde8.png)


### Short video

_________

https://user-images.githubusercontent.com/17598805/172030444-0684c540-8a29-4c2d-88e9-f781885c4480.mp4

___________________
ROS, C++, Obstacle Avoidance
