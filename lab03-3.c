#pragma config(Sensor, S3,     light,          sensorLightActive) //Light sensor is named "light"
#pragma config(Motor,  motorA,          leftMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          rightMotor,    tmotorNXT, PIDControl, encoder)

#define BLACK 45
#define POWER 60
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**********************************************
 * Lab 3 : Starter code
 * Written by Kaushik Viswanathan,
 * Modified by Allan Wang (Jan 2017)
 * Feel free to modify any part of these codes.
 **********************************************/

//Global variables - you will need to change some of these
//Robot's positions
float robot_X = 0.0, robot_Y = 0.0, robot_TH = 0.0;

int velocityUpdateInterval = 5;
int PIDUpdateInterval = 2;

//Change these during demo
int inputStraight[2] = {0, 0}; // in mm
int inputTurn[2] = {0, 0}; // in degrees, negative means clockwise rotation
int motorPower = 50;

/*****************************************
 * Complete this function so that it
 * continuously updates the robot's position
 *****************************************/
void turn(int degrees){

}

task dead_reckoning()
{

    while(1)
    {
        //
        // Fill in code for numerical integration / position estimation here
        //

        /*Code that plots the robot's current position and also prints it out as text*/
        nxtSetPixel(50 + (int)(100.0 * robot_X), 32 + (int)(100.0 * robot_Y));
        nxtDisplayTextLine(0, "X: %f", robot_X);
        nxtDisplayTextLine(1, "Y: %f", robot_Y);
        nxtDisplayTextLine(2, "t: %f", robot_TH);

        wait1Msec(velocityUpdateInterval);
    }
}

/*****************************************
 * Function that draws a grid on the LCD
 * for easier readout of whatever is plot
 *****************************************/
void draw_grid()
{
    for(int i = 0; i < 65; i++)
    {
        nxtSetPixel(50, i);
        int grid5 = (i - 32) % 5;
        int grid10 = (i - 32) % 10;
        if(!grid5 && grid10)
        {
            for(int j = -2; j < 3; j++)
            {
                nxtSetPixel(50 + j, i);
            }
        }
        else if(!grid10)
        {
            for(int j = -4; j < 5; j++)
            {
                nxtSetPixel(50 + j, i);
            }
        }
    }
    for(int i = 0; i < 101; i++)
    {
        nxtSetPixel(i, 32);
        int grid5 = (i - 100) % 5;
        int grid10 = (i - 100) % 10;
        if(!grid5 && grid10)
        {
            for(int j = -2; j < 3; j++)
            {
                nxtSetPixel(i, 32 + j);
            }
        }
        else if(!grid10)
        {
            for(int j = -4; j < 5; j++)
            {
                nxtSetPixel(i, 32 + j);
            }
        }
    }
}

/**********************************************
 * Function that judges if two floats are equal
 **********************************************/
 bool equal(float a, float b) {
   float epsilon = 1;
   if (abs(a-b) < epsilon) {
     return true;
   }
   else {
     return false;
   }
 }
/*Converting NXT into state machine
dir = 0 -> straight
dir = 1 -> left
dir = 2 -> right
*/
task followLine(){
	int temp = 0;
	int bkw = 28*2;
	//start sequence
				int max_sweeps = 20; //# of times robot will sweep to find line
        int sweeps = 0; // iterator for sweeps
        int sweep_time = 1; // duration of sweep
        int dir = 2; // check right first
        while(true){
            sweep_time += 2*sweeps*sweeps;
            if(dir == 1){ //Turn left
                motor[leftMotor] = -30;
                motor[rightMotor] = 30;
            }
            else if(dir == 2){ //Turn right
                motor[leftMotor] = 30;
                motor[rightMotor] = -30;
            }
            int temp = 0;
            for(int t = 0;t<sweep_time;t++){ // time spent sweeping
	            if(SensorValue(light) < BLACK){ // found line
	                dir = 0; // go straight again
	                temp = 1;
	                break;
	            }
	            wait1Msec(1);
          	}
          	if(temp) break;
            if(sweeps > max_sweeps) break; // reached max n# of sweeps
            // switch direction of sweep
            if(dir == 1) dir = 2;
            else dir = 1;
            sweeps += 1;
      		}
	while(true){
		if(SensorValue(light)<BLACK){
			motor[leftMotor] = -25;
			motor[rightMotor] = 20;
			temp = 1;
			wait1Msec(10);
		}
		if(SensorValue(light)>BLACK){//on white
			motor[leftMotor] = 28+(28*2-bkw)/6;
			motor[rightMotor] = bkw/2+(28*2-bkw)/6;
			if(temp==0&&bkw>-28*2)
				bkw-=1;
			else if(temp){
				bkw = 28*2;
				temp = 0;
			}
			wait1Msec(10);
		}
	}

}
/*****************************************
 * Main function - Needs changing
 *****************************************/
task main()
{
    /* Reset encoders and turn on PID control */
    nMotorEncoder[motorB] = 0;
    nMotorEncoder[motorC] = 0;
    nMotorPIDSpeedCtrl[motorB] = mtrSpeedReg;
    nMotorPIDSpeedCtrl[motorC] = mtrSpeedReg;
    nPidUpdateInterval = PIDUpdateInterval;

    int goalStraight = 0;
    int goalTurn = 0;
    float start_X = 0;
    float start_Y = 0;
    float distTravelled = 0;

    draw_grid();
    startTask(followLine);
    while(true){}
    startTask(dead_reckoning);

    for(int i = 0; i < 2; i++)
    {
        goalStraight = inputStraight[i];
        goalTurn = inputTurn[i];

        //
        // Write your own codes for turning
        //

        start_X = robot_X;
        start_Y = robot_Y;

        /* Example codes for moving in a striaght line a certain distance,
         * you need to change this for MUCH better performance */
        motor[motorA] = motorPower;
        motor[motorB] = motorPower;
        distTravelled = sqrt(pow(robot_X - start_X, 2) + pow(robot_Y - start_Y, 2));
        while (!equal(distTravelled, goalStraight)) {
            distTravelled = sqrt(pow(robot_X - start_X, 2) + pow(robot_Y - start_Y, 2));
        }


        wait1Msec(100 * 5);
    }
    motor[motorA] = 0;
    motor[motorB] = 0;
    nNxtButtonTask  = 0;
    while(nNxtButtonPressed != kExitButton) {}
}
