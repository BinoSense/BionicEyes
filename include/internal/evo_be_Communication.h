//
// Created by be on 18-9-29.
//

#ifndef PROJECT_BE_COMMUNICATION_H
#define PROJECT_BE_COMMUNICATION_H

#ifdef _WIN32
#ifdef EVO_BE_COMMUNICATION_EXPORTS
#define EVO_BE_COMMUNICATION_API __declspec(dllexport)
#else
#define EVO_BE_COMMUNICATION_API __declspec(dllimport)
#endif	// EVO_BE_COMMUNICATION_EXPORTS
#else
#define EVO_BE_COMMUNICATION_API
#endif	// _WIN32

#include <evo_be_Common.h>
#include <vector>

using namespace evo_be;
using namespace std;

namespace evo_be
{
    /*!
    * @brief : Enumeration defines the possible data bit type for the serial port.
    */
    enum Data_Bit
    {
        enumFiveBits = 5,
        enumSixBits = 6,
        enumSevenBits = 7,
        enumEightBits = 8,
    };

    /*!
    * @brief : Enumeration defines the possible parity bit type for the serial port.
    */
    enum Parity_Bit
    {
        enumParityNone = 0,
        enumParityOdd = 1,
        enumParityEven = 2,
        enumParityMark = 3,
        enumParitySpace = 4,
    };

    /*!
    * @brief : Enumeration defines the possible stop bit types for the serial port.
    */
    enum Stop_Bit
    {
        enumStopBitsOne = 1,
        enumStopBitsTwo = 2,
        enumStopBitsOnePointFive = 3,
    };

    /*!
    * @brief : Enumeration defines the possible flow control types for the serial port.
    */
    enum Flow_Control
    {
        enumFlowControlNone = 0,
        enumFlowControlRtsCts = 1,
        enumFlowControlXonXoff = 2
    };

    /*!
    * @brief : Structure that describes a serial device.
    */
    struct SerialPortInfo
    {
        /*! Address of the serial port (this can be passed to the constructor of Serial). */
        std::string port;

        /*! Human readable description of serial device if available. */
        std::string description;

        /*! Hardware ID (e.g. VID:PID of USB serial devices) or "n/a" if not available. */
        std::string hardware_id;
    };

    /*!	Structure decode
    * @brief : User-defined structure decode.
    */
    template <class T>
    T StruDecode(void* p)
    {
        T re;
        //memset(&re, 0, sizeof(re));
        memcpy(&re, p, sizeof(re));
        return re;
    }

    template <class T>
    void StruDecode(void* p, T &re)
    {
        //memset(&re, 0, sizeof(re));
        memcpy(&re, p, sizeof(re));
        return;
    }

    /*!	Structure encode
    * @brief : User-defined structure encode.
    * @attention : Need to release memory after using this. need "delete[] p;"
    */
    template <class T>
    char* StruEncode(T &data)
    {
        char *ch = new char[sizeof(data)];
        memcpy(ch, &data, sizeof(data));
        return ch;
    }


    enum BE_SerialConnectType
    {
        enumByPortName,             // "/dev/ttyUSB0"   "COM1"
        enumByDescription,          // "USB2.0-Serial"
        enumByHardwareId,           // "USB VID:PID=1a86:7523"
    };

    class EVO_BE_COMMUNICATION_API CBE_Communication_Serial
    {
    public:
		static CBE_Communication_Serial *create(BE_SerialConnectType nameType = enumByPortName,
                                        const std::string &port = "",
                                        uint32_t baudrate = 115200,
                                        Data_Bit databits = Data_Bit::enumEightBits,
                                        Parity_Bit parity = Parity_Bit::enumParityNone,
                                        Stop_Bit stopbits = Stop_Bit::enumStopBitsOne,
                                        Flow_Control flowcontrol = Flow_Control::enumFlowControlNone,
                                        void *logger_ptr = NULL);

        virtual ~CBE_Communication_Serial();

        virtual vector<SerialPortInfo> getListPorts() = 0;

        virtual void flushInputBuffer() = 0;
        virtual void flushOutputBuffer() = 0;
        virtual int getReadableSize() = 0;

        virtual void openSerialPort() = 0;
        virtual bool isOpenSerialPort() = 0;
        virtual void closeSerialPort() = 0;
        virtual void findAndInitSerialDevice() = 0;
        virtual string findSerialDevice(string portname, BE_SerialConnectType nameType = enumByPortName) = 0;

        virtual bool sendData(const uint8_t *data, size_t len) = 0;
        virtual bool receiveData(uint8_t *data, size_t len) = 0;
    };

    enum Ethernet_Connect_Type
    {
        enumEth_Pair = 0,		// P2P communication
        enumEth_Pub = 1,		// publish mode
        enumEth_Sub = 2,		// Subscription mode
        enumEth_Req = 3,		// request mode
        enumEth_Rep = 4,		// reply mode
        enumEth_Dealer = 5,		// dealer mode (multi req)
        enumEth_Router = 6,		// router mode (multi rep)
        enumEth_Pull = 7,		// pull mode
        enumEth_Push = 8,		// push mode
    };

    enum Ethernet_Protocal_Type
    {
        enumProt_Tcp = 0,		// Using tcp to achieve unicast communication
        enumProt_Ipc = 1,		// Local inter-process communication transport
        enumProt_Inproc = 2,	// local in-process (inter-thread) communication transport
        enumProt_Pgm = 3,		// Using PGM to achieve multicast communication
        enumProt_Epgm = 4,		// Using ePGM to achieve multicast communication
    };

    enum Ethernet_SendRecv_Type
    {
        enumDontWait = 1,
        enumSndMore = 2,
    };

    enum Ethernet_SocketOption
    {
        enumEth_Sockopt_IDENTITY = 5,		// (TYPE: binary) Set socket identity
        enumEth_Sockopt_SUBSCRIBE = 6,		// (TYPE: binary) Establish message filter
        enumEth_Sockopt_UNSUBSCRIBE = 7,	// (TYPE: binary) Remove message filter
        enumEth_Sockopt_RATE = 8,			// (TYPE: int, UNIT: kbits/s, default: 100) Set multicast data rate
        enumEth_Sockopt_SNDBUF = 11,		// (TYPE: int, UNIT: Byte, default: -1, OS default) Set kernel transmit buffer size
        enumEth_Sockopt_RCVBUF = 12,		// (TYPE: int, UNIT: Byte, default: -1, OS default) Set kernel receive buffer size
        enumEth_Sockopt_SNDHWM = 23,		// (TYPE:int, 0: no limit, default: 1000) Set high water mark for outbound messages
        enumEth_Sockopt_RCVHWM = 24,		// (TYPE:int, 0: no limit, default: 1000) Set high water mark for inbound messages
        enumEth_Sockopt_CONFLATE = 54,		// (TYPE:int, UNIT: bool, default: false) Keep only last message
    };

    class EVO_BE_COMMUNICATION_API CBE_Communication_Ethernet
    {
    public:
		static CBE_Communication_Ethernet *create(Ethernet_Connect_Type connectType = enumEth_Dealer, void *logger_ptr = NULL);
        virtual ~CBE_Communication_Ethernet();

        /*!
        * @brief : Server part
        *
        */
        virtual void bind(string ipAddr, int port, Ethernet_Protocal_Type procType = enumProt_Tcp) = 0;

        /*!
        * @brief : Client part
        *
        */
        virtual void connect(string ipAddr, int port, Ethernet_Protocal_Type procType = enumProt_Tcp) = 0;

        virtual void setsockopt(Ethernet_SocketOption option, const void * optval, size_t optvallen) = 0;
        virtual void getsockopt(Ethernet_SocketOption option, void * optval, size_t *optvallen) const = 0;

        virtual int receive(string &string, int flag = 0) = 0;

        // need allocate memory!!! for example: char* data = new char[len];
        virtual int receive(void *data, size_t len, int flag = 0) = 0;
        virtual int receive(vector<unsigned char> &vec, int flag = 0) = 0;
        // need to delete memory!!! for example: delete[] data;
        virtual int receive(uchar *data, int flag = 0) = 0;

        virtual bool send(const string &string, int flag = 0) = 0;
        virtual bool send(const void *data, size_t len, int flag = 0) = 0;
        virtual bool send(const vector<unsigned char> &vec, int flag = 0) = 0;

        virtual bool receive_Special(void *data, size_t len, int flag = 0) = 0;
        virtual bool send_Special(const void *data, size_t len, int flag = 0) = 0;

        virtual void disConnect() = 0;
        virtual void unBind() = 0;
        virtual void closeCommunication() = 0;

    };

	class EVO_BE_COMMUNICATION_API CBE_Communication_Ethernet_Special
	{
	public:
		static CBE_Communication_Ethernet_Special *create(Ethernet_Connect_Type connectType, void *logger_ptr = NULL);
		virtual ~CBE_Communication_Ethernet_Special();
		virtual bool requestBeIpAddress(vector<BE_IpInfo> &result) = 0;
		virtual bool replyToSendAddrInfo() = 0;
		virtual string ipAddressTran(unsigned int ipaddr) = 0;
		virtual uint32_t ipAddressTran(string ipaddr) = 0;
		virtual void inputDeviceName(char* name) = 0;
	};
}




#endif //PROJECT_BE_COMMUNICATION_H
