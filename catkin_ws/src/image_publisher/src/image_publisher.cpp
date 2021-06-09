#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Header.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("camera/image", 0);


    cv::VideoCapture cap(0);
    // Check if video device can be opened with the given index
    if(!cap.isOpened()){
        ROS_INFO("couldn't connect to /dev/video0");
        return 1;   
    }
    cv::Mat frame;
    sensor_msgs::ImagePtr msg;
    std_msgs::Header header;
    ros::Rate loop_rate(15);
    header.frame_id = "camera";
    while (nh.ok()) {
        cap >> frame;
        // Check if grabbed frame is actually full with some content
        if(!frame.empty()) {
            header.stamp = ros::Time::now();
            msg = cv_bridge::CvImage(header, "bgr8", frame).toImageMsg();
            pub.publish(msg);
            // cv::waitKey(1);
        }
        else {
            ROS_INFO("couldn't capture frame");
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
}