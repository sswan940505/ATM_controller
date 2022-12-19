#include "ATMcontroller.h"

using namespace std;

ATMController::ATMController()
{
  m_bankQuery = n.advertise<std_msgs::String>("OUT/toBank",100);
  static ros::Subscriber sub_bank = n.subscribe("IN/fromBank", 1000, &ATMController::callback_bank,this);
}

ATMController::~ATMController()
{

}

void ATMController::callback_bank(const std_msgs::String::ConstPtr& msg)
{
  vector<string> info = split(msg->data,',');
  if(info[0] == "C") //PIN CHECKED
  {
    for(int i=1;i<info.size();i++)
    {
      m_accountBalances.push_back(stoi(info[i]));
    }
    m_outputText = "Select accounts [1-"+to_string(info.size()-1)+"]";
  }
  else if(info[0] == "I") //PIN INCORRECT
  {
    m_outputText = "Your pin is wrong. type again";
    m_pin.clear();
  }
  else if(info[0] == "B") //BALANCE UPDATED
  {
    m_outputText = "Balance updated! Account ["+to_string(m_selectedAccount)+"] BALANCE :"+info[1];
    clearState();
  }
}

void ATMController::clearState()
{
  m_cardNum.clear();
  m_pin.clear();
  m_accountBalances.clear();
  m_selectedAccount = -1;
  giveCardBack();
}

void ATMController::insertCard(string card)
{
  m_cardNum = card;
}

bool ATMController::isCardInserted()
{
  return m_cardNum.size()>0;
}

void ATMController::insertPIN(string pin)
{
  m_pin = pin;
  std_msgs::String checkPinMsg;
  checkPinMsg.data = "C,";
  checkPinMsg.data += m_cardNum;
  checkPinMsg.data += ",";
  checkPinMsg.data += m_pin;
  m_bankQuery.publish(checkPinMsg);
}

bool ATMController::isPinNeeded()
{
  return m_pin.size()==0;
}

bool ATMController::insertCash(int cash)
{
  if(m_accountBalances[m_selectedAccount] + cash < 0 ) return false;
  if(cash<0)
  {
    withdrawCash(-cash);
  }
  std_msgs::String updateBalanceMsg;
  updateBalanceMsg.data = "B,";
  updateBalanceMsg.data += m_cardNum;
  updateBalanceMsg.data += ",";
  updateBalanceMsg.data += m_pin;
  updateBalanceMsg.data += ",";
  updateBalanceMsg.data += to_string(m_selectedAccount);
  updateBalanceMsg.data += ",";
  updateBalanceMsg.data += to_string(cash);
  m_bankQuery.publish(updateBalanceMsg);
  return true;
}

bool ATMController::selectAccount(int select)
{
  if(select>=0 && select<m_accountBalances.size())
  {
    m_selectedAccount = select;
    return true;
  }
  return false;
}

bool ATMController::isAccountSelected()
{
  return m_selectedAccount!=-1;
}

int ATMController::checkBalance()
{
  int balance =  m_accountBalances[m_selectedAccount];
  clearState();
  return balance;
}

bool ATMController::UIoutput(string &text)
{
  text = m_outputText;
  m_outputText.clear();
  return text.size()>0;
}

vector<string> ATMController::split(string input, char delimiter) {
    vector<string> answer;
    stringstream ss(input);
    string temp;

    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
    }

    return answer;
}

void ATMController::withdrawCash(int cash)
{

}

void ATMController::giveCardBack()
{

}
