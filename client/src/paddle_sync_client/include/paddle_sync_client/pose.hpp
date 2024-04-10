#ifndef POSE_H
#define POSE_H

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PoseStamped.h>

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

namespace Pose {
class PoseNode {
private:
    ros::NodeHandle nh_;
    ros::NodeHandle pnh_;

    ros::Subscriber imu_sub;
    ros::Publisher pose_pub;
    void ImuCallback(const sensor_msgs::Imu::ConstPtr& imu);

    double prev_time{0.0};
    double dt;

    Eigen::Vector3d cur_omega;
    Eigen::Vector3d cur_accl;

    Eigen::Quaterniond q;
    Eigen::Vector3d angle{0, 0, 0};
    Eigen::Vector3d vel{0, 0, 0};
    Eigen::Vector3d pos{0, 0, 0};

public:
    PoseNode(const ros::NodeHandle& nh, const ros::NodeHandle& pnh);
    PoseNode() : PoseNode(ros::NodeHandle(), ros::NodeHandle("~")) {}

    ~PoseNode() {}
};
} // end of namespace Pose
#endif