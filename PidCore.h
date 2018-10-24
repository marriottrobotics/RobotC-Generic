/* PID-related stuff */

#define LIMIT(x, min, max) (((x) <= (max)) ? (((x) >= (min)) ? (x) : (min)) : (max))
#define ABS(x) (((x) >= 0) ? (x) : (-(x)))
#define LOWPASS(x, n) (((ABS((x))) >= (n) || (x) <= (-(n))) ? (true) : (false))
#define SIGN(x) (((x) >= 0) ? (1) : (-1))

//The structure that defines a pid loop.
struct pid {
	tMotor mport;
	long mtarget;
	float pgain;

	float isum;
	float igain;
	long ilimit;

	long lastPos;
	float dgain;

	//short corrcap;
	int lowpass;
	bool enable;

	long min, max;
};

short lowpassS(short input, short minimum);
int lowpassI(int input, int minimum);

//Initalizes basic variables for the pid struct provided.
void pid_init(struct pid *p) {
	nMotorEncoder[p->mport] = 0;
	p->mtarget = 0;
	p->pgain = 0.70;
	//p->corrcap = 30;
	p->enable = true;
	p->min = 0;
	p->max = 0;
	p->igain = 0.01;
	p->isum = 0;
	p->ilimit = 1000;
	p->lastPos = 0;
	p->dgain = 0;
	p->lowpass = 8;
}

//Checks if the pid motor is close enough to it's target to be within an acceptable range.
bool pid_ontarget(struct pid *p)
{
	return ABS(p->mtarget - nMotorEncoder[p->mport]) < 64;
}

//Updates the pid loop to include the latest information from this tick.
void pid_update(struct pid *p) {
	//Limit the pid loop
	if (p->min != 0 || p->max != 0) {
		p->mtarget = LIMIT(p->mtarget, p->min, p->max);
	}

	//Calculate initial values
	int enc = nMotorEncoder[p->mport];

	//Calculate err using the modified enc value and the target.
	long err = p->mtarget - enc;

	//P Component
	float corr = err * p->pgain;

	//I Component
	p->isum+=err;
	if(p->isum >= p->ilimit){
		p->isum = p->ilimit;
	}else if(p->isum < 0){
		if(p->ilimit < 0 && p->isum < p->ilimit){
			p->isum = p->ilimit;
		}else if(p->ilimit > 0 && p->isum < 0){
			p->isum = -p->ilimit;
		}
	}

	corr += p->isum * p->igain;

	float dAvg = p->lastPos - enc;
	corr += dAvg * p->dgain;

	p->lastPos = enc;

	//writeDebugStreamLine("Powering motor at %d", corr);
	corr = lowpassI(corr, p->lowpass);
	motor[p->mport] = corr;
	//writeDebugStreamLine("Motor had power at %d", corr);
}

//The array of pid loops to be used in the update task.
struct pid *pid_arr[10];
//The amount of currently registered pid loops.
int pid_count = 0;

//Convenience method calls pid_init for all pid loops in pid_arr.
void pid_init_all()
{
	for (int i = 0; i < pid_count; ++i) {
		pid_init(pid_arr[i]);
	}
}

//A task to be run on the scheduler
task pid_run_loops() {
	while(1) {
		for (int i = 0; i < pid_count; ++i) {
			if(pid_arr[i]->enable) {
				pid_update(pid_arr[i]);
			}
		}
		wait1Msec(10);
	}
};

//A lowpass to handle a short input with negative values.
short lowpassS(short input, short minimum){
	if(input <= -minimum || input >= minimum){
		return input;
	}else{
		return 0;
	}
}

int lowpassI(int input, int minimum){
	if(input <= -minimum || input >= minimum){
		return input;
	}else{
		return 0;
	}
}

//Loops until pid_ontarget is true for the supplied loop.
void wait_ontarget(struct pid *p)
{
	//writeDebugStreamLine("Waiting for pid to finish");
	long orig_pos = p->mtarget;
	long last_pos = orig_pos;

	// kills the wait if we exceed a timer
	unsigned long start_time = nPgmTime;
	const unsigned long wait_time = 2500;

	while (!pid_ontarget(p)) {
		//writeDebugStreamLine("In loop with orig_pos %d and last_pos %d", orig_pos, last_pos);

		last_pos = nMotorEncoder[p->mport];
		wait1Msec(25);

		// if we get stuck, stop waiting
		if (nPgmTime-start_time > wait_time)
			break;
	}
	//writeDebugStreamLine("Ended waiting");
}
