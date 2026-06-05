from launch import LaunchDescription
from launch.actions import ExecuteProcess  # 引入系统命令执行器
from launch_ros.actions import Node

def generate_launch_description():
    
    # 地图可视化节点
    map_visualizer_node = Node(
        package='cone_map_visualization',     # 功能包名
        executable='map_visualizer',         # 可执行文件名（CMakeLists 里面指定的）
        name='map_visualizer_node',          # 节点在系统里的别名
        output='screen',                     # 让 RCLCPP_INFO 能够打印在主终端里
        parameters=[{'use_sim_time': True}]
    )

    # rviz节点
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        parameters=[{'use_sim_time': True}]
    )

    # 绝对路径
    play_bag_cmd = ExecuteProcess(
        cmd=[
            'ros2', 'bag', 'play', 
            '/home/lend/work_place/Foxmax-intership/ROS2课程资料/ros2_learning-main/ros2_homework_advanced/map_to_visualize/'
            '--loop'
            '--clock'
        ],
        output='screen'
    )

    return LaunchDescription([
        play_bag_cmd,          
        map_visualizer_node,   
        rviz_node,             
    ])