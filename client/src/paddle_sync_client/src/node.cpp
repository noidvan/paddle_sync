#include "paddle_sync_client/pose.hpp"

int main(int argc, char** argv) {
    ros::init(argc, argv, "pose");
    Pose::PoseNode node;
    
    ros::spin();

    return 0;
}