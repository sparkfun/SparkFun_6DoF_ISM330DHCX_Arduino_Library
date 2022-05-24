#include "qwiic_i2c.h"
#include "sfe_ism_shim.h"


#define ISM330DHCX_ADDRESS_LOW 0x6A
#define ISM330DHCX_ADDRESS_HIGH 0x6B

struct sfe_raw_data_t
{
	int16_t xData;	
	int16_t yData;	
	int16_t zData;
};

struct sfe_ism330dhcx_settings
{
	bool accel2g; 
	bool accel4g; 
	bool accel8g; 
	bool accel16g; 
	bool gryo2g; 
	bool gyro4g; 
	bool gyro8g; 
	bool gyro16g; 
};

struct sfe_hub_sensor_settings_t
{
	uint8_t address;
	uint8_t subAddress;
	uint8_t length;
};

class QwDevISM330DHCX
{
	public: 

		QwDevISM330DHCX() : _i2cBus{nullptr}, _i2cAddress{0} {};
			

    ///////////////////////////////////////////////////////////////////////
    // init()
    //
    // Called to init the system. Connects to the device and sets it up for 
    // operation

    bool init();

    ///////////////////////////////////////////////////////////////////////
    // isConnected()
    //
    //
    //  Parameter   Description
    //  ---------   -----------------------------
    //  retval      true if device is connected, false if not connected

    bool isConnected(); // Checks if sensor ack's the I2C request

    int32_t writeRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length);

    //////////////////////////////////////////////////////////////////////////////////
    // readRegisterRegion()
    //
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  reg          register to read from
    //  data         Array to store data in
    //  length       Length of the data to read
    //  retval       -1 = error, 0 = success

    int32_t readRegisterRegion(uint8_t reg, uint8_t *data, uint16_t length);

    //////////////////////////////////////////////////////////////////////////////////
    // setCommunicationBus()
    //
    // Called to set the Communication Bus object to use
    //
    //  Parameter    Description
    //  ---------    -----------------------------
    //  theBus       The Bus object to use
    //  idBus        The bus ID for the target device.
    //

    void setCommunicationBus(QwI2C &theBus, uint8_t idBus);

		bool setAccelFullScale(uint8_t val);
		uint8_t getAccelFullScale();

		uint8_t getUniqueId();

		// Linear, Angular, and Temp Data retrieval 
		int16_t getTemp();
		sfe_raw_data_t getRawAccel();
		sfe_raw_data_t getRawGyro();

		// General Settings
		bool setAccelDataRate(uint8_t rate);
		bool setGyroDataRate(uint8_t rate);
		bool setAccelStatustoInt();
		bool setBlockDataUpdate(bool set);
		uint8_t getBlockDataUpdate();
		
		// Sensor Hub Settings
		bool setHubODR(uint8_t rate);
		bool setHubSensor(uint8_t sensor, sfe_hub_sensor_settings_t* settings);

		// Status
		bool checkStatus();
		bool checkAccelStatus();
		bool checkGyroStatus();
		bool checkTempStatus();

		// Conversions
		void convert2gToMg(int16_t* data, uint8_t len);
		void convert4gToMg(int16_t* data, uint8_t len);
		void convert8gToMg(int16_t* data, uint8_t len);
		void convert16gToMg(int16_t* data, uint8_t len);
		void convert125dpsToMdps(int16_t* data, uint8_t len);
		void convert250dpsToMdps(int16_t* data, uint8_t len);
		void convert500dpsToMdps(int16_t* data, uint8_t len);
		void convert1000dpsToMdps(int16_t* data, uint8_t len);
		void convert2000dpsToMdps(int16_t* data, uint8_t len);
		void convert4000dpsToMdps(int16_t* data, uint8_t len);
		void convertToCelsius(int16_t* data, uint8_t len);


		//Class structs - Better way to do this than instancing global structs??
		sfe_raw_data_t sfeAccelData;
		sfe_raw_data_t sfeGyroData;


	private: 

		QwI2C *_i2cBus; 
		uint8_t _i2cAddress;
		stmdev_ctx_t sfe_dev; 
};



