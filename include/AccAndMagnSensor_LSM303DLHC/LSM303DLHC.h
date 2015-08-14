#ifndef LSM303DLHC_H
#define LSM303DLHC_H

#include <stdio.h>
#include <mraa.hpp>


#include "i2c_smf.h"

#define LSM303DLHC_AccAddress 0x19
#define LSM303DLHC_MagAddress 0x1e

 

class LSM303DLHC
{
public:
	LSM303DLHC(mraa_result_t &response, mraa::I2c *i2c, i2c_smf *smf, uint8_t Accaddr = LSM303DLHC_AccAddress, uint8_t Magnaddr = LSM303DLHC_MagAddress, uint8_t Accscale = 0x03, uint8_t Magnscale = 0x02, int mx_offset = 935, int my_offset = 320, int mz_offset = 730,  double ar = 0.2, double mr = 0.2);
	~LSM303DLHC();
	mraa_result_t setAccAddr(uint8_t Accaddr = LSM303DLHC_AccAddress);
	mraa_result_t setMagnAddr(uint8_t Magnaddr = LSM303DLHC_MagAddress);
	bool AccSensor_Exist();
	bool MagnSensor_Exist();
	void setAccCoefficient(double ar = 0.2);
	void setMagnCoefficient(double mr = 0.2);
	void setAccScale(uint8_t Accscale = 0x03);
	void setMagnScale(uint8_t Magnscale = 0x02);
	void setOffset(int mx_offset = 935, int my_offset = 320, int mz_offset = 730);
	
	void reset(void);
	void getAcc(double &ax, double &ay, double &az);
	void getMagn(double &mx, double &my, double &mz);
	double getTemp();

	void getAccData(double &ax, double &ay, double &az);
	void getMagnData(double &mx, double &my, double &mz);
	double getTempData();
	
	void getOrientation(double &rx, double &ry, double &rz);

	void setAccRange(uint8_t scale);
	
	void setMagnRange(uint8_t scale);
	
	enum regAddr
	{
	      TEMP_OUT_L        = 0x05,
	      TEMP_OUT_H        = 0x06,

	      STATUS_M          = 0x07,

	      INT_CTRL_M        = 0x12,
	      INT_SRC_M         = 0x13,
	      INT_THS_L_M       = 0x14,
	      INT_THS_H_M       = 0x15,

	      OFFSET_X_L_M      = 0x16,
	      OFFSET_X_H_M      = 0x17,
	      OFFSET_Y_L_M      = 0x18,
	      OFFSET_Y_H_M      = 0x19,
	      OFFSET_Z_L_M      = 0x1A,
	      OFFSET_Z_H_M      = 0x1B,
	      REFERENCE_X       = 0x1C,
	      REFERENCE_Y       = 0x1D,
	      REFERENCE_Z       = 0x1E,

	      CTRL0             = 0x1F,
	      CTRL1             = 0x20,
	      CTRL_REG1_A       = 0x20,
	      CTRL2             = 0x21,
	      CTRL_REG2_A       = 0x21,
	      CTRL3             = 0x22,
	      CTRL_REG3_A       = 0x22,
	      CTRL4             = 0x23,
	      CTRL_REG4_A       = 0x23,
	      CTRL5             = 0x24,
	      CTRL_REG5_A       = 0x24,
	      CTRL6             = 0x25,
	      CTRL_REG6_A       = 0x25,
	      HP_FILTER_RESET_A = 0x25,
	      CTRL7             = 0x26,
	      REFERENCE_A       = 0x26,
	      STATUS_A          = 0x27,
	      STATUS_REG_A      = 0x27,

	      OUT_X_L_A         = 0x28,
	      OUT_X_H_A         = 0x29,
	      OUT_Y_L_A         = 0x2A,
	      OUT_Y_H_A         = 0x2B,
	      OUT_Z_L_A         = 0x2C,
	      OUT_Z_H_A         = 0x2D,

	      FIFO_CTRL         = 0x2E,
	      FIFO_CTRL_REG_A   = 0x2E,
	      FIFO_SRC          = 0x2F,
	      FIFO_SRC_REG_A    = 0x2F,

	      IG_CFG1           = 0x30,
	      INT1_CFG_A        = 0x30,
	      IG_SRC1           = 0x31,
	      INT1_SRC_A        = 0x31,
	      IG_THS1           = 0x32,
	      INT1_THS_A        = 0x32,
	      IG_DUR1           = 0x33,
	      INT1_DURATION_A   = 0x33,
	      IG_CFG2           = 0x34,
	      INT2_CFG_A        = 0x34,
	      IG_SRC2           = 0x35,
	      INT2_SRC_A        = 0x35,
	      IG_THS2           = 0x36,
	      INT2_THS_A        = 0x36,
	      IG_DUR2           = 0x37,
	      INT2_DURATION_A   = 0x37,

	      CLICK_CFG         = 0x38,
	      CLICK_CFG_A       = 0x38,
	      CLICK_SRC         = 0x39,
	      CLICK_SRC_A       = 0x39,
	      CLICK_THS         = 0x3A,
	      CLICK_THS_A       = 0x3A,
	      TIME_LIMIT        = 0x3B,
	      TIME_LIMIT_A      = 0x3B,
	      TIME_LATENCY      = 0x3C,
	      TIME_LATENCY_A    = 0x3C,
	      TIME_WINDOW       = 0x3D,
	      TIME_WINDOW_A     = 0x3D,

	      Act_THS           = 0x3E,
	      Act_DUR           = 0x3F,

	      CRA_REG_M         = 0x00,
	      CRB_REG_M         = 0x01,
	      MR_REG_M          = 0x02,

	      SR_REG_M          = 0x09,
	      IRA_REG_M         = 0x0A,
	      IRB_REG_M         = 0x0B,
	      IRC_REG_M         = 0x0C,

	      WHO_AM_I          = 0x0F,
	      WHO_AM_I_M        = 0x0F,

	      TEMP_OUT_H_M      = 0x31,
	      TEMP_OUT_L_M      = 0x32,



	      OUT_X_H_M         = -1,
	      OUT_X_L_M         = -2,
	      OUT_Y_H_M         = -3,
	      OUT_Y_L_M         = -4,
	      OUT_Z_H_M         = -5,
	      OUT_Z_L_M         = -6,


	      DLH_OUT_X_H_M     = 0x03,
	      DLH_OUT_X_L_M     = 0x04,
	      DLH_OUT_Y_H_M     = 0x05,
	      DLH_OUT_Y_L_M     = 0x06,
	      DLH_OUT_Z_H_M     = 0x07,
	      DLH_OUT_Z_L_M     = 0x08,

	      DLM_OUT_X_H_M     = 0x03,
	      DLM_OUT_X_L_M     = 0x04,
	      DLM_OUT_Z_H_M     = 0x05,
	      DLM_OUT_Z_L_M     = 0x06,
	      DLM_OUT_Y_H_M     = 0x07,
	      DLM_OUT_Y_L_M     = 0x08,

	      DLHC_OUT_X_H_M    = 0x03,
	      DLHC_OUT_X_L_M    = 0x04,
	      DLHC_OUT_Z_H_M    = 0x05,
	      DLHC_OUT_Z_L_M    = 0x06,
	      DLHC_OUT_Y_H_M    = 0x07,
	      DLHC_OUT_Y_L_M    = 0x08,

	      D_OUT_X_L_M       = 0x08,
	      D_OUT_X_H_M       = 0x09,
	      D_OUT_Y_L_M       = 0x0A,
	      D_OUT_Y_H_M       = 0x0B,
	      D_OUT_Z_L_M       = 0x0C,
	      D_OUT_Z_H_M       = 0x0D
	};

	enum Ascale {
	  AFS_2G = 0,
	  AFS_4G,
	  AFS_8G,
	  AFS_16G
	};

	enum Mscale {
	  MFS_1300mG = 0,
	  MFS_1900mG,
	  MFS_2500mG,
	  MFS_4000mG,
	  MFS_4700mG,
	  MFS_5600mG,
	  MFS_8100mG,
	};
private:
	
	//int _i2channel;

	void readByte(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data);
	void writeByte(uint8_t Address, uint8_t Register, uint8_t Data);
	
	uint8_t _Accaddr;
	uint8_t _Magnaddr;
	mraa::I2c *_i2c;
	double _ar, _mr;
	double lastAX, lastAY, lastAZ;
	double lastMX, lastMY, lastMZ;
	
	double lastTemp;
	
	uint8_t _Ascale;
	uint8_t _Mscale;

	i2c_smf *_smf;

	int _mx_offset, _my_offset, _mz_offset;
};





#endif