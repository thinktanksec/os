#include "myos.h"

//==========================
//		PORCESS STUFF
//==========================
struct processStruct {
	PID pid;
	MEMORY stack_addr;
	unsigned int state;
	unsigned int name;
	unsigned int execTime;
	unsigned int level;
	int yield;
	MEMORY sp;
	int procLock;
	int done;
	void (*process)(void);
	int arg;
	unsigned int jmp_addr;
	struct processStruct *next;
	unsigned int registers[32];
};

struct processStruct processArray[MAXPROCESS];
struct processStruct currentProc;
struct processStruct currentDevice;
struct processStruct sporadicQueue[15];
struct processStruct deviceQueue[15];


extern int processCount;
extern int sporadicCount;
extern int deviceCount;

struct processStruct findName(int name);
