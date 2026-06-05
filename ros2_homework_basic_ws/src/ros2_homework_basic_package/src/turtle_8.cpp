#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include <chrono>   // time库
#include <memory>

using namespace std::chrono_literals;

class Turtle8Node: public rclcpp::Node  // 等同于Turtle8Node（node）,可被子类继承这一特性
{
private:
    // timer_是指向 rclcpp::TimerBase（定时器类）的ptr,自动管理
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_; // 发布者的智能指针
    int count_=0;
public: // 类的公开接口函数
    // explicit是显示声明关键词;const,让node_name不可修改;&：直接使用不copy
    // std::string：标准库里的str;黄的是参数;Node(node_name)='super(rclcpp::Node)';":"先做某事
    explicit Turtle8Node(const std::string& node_name):Node(node_name)  // 定义构造函数
    {
        // 'this->'='self.';'<>':限定只能发布xxx;10表示网卡发不出则最多堆叠10条;Twist表示各种速度
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);

        // 给一个函数传参的同时把Node和这个函数绑定，成员函数必须和类的实例绑定才能用
        timer_ = this->create_wall_timer(10ms,std::bind(&Turtle8Node::timer_callback,this));
    }
    void timer_callback()
    {   
        // xyz:前左上
        auto msg = geometry_msgs::msg::Twist();
        msg.linear.x=1.0;
        if (count_<=314){
            msg.angular.z=2;
        }
        else{
            msg.angular.z=-2;
        }
        publisher_->publish(msg);
        count_++;
    }
};

int main(int argc,char* argv[])
{
    rclcpp::init(argc,argv);
    
    // make_shared是创建SharedPtr的方法,这里在创建节点
    auto node =std::make_shared<Turtle8Node>("turtle_8");
    rclcpp::spin(node); // 每次启动就会调用一次void
    rclcpp::shutdown();
    return 0;
}