#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "rt1_assignment2/Teleop_Robot.h"
 
ros::Publisher pub;
ros::Subscriber sub;
ros::ServiceServer service;

float linear_vel = 5.0;		//initial linear velocy of robot

/*------callback function is used to get the laserscanner data and 
	publish the velocity according to the given conditions-----*/

void Callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
	float range_right = msg->ranges[0];		//index [0] has the right most value of laser scanner at 0 degree
	float range_center = msg->ranges[360];	//index [360] has the front value of laser scanner at 90 degree	
	float range_left = msg->ranges[720];		//index [720] has the right most value of laser scanner at 180 degree
	
	ROS_INFO("Range Ahead = %0.2f, Range Left = %0.2f, Range Right = %0.2f", range_center, range_left, range_right);
	
	geometry_msgs::Twist my_vel;
	
	float th_front = 0.6;
	float th_lt_rt = 0.4;	
	
	if(range_left > range_right && range_left > range_center || range_right < th_lt_rt)			//turn left
	{		
		my_vel.linear.x = 0.02*linear_vel;
		my_vel.angular.z = 10.0*linear_vel;
		ROS_INFO("TURN LEFT");
	}
	else if(range_right > range_left && range_right > range_center || range_left < th_lt_rt)		//turn right
	{
		my_vel.linear.x = 0.02*linear_vel;
		my_vel.angular.z = -10.0*linear_vel;
		ROS_INFO("TURN RIGHT");
	}
	else if (range_center > range_left && range_center > range_right || th_front) 	//go forward
	{
		my_vel.linear.x = 0.3*linear_vel;
		my_vel.angular.z = 0.0*linear_vel;
		ROS_INFO("GO FORWARD!");
	}
	else{
		ROS_INFO("ROBOT NOT MOVING...!");
	}	
	pub.publish(my_vel);
	
}

/*------teleop_callback function is used to get the commands from user_control node and 
	increase/decrease the velocity of the robot-----*/
bool teleop_callback(rt1_assignment2::Teleop_Robot::Request &req, rt1_assignment2::Teleop_Robot::Response &res)
{	
	if(req.command=='i')
	{
		linear_vel += linear_vel*0.1;
		res.current_linear_vel = linear_vel;
		
	}
	else if (req.command=='k')
	{
		linear_vel -= linear_vel*0.1;
		res.current_linear_vel = linear_vel;		
	
	}

return 1;

}


int main (int argc, char **argv)
{
	ros::init(argc, argv, "robot_control");
	ros::NodeHandle nh;

	pub = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);	//publishing linear and angular velocities	
		
	sub = nh.subscribe("/base_scan", 1, Callback);		//subscribing node to get the laser scanner data
	service = nh.advertiseService("/teleop_robot", teleop_callback);	//teleop robot service
	
	ros::spin();
	return 0;
}

