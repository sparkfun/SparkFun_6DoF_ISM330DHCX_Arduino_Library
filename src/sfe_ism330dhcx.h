#include "sfe_bus.h"
#include "sfe_ism330dhcx_defs.h"
#include "sfe_ism_shim.h"

#define ISM330DHCX_ADDRESS_LOW 0x6A
#define ISM330DHCX_ADDRESS_HIGH 0x6B

struct sfe_ism_raw_data_t
{
    int16_t xData;
    int16_t yData;
    int16_t zData;
};

struct sfe_ism_data_t
{
    float xData;
    float yData;
    float zData;
};

struct sfe_hub_sensor_settings_t
{
    uint8_t address;
    uint8_t subAddress;
    uint8_t lenData;
};

class QwDevISM330DHCX
{
  public:
    QwDevISM330DHCX() : _i2cAddress{0}, _cs{0} {};

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

    void setCommunicationBus(sfe_ISM330DHCX::QwIDeviceBus &theBus, uint8_t i2cAddress);
    void setCommunicationBus(sfe_ISM330DHCX::QwIDeviceBus &theBus);

    bool setAccelFullScale(uint8_t val);
    bool setGyroFullScale(uint8_t val);
    uint8_t getAccelFullScale();
    uint8_t getGyroFullScale();

    uint8_t getUniqueId();

    // Linear, Angular, and Temp Data retrieval
    int16_t getTemp();
    bool getRawAccel(sfe_ism_raw_data_t *accelData);
    bool getRawGyro(sfe_ism_raw_data_t *gyroData);
    bool getAccel(sfe_ism_data_t *accelData);
    bool getGyro(sfe_ism_data_t *gyroData);

    // General Settings
    bool setDeviceConfig(bool enable = true);
    bool deviceReset();
    bool getDeviceReset();
    bool setAccelSlopeFilter(uint8_t val);
    bool setAccelFilterLP2(bool enable = true);
    bool setGyroFilterLP1(bool enable = true);
    bool setGyroLP1Bandwidth(uint8_t val);
    bool setBlockDataUpdate(bool enable = true);
    uint8_t getBlockDataUpdate();
    bool setAccelDataRate(uint8_t rate);
    bool setGyroDataRate(uint8_t rate);
    bool enableTimestamp(bool enable = true);
    bool resetTimestamp();

    // Interrupt Settings
    bool setAccelStatustoInt1(bool enable = true);
    bool setAccelStatustoInt2(bool enable = true);
    bool setGyroStatustoInt1(bool enable = true);
    bool setGyroStatustoInt2(bool enable = true);
    bool setIntNotification(uint8_t val);
    bool setDataReadyMode(uint8_t val);
    bool setPinMode(bool activeLow = true);

    // FIFO Settings
    bool setFifoWatermark(uint16_t val);
    bool setFifoMode(uint8_t val);
    bool setAccelFifoBatchSet(uint8_t val);
    bool setGyroFifoBatchSet(uint8_t val);
    bool setFifoTimestampDec(uint8_t val);
    bool setFIFOThresholdInt1(bool enable);
    bool setBatchCounterInt1(bool enable);

        // Sensor Hub Settings
        bool setHubODR(uint8_t rate);
    bool setHubSensorRead(uint8_t sensor, sfe_hub_sensor_settings_t *settings);
    bool setHubSensorWrite(sfe_hub_sensor_settings_t *settings);
    bool setNumberHubSensors(uint8_t numSensors);
    bool enableSensorI2C(bool enable);
    bool readPeripheralSensor(uint8_t *shReg, uint8_t len);
    bool setHubWriteMode(uint8_t config);
    bool readMMCMagnetometer(uint8_t *magData, uint8_t len);
    bool setHubPassThrough(bool enable = true);
    bool setHubFifoBatching(bool enable = true);
    bool setHubPullUps(bool enable = true);
    bool getHubStatus();
    bool getExternalSensorNack(uint8_t sensor);
    bool resetSensorHub();

    // Self Test
    bool setAccelSelfTest(uint8_t val);
    bool setGyroSelfTest(uint8_t val);

    // Status
    bool checkStatus();
    bool checkAccelStatus();
    bool checkGyroStatus();
    bool checkTempStatus();

    // Conversions
    float convert2gToMg(int16_t data);
    float convert4gToMg(int16_t data);
    float convert8gToMg(int16_t data);
    float convert16gToMg(int16_t data);
    float convert125dpsToMdps(int16_t data);
    float convert250dpsToMdps(int16_t data);
    float convert500dpsToMdps(int16_t data);
    float convert1000dpsToMdps(int16_t data);
    float convert2000dpsToMdps(int16_t data);
    float convert4000dpsToMdps(int16_t data);
    float convertToCelsius(int16_t data);

  private:
    sfe_ISM330DHCX::QwIDeviceBus *_sfeBus;
    uint8_t _i2cAddress;
    uint8_t _cs;
    stmdev_ctx_t sfe_dev;
    uint8_t fullScaleAccel = 0; // Powered down by default
    uint8_t fullScaleGyro = 0;  // Powered down by default
};
