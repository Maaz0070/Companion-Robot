//
//  main.c
//  CompanionBot
//
//  Created by Maaz Adil on 9/12/21.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
    // insert code here...
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
    
    SetDigitalOutput(10, freq);
    
    while(state == 1)
    {
        Read_PD();
        find_max();
        move();
        press_button();
        check_Pressed_Button();
        SetDigitalOutput(10, freq);
        
    }
    
    while(state == 2)
    {
        Read_PD();
        find_max();
        move();
        press_button();
        check_Pressed_Button();
    }
    
    while(state == 3)
    {
        escape();
    }
    
    return 0;
}


void move(void)
{
    arm_button = GetDigitalInput(1);
    error = 4-max_no;
    steer = error*steer_sensitivity;
    speed = forward_speed;
    
    if(PD_sum < ambient_level)
    {
        speed = 0;
        steer = spin_speed;
    }
    if(PD_sum > slow_level)
    {
        speed = slow_speed;
    }
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

void escape(void)
{
    StartUltrasonic(3, 6);
    distance = GetUltrasonic(3, 6);
    SetMotor(2, -60);
    SetMotor(9, 60);
    
    if(distance < 20)
    {
        SetMotor(2, 60);
        SetMotor(9, 60);
        Wait(1000);
        SetMotor(2, -60);
        SetMotor(9, 60);
    }
}

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

void press_button(void)
{
    arm_button = GetDigitalInput(1);
    if(arm_button == 0 && PD_sum > stop_level)
    {
        if(state == 1)
        {
            moveArm(1);
            is_Button_Red_Pressed = 1;
        }
        if(state == 2)
        {
            moveArm(1);
            is_Button_Green_Pressed = 1;
        }
    }
}
