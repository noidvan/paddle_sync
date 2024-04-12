#include "paddle_sync_client/stroke_rate.hpp"

using namespace Stroke;

StrokeRateNode::StrokeRateNode(const ros::NodeHandle& nh, const ros::NodeHandle& pnh) : nh_(nh), pnh_(pnh) {
    imu_sub = nh_.subscribe("/imu", 1000, &StrokeRateNode::ImuCallback, this);
    stroke_rate_pub = nh_.advertise<std_msgs::Int64>("/stroke_rate", 1000);
}

void StrokeRateNode::ImuCallback(const sensor_msgs::Imu::ConstPtr& imu) {
    cur_accel = imu->linear_acceleration.z;
    if (cur_accel >= 2.0 && (imu->header.stamp.toSec() - prev_time) >= 0.5) {
        dt = imu->header.stamp.toSec() - prev_time;
        prev_time = imu->header.stamp.toSec();

        dt_arr.insert(dt_arr.begin(), dt);
        dt_arr.pop_back();

        sum = 0.0;
        for (int i =0; i<5; i++) {
            sum += dt_arr[i];
        }
        cur_stroke_rate = 60 / (sum / 5);
        
        stroke.data = cur_stroke_rate;
        stroke_rate_pub.publish(stroke);
        ROS_INFO("Stroke detected at %f", prev_time);
        ROS_INFO("Current Stroke rate in [rpm]: %d", cur_stroke_rate);
    } else {
        stroke_rate_pub.publish(stroke);
    }
}