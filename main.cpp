/*
for UDP config
mbed_app.json

{
    "target_overrides": {
        "*": {
            "platform.stdio-convert-newlines": true,
            "target.network-default-interface-type": "ETHERNET"
        }
    }
}
*/

#include "mbed.h"
#include "ros2mbed.h"
#include "subscription.hpp"
#include "publisher.hpp"
#include "Twist.hpp"
#include <memory>
#if USER_TRANSPORT_TYPE == 0 || USER_TRANSPORT_TYPE == 1
#include "EthernetInterface.h"
#endif 

#include <string>


class sample_twist : public ros2::Node
{
public:
    sample_twist()
    : Node("sample_twist"), count_(0)
    {
        pub_twist = this->create_publisher<geometry_msgs::Twist>("turtle1/cmd_vel");

        sub_twist = this->create_subscription<geometry_msgs::Twist>(
            "sub_vel",
            [&](std::shared_ptr<geometry_msgs::Twist> msg)
            {
                auto send_data = std::make_shared<geometry_msgs::Twist>();
                send_data->linear.x = msg->linear.x;
                send_data->linear.y = msg->linear.y;
                send_data->angular.y = msg->angular.x * 5;

                pub_twist->publish(send_data);
            });

    }

private:
    int count_;
    ros2::Publisher<geometry_msgs::Twist>::SharedPtr pub_twist;
    ros2::Subscription<geometry_msgs::Twist>::SharedPtr sub_twist;
    
    ros2::TimerBase::SharedPtr timer_;
};


int main()
{
#if USER_TRANSPORT_TYPE == 0 || USER_TRANSPORT_TYPE == 1
    EthernetInterface net;
    ros2::init(&net, "192.168.2.101", 2018);
#endif 

#if USER_TRANSPORT_TYPE == 2
    BufferedSerial pc(USBTX, USBRX, 115200);
    ros2::init(&pc);
#endif

    sample_twist sample_twist;
    ros2::spin(&sample_twist);
}