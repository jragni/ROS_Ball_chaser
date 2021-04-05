

#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <sensor_msgs/LaserScan.h>

// Define a global client that can request services
ros::ServiceClient client;

bool objectClose = false; 


void scan_callback(const sensor_msgs::LaserScan::ConstPtr& scan){
    // check entire scan
    for(auto distance:scan->ranges){
        if(distance < .05){
            objectClose = true;
        }
    }
}

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot--- complete
    ball_chaser::DriveToTarget srv;

    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;
 
    if(!client.call(srv)){
        ROS_ERROR("Failed to send call service ball_chaser");                                                                                                  
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    float max_vel = .1;
    int imageThirds = img.width/3;
    bool foundWhiteBall = false;
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera --- complete
    for(int i = 0; i < img.width*img.height; i++){

        if(img.data[3*i] == white_pixel){
            int j = i%img.width;

            if( j > 2*imageThirds){
                drive_robot(0,max_vel);
            }else if( j > imageThirds){
                drive_robot(max_vel,0);
            }else if( j<=imageThirds){
                drive_robot(0.0,max_vel);
            }

            if(objectClose){
                ROS_WARN("Close to object!");
            } 
            foundWhiteBall = true;
            break;
        }
    }

    if(!foundWhiteBall){
        drive_robot(0.0,0.0);
    }
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Subscribe to /hokuyo_scan
    ros::Subscriber sub2 = n.subscribe("/hokuyo_scan",10, scan_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
