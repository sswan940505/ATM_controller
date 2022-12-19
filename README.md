# ATM_controller
Tester :  It processes user input and passes it to the controller. 

(Card number insert, PIN insert, cash deposit/withdraw amount insert)

Controller : It communicates with the bank and controls the devices. 

(cashbin withdrawal, returning card)

- Note that the deposit is checked by cashbin (Integrated in tester)

# HOWTO BUILD

Assume that the ROS is installed. (tested in ros melodic, ubuntu 18.04)

``` bash
$ source /opt/ros/melodic/setup.sh
$ mkdir -p ~/catkin_ws/src
$ cd ~/catkin_ws/src
$ catkin_init_workspace
$ git clone https://github.com/sswan940505/ATM_controller
$ cd ~/catkin_ws/
$ catkin_make
```

# HOWTO TEST
ros message with <span style="color:green">green color</span> : user input

ros message with <span style="color:blue">blue color</span> : device input

ros message with <span style="color:red">red color</span> : communicate with bank.

You can type the commands in your CLI.

0. Run Tester  : rosrun ATM_controller atmTest

1. Insert card :  <span style="color:green">rostopic pub IN/card std_msgs/String "data: '0000111122223333'" </span>

2. Insert pin  :  <span style="color:green">rostopic pub IN/pin std_msgs/String "data: '1234'"</span>

    2-1. If card and pin are right : 
 <span style="color:red">rostopic pub IN/fromBank std_msgs/String "data: 'C,10,100'"</span> # 2 accounts with balance 10 and 100.

    2-2. If card and pin are wrong : 
    <span style="color:red">rostopic pub IN/fromBank std_msgs/String "data: 'I'"</span>

3. Select account : <span style="color:green">rostopic pub IN/selection std_msgs/Int32 "data: 1"</span>

- Balance Checking
4. Select menu (balance) : <span style="color:green">rostopic pub IN/menu std_msgs/Int32 "data: 0"</span>

- Deposit Cash
4. Select menu (deposit) : <span style="color:green">rostopic pub IN/menu std_msgs/Int32 "data: 1"</span>
5. Deposit cash : <span style="color:blue">rostopic pub IN/cash std_msgs/Int32 "data: 10"</span>
6. After update cash : <span style="color:red">rostopic pub IN/fromBank std_msgs/String "data: 'B,110'"</span>

- Withdraw Cash
4. Select menu (withdraw) : <span style="color:green">rostopic pub IN/menu std_msgs/Int32 "data: 2"</span>
5. Withdraw cash : <span style="color:green">rostopic pub IN/cash std_msgs/Int32 "data: -10"</span>
6. After update cash : <span style="color:red">rostopic pub IN/fromBank std_msgs/String "data: 'B,90'"</span>

- Withdraw Cash (over balance)
4. Select menu (withdraw) : <span style="color:green">rostopic pub IN/menu std_msgs/Int32 "data: 2"</span>
5. Withdraw cash : <span style="color:green">rostopic pub IN/cash std_msgs/Int32 "data: -110"</span>