#include "planner.h"

int main(int argc, char **argv) {

    ros::init(argc, argv, "finder");
    ros::NodeHandle n("~koenig");
    nav_msgs::GetMap srv_map;
    nav_msgs::Path custom_path;
    custom_path.header.frame_id = "map";
    planner custom(&custom_path);
    
    //init Server
    Server ser(n, "jonny_plan",boost::bind(&planner::server_callback, &custom, _1, &ser), false);
    ROS_INFO("goal server init");
    
    //init map Client
    ros::ServiceClient map_service_client = n.serviceClient<nav_msgs::GetMap>("/static_map");
    ROS_INFO("map client init");

    //init plan publisher
    ros::Publisher path = n.advertise<nav_msgs::Path>("/custom_path", 1);
    ROS_INFO("pub init");

    //get map
    map_service_client.waitForExistence();
    ROS_INFO("mapserver available");
    if (map_service_client.call(srv_map))
    {
        ROS_INFO("Map service called successfully");
        //load map into planner
        custom.set_map(srv_map.response.map);
        ROS_INFO("loaded map into planner");
    }
    else
    {
        ROS_ERROR("Failed to call map service");
        return 0;
    }
    
    // start server
    ser.start();

    //wait until path was calculaded
    //while (custom_path.poses.empty());
    ros::Rate loop_rate(1);
    while(ros::ok() && custom_path.poses.empty()){
        ros::spinOnce();
    }

    for(int i = 0; i < custom_path.poses.size(); i++){
        ROS_INFO_STREAM("X: " << custom_path.poses[i].pose.position.x); 
        ROS_INFO_STREAM("Y: " << custom_path.poses[i].pose.position.y); 
    }

    ROS_INFO("publishing path");
    
    while(ros::ok()){
        path.publish(custom_path);
        ros::spinOnce();
    }
    ros::spin();
    return 0;
}