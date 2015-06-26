#include "os.h"
//#include "nios2_ctrl_reg_macros.h"
//#include "nios2.h"

//#define NULL 0
//Makros for process states
#define NEW 			3
#define RUNNING			4
#define WAITING			5
#define TERMINATED		6

//Memory address area for processes (512 Byte * 16)
#define MIN_TASK_ADDR	0x00000401
#define MAX_TASK_ADDR	0x00002400
#define INVALIDMEM		0x00000000
#define WORKSPACE       512
#define PPPLen			7


void saveRegisters();
void loadRegisters();
void startTimer(int msec);
void stopTimer();
int schedule();
void offToDeath();
extern int terminatedProcs[];
extern int deviceWaiting;


//===================================================
// 			Macro for resetting the board!
//===================================================
#define NIOS2_RESET_ADDR 0x00900C00
#define NIOS2_RESET() \
	NIOS2_WRITE_STATUS(0);\
	NIOS2_WRITE_IENABLE(0);\
	((void (*) (void)) NIOS2_RESET_ADDR)()

