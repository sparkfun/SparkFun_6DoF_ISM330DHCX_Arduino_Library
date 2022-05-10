#include "qwiic_i2c.h"
#include "sfe_ism_shim.h"


#define ISM330DHCX_ADDRESS_LOW 0x6A
#define ISM330DHCX_ADDRESS_HIGH 0x6B

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

		int32_t setAccelFullScale(uint8_t val);

		uint8_t getUniqueId();

	private: 

		bool _isInitialized;
		QwI2C *_i2cBus; 
		uint8_t _i2cAddress;
};



