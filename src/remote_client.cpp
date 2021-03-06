#include "udp_bridge/udp_bridge.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "geographic_msgs/GeoPointStamped.h"
#include "marine_msgs/Heartbeat.h"
#include "marine_msgs/Contact.h"
#include "marine_msgs/NavEulerStamped.h"
#include "marine_msgs/RadarSectorStamped.h"
#include "diagnostic_msgs/DiagnosticArray.h"
#include "sensor_msgs/NavSatFix.h"
#include "sensor_msgs/PointCloud.h"
#include "marine_msgs/Helm.h"
#include "geometry_msgs/TwistStamped.h"
#include "geographic_msgs/GeoPath.h"
#include "geographic_visualization_msgs/GeoVizItem.h"
#include <regex>
#include "boost/date_time/posix_time/posix_time.hpp"

std::map<udp_bridge::Channel,std::string> udp_bridge::UDPROSNode::topic_map;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "udp_bridge_remote_client");

    std::string host = "localhost";
    if (argc > 1)
        host = argv[1];

    int send_port = 4203;
    int receive_port = 4202;
    
    udp_bridge::UDPROSNode n(host,send_port,receive_port);
    
    n.addReceiver<geographic_msgs::GeoPointStamped,udp_bridge::position>("/udp/position");
    n.addReceiver<geographic_msgs::GeoPoint,udp_bridge::origin>("/udp/origin");
    n.addReceiver<marine_msgs::Heartbeat,udp_bridge::heartbeat>("/udp/heartbeat");
    n.addReceiver<marine_msgs::NavEulerStamped, udp_bridge::heading>("/udp/heading");
    n.addReceiver<marine_msgs::Contact, udp_bridge::contact>("/udp/contact");
    n.addReceiver<marine_msgs::NavEulerStamped,udp_bridge::posmv_orientation>("/udp/posmv/orientation");
    n.addReceiver<sensor_msgs::NavSatFix,udp_bridge::posmv_position>("/udp/posmv/position");
    n.addReceiver<geometry_msgs::TwistStamped,udp_bridge::sog>("/udp/sog");
    n.addReceiver<marine_msgs::RadarSectorStamped,udp_bridge::radar>("/udp/radar");
    n.addReceiver<geographic_visualization_msgs::GeoVizItem, udp_bridge::display>("/udp/project11/display");

    n.spin();

    return 0;
}
