#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <camera_info_manager/camera_info_manager.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Header.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle nh;
    image_transport::CameraPublisher imagePub;
    image_transport::ImageTransport it(nh);
    imagePub = it.advertiseCamera("camera/image_raw", 1);
    boost::shared_ptr<camera_info_manager::CameraInfoManager> cameraInfoManager;


    cameraInfoManager.reset(new camera_info_manager::CameraInfoManager(nh, "camera", "package://image_publisher/calibration/camera_calibration.yaml"));
    cv::VideoCapture cap(0);
    // Check if video device can be opened with the given index
    if(!cap.isOpened()){
        ROS_INFO("couldn't connect to /dev/video0");
        return 1;   
    }
    cv::Mat frame;
    sensor_msgs::ImagePtr image;
    sensor_msgs::CameraInfoPtr cameraInfo(new sensor_msgs::CameraInfo(cameraInfoManager->getCameraInfo()));
    std_msgs::Header header;
    int fps;
    std::string nameOfNode = ros::this_node::getName();
    nh.param(nameOfNode+ "/fps", fps, 20);
    ros::Rate loop_rate(fps);
    ROS_INFO("fps set to %d", fps);
    header.frame_id = "camera";
    cameraInfo->header.frame_id = header.frame_id;
    while (nh.ok()) {
        cap >> frame;
        // Check if grabbed frame is actually full with some content
        if(!frame.empty()) {
            header.stamp = ros::Time::now();
            image = cv_bridge::CvImage(header, "bgr8", frame).toImageMsg();
            cameraInfo->header.stamp = header.stamp;
            imagePub.publish(*image, *cameraInfo);
            // cv::waitKey(1);
        }
        else {
            ROS_INFO("couldn't capture frame");
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
}