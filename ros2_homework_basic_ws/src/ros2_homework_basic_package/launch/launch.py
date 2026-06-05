import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # 1. 获取你当前功能包的路径
    package_dir = get_package_share_directory('ros2_homework_basic_package')
    yaml_config_path = os.path.join(package_dir, 'config', 'turtle_config.yaml')

    # 2. 启动小乌龟原生界面节点
    turtlesim_node = Node(
        package='turtlesim',
        executable='turtlesim_node',
        name='turtlesim_node',
        output='screen'
    )

    # 3. 启动你写的控制节点（加载 YAML 参数）
    turtle_controller_node = Node(
        package='ros2_homework_basic_package',
        executable='turtle_8',  
        name='turtle_8',       
        parameters=[yaml_config_path],   
        output='screen'
    )

    return LaunchDescription([
        turtlesim_node,
        turtle_controller_node
    ])