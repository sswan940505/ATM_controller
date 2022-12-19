
#include <ros/ros.h>
#include <std_msgs/String.h>

#include "ATMcontroller/ATMcontroller.h"

ATMController * controller;

void cardCallback(const std_msgs::String::ConstPtr& msg)
{
  if(controller->isCardInserted()) ROS_INFO("CARD ALREADY INSERTED.");
  else {
    ROS_INFO("[ATM Machine] CARD INSERT: %s", msg->data.c_str());
    controller->insertCard(msg->data);
  }
}

void pinCallback(const std_msgs::String::ConstPtr& msg)
{
  if(controller->isCardInserted())
  {
    if(controller->isPinNeeded())
    {
      ROS_INFO("[ATM Machine] PIN INSERT: %s", msg->data.c_str());
      ROS_INFO("[ATM Machine] WAIT FOR CHECKING PIN");
      controller->insertPIN(msg->data);
    }
    else {
      ROS_INFO("[ATM Machine] PIN ALREADY INSERTED");
    }
  }
  else{
    ROS_INFO("[ATM Machine] CARD NOT INSERTED.");
  }
}

void selectCallback(const std_msgs::Int32::ConstPtr& msg)
{
  if(controller->isAccountSelected())
  {
    ROS_INFO("[ATM Machine] Account already selected");
  }
  else if(controller->selectAccount(msg->data))
  {
    ROS_INFO("[ATM Machine] Accont %d selected",msg->data);
  }
  else
  {
    ROS_INFO("[ATM Machine] Account not exist. Type again");
  }
}

void menuCallback(const std_msgs::Int32::ConstPtr& msg)
{
  if(msg->data==0)
  {
    ROS_INFO("[ATM Machine] BALANCE : %d",controller->checkBalance());
    ROS_INFO("[ATM Machine] THANK YOU FOR USING OUR ATM. RETURN YOUR CARD.");
  }
  else if(msg->data==1)
  {
    ROS_INFO("[ATM Machine] DEPOSIT BEGIN (OPEN CASHBIN)");
  }
  else if(msg->data==2)
  {
    ROS_INFO("[ATM Machine] WITHDRAWAL BEGIN (INSERT AMOUNT)");
  }
}


void cashCallback(const std_msgs::Int32::ConstPtr& msg)
{
  if(controller->insertCash(msg->data))
  {
    ROS_INFO("[ATM Machine] WITHDRAW / DEPOSIT FINISHED. WAIT FOR PROCESSING..");
  }
  else
  {
    ROS_INFO("[ATM Machine] CANNOT WITHDRAW CASH OVER BALANCE");
  }
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "atmTester");
  ros::NodeHandle n;
  ros::Subscriber sub1 = n.subscribe("IN/card", 1, cardCallback);
  ros::Subscriber sub2 = n.subscribe("IN/pin", 1, pinCallback);
  ros::Subscriber sub3 = n.subscribe("IN/selection", 1, selectCallback);
  ros::Subscriber sub4 = n.subscribe("IN/menu", 1, menuCallback);
  ros::Subscriber sub5 = n.subscribe("IN/cash", 1, cashCallback);
  controller = new ATMController();

  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    string text;
    if(controller->UIoutput(text))
    {
      ROS_INFO("[ATM Controller] %s", text.c_str());
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
