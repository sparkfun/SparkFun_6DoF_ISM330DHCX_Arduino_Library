//Accelerometer Full Scale
#define ISM_2g    0
#define ISM_16g   1
#define ISM_4g    2
#define ISM_8g    3

//Gyroscope Full Scale
#define ISM_125dps    2
#define ISM_250dps    0
#define ISM_500dps    4
#define ISM_1000dps   8
#define ISM_2000dps   12
#define ISM_4000dps   1

//Acceleromter Output Data Rate
#define ISM_XL_ODR_OFF     0
#define ISM_XL_ODR_12Hz5   1
#define ISM_XL_ODR_26Hz    2
#define ISM_XL_ODR_52Hz    3
#define ISM_XL_ODR_104Hz   4
#define ISM_XL_ODR_208Hz   5
#define ISM_XL_ODR_416Hz   6
#define ISM_XL_ODR_833Hz   7
#define ISM_XL_ODR_1666Hz  8
#define ISM_XL_ODR_3332Hz  9
#define ISM_XL_ODR_6667Hz  10
#define ISM_XL_ODR_1Hz6    11

//Gyroscope Output Data Rate
#define ISM_GY_ODR_OFF     0
#define ISM_GY_ODR_12Hz    1
#define ISM_GY_ODR_26Hz    2
#define ISM_GY_ODR_52Hz    3
#define ISM_GY_ODR_104Hz   4
#define ISM_GY_ODR_208Hz   5
#define ISM_GY_ODR_416Hz   6
#define ISM_GY_ODR_833Hz   7
#define ISM_GY_ODR_1666Hz  8
#define ISM_GY_ODR_3332Hz  9
#define ISM_GY_ODR_6667Hz  10

//Gyro Bandwidth set
#define ISM_ULTRA_LIGHT   0
#define ISM_VERY_LIGHT    1
#define ISM_LIGHT         2
#define ISM_MEDIUM        3
#define ISM_STRONG        4
#define ISM_VERY_STRONG   5
#define ISM_AGGRESSIVE    6
#define ISM_XTREME        7

// Gyro high performance filters												
#define ISM_HP_FILTER_NONE    0x00
#define ISM_HP_FILTER_16mHz   0x80
#define ISM_HP_FILTER_65mHz   0x81
#define ISM_HP_FILTER_260mHz  0x82
#define ISM_HP_FILTER_1Hz04   0x83

//Accel Regular Performance Filter Settings
#define ISM_HP_PATH_DISABLE_ON_OUT  0x00
#define ISM_SLOPE_ODR_DIV_4         0x10
#define ISM_HP_ODR_DIV_10           0x11
#define ISM_HP_ODR_DIV_20           0x12
#define ISM_HP_ODR_DIV_45           0x13
#define ISM_HP_ODR_DIV_100          0x14
#define ISM_HP_ODR_DIV_200          0x15
#define ISM_HP_ODR_DIV_400          0x16
#define ISM_HP_ODR_DIV_800          0x17
#define ISM_HP_REF_MD_ODR_DIV_10    0x31
#define ISM_HP_REF_MD_ODR_DIV_20    0x32
#define ISM_HP_REF_MD_ODR_DIV_45    0x33
#define ISM_HP_REF_MD_ODR_DIV_100   0x34
#define ISM_HP_REF_MD_ODR_DIV_200   0x35
#define ISM_HP_REF_MD_ODR_DIV_400   0x36
#define ISM_HP_REF_MD_ODR_DIV_800   0x37
#define ISM_LP_ODR_DIV_10           0x01
#define ISM_LP_ODR_DIV_20           0x02
#define ISM_LP_ODR_DIV_45           0x03
#define ISM_LP_ODR_DIV_100          0x04
#define ISM_LP_ODR_DIV_200          0x05
#define ISM_LP_ODR_DIV_400          0x06
#define ISM_LP_ODR_DIV_800          0x07

//FIFO Mode Settings
#define ISM_BYPASS_MODE           0x00
#define ISM_FIFO_MODE             0x01
#define ISM_STREAM_TO_FIFO_MODE   0x03
#define ISM_BYPASS_TO_STREAM_MODE 0x04
#define ISM_STREAM_MODE           0x06
#define ISM_BYPASS_TO_FIFO_MODE   0x07

//FIFO Accelerometer Batch Settings
#define ISM_XL_NOT_BATCHED       0x00
#define ISM_XL_BATCH_AT_12Hz5    0x01
#define ISM_XL_BATCH_AT_26Hz     0x02
#define ISM_XL_BATCH_AT_52Hz     0x03
#define ISM_XL_BATCH_AT_104Hz    0x04
#define ISM_XL_BATCH_AT_208Hz    0x05
#define ISM_XL_BATCH_AT_417Hz    0x06
#define ISM_XL_BATCH_AT_833Hz    0x07
#define ISM_XL_BATCH_AT_1667Hz   0x08
#define ISM_XL_BATCH_AT_3333Hz   0x09
#define ISM_XL_BATCH_AT_6667Hz   0x0A
#define ISM_XL_BATCH_6Hz5        0x0B

//FIFO Gyroscope Batch Settings
#define ISM_GY_NOT_BATCHED      0x00
#define ISM_GY_BATCH_AT_12Hz5    0x01
#define ISM_GY_BATCH_AT_26Hz     0x02
#define ISM_GY_BATCH_AT_52Hz     0x03
#define ISM_GY_BATCH_AT_104Hz    0x04
#define ISM_GY_BATCH_AT_208Hz    0x05
#define ISM_GY_BATCH_AT_417Hz    0x06
#define ISM_GY_BATCH_AT_833Hz    0x07
#define ISM_GY_BATCH_AT_1667Hz   0x08
#define ISM_GY_BATCH_AT_3333Hz   0x09
#define ISM_GY_BATCH_AT_6667Hz   0x0A
#define ISM_GY_BATCH_6Hz5        0x0B

//Decimation rate
#define ISM_NO_DECIMATION 0x00
#define ISM_DEC_1         0x01
#define ISM_DEC_8         0x02
#define ISM_DEC_32        0x03

//Interrupt pin notification settings.
#define ISM_ALL_INT_PULSED            0x00
#define ISM_BASE_LATCHED_EMB_PULSED   0x01
#define ISM_BASE_PULSED_EMB_LATCHED   0x02
#define ISM_ALL_INT_LATCHED           0x03

#define ISM_SH_ODR_104Hz 0x00
#define ISM_SH_ODR_52Hz  0x01
#define ISM_SH_ODR_26Hz  0x02
#define ISM_SH_ODR_13Hz  0x03
