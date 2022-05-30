#include "ros/ros.h"
#include "rt1_assignment2/Teleop_Robot.h"
#include "geometry_msgs/Twist.h"
#include "std_srvs/Empty.h"
#include "rt1_assignment2/Vel.h"
#include <iostream>
 
ros::Publisher pub;
ros::Subscriber sub;

ros::ServiceClient client_reset_pos;
ros::ServiceClient client_teleop_robot;

char key = 0;
float linear_vel = 5.0;

int main (int argc, char **argv)
{
	ros::init(argc, argv, "user_control");
	ros::NodeHandle nh;

	pub = nh.advertise<rt1_assignment2::Vel>("/mv_vel",1);

	client_reset_pos = nh.serviceClient<std_srvs::Empty>("/reset_positions"); 	//reset to the default position
	client_teleop_robot = nh.serviceClient<rt1_assignment2::Teleop_Robot>("/teleop_robot"); 	//increase decrease speed of the robot
	
	
	while(true){

    // Get the pressed key
	std::cin>>key;
	ROS_INFO_STREAM("entered character is: " << key);
	
	geometry_msgs::Twist my_vel;
	switch(key){
	
	//increase the velocity of the robot
	case 'i':
	{
		rt1_assignment2::Teleop_Robot tr;
		tr.request.command = 'i';
		
		client_teleop_robot.waitForExistence();
		client_teleop_robot.call(tr);
		my_vel.linear.x = tr.response.current_linear_vel;
		
		rt1_assignment2::Vel new_vel;
		
		linear_vel += linear_vel*0.1;
		new_vel.command = 'i';
		new_vel.vel = my_vel.linear.x;
		pub.publish(new_vel);
	};
	break;

	//decrease the velocity of the robot
	case 'k':
	{
		rt1_assignment2::Teleop_Robot tr;
		tr.request.command = 'k';
		
		client_teleop_robot.waitForExistence();
		client_teleop_robot.call(tr);
		my_vel.linear.x = tr.response.current_linear_vel;
		
		rt1_assignment2::Vel new_vel;
		
		linear_vel += linear_vel*0.1;

		new_vel.command = 'k';
		new_vel.vel = my_vel.linear.x;
		pub.publish(new_vel);
	};
	break;
	
	//reset to default position
	case 's':
	{
		std_srvs::Empty rp;
		client_reset_pos.waitForExistence();
		client_reset_pos.call(rp);
	};
	break;
	default: 
		break;	

    }
	fflush(stdin);
}			
	ros::spin();
	return 0;
}

