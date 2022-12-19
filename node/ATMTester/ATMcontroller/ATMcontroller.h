#ifndef ATMCONTROLLER_H
#define ATMCONTROLLER_H

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Int32.h>
#include <mutex>
#include <math.h>
#include <limits.h>

using namespace std;

class ATMController
{
private:
  ros::NodeHandle n;
  ros::Publisher  m_bankQuery;

  string m_cardNum;
  string m_pin;
  int    m_selectedAccount = -1;
  vector<int> m_accountBalances;
  string m_outputText;

  void clearState();
  void callback_bank(const std_msgs::String::ConstPtr& msg);
  vector<string> split(string input, char delimiter);

  void withdrawCash(int cash); //For cash bin
  void giveCardBack();         //For card machine

public:
  void insertCard(string card);
  void insertPIN(string pin);
  bool insertCash(int cash); // cash is negative if withdraw, positive if deposit.
  bool selectAccount(int select);
  int checkBalance();
  bool isCardInserted();
  bool isPinNeeded();
  bool isAccountSelected();
  bool UIoutput(string & text);

  ATMController();
  ~ATMController();

};


#endif

