#include "paddle_sync_client/stroke_rate.hpp"

int main(int argc, char** argv) {
    ros::init(argc, argv, "stroke");
    Stroke::StrokeRateNode node;
    
    ros::spin();

    return 0;
}