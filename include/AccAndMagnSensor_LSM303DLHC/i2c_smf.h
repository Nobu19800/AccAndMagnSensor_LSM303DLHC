#ifndef i2c_smf_H
#define i2c_smf_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>





class i2c_smf
{
public:
	i2c_smf(char *name = "/tmp/edisoni2csmf", int val = 1);
	~i2c_smf();
	int sem_get(char *path);
	int sem_lock();
	int sem_unlock();
	int sem_ctl();
	int sid;
private:
	
};





#endif
