#pragma config(Sensor, S1,     light,          sensorLightActive)
#pragma config(Motor,  motorA,          leftMotor,     tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          rightMotor,    tmotorNXT, PIDControl, encoder)

#define BLACK 40
#define POWER 50

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
int inputStraight[2] = {12, 12}; // in in
int inputTurn[2] = {-90, 90}; // in degrees, negative means clockwise rotation
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
	/*	int fact=5*120;
		int dist=fact/138;  //distance to travel
    int od=2.25;     //Wheel diameter
    int cir=od*PI; //Wheel circumference
    int ticks=360/cir;  //ticks per inch
    while(nMotorEncoder(leftMotor)<dist*ticks){
   		motor[leftMotor]=POWER;
   		motor[rightMotor]=POWER;
    }
    motor[leftMotor]=0;
    motor[rightMotor]=0;
*/
		/*Code that plots the robot's current position and also prints it out as text*/
		nxtSetPixel(50 + (int)(100.0 * robot_X), 32 + (int)(100.0 * robot_Y));
		//nxtDisplayTextLine(0, "X: %f", robot_X);
		//nxtDisplayTextLine(1, "Y: %f", robot_Y);
		//nxtDisplayTextLine(2, "t: %f", robot_TH);

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
   } else {
     return false;
   }
 }

task followLine(){
	int temp = 0;
	int bkw = 35*1.5;
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
			motor[leftMotor] = -60;
			motor[rightMotor] = 30;
			temp = 1;
			wait1Msec(10);
		}
		if(SensorValue(light)>BLACK){//on white
			motor[leftMotor] = 32+(32*1.5-bkw)/6;
			motor[rightMotor] = bkw/1.5+(32/3-bkw)/6;
			if(temp==0&&bkw>-32*1.5-(32*1.5-bkw)/6)
				bkw-=1;
			else if(temp){
				bkw = 32*1.5;
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
	nMotorEncoder[leftMotor] = 0;
	nMotorEncoder[rightMotor] = 0;
	nMotorPIDSpeedCtrl[leftMotor] = mtrSpeedReg;
	nMotorPIDSpeedCtrl[rightMotor] = mtrSpeedReg;
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
		//Turn
		float od=2.25;     //Wheel diameter
		float turnAng=(float)goalTurn; //angle to turn CHANGE ME
		float dwb=5.75; //wheel base diameter
		float turnfactor=1.033;
		float wheelAng=((dwb*turnAng)/od);//Angle we want the wheel to turn
		nxtDisplayTextLine(0, "Left: %f", nMotorEncoder(leftMotor));
		nxtDisplayTextLine(1, "Right: %f", nMotorEncoder(rightMotor));
		while(nMotorEncoder(leftMotor) < (int)wheelAng||(wheelAng<0&&nMotorEncoder(leftMotor)>(int)wheelAng)){
			if(wheelAng>0){
   			motor[leftMotor]=20;
   			motor[rightMotor]=-20;
   		}
   		else{
   			motor[leftMotor]=-20;
   			motor[rightMotor]=20;
   		}
	  }	
	  motor[rightMotor] = 0;
	  motor[leftMotor] = 0;
	  nMotorEncoder[leftMotor] = 0;
		nMotorEncoder[rightMotor] = 0;
		
		wait1Msec(100 * 5);
		//
		//Go Straight
		float straightfactor=1.045;  //Factor to multiply distance by
		float dist=straightfactor*goalStraight;  //distance to travel
 
    float cir=od*PI; //Wheel goalStraightumference
    float ticksperin=360/cir;  //ticks per inch
    float ticks=dist*ticksperin; //total ticks 
    while(nMotorEncoder(leftMotor)<(int)ticks){
   		motor[leftMotor]=POWER;
   		motor[rightMotor]=POWER;
   	}
   	motor[leftMotor]=0;
    motor[rightMotor]=0;
    nMotorEncoder[leftMotor] = 0;
		nMotorEncoder[rightMotor] = 0;
   	//wait1Msec(1000);
   	

		start_X = robot_X;
		start_Y = robot_Y;

		/* Example codes for moving in a striaght line a certain distance,
		 * you need to change this for MUCH better performance 
		motor[motorA] = motorPower;
		motor[motorB] = motorPower;
		distTravelled = sqrt(pow(robot_X - start_X, 2) + pow(robot_Y - start_Y, 2));
		while (!equal(distTravelled, goalStraight)) {
			distTravelled = sqrt(pow(robot_X - start_X, 2) + pow(robot_Y - start_Y, 2));
		}
*/

		wait1Msec(100 * 5);
	}
	motor[motorA] = 0;
	motor[motorB] = 0;
	nNxtButtonTask  = 0;
	while(nNxtButtonPressed != kExitButton) {}
}
