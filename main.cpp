#include <QCoreApplication>

#include <thread>
#include <utils/timer/timer.h>
#include <clients/vision/visionclient.h>
#include <clients/referee/refereeclient.h>
#include <clients/actuator/actuatorclient.h>
#include <clients/replacer/replacerclient.h>

#include <iostream>

#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/loggers/groot2_publisher.h"

static bool test = false;

static VisionClient *visionClient;
static RefereeClient *refereeClient;
static ReplacerClient *replacerClient;
static ActuatorClient *actuatorClient;

class IsAtack : public BT::SyncActionNode
{
public:
    IsAtack(const std::string& name)
        : BT::SyncActionNode(name, {}) {}

    BT::NodeStatus tick() override
    {
        std::cout << "IsAtack\n";

        fira_message::sim_to_ref::Environment lastEnv = visionClient->getLastEnvironment();
        if(lastEnv.has_frame()) {
            fira_message::Frame lastFrame = lastEnv.frame();

            QString robotDebugStr = QString("Robot %1 -> x: %2 y: %3 ori: %4")
                        .arg(lastFrame.robots_yellow(0).robot_id())
                        .arg(lastFrame.robots_yellow(0).x())
                        .arg(lastFrame.robots_yellow(0).y())
                        .arg(lastFrame.robots_yellow(0).orientation());
                std::cout << robotDebugStr.toStdString() + '\n';

            if(lastFrame.robots_yellow(0).x() < 0.0) {
                return BT::NodeStatus::SUCCESS;
            }
        } 

        return BT::NodeStatus::FAILURE;
    }
};

class Defense : public BT::SyncActionNode
{
public:
    Defense(const std::string& name)
        : BT::SyncActionNode(name, {}) {}

    BT::NodeStatus tick() override
    {
        std::cout << "Defense\n";
        actuatorClient->sendCommand(0, 15, 15);
        return BT::NodeStatus::SUCCESS;
    }
};

class Atack : public BT::SyncActionNode
{
public:
    Atack(const std::string& name)
        : BT::SyncActionNode(name, {}) {}

    BT::NodeStatus tick() override
    {
        std::cout << "Atack\n";
        actuatorClient->sendCommand(0, -15, -15);
        return BT::NodeStatus::SUCCESS;
    }
};

int main(int argc, char *argv[])
{

    BT::BehaviorTreeFactory factory;

    factory.registerNodeType<IsAtack>("IsAtack");
    factory.registerNodeType<Defense>("Defense");
    factory.registerNodeType<Atack>("Atack");

    auto tree = factory.createTreeFromFile("../bht/basic_bht.xml");

    BT::Groot2Publisher publisher(tree);

    Timer timer;

    visionClient = new VisionClient("224.0.0.1", 10002);
    refereeClient = new RefereeClient("224.5.23.2", 10003);
    replacerClient = new ReplacerClient("224.5.23.2", 10004);
    actuatorClient = new ActuatorClient("127.0.0.1", 20011);

    VSSRef::Color ourColor = VSSRef::Color::YELLOW;
    bool ourSideIsLeft = false;

    float freq = 60.0;

    actuatorClient->setTeamColor(ourColor);
    replacerClient->setTeamColor(ourColor);


    while (true)
    {
        visionClient->run();
        refereeClient->run();

        tree.tickOnce(); 
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
