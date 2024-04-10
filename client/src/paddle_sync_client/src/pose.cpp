#include "paddle_sync_client/pose.hpp"

using namespace Pose;

PoseNode::PoseNode(const ros::NodeHandle& nh, const ros::NodeHandle& pnh) : nh_(nh), pnh_(pnh) {
    imu_sub = nh_.subscribe("/imu", 1000, &PoseNode::ImuCallback, this);
    pose_pub = nh_.advertise<geometry_msgs::PoseStamped>("/pose", 1000);
}

void PoseNode::ImuCallback(const sensor_msgs::Imu::ConstPtr& imu) {
    if (prev_time == 0.0) {
        prev_time = imu->header.stamp.toSec();
        return;
    }
    
    geometry_msgs::PoseStamped pose;
    pose.header.frame_id = "world";
    
    dt = imu->header.stamp.toSec() - prev_time;
    prev_time = imu->header.stamp.toSec();

    cur_omega[0] = imu->angular_velocity.x;
    cur_omega[1] = imu->angular_velocity.y;
    cur_omega[2] = imu->angular_velocity.z;
    cur_accl[0] = imu->linear_acceleration.x;
    cur_accl[1] = imu->linear_acceleration.y;
    cur_accl[2] = imu->linear_acceleration.z;

    angle += cur_omega * dt;
    
    q = Eigen::AngleAxisd(angle[0], Eigen::Vector3d::UnitX()) *
        Eigen::AngleAxisd(angle[1], Eigen::Vector3d::UnitY()) *
        Eigen::AngleAxisd(angle[2], Eigen::Vector3d::UnitZ());

    cur_accl = q * cur_accl;
    cur_accl[2] += 9.81;
    pos += vel * dt + 0.5 * cur_accl * dt * dt;
    vel += cur_accl * dt;

    pose.pose.position.x = pos[0];
    pose.pose.position.y = pos[1];
    pose.pose.position.z = pos[2];

    pose.pose.orientation.x = q.x();
    pose.pose.orientation.y = q.y();
    pose.pose.orientation.z = q.z();
    pose.pose.orientation.w = q.w();

    pose.header.stamp = ros::Time::now();
    pose_pub.publish(pose);
}




