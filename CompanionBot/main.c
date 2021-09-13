//
//  main.c
//  CompanionBot
//
//  Created by Maaz Adil on 9/12/21.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    //Declare initial value of our variables to be used
    int freq = 0;
    double ambient_level = 250;
    double slow_level = 3500;
    double stop_level = 4500;
    double expose_time = 3;
    double steer_sensitivity = 20;
    double forward_speed = 35;
    double slow_speed = 25;
    double spin_speed = 50;
    _Bool is_Button_Red_Pressed = 0;
    _Bool is_Button_Green_Pressed = 0;
    
    //Display on screen
    SetDigitalOutput(10, freq);
    
    //states are predefined conditions the robot will find itself in
    //IN state 1 we need to find maximum level of green light, move toward it, and press its button.
    while(state == 1)
    {
        Read_PD();
        find_max();
        move();
        press_button();
        check_Pressed_Button();
        SetDigitalOutput(10, freq);
        
    }
    
    //IN state 2, after pressing green light, we go to second beacon and press its button
    while(state == 2)
    {
        Read_PD();
        find_max();
        move();
        press_button();
        check_Pressed_Button();
    }
    
    //IN state 3, after pressing both buttons we take the robot out of the arena with the escape button
    while(state == 3)
    {
        escape();
    }
    
    return 0;
}

//Robot move around in circular motion until it finds the beacon depending on light sensor
void move(void)
{
    arm_button = GetDigitalInput(1);
    error = 4-max_no;
    steer = error*steer_sensitivity;
    speed = forward_speed;
    
    //Continue spinning if desired light isn't found
    if(PD_sum < ambient_level)
    {
        speed = 0;
        steer = spin_speed;
    }
    //slowly travel to beacon if desired light is sensed
    if(PD_sum > slow_level)
    {
        speed = slow_speed;
    }
    //if beacon is in front and arm is up we stop the Robot
    if(PD_sum > stop_level && arm_button == 0)
    {
        speed = 0;
        steer = 0;
    }
    
    temp = limit_pwm(0+steer+speed);
    SetMotor(2, temp);
    temp = limit_pwm(0 + steer_speed);
    SetMotor(9, temp);
}

//Checks if button is pressed. If it is not pressed we must keep moving the arm to try
void check_Pressed_Button(void)
{
    if(is_Button_Red_Pressed == 1 && state == 1 && PD_sum < ambient_level)
    {
        state = 2;
        freq = 1;
        slow_level = 100;
        stop_level = 180;
        ambient_level = 40;
        moveArm(2);
        SetMotor(2, -60);
        SetMotor(9, 60);
        Wait(4000);
    }
    if(PD_sum <= ambient_level && is_Button_Green_Pressed == 1 && state == 2)
    {
        state = 3;
        SetMotor(4, -40);
        Wait(1000);
        SetMotor(4, 0);
    }
}

//Lets Robot safely get out of arena without running into anything
void escape(void)
{
    StartUltrasonic(3, 6);
    distance = GetUltrasonic(3, 6);
    SetMotor(2, -60);
    SetMotor(9, 60);
    
    //If Robot is near an object it will continue to move around
    if(distance < 20)
    {
        SetMotor(2, 60);
        SetMotor(9, 60);
        Wait(1000);
        SetMotor(2, -60);
        SetMotor(9, 60);
    }
}

//Move arm on the robot up or down
void moveArm(int direction)
{
    if(direction == 1)
    {
        SetMotor(4, -40);
        Wait(1000);
        SetMotor(4, 45);
        Wait(1000);
        SetMotor(4, 0);
    }
    else if(direction == 2)
    {
        SetMotor(4, 60);
        Wait(1000);
        SetMotor(4, 0);
    }
}

//Tells the arm to go downwards
void press_button(void)
{
    arm_button = GetDigitalInput(1);
    //if arm is up and we are near a beacon we must move arm to turn the beacon off depending on which beacon we are near
    if(arm_button == 0 && PD_sum > stop_level)
    {
        //for the first beacon we must move the arm down and check if red button is pressed
        if(state == 1)
        {
            moveArm(1);
            is_Button_Red_Pressed = 1;
        }
        //for the second beacon we must move the arm down and check if green button is pressed
        if(state == 2)
        {
            moveArm(1);
            is_Button_Green_Pressed = 1;
        }
    }
}
