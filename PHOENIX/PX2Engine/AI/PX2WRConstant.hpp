// PX2WRConstant.hpp

#ifndef PX2WRCONSTANT_HPP
#define PX2WRCONSTANT_HPP

#define RECV_BUFFER_SIZE                65536		
#define CMD_FRAME_HEADER_START          0
#define CMD_FRAME_HEADER_LENGTH_H       4
#define CMD_FRAME_HEADER_LENGTH_L       5
#define CMD_FRAME_HEADER_CHECK_SUM      6
#define CMD_FRAME_HEADER_TYPE           7
#define CMD_FRAME_HEADER_TOTAL_INDEX_H  8
#define CMD_FRAME_HEADER_TOTAL_INDEX_L  9
#define CMD_FRAME_HEADER_SUB_PKG_NUM    10  /* 包含的子包数目 */
#define CMD_FRAME_HEADER_SUB_INDEX      11
#define CMD_FRAME_DATA_START            12
#define INDEX_RANGE_MAX                 65415       //65535-(20*60)
#define INDEX_RANGE_MIN                 (20*60)     //60 sencod
#define CMD_FRAME_MAX_LEN               1500
#define CMD_FRAME_MAX_SUB_PKG_NUM       4
#define CMD_FRAME_MIN_SUB_PKG_NUM       2

const char CMD_START_SCAN[]					 = { (char)0xFA, (char)0x5A, (char)0xA5, (char)0xAA, (char)0x00, (char)0x02, (char)0x01, (char)0x01};
const char CMD_STOP_SCAN[]					 = { (char)0xFA, (char)0x5A, (char)0xA5, (char)0xAA, (char)0x00, (char)0x02, (char)0x02, (char)0x02};
const char CMD_REBOOT[]                      = { (char)0xFA, (char)0x5A, (char)0xA5, (char)0xAA, (char)0x00, (char)0x02, (char)0x03, (char)0x03};
const char CMD_GET_SCAN_STATE[]              = { (char)0xFA, (char)0x5A, (char)0xA5, (char)0xAA, (char)0x00, (char)0x02, (char)0x04, (char)0x04};
const char CMD_GET_SERIAL_NUMBER[]			 = { (char)0xFA, (char)0x5A, (char)0xA5, (char)0xAA, (char)0x00, (char)0x02, (char)0x05, (char)0x05};
const char CMD_GET_SCAN_HEART[]				 = { (char)0xFA, (char)0x5A, (char)0xA5, (char)0xAA, (char)0x00, (char)0x02, (char)0x06, (char)0x06};
#define SIZE_OF_CMD sizeof(CMD_START_SCAN)


const char STX = 0xAA;
const char ETX = 0x66;

const unsigned int  FRAME_LENGTH = 1622; //1642;    //FRAME_LENGTH = 1631;

#define SWITCH_UINT16( _Value )   ((uint16_t) (((uint16_t)( _Value ) << 8) | \
                                   ((uint16_t)( _Value ) >> 8)) )

#endif