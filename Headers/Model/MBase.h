#ifndef MBASE
#define MBASE

/*
 *   数字量通道的参数 枚举类型说明
*/
typedef enum
{
    UNUSED=0,   //未使用
    NORMAL=1,   //正常使用
    ERROR=2     //有错误
} COM_STATUS;

typedef enum
{
    CHECK_NONE=0,
    CHECK_ODD=1,
    CHECK_EVEN=2
} DATA_CHECK_TYPE;

typedef enum
{
    H4321=0,
    H1234=1,
    H3412=2
} PLC_FLOAT_TYPE;


/*
 *   模拟量通道的参数 枚举类型说明
*/

typedef enum
{
    VOLTAGE=0,  //电压
    CURRENT=1   //电流
} SIGNAL_TYPE;

/*
 *   开关量通道的参数 枚举类型说明
*/

typedef enum
{
    POSITIVE_LOGIC=1, //开关量正逻辑
    NEGATIVE_LOGIC=-1 //开关量负逻辑
} LOGIC_TYPE;


/*
 *  继电器通道的参数 枚举类型说明
 */

typedef enum
{
    ALARM = 1, //告警
    THREE_COUNT = 2, //3秒计数
    SPARE = 3  //备用
} CH_STATUS_TYPE;

class MBase
{


};

#endif // MBASE

