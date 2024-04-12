#ifndef STROKE_H
#define STROKE_H

#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Int64.h>
#include <vector>

namespace Stroke
{
    class StrokeRateNode {
        private:
        ros::NodeHandle nh_;
        ros::NodeHandle pnh_;

        ros::Subscriber imu_sub;
        ros::Publisher stroke_rate_pub;
        void ImuCallback(const sensor_msgs::Imu::ConstPtr& imu);

        std::vector<double> dt_arr{0.0, 0.0, 0.0, 0.0, 0.0};

        double prev_time;
        double dt;

        double cur_accel;
        double sum;
        int cur_stroke_rate{0};

        public:
        StrokeRateNode(const ros::NodeHandle& nh, const ros::NodeHandle& pnh);
        StrokeRateNode() : StrokeRateNode(ros::NodeHandle(), ros::NodeHandle("~")) {}

        ~StrokeRateNode() {}

        std_msgs::Int64 stroke;
    };
} // namespace Stroke


#endif