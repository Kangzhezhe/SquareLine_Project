#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// #pragma warning(disable:4996)
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mysql.h>
#include <stdio.h>
#include <time.h>
char buffertime[80];
char* timetest()
{
	time_t rawtime;
	struct tm* info;
	time(&rawtime);
	info = localtime(&rawtime);
	sprintf(buffertime,"%d %d %d %d", info->tm_mon+1,info->tm_mday,info->tm_hour,info->tm_min);
	printf("%s", buffertime);
	return buffertime;
}

//---------------------------------------MQTT协议相关的子函数声明-------------------------------------------------------
//发布主题
unsigned char MQTT_PublishData(char* topic, char* message, unsigned char qos);
//订阅或者取消订阅主题
unsigned char MQTT_SubscribeTopic(char* topic, unsigned char qos, unsigned char whether);
//登录MQTT服务器
unsigned char MQTT_Connect(char* ClientID, char* Username, char* Password);
//MQTT协议缓冲区初始化
void MQTT_Init(void);
//调用底层接口发送数据包
void MQTT_SendBuf(unsigned char* buf, unsigned short len);
//MQTT协议里最底层的接口，最底层的如果要移植协议到其他地方运行，那么改这里就行了。其他地方不用改的。
int Client_SendData(unsigned char* buff, unsigned int len);//发送数据到服务器
int Client_GetData(unsigned char* buff);//从服务器获取数据
void ReceiveData(void);
bool connectDB(MYSQL& mysql);
bool addTableData(char* faceid, char* tiwen, char* xueyang, char* xinlv, char* tabelname);
bool queTableData(char* faceid, char* tablename);
void parseAndGetValue(char* buffer, const char* keyword, char* result);
bool addTableData_yao(char* faceid, char* yao, char* tabelname);
//---------------------------------------全局变量定义--------------------------------------------------------------------

#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
#define SQL_MAX 256	

unsigned char mqtt_rxbuf[1024 * 1024];//发送数据缓存区
unsigned char mqtt_txbuf[1024 * 1024];//接收数据缓存区
unsigned int mqtt_rxlen;
unsigned int mqtt_txlen;


typedef enum
{
	//名字 	    值 			报文流动方向 	描述
	M_RESERVED1 = 0,	//	禁止	保留
	M_CONNECT,	//	客户端到服务端	客户端请求连接服务端
	M_CONNACK,	//	服务端到客户端	连接报文确认
	M_PUBLISH,	//	两个方向都允许	发布消息
	M_PUBACK,	//	两个方向都允许	QoS 1消息发布收到确认
	M_PUBREC,	//	两个方向都允许	发布收到（保证交付第一步）
	M_PUBREL,	//	两个方向都允许	发布释放（保证交付第二步）
	M_PUBCOMP,	//	两个方向都允许	QoS 2消息发布完成（保证交互第三步）
	M_SUBSCRIBE,	//	客户端到服务端	客户端订阅请求
	M_SUBACK,	//	服务端到客户端	订阅请求报文确认
	M_UNSUBSCRIBE,	//	客户端到服务端	客户端取消订阅请求
	M_UNSUBACK,	//	服务端到客户端	取消订阅报文确认
	M_PINGREQ,	//	客户端到服务端	心跳请求
	M_PINGRESP,	//	服务端到客户端	心跳响应
	M_DISCONNECT,	//	客户端到服务端	客户端断开连接
	M_RESERVED2,	//	禁止	保留
}_typdef_mqtt_message;

//连接成功服务器回应 20 02 00 00
//客户端主动断开连接 e0 00
const unsigned char parket_connetAck[] = { 0x20,0x02,0x00,0x00 };
const unsigned char parket_disconnet[] = { 0xe0,0x00 };
const unsigned char parket_heart[] = { 0xc0,0x00 };
const unsigned char parket_heart_reply[] = { 0xc0,0x00 };
const unsigned char parket_subAck[] = { 0x90,0x03 };

void MQTT_Init(void)
{
	//缓冲区赋值
	mqtt_rxlen = sizeof(mqtt_rxbuf);
	mqtt_txlen = sizeof(mqtt_txbuf);
	memset(mqtt_rxbuf, 0, mqtt_rxlen);
	memset(mqtt_txbuf, 0, mqtt_txlen);

}


/*
函数功能: 登录服务器
函数返回值: 0表示成功 1表示失败
*/
unsigned char MQTT_Connect(char* ClientID, char* Username, char* Password)
{
	unsigned short i, j;
	int ClientIDLen = (int)strlen(ClientID);
	int UsernameLen = (int)strlen(Username);
	int PasswordLen = (int)strlen(Password);
	unsigned int DataLen;
	mqtt_txlen = 0;
	unsigned int size = 0;
	unsigned char buff[256];

	//可变报头+Payload  每个字段包含两个字节的长度标识
	DataLen = 10 + (ClientIDLen + 2) + (UsernameLen + 2) + (PasswordLen + 2);

	//固定报头
	//控制报文类型
	mqtt_txbuf[mqtt_txlen++] = 0x10;		//MQTT Message Type CONNECT
	//剩余长度(不包括固定头部)
	do
	{
		unsigned char encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if (DataLen > 0)
			encodedByte = encodedByte | 128;
		mqtt_txbuf[mqtt_txlen++] = encodedByte;
	} while (DataLen > 0);

	//可变报头
	//协议名
	mqtt_txbuf[mqtt_txlen++] = 0;        	// Protocol Name Length MSB    
	mqtt_txbuf[mqtt_txlen++] = 4;           // Protocol Name Length LSB    
	mqtt_txbuf[mqtt_txlen++] = 'M';        	// ASCII Code for M    
	mqtt_txbuf[mqtt_txlen++] = 'Q';        	// ASCII Code for Q    
	mqtt_txbuf[mqtt_txlen++] = 'T';        	// ASCII Code for T    
	mqtt_txbuf[mqtt_txlen++] = 'T';        	// ASCII Code for T    
	//协议级别
	mqtt_txbuf[mqtt_txlen++] = 4;        		// MQTT Protocol version = 4    
	//连接标志
	mqtt_txbuf[mqtt_txlen++] = 0xc2;        	// conn flags 
	mqtt_txbuf[mqtt_txlen++] = 0;        		// Keep-alive Time Length MSB    
	mqtt_txbuf[mqtt_txlen++] = 100;        	// Keep-alive Time Length LSB  100S心跳包  

	mqtt_txbuf[mqtt_txlen++] = BYTE1(ClientIDLen);// Client ID length MSB    
	mqtt_txbuf[mqtt_txlen++] = BYTE0(ClientIDLen);// Client ID length LSB  	
	memcpy(&mqtt_txbuf[mqtt_txlen], ClientID, ClientIDLen);
	mqtt_txlen += ClientIDLen;
	if (UsernameLen > 0)
	{
		mqtt_txbuf[mqtt_txlen++] = BYTE1(UsernameLen);		//username length MSB    
		mqtt_txbuf[mqtt_txlen++] = BYTE0(UsernameLen);    	//username length LSB    
		memcpy(&mqtt_txbuf[mqtt_txlen], Username, UsernameLen);
		mqtt_txlen += UsernameLen;
	}

	if (PasswordLen > 0)
	{
		mqtt_txbuf[mqtt_txlen++] = BYTE1(PasswordLen);		//password length MSB    
		mqtt_txbuf[mqtt_txlen++] = BYTE0(PasswordLen);    	//password length LSB  
		memcpy(&mqtt_txbuf[mqtt_txlen], Password, PasswordLen);
		mqtt_txlen += PasswordLen;
	}
	for (i = 0; i < 5; i++)
	{
		memset(mqtt_rxbuf, 0, mqtt_rxlen);
		MQTT_SendBuf(mqtt_txbuf, mqtt_txlen);
		size = Client_GetData(buff);//从服务器获取数据
		if (size <= 0)continue;
		memcpy(mqtt_rxbuf, buff, size);

		printf("登录应答:\r\n");
		for (j = 0; j < size; j++)
		{
			printf("%#X ", buff[j]);
		}
		printf("\r\n");

		if (mqtt_rxbuf[0] == parket_connetAck[0] && mqtt_rxbuf[1] == parket_connetAck[1]) //连接成功			   
		{
			return 0;//连接成功
		}
	}
	return 1;
}

/*
函数功能: MQTT订阅/取消订阅数据打包函数
函数参数:
	topic       主题
	qos         消息等级 0:最多分发一次  1: 至少分发一次  2: 仅分发一次
	whether     订阅/取消订阅请求包 (1表示订阅,0表示取消订阅)
返回值: 0表示成功 1表示失败
*/
unsigned char MQTT_SubscribeTopic(char* topic, unsigned char qos, unsigned char whether)
{
	unsigned char i, j;
	mqtt_txlen = 0;
	unsigned int size = 0;
	unsigned char buff[256];
	unsigned int topiclen = (int)strlen(topic);
	unsigned int DataLen = 2 + (topiclen + 2) + (whether ? 1 : 0);//可变报头的长度（2字节）加上有效载荷的长度
	//固定报头
	//控制报文类型
	if (whether)mqtt_txbuf[mqtt_txlen++] = 0x82; //消息类型和标志订阅
	else	mqtt_txbuf[mqtt_txlen++] = 0xA2;    //取消订阅
	//剩余长度
	do
	{
		unsigned char encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if (DataLen > 0)
			encodedByte = encodedByte | 128;
		mqtt_txbuf[mqtt_txlen++] = encodedByte;
	} while (DataLen > 0);
	//可变报头
	mqtt_txbuf[mqtt_txlen++] = 0;			//消息标识符 MSB
	mqtt_txbuf[mqtt_txlen++] = 0x01;        //消息标识符 LSB
	//有效载荷
	mqtt_txbuf[mqtt_txlen++] = BYTE1(topiclen);//主题长度 MSB
	mqtt_txbuf[mqtt_txlen++] = BYTE0(topiclen);//主题长度 LSB   
	memcpy(&mqtt_txbuf[mqtt_txlen], topic, topiclen);
	mqtt_txlen += topiclen;
	if (whether)
	{
		mqtt_txbuf[mqtt_txlen++] = qos;//QoS级别
	}
	for (i = 0; i < 100; i++)
	{
		memset(mqtt_rxbuf, 0, mqtt_rxlen);
		MQTT_SendBuf(mqtt_txbuf, mqtt_txlen);
		//printf("订阅消息发布成功\n");
		size = Client_GetData(buff);//从服务器获取数据
		if (size <= 0)
		{
			continue;
		}
		memcpy(mqtt_rxbuf, buff, size);

		printf("订阅应答:\r\n");
		for (j = 0; j < size; j++)
		{
			printf("%#X ", buff[j]);
		}
		printf("\r\n");

		if (mqtt_rxbuf[0] == parket_subAck[0] && mqtt_rxbuf[1] == parket_subAck[1]) //连接成功			   
		{
			return 0;//连接成功
		}
		Sleep(1000);
	}
	return 1; //失败
}



//MQTT发布数据打包函数
//topic   主题 
//message 消息
//qos     消息等级 
unsigned char MQTT_PublishData(char* topic, char* message, unsigned char qos)
{
	unsigned int topicLength = (int)strlen(topic);
	unsigned int messageLength = (int)strlen(message);
	unsigned short id = 0;
	unsigned int DataLen;
	mqtt_txlen = 0;

	printf("上报JSON消息长度:%d\r\n", messageLength);
	printf("message=%s\r\n", message);
	//有效载荷的长度这样计算：用固定报头中的剩余长度字段的值减去可变报头的长度
	//QOS为0时没有标识符
	//数据长度             主题名   报文标识符   有效载荷
	if (qos)	DataLen = (2 + topicLength) + 2 + messageLength;
	else	DataLen = (2 + topicLength) + messageLength;

	//固定报头
	//控制报文类型
	mqtt_txbuf[mqtt_txlen++] = 0x30;    // MQTT Message Type PUBLISH  

	//剩余长度
	do
	{
		unsigned char encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if (DataLen > 0)
			encodedByte = encodedByte | 128;
		mqtt_txbuf[mqtt_txlen++] = encodedByte;
	} while (DataLen > 0);
	mqtt_txbuf[mqtt_txlen++] = BYTE1(topicLength);//主题长度MSB
	mqtt_txbuf[mqtt_txlen++] = BYTE0(topicLength);//主题长度LSB 
	memcpy(&mqtt_txbuf[mqtt_txlen], topic, topicLength);//拷贝主题
	mqtt_txlen += topicLength;

	//报文标识符
	if (qos)
	{
		mqtt_txbuf[mqtt_txlen++] = BYTE1(id);
		mqtt_txbuf[mqtt_txlen++] = BYTE0(id);
		id++;
	}
	memcpy(&mqtt_txbuf[mqtt_txlen], message, messageLength);
	mqtt_txlen += messageLength;

	MQTT_SendBuf(mqtt_txbuf, mqtt_txlen);
	return mqtt_txlen;
}


void MQTT_SendBuf(unsigned char* buf, unsigned short len)
{
	Client_SendData(buf, len);//发送数据到服务器
}





//-----------------------------------------MQTT服务器的参数------------------------------------------------------------
//服务器IP
#define SERVER_IP "2407:c080:0801:fffe:0000:0000:7c46:da83"
#define SERVER_PORT 1883 //端口号

//MQTT三元组
#define ClientID "6687def786799a26c45e94d5_window_0_0_2024070605"
#define Username "6687def786799a26c45e94d5_window"
#define Password "a675dfa234557eaae463fd23c0fa10e0831a058d3171c56778a881f93fd72ffe"//密文 

//订阅主题:
#define SET_TOPIC  "/test/M2M/aa/#"//订阅
//发布主题:
#define POST_TOPIC "/test/M2M/hh"
#define USELESS_TOPIC "$oc/devices/{device_id}/sys/messages/up"
//#define POST_TOPIC "$oc/devices/6685084786799a26c45e16f9_1111/sys/properties/report"//发布



//-----------------------------------------主函数------------------------------------------------------------

char mqtt_message[2024];//数据缓存区
char mqtt_message1[2024];//数据缓存区

SOCKET connectSocket; //网络套接字
WSADATA wsaData; //创建一个结构体变量，用于存储关于Winsock库的信息

double TEMP = 10.0;
extern int main1();
int main()
{
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData); //初始化Winsock库，指定版本号2.2，检查返回值
	if (result != 0)
	{
		printf("WSAStartup failed: %d\r\n", result);//输出错误信息并退出程序
		return 1;
	}

	connectSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); //创建一个TCP套接字，检查返回值
	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %d", WSAGetLastError());//输出错误信息并退出程序
		WSACleanup(); //清除Winsock库
		return 1;
	}


	sockaddr_in6 service; //创建一个结构体变量，用于存储服务器地址信息
	memset(&service, 0, sizeof(sockaddr_in6));
	service.sin6_port = htons(SERVER_PORT); //将端口号从主机字节序转换为网络字节序，并存储在结构体中
	inet_pton(AF_INET6, SERVER_IP, &service.sin6_addr); //将字符串类型的IP地址转换为二进制网络字节序的IP地址，并存储在结构体中
	service.sin6_family = AF_INET6; //指定地址族为IPv4
	//service.sin6_scope_id = % 3;
	result = connect(connectSocket, (SOCKADDR*)&service, sizeof(service)); //连接到服务器，检查返回值
	if (result == SOCKET_ERROR)
	{
		std::cout << "connect failed with error: " << WSAGetLastError() << std::endl; //输出错误信息并退出程序
		closesocket(connectSocket); //关闭套接字
		WSACleanup(); //清除Winsock库
		return 1;
	}

	std::cout << "Connected to server." << std::endl; //连接成功，输出消息

	MQTT_Init();


	while (1)
	{
		/*登录服务器*/
		if (MQTT_Connect((char*)ClientID, (char*)Username, (char*)Password) == 0)
		{
			break;
		}
		// 延时1000毫秒，即1秒
		Sleep(1000);
		printf("MQTT服务器登录校验中....\n");
	}


	printf("连接成功_666\r\n");
	//订阅物联网平台数据
	int stat = MQTT_SubscribeTopic((char*)SET_TOPIC, 1, 1);
	if (stat)
	{
		printf("订阅失败\r\n");
		closesocket(connectSocket); //关闭套接字
		WSACleanup(); //清除Winsock库
		return 1;
	}
	printf("订阅成功\r\n");
	MYSQL mysql;
	bool xx = connectDB(mysql);
	//xx = addTableData(2, 35.7, (char*)"35.7");
	//xx = queTableData((char*)"2");

	/*创建线程*/
		/*创建线程*/
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveData, NULL, 0, NULL);
	if (hThread == NULL) {
		printf("CreateThread failed.\n");
		return 1;
	}


	while (1)
	{
		//sprintf(mqtt_message, "{\"services\": [{\"service_id\": \"1\",\"properties\":{\"faceid\":%d}}]}", (int)(2));//温度
		//sprintf(mqtt_message, "12333333");//温度
		////发布主题
		//MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);
		//printf("发布消息成功\r\n");
		MQTT_PublishData((char*)USELESS_TOPIC,(char*) "PING", 0);
		Sleep(10000);
	 //timetest();
		//addTableData((char*)"3",(char*)"20", (char*)"90", (char*)"110", (char*)"NULL", (char*)"NULL", (char*)"NULL", (char*)"NULL", (char*)"facetest");
	}
}



/*发送数据到服务器*/
int Client_SendData(unsigned char* buff, unsigned int len)
{
	int result = send(connectSocket, (const char*)buff, len, 0); //向服务器发送数据，检查返回值
	if (result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl; //输出错误信息并退出程序
		return -1;
	}
	return 0;
}



/*获取服务器下发数据*/
int Client_GetData(unsigned char* buff)
{
	int result = recv(connectSocket, (char*)buff, 200, 0); //从服务器接收数据，检查返回值
	if (result == SOCKET_ERROR)
	{
		std::cout << "recv failed with error: " << WSAGetLastError() << std::endl; //输出错误信息并退出程序
		return -1;
	}
	return result;
}

// 处理服务器下发的数据
void ReceiveData(void)
{
	// 接收数据
	char buffer[1024];
	char request_id[100];
	char send_cmd[500];
	char faceid[6];
	char tiwen[6];
	char xueyang[6];
    char yao[256];
	char xinlv[6];
	char result[100]; // Buffer to store the result
	int recvSize;
	while (1)
	{
		//等待服务器下发消息
        memset(buffer, 0, 1024);
		recvSize = recv(connectSocket, buffer, 1024, 0);
		if (recvSize == SOCKET_ERROR)
		{
			std::cout << "网络断开连接: " << WSAGetLastError() << std::endl; //输出错误信息并退出程序
			return;
		}
		if (recvSize > 0)
		{
			printf("服务器下发消息:\r\n");
			//接收下发的数据
			for (int i = 0; i < recvSize; i++)
			{
				printf("%c", buffer[i]);
			}
			printf("\r\n");
			//处理信息
			if (strstr((char*)&buffer[5], "test/M2M"))
			{
				printf("(命令)发布11\r\n");
				char* p = NULL;
				char* q = NULL;
				int x = 0;
				p = strstr((char*)&buffer[5], "faceid");
				q = strstr((char*)&buffer[5], "tiwen");
				if (p)
				{
					if (q)
						x = 1;//x=1说明是要上传数据
                    else if(strstr((char*)&buffer[5], "yao"))
                        x = 2;
					printf("mingling\r\n");
				}
				if (x == 1)
				{
					parseAndGetValue(buffer, "faceid", faceid);
					printf("faceid:%s\r\n", faceid);
					parseAndGetValue(buffer, "tiwen", tiwen);
					printf("tiwen:%s\r\n", tiwen);
					parseAndGetValue(buffer, "xueyang", xueyang);
					parseAndGetValue(buffer, "xinlv", xinlv);
					addTableData(faceid, tiwen, xueyang, xinlv , (char*)"information");
				}else if(x == 2){
                    parseAndGetValue(buffer, "faceid", faceid);
					printf("faceid:%s\r\n", faceid);
					parseAndGetValue(buffer, "yao", yao);
					printf("yao:%s\r\n", yao);
					addTableData_yao(faceid, yao , (char*)"yao");
                }
				else
				{
					parseAndGetValue(buffer, "faceid", faceid);
					//strncpy(faceid, p + 7, 1);
					//faceid[1] = '\0';
					queTableData(faceid,(char*)"user");
				}
				//上报数据
				//sprintf(mqtt_message, "{\"result_code\":0,\"response_name\":\"COMMAND_RESPONSE\",\"paras\":{\"result\":\"success\"}}");

				//sprintf(send_cmd, "$oc/devices/65697df3585c81787ad4da82_stm32/sys/commands/response/%s", request_id);

				//MQTT_PublishData(send_cmd, mqtt_message, 0);

				//printf("(命令)发布主题:%s\r\n", send_cmd);
				//printf("(命令)发布数据:%s\r\n", mqtt_message);
			}
		}
	}
}


bool connectDB(MYSQL& mysql) {
	// 1.初始化数据库句柄
	mysql_init(&mysql);

	// 2.设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");

	// 3.连接数据库										// 账号	  密码         数据库名
	MYSQL* ret = mysql_real_connect(&mysql, "127.0.0.1", "root", "123456", "testc", 3306, NULL, 0);
	if (ret == NULL) {
		printf("数据库连接失败！失败原因：%s\n", mysql_error(&mysql));
		return false;
	}

	printf("数据连接成功！\n");

	// 选择数据库，成功返回0，失败返回非0
	int res = mysql_select_db(&mysql, "testc");
	if (res) {
		printf("选择数据库失败！失败原因%s\n", mysql_error(&mysql));
		return false;
	}

	printf("数据库选择成功！\n");

	return true;
}

bool addTableData_yao(char* faceid, char* yao, char* tabelname) {
	MYSQL mysql;			// 数据库句柄
	char sql[SQL_MAX];		// 存储sql语句
	timetest();
	// 连接数据库
	if (!connectDB(mysql)) {
		return false;
	}
	// C语言字符串组合
	snprintf(sql, SQL_MAX, "INSERT INTO %s VALUES(%s, \"%s\", \"%s\");", tabelname,faceid, yao, (char*)buffertime);
	printf("插入sql语句：%s\n", sql);
	// 如果id字段用了auto_increment进行修饰，那么可以使用以下数据库语句：
	// snprintf(sql, SQL_MAX, "INSERT INTO student(name, age, sex) VALUES('%s', %d, '%s');", name, age, sex);

	// 执行sql语句，成功返回0
	//int ret = mysql_query(&mysql, sql);		
	int ret = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("执行插入语句，插入返回结果：%d\n", ret);

	if (ret) {
		printf("插入表数据失败！失败原因：%s\n", mysql_error(&mysql));
		return false;
	}
	printf("插入表数据成功！\n");

	// 关闭数据库
	mysql_close(&mysql);

	return true;
}

bool addTableData(char* faceid, char* tiwen, char* xueyang, char* xinlv, char* tabelname) {
	MYSQL mysql;			// 数据库句柄
	char sql[SQL_MAX];		// 存储sql语句
	timetest();
	// 连接数据库
	if (!connectDB(mysql)) {
		return false;
	}
	// C语言字符串组合
	snprintf(sql, SQL_MAX, "INSERT INTO %s VALUES(%s, %s, %s, %s, \"%s\");", tabelname,faceid, tiwen, xueyang, xinlv, (char*)buffertime);
	printf("插入sql语句：%s\n", sql);
	// 如果id字段用了auto_increment进行修饰，那么可以使用以下数据库语句：
	// snprintf(sql, SQL_MAX, "INSERT INTO student(name, age, sex) VALUES('%s', %d, '%s');", name, age, sex);

	// 执行sql语句，成功返回0
	//int ret = mysql_query(&mysql, sql);		
	int ret = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("执行插入语句，插入返回结果：%d\n", ret);

	if (ret) {
		printf("插入表数据失败！失败原因：%s\n", mysql_error(&mysql));
		return false;
	}
	printf("插入表数据成功！\n");

	// 关闭数据库
	mysql_close(&mysql);

	return true;
}

bool queTableData(char* faceid, char* tablename) {
	MYSQL mysql;		// 数据库句柄
	MYSQL_RES* res;		// 查询结果集
	MYSQL_ROW row;		// 记录结构体
	char sql[SQL_MAX];	// SQL语句

	// 连接数据库
	if (!connectDB(mysql)) {
		return false;
	}
		// C语言组合字符串
	snprintf(sql, SQL_MAX, "SELECT xingming,xingbie,nianling,zhengduan,yijian,yao FROM %s WHERE faceid= %s;", tablename, faceid);
	printf("查询sql语句：%s\n", sql);
	// 查询数据
	int ret = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("执行查询语句，查询返回结果：%d\n", ret);

	if (ret) {
		printf("数据查询失败！失败原因：%s\n", mysql_error(&mysql));
		return false;
	}
	printf("数据查询成功！\n");
	// 获取结果集
	res = mysql_store_result(&mysql);
	// 获取查询到的一行数据
	// 给row赋值，判断row是否为空，不为空就打印数据。
    while (row = mysql_fetch_row(res)) 
    {
        sprintf(mqtt_message1, "xingming:%s,xingbie:%s,nianling:%s,zhengduan:%s,yijian:%s,yao:%s;", row[0], row[1], row[2], row[3],row[4], row[5]);//温度
        //发布主题
        strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
        //MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);
    }
	// 释放结果集
	mysql_free_result(res);
	// 关闭数据库
	mysql_close(&mysql);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!connectDB(mysql)) {
		return false;
	}
	// C语言组合字符串
	snprintf(sql, SQL_MAX, "SELECT tiwen,xueyang,xinlv,time FROM %s WHERE faceid= %s;", (char*)"information", faceid);
	printf("查询sql语句：%s\n", sql);
	// 查询数据
	int ret2 = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("执行查询语句，查询返回结果：%d\n", ret2);
	if (ret2) {
		printf("数据查询失败！失败原因：%s\n", mysql_error(&mysql));
		return false;
	}
	printf("数据查询成功！\n");
	// 获取结果集
	res = mysql_store_result(&mysql);
	// 获取查询到的一行数据
	// 给row赋值，判断row是否为空，不为空就打印数据。
	int i = 0;
	while (row = mysql_fetch_row(res)) 
	{
		sprintf(mqtt_message1, "tiwen:%s,xueyang:%s,xinlv:%s,time:%s;", row[0], row[1], row[2], row[3]);//温度
		//发布主题
		 strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
		//  i++;
		//  if (i > 2) break;
		//MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);
	}


    // 释放结果集
	mysql_free_result(res);
	// 关闭数据库
	mysql_close(&mysql);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!connectDB(mysql)) {
		return false;
	}
	// C语言组合字符串
	snprintf(sql, SQL_MAX, "SELECT yao,time FROM %s WHERE faceid= %s;", (char*)"yao", faceid);
	printf("查询sql语句：%s\n", sql);
	// 查询数据
	ret2 = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("执行查询语句，查询返回结果：%d\n", ret2);
	if (ret2) {
		printf("数据查询失败！失败原因：%s\n", mysql_error(&mysql));
		return false;
	}
	printf("数据查询成功！\n");
	// 获取结果集
	res = mysql_store_result(&mysql);
	// 获取查询到的一行数据
	// 给row赋值，判断row是否为空，不为空就打印数据。
	while (row = mysql_fetch_row(res)) 
	{
		sprintf(mqtt_message1, "yao:%s,time:%s;", row[0], row[1]);//温度
		strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
	}

	//strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
	MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);

	// 释放结果集
	mysql_free_result(res);
	// 关闭数据库
	mysql_close(&mysql);
	mqtt_message1[0] = '\0';
	mqtt_message[0] = '\0';
	return true;
}

void parseAndGetValue(char* buffer, const char* keyword, char* result) {
	char* q = strstr((char*)&buffer[5], keyword);
	if (q != NULL) {
		char* start = strchr(q, ':')+1 ;
		char* end = strchr(start, ',');
		if (end == NULL) {
			end = strchr(start, '\"');
			if(end == NULL)
				end = strchr(start, '\0');
		}
		int length = end - start;
		strncpy(result, start,length);
		//strncpy_s(result, length, start, 1000);
		result[length] = '\0';
	}
	else {
		result[0] = '\0'; // Set result to empty string if keyword not found
	}
}


