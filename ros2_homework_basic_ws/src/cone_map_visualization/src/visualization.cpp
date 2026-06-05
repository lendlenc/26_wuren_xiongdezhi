#include <string>
#include <chrono>
#include <memory>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker_array.hpp" 
#include "fsd_common_msgs/msg/map.hpp"

// 由于对marker不是很了解，所以通过AI来辅助学习怎么编写
using namespace std::chrono_literals;

class MapVisualizer : public rclcpp::Node
{
public:
    MapVisualizer() : Node("map_visualizer")
    {
        // 订阅 Rosbag 发布的赛道地图话题
        map_sub_ = this->create_subscription<fsd_common_msgs::msg::Map>(
            "/estimation/slam/map", 10, std::bind(&MapVisualizer::mapCallback, this, std::placeholders::_1));

        // 发布用于在 RViz 里显示的 3D 锥桶数组
        marker_pub_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("visualization_marker_array", 10);

    }

private:
    void mapCallback(const fsd_common_msgs::msg::Map::SharedPtr msg)
    {
        visualization_msgs::msg::MarkerArray marker_array;
        int id = 0; 

        auto process_cones = [&](const auto & cone_list, float r, float g, float b, const std::string & ns) 
        {
            for (const auto & cone : cone_list) 
            {
                visualization_msgs::msg::Marker marker;
                
                marker.header.frame_id = "odom";
                marker.header.stamp = this->now();
                marker.ns = ns;
                marker.id = id++; 
                
                marker.type = visualization_msgs::msg::Marker::CYLINDER; // 圆柱体
                marker.action = visualization_msgs::msg::Marker::ADD;

                marker.pose.position.x = cone.position.x; 
                marker.pose.position.y = cone.position.y;
                marker.pose.position.z = cone.position.z; 
                // 直径 0.2 米，高 0.4 米
                marker.scale.x = 0.2;
                marker.scale.y = 0.2;
                marker.scale.z = 0.4;

                // 赋予对应的颜色
                marker.color.r = r;
                marker.color.g = g;
                marker.color.b = b;
                marker.color.a = 1.0f;           

                marker.pose.orientation.w = 1.0;
                marker.lifetime = rclcpp::Duration::from_seconds(0.5); 
                marker_array.markers.push_back(marker);
            }
        };

        process_cones(msg->cone_blue,    0.0f, 0.0f, 1.0f, "blue_cones");    // 左：蓝色
        process_cones(msg->cone_yellow,  1.0f, 1.0f, 0.0f, "yellow_cones");  // 右：黄色
        process_cones(msg->cone_red,     1.0f, 0.0f, 0.0f, "red_cones");     // 起终点：红色
        process_cones(msg->cone_unknown, 0.5f, 0.5f, 0.5f, "unknown_cones"); // 未知：灰色

        // 最终发布
        if (!marker_array.markers.empty()) {
            marker_pub_->publish(marker_array);
        }
    }

    rclcpp::Subscription<fsd_common_msgs::msg::Map>::SharedPtr map_sub_;
    rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_pub_;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MapVisualizer>());
    rclcpp::shutdown();
    return 0;
}