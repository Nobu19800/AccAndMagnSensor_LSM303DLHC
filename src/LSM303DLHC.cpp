#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <mraa.h>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include "LSM303DLHC.h"


#define DLM_WHO_ID  0x3C

LSM303DLHC::LSM303DLHC(mraa_result_t &response, mraa::I2c *i2c, i2c_smf *smf, uint8_t Accaddr, uint8_t Magnaddr, uint8_t Accscale, uint8_t Magnscale, int mx_offset, int my_offset, int mz_offset, double ar, double mr) {
	_i2c = i2c;
	
	
	_ar = ar;
	_mr = mr;
	
	
	_Accaddr = Accaddr;
	_Magnaddr = Magnaddr;

	//_smf = new i2c_smf();
	_smf = smf;

	_mx_offset = mx_offset;
	_my_offset = my_offset;
	_mz_offset = mz_offset;

	_Ascale = Accscale;
	_Mscale = Magnscale;

	if(!AccSensor_Exist())
	{
		response = MRAA_ERROR_UNSPECIFIED;
		return;
	}
	if(!MagnSensor_Exist())
	{
		response = MRAA_ERROR_UNSPECIFIED;
		return;
	}

	reset();

	response = MRAA_SUCCESS;
}

LSM303DLHC::~LSM303DLHC() {
	//delete _smf;
}

bool LSM303DLHC::AccSensor_Exist()
{
	//uint8_t Buf[2];
	//readByte(_Accaddr,WHO_AM_I,1,Buf);

	return true;
}

bool LSM303DLHC::MagnSensor_Exist()
{
	uint8_t Buf[2];
	readByte(_Magnaddr,WHO_AM_I_M,1,Buf);

	
	if(Buf[0] != DLM_WHO_ID)return false;
	return true;
}


mraa_result_t LSM303DLHC::setAccAddr(uint8_t Accaddr)
{
	if(_Accaddr != Accaddr)
	{
		_Accaddr = Accaddr;
		if(!AccSensor_Exist())return MRAA_ERROR_UNSPECIFIED;
		reset();
	}
	return MRAA_SUCCESS;

}

mraa_result_t LSM303DLHC::setMagnAddr(uint8_t Magnaddr)
{
	if(_Magnaddr != Magnaddr)
	{
		_Magnaddr = Magnaddr;
		if(!MagnSensor_Exist())return MRAA_ERROR_UNSPECIFIED;
		reset();
	}
	return MRAA_SUCCESS;

}

void LSM303DLHC::setAccCoefficient(double ar)
{
	_ar = ar;
}
void LSM303DLHC::setMagnCoefficient(double mr)
{
	_mr = mr;
}


void LSM303DLHC::setAccScale(uint8_t Accscale)
{
	if(_Ascale != Accscale)
	{
		_Ascale = Accscale;
		reset();
	}
	
}


void LSM303DLHC::setMagnScale(uint8_t Magnscale)
{
	if(_Mscale != Magnscale)
	{
		_Mscale = Magnscale;
		reset();
	}
}



void LSM303DLHC::setOffset(int mx_offset, int my_offset, int mz_offset)
{
	_mx_offset = mx_offset;
	_my_offset = my_offset;
	_mz_offset = mz_offset;
}


void LSM303DLHC::setAccRange(uint8_t scale)
{
	_Ascale = scale;
	writeByte(_Accaddr, CTRL_REG4_A, _Ascale << 3);
	
	
}



void LSM303DLHC::setMagnRange(uint8_t scale){
	_Mscale = scale;
	writeByte(_Magnaddr, CRB_REG_M, _Mscale << 3);
	
}

void LSM303DLHC::reset(void) {
	
	
	

	
	//_i2c->address(_Accaddr);

	
	writeByte(_Accaddr,CTRL_REG1_A,0x27);

	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 200000000;
	nanosleep(&ts, NULL);
	
	setAccRange(_Ascale);
	
	
	
	writeByte(_Magnaddr,CRA_REG_M,0x14);
	nanosleep(&ts, NULL);
	//writeByte(_Magnaddr,CRB_REG_M,0x20);
	writeByte(_Magnaddr,MR_REG_M,0x00);
	nanosleep(&ts, NULL);
	setMagnRange(_Mscale);
	


	nanosleep(&ts, NULL);
	
  	
  	
  	
 
  	
  	

	const double count = 10;
	double ax,ay,az;
	lastAX = 0;
	lastAY = 0;
	lastAZ = 0;
	
	for(int i=0;i < count;i++)
	{
		getAccData(ax,ay,az);

		lastAX += ax/count;
		lastAY += ay/count;
		lastAZ += az/count;
		usleep(10000);
	}


	

	double mx,my,mz;
	
	lastMX = 0;
	lastMY = 0;
	lastMZ = 0;
	for(int i=0;i < count;i++)
	{
		getMagnData(mx,my,mz);
		lastMX += mx/count;
		lastMY += my/count;
		lastMZ += mz/count;
		usleep(10000);
	}

	
}

void LSM303DLHC::getOrientation(double &rx, double &ry, double &rz)
{
	

	ry = atan2(lastAY,lastAZ);
	rx = atan2(-lastAX,sqrt(lastAY*lastAY+lastAZ*lastAZ));

	

	double mxi = cos(ry)*lastMX + sin(rx)*sin(ry)*lastMY + cos(rx)*sin(ry)*lastMZ;
	double myi = cos(rx)*lastMY - sin(rx)*lastMZ;

	

	rz = atan2(-myi,mxi);

	//std::cout << rz << "\t" << lastMX << "\t" << lastMY << "\t" << lastMZ << std::endl;
}


void LSM303DLHC::getAcc(double &ax, double &ay, double &az)
{
	getAccData(ax,ay,az);

	lastAX = _ar*ax + (1-_ar)*lastAX;
	lastAY = _ar*ay + (1-_ar)*lastAY;
	lastAZ = _ar*az + (1-_ar)*lastAZ;
	

}

void LSM303DLHC::getMagn(double &mx, double &my, double &mz)
{
	getMagnData(mx,my,mz);
	//mx = _mr*mx + (1-_mr)*lastMX;
	//my = _mr*my + (1-_mr)*lastMY;
	//mz = _mr*mz + (1-_mr)*lastMZ;

	lastMX = _mr*mx + (1-_mr)*lastMX;
	lastMY = _mr*my + (1-_mr)*lastMY;
	lastMZ = _mr*mz + (1-_mr)*lastMZ;

	//std::cout << mx << "\t" << my << "\t" << mz << std::endl;
}


double LSM303DLHC::getTemp()
{
	lastTemp = getTempData();
	return lastTemp;
}





void LSM303DLHC::getAccData(double &ax, double &ay, double &az) {
	uint8_t Buf[6];
	readByte(_Accaddr, OUT_X_L_A|0x80, 6, Buf);

	

	short AccelRaw_x = ((Buf[1] << 8) | Buf[0]);
	short AccelRaw_y = ((Buf[3] << 8) | Buf[2]);
	short AccelRaw_z = ((Buf[5] << 8) | Buf[4]);

	double aRes;
	if(_Ascale == AFS_2G)
		aRes = 4.0/32768.0;
	else if(_Ascale == AFS_4G)
		aRes = 8.0/32768.0;
	else if(_Ascale == AFS_8G)
		aRes = 16.0/32768.0;
	else
		aRes = 32.0/32768.0;
	ax = (double)AccelRaw_x * aRes;
	ay = (double)AccelRaw_y * aRes;
	az = (double)AccelRaw_z * aRes;

	

	
}

void LSM303DLHC::getMagnData(double &mx, double &my, double &mz) {
	
	

	uint8_t Mag[6];

	readByte(_Magnaddr, DLH_OUT_X_H_M, 6, Mag);

	short MagnRaw_x = ((Mag[0] << 8) | Mag[1]);
	short MagnRaw_y = ((Mag[4] << 8) | Mag[5]);
	short MagnRaw_z = ((Mag[2] << 8) | Mag[3]);
	
	MagnRaw_x += _mx_offset;
	MagnRaw_y += _my_offset;
	MagnRaw_z += _mz_offset;

	//std::cout << MagnRaw_x << "\t" << MagnRaw_y << "\t" << MagnRaw_z << std::endl;

	double mRes;
	if(_Mscale == MFS_1300mG)
		mRes = 2.6/32768.0;
	else if(_Mscale == MFS_1900mG)
		mRes = 3.8/32768.0;
	else if(_Mscale == MFS_2500mG)
		mRes = 5.0/32768.0;
	else if(_Mscale == MFS_4000mG)
		mRes = 8.0/32768.0;
	else if(_Mscale == MFS_4700mG)
		mRes = 9.4/32768.0;
	else if(_Mscale == MFS_5600mG)
		mRes = 11.2/32768.0;
	else
		mRes = 16.2/32768.0;

	mx = (double)MagnRaw_x * mRes;
	my = (double)MagnRaw_y * mRes;
	mz = (double)MagnRaw_z * mRes;

	
}

double LSM303DLHC::getTempData()
{
	uint8_t Buf[2];
	readByte(_Magnaddr,TEMP_OUT_H_M,1,Buf);

	short Temp = Buf[0] << 8;

	readByte(_Magnaddr,TEMP_OUT_L_M,1,Buf);

	Temp = Temp | Buf[0];


	
	
	
	
	return (double)Temp/8.;
}


void LSM303DLHC::writeByte(uint8_t Address, uint8_t Register, uint8_t Data) {

	_smf->sem_lock();
	_i2c->address(Address);
	
	_i2c->writeReg(Register, Data);
	
	_smf->sem_unlock();

  	
}

void LSM303DLHC::readByte(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data) {
	
	_smf->sem_lock();

	_i2c->address(Address);
	_i2c->writeByte(Register);
	
	
	_i2c->read(Data,Nbytes);
	
	_smf->sem_unlock();
}