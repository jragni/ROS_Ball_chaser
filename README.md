# ROS_Ball_chaser
A ROS package that simulates a differential-drive robot in Gazebo. Upon launch, the robot will use camera and lidar data to move to the white ball's location. 

    This ROS package allows a robot to follow a white ball.

#This package:

    Creates the node drive_bot with service /ball_chaser/command_robot that receives a \Twist message and publishes velocities to /cmd_vel
    Creates the node process_image that subscribes to /camera/rgb/image_raw. Depending on the position of a white ball, triggers the service /ball_chaser/command_robot and moves the robot left, right or forward (See image below).

Use this package with my robot repository.
Requirements

    ROS Noetic or up.
    Gazebo 7.16.0 or up.

Running

    Build the package:

$ catkin_make

    Source your environment:

$ source devel/setup.bash

    Run launch file:

roslaunch ball_chaser ball_chaser.launch
