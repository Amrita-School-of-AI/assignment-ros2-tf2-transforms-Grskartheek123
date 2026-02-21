#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"

using namespace std::chrono_literals;

class TFBroadcaster : public rclcpp::Node
{
public:
  TFBroadcaster() : Node("tf_broadcaster")
  {
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    start_time_ = this->now();

    timer_ = this->create_wall_timer(
      100ms,
      std::bind(&TFBroadcaster::broadcast_tf, this)
    );

    RCLCPP_INFO(this->get_logger(), "TFBroadcaster started.");
  }

private:
  void broadcast_tf()
  {
    // elapsed time in seconds
    double t = (this->now() - start_time_).seconds();

    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = this->now();
    transform.header.frame_id = "world";
    transform.child_frame_id = "robot";

    // circle motion radius = 2.0
    transform.transform.translation.x = 2.0 * std::cos(t);
    transform.transform.translation.y = 2.0 * std::sin(t);
    transform.transform.translation.z = 0.0;

    // identity rotation (no rotation)
    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = 0.0;
    transform.transform.rotation.w = 1.0;

    tf_broadcaster_->sendTransform(transform);
  }

  rclcpp::Time start_time_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TFBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
