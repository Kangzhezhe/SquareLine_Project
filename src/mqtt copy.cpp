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

//---------------------------------------MQTTЭ����ص��Ӻ�������-------------------------------------------------------
//��������
unsigned char MQTT_PublishData(char* topic, char* message, unsigned char qos);
//���Ļ���ȡ����������
unsigned char MQTT_SubscribeTopic(char* topic, unsigned char qos, unsigned char whether);
//��¼MQTT������
unsigned char MQTT_Connect(char* ClientID, char* Username, char* Password);
//MQTTЭ�黺������ʼ��
void MQTT_Init(void);
//���õײ�ӿڷ������ݰ�
void MQTT_SendBuf(unsigned char* buf, unsigned short len);
//MQTTЭ������ײ�Ľӿڣ���ײ�����Ҫ��ֲЭ�鵽�����ط����У���ô����������ˡ������ط����øĵġ�
int Client_SendData(unsigned char* buff, unsigned int len);//�������ݵ�������
int Client_GetData(unsigned char* buff);//�ӷ�������ȡ����
void ReceiveData(void);
bool connectDB(MYSQL& mysql);
bool addTableData(char* faceid, char* tiwen, char* xueyang, char* xinlv, char* tabelname);
bool queTableData(char* faceid, char* tablename);
void parseAndGetValue(char* buffer, const char* keyword, char* result);
bool addTableData_yao(char* faceid, char* yao, char* tabelname);
//---------------------------------------ȫ�ֱ�������--------------------------------------------------------------------

#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
#define SQL_MAX 256	

unsigned char mqtt_rxbuf[1024 * 1024];//�������ݻ�����
unsigned char mqtt_txbuf[1024 * 1024];//�������ݻ�����
unsigned int mqtt_rxlen;
unsigned int mqtt_txlen;


typedef enum
{
	//���� 	    ֵ 			������������ 	����
	M_RESERVED1 = 0,	//	��ֹ	����
	M_CONNECT,	//	�ͻ��˵������	�ͻ����������ӷ����
	M_CONNACK,	//	����˵��ͻ���	���ӱ���ȷ��
	M_PUBLISH,	//	������������	������Ϣ
	M_PUBACK,	//	������������	QoS 1��Ϣ�����յ�ȷ��
	M_PUBREC,	//	������������	�����յ�����֤������һ����
	M_PUBREL,	//	������������	�����ͷţ���֤�����ڶ�����
	M_PUBCOMP,	//	������������	QoS 2��Ϣ������ɣ���֤������������
	M_SUBSCRIBE,	//	�ͻ��˵������	�ͻ��˶�������
	M_SUBACK,	//	����˵��ͻ���	����������ȷ��
	M_UNSUBSCRIBE,	//	�ͻ��˵������	�ͻ���ȡ����������
	M_UNSUBACK,	//	����˵��ͻ���	ȡ�����ı���ȷ��
	M_PINGREQ,	//	�ͻ��˵������	��������
	M_PINGRESP,	//	����˵��ͻ���	������Ӧ
	M_DISCONNECT,	//	�ͻ��˵������	�ͻ��˶Ͽ�����
	M_RESERVED2,	//	��ֹ	����
}_typdef_mqtt_message;

//���ӳɹ���������Ӧ 20 02 00 00
//�ͻ��������Ͽ����� e0 00
const unsigned char parket_connetAck[] = { 0x20,0x02,0x00,0x00 };
const unsigned char parket_disconnet[] = { 0xe0,0x00 };
const unsigned char parket_heart[] = { 0xc0,0x00 };
const unsigned char parket_heart_reply[] = { 0xc0,0x00 };
const unsigned char parket_subAck[] = { 0x90,0x03 };

void MQTT_Init(void)
{
	//��������ֵ
	mqtt_rxlen = sizeof(mqtt_rxbuf);
	mqtt_txlen = sizeof(mqtt_txbuf);
	memset(mqtt_rxbuf, 0, mqtt_rxlen);
	memset(mqtt_txbuf, 0, mqtt_txlen);

}


/*
��������: ��¼������
��������ֵ: 0��ʾ�ɹ� 1��ʾʧ��
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

	//�ɱ䱨ͷ+Payload  ÿ���ֶΰ��������ֽڵĳ��ȱ�ʶ
	DataLen = 10 + (ClientIDLen + 2) + (UsernameLen + 2) + (PasswordLen + 2);

	//�̶���ͷ
	//���Ʊ�������
	mqtt_txbuf[mqtt_txlen++] = 0x10;		//MQTT Message Type CONNECT
	//ʣ�೤��(�������̶�ͷ��)
	do
	{
		unsigned char encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if (DataLen > 0)
			encodedByte = encodedByte | 128;
		mqtt_txbuf[mqtt_txlen++] = encodedByte;
	} while (DataLen > 0);

	//�ɱ䱨ͷ
	//Э����
	mqtt_txbuf[mqtt_txlen++] = 0;        	// Protocol Name Length MSB    
	mqtt_txbuf[mqtt_txlen++] = 4;           // Protocol Name Length LSB    
	mqtt_txbuf[mqtt_txlen++] = 'M';        	// ASCII Code for M    
	mqtt_txbuf[mqtt_txlen++] = 'Q';        	// ASCII Code for Q    
	mqtt_txbuf[mqtt_txlen++] = 'T';        	// ASCII Code for T    
	mqtt_txbuf[mqtt_txlen++] = 'T';        	// ASCII Code for T    
	//Э�鼶��
	mqtt_txbuf[mqtt_txlen++] = 4;        		// MQTT Protocol version = 4    
	//���ӱ�־
	mqtt_txbuf[mqtt_txlen++] = 0xc2;        	// conn flags 
	mqtt_txbuf[mqtt_txlen++] = 0;        		// Keep-alive Time Length MSB    
	mqtt_txbuf[mqtt_txlen++] = 100;        	// Keep-alive Time Length LSB  100S������  

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
		size = Client_GetData(buff);//�ӷ�������ȡ����
		if (size <= 0)continue;
		memcpy(mqtt_rxbuf, buff, size);

		printf("��¼Ӧ��:\r\n");
		for (j = 0; j < size; j++)
		{
			printf("%#X ", buff[j]);
		}
		printf("\r\n");

		if (mqtt_rxbuf[0] == parket_connetAck[0] && mqtt_rxbuf[1] == parket_connetAck[1]) //���ӳɹ�			   
		{
			return 0;//���ӳɹ�
		}
	}
	return 1;
}

/*
��������: MQTT����/ȡ���������ݴ������
��������:
	topic       ����
	qos         ��Ϣ�ȼ� 0:���ַ�һ��  1: ���ٷַ�һ��  2: ���ַ�һ��
	whether     ����/ȡ����������� (1��ʾ����,0��ʾȡ������)
����ֵ: 0��ʾ�ɹ� 1��ʾʧ��
*/
unsigned char MQTT_SubscribeTopic(char* topic, unsigned char qos, unsigned char whether)
{
	unsigned char i, j;
	mqtt_txlen = 0;
	unsigned int size = 0;
	unsigned char buff[256];
	unsigned int topiclen = (int)strlen(topic);
	unsigned int DataLen = 2 + (topiclen + 2) + (whether ? 1 : 0);//�ɱ䱨ͷ�ĳ��ȣ�2�ֽڣ�������Ч�غɵĳ���
	//�̶���ͷ
	//���Ʊ�������
	if (whether)mqtt_txbuf[mqtt_txlen++] = 0x82; //��Ϣ���ͺͱ�־����
	else	mqtt_txbuf[mqtt_txlen++] = 0xA2;    //ȡ������
	//ʣ�೤��
	do
	{
		unsigned char encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if (DataLen > 0)
			encodedByte = encodedByte | 128;
		mqtt_txbuf[mqtt_txlen++] = encodedByte;
	} while (DataLen > 0);
	//�ɱ䱨ͷ
	mqtt_txbuf[mqtt_txlen++] = 0;			//��Ϣ��ʶ�� MSB
	mqtt_txbuf[mqtt_txlen++] = 0x01;        //��Ϣ��ʶ�� LSB
	//��Ч�غ�
	mqtt_txbuf[mqtt_txlen++] = BYTE1(topiclen);//���ⳤ�� MSB
	mqtt_txbuf[mqtt_txlen++] = BYTE0(topiclen);//���ⳤ�� LSB   
	memcpy(&mqtt_txbuf[mqtt_txlen], topic, topiclen);
	mqtt_txlen += topiclen;
	if (whether)
	{
		mqtt_txbuf[mqtt_txlen++] = qos;//QoS����
	}
	for (i = 0; i < 100; i++)
	{
		memset(mqtt_rxbuf, 0, mqtt_rxlen);
		MQTT_SendBuf(mqtt_txbuf, mqtt_txlen);
		//printf("������Ϣ�����ɹ�\n");
		size = Client_GetData(buff);//�ӷ�������ȡ����
		if (size <= 0)
		{
			continue;
		}
		memcpy(mqtt_rxbuf, buff, size);

		printf("����Ӧ��:\r\n");
		for (j = 0; j < size; j++)
		{
			printf("%#X ", buff[j]);
		}
		printf("\r\n");

		if (mqtt_rxbuf[0] == parket_subAck[0] && mqtt_rxbuf[1] == parket_subAck[1]) //���ӳɹ�			   
		{
			return 0;//���ӳɹ�
		}
		Sleep(1000);
	}
	return 1; //ʧ��
}



//MQTT�������ݴ������
//topic   ���� 
//message ��Ϣ
//qos     ��Ϣ�ȼ� 
unsigned char MQTT_PublishData(char* topic, char* message, unsigned char qos)
{
	unsigned int topicLength = (int)strlen(topic);
	unsigned int messageLength = (int)strlen(message);
	unsigned short id = 0;
	unsigned int DataLen;
	mqtt_txlen = 0;

	printf("�ϱ�JSON��Ϣ����:%d\r\n", messageLength);
	printf("message=%s\r\n", message);
	//��Ч�غɵĳ����������㣺�ù̶���ͷ�е�ʣ�೤���ֶε�ֵ��ȥ�ɱ䱨ͷ�ĳ���
	//QOSΪ0ʱû�б�ʶ��
	//���ݳ���             ������   ���ı�ʶ��   ��Ч�غ�
	if (qos)	DataLen = (2 + topicLength) + 2 + messageLength;
	else	DataLen = (2 + topicLength) + messageLength;

	//�̶���ͷ
	//���Ʊ�������
	mqtt_txbuf[mqtt_txlen++] = 0x30;    // MQTT Message Type PUBLISH  

	//ʣ�೤��
	do
	{
		unsigned char encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		// if there are more data to encode, set the top bit of this byte
		if (DataLen > 0)
			encodedByte = encodedByte | 128;
		mqtt_txbuf[mqtt_txlen++] = encodedByte;
	} while (DataLen > 0);
	mqtt_txbuf[mqtt_txlen++] = BYTE1(topicLength);//���ⳤ��MSB
	mqtt_txbuf[mqtt_txlen++] = BYTE0(topicLength);//���ⳤ��LSB 
	memcpy(&mqtt_txbuf[mqtt_txlen], topic, topicLength);//��������
	mqtt_txlen += topicLength;

	//���ı�ʶ��
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
	Client_SendData(buf, len);//�������ݵ�������
}





//-----------------------------------------MQTT�������Ĳ���------------------------------------------------------------
//������IP
#define SERVER_IP "2407:c080:0801:fffe:0000:0000:7c46:da83"
#define SERVER_PORT 1883 //�˿ں�

//MQTT��Ԫ��
#define ClientID "6687def786799a26c45e94d5_window_0_0_2024070605"
#define Username "6687def786799a26c45e94d5_window"
#define Password "a675dfa234557eaae463fd23c0fa10e0831a058d3171c56778a881f93fd72ffe"//���� 

//��������:
#define SET_TOPIC  "/test/M2M/aa/#"//����
//��������:
#define POST_TOPIC "/test/M2M/hh"
#define USELESS_TOPIC "$oc/devices/{device_id}/sys/messages/up"
//#define POST_TOPIC "$oc/devices/6685084786799a26c45e16f9_1111/sys/properties/report"//����



//-----------------------------------------������------------------------------------------------------------

char mqtt_message[2024];//���ݻ�����
char mqtt_message1[2024];//���ݻ�����

SOCKET connectSocket; //�����׽���
WSADATA wsaData; //����һ���ṹ����������ڴ洢����Winsock�����Ϣ

double TEMP = 10.0;
extern int main1();
int main()
{
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData); //��ʼ��Winsock�⣬ָ���汾��2.2����鷵��ֵ
	if (result != 0)
	{
		printf("WSAStartup failed: %d\r\n", result);//���������Ϣ���˳�����
		return 1;
	}

	connectSocket = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP); //����һ��TCP�׽��֣���鷵��ֵ
	if (connectSocket == INVALID_SOCKET)
	{
		printf("socket failed with error: %d", WSAGetLastError());//���������Ϣ���˳�����
		WSACleanup(); //���Winsock��
		return 1;
	}


	sockaddr_in6 service; //����һ���ṹ����������ڴ洢��������ַ��Ϣ
	memset(&service, 0, sizeof(sockaddr_in6));
	service.sin6_port = htons(SERVER_PORT); //���˿ںŴ������ֽ���ת��Ϊ�����ֽ��򣬲��洢�ڽṹ����
	inet_pton(AF_INET6, SERVER_IP, &service.sin6_addr); //���ַ������͵�IP��ַת��Ϊ�����������ֽ����IP��ַ�����洢�ڽṹ����
	service.sin6_family = AF_INET6; //ָ����ַ��ΪIPv4
	//service.sin6_scope_id = % 3;
	result = connect(connectSocket, (SOCKADDR*)&service, sizeof(service)); //���ӵ�����������鷵��ֵ
	if (result == SOCKET_ERROR)
	{
		std::cout << "connect failed with error: " << WSAGetLastError() << std::endl; //���������Ϣ���˳�����
		closesocket(connectSocket); //�ر��׽���
		WSACleanup(); //���Winsock��
		return 1;
	}

	std::cout << "Connected to server." << std::endl; //���ӳɹ��������Ϣ

	MQTT_Init();


	while (1)
	{
		/*��¼������*/
		if (MQTT_Connect((char*)ClientID, (char*)Username, (char*)Password) == 0)
		{
			break;
		}
		// ��ʱ1000���룬��1��
		Sleep(1000);
		printf("MQTT��������¼У����....\n");
	}


	printf("���ӳɹ�_666\r\n");
	//����������ƽ̨����
	int stat = MQTT_SubscribeTopic((char*)SET_TOPIC, 1, 1);
	if (stat)
	{
		printf("����ʧ��\r\n");
		closesocket(connectSocket); //�ر��׽���
		WSACleanup(); //���Winsock��
		return 1;
	}
	printf("���ĳɹ�\r\n");
	MYSQL mysql;
	bool xx = connectDB(mysql);
	//xx = addTableData(2, 35.7, (char*)"35.7");
	//xx = queTableData((char*)"2");

	/*�����߳�*/
		/*�����߳�*/
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveData, NULL, 0, NULL);
	if (hThread == NULL) {
		printf("CreateThread failed.\n");
		return 1;
	}


	while (1)
	{
		//sprintf(mqtt_message, "{\"services\": [{\"service_id\": \"1\",\"properties\":{\"faceid\":%d}}]}", (int)(2));//�¶�
		//sprintf(mqtt_message, "12333333");//�¶�
		////��������
		//MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);
		//printf("������Ϣ�ɹ�\r\n");
		MQTT_PublishData((char*)USELESS_TOPIC,(char*) "PING", 0);
		Sleep(10000);
	 //timetest();
		//addTableData((char*)"3",(char*)"20", (char*)"90", (char*)"110", (char*)"NULL", (char*)"NULL", (char*)"NULL", (char*)"NULL", (char*)"facetest");
	}
}



/*�������ݵ�������*/
int Client_SendData(unsigned char* buff, unsigned int len)
{
	int result = send(connectSocket, (const char*)buff, len, 0); //��������������ݣ���鷵��ֵ
	if (result == SOCKET_ERROR)
	{
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl; //���������Ϣ���˳�����
		return -1;
	}
	return 0;
}



/*��ȡ�������·�����*/
int Client_GetData(unsigned char* buff)
{
	int result = recv(connectSocket, (char*)buff, 200, 0); //�ӷ������������ݣ���鷵��ֵ
	if (result == SOCKET_ERROR)
	{
		std::cout << "recv failed with error: " << WSAGetLastError() << std::endl; //���������Ϣ���˳�����
		return -1;
	}
	return result;
}

// ����������·�������
void ReceiveData(void)
{
	// ��������
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
		//�ȴ��������·���Ϣ
        memset(buffer, 0, 1024);
		recvSize = recv(connectSocket, buffer, 1024, 0);
		if (recvSize == SOCKET_ERROR)
		{
			std::cout << "����Ͽ�����: " << WSAGetLastError() << std::endl; //���������Ϣ���˳�����
			return;
		}
		if (recvSize > 0)
		{
			printf("�������·���Ϣ:\r\n");
			//�����·�������
			for (int i = 0; i < recvSize; i++)
			{
				printf("%c", buffer[i]);
			}
			printf("\r\n");
			//������Ϣ
			if (strstr((char*)&buffer[5], "test/M2M"))
			{
				printf("(����)����11\r\n");
				char* p = NULL;
				char* q = NULL;
				int x = 0;
				p = strstr((char*)&buffer[5], "faceid");
				q = strstr((char*)&buffer[5], "tiwen");
				if (p)
				{
					if (q)
						x = 1;//x=1˵����Ҫ�ϴ�����
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
				//�ϱ�����
				//sprintf(mqtt_message, "{\"result_code\":0,\"response_name\":\"COMMAND_RESPONSE\",\"paras\":{\"result\":\"success\"}}");

				//sprintf(send_cmd, "$oc/devices/65697df3585c81787ad4da82_stm32/sys/commands/response/%s", request_id);

				//MQTT_PublishData(send_cmd, mqtt_message, 0);

				//printf("(����)��������:%s\r\n", send_cmd);
				//printf("(����)��������:%s\r\n", mqtt_message);
			}
		}
	}
}


bool connectDB(MYSQL& mysql) {
	// 1.��ʼ�����ݿ���
	mysql_init(&mysql);

	// 2.�����ַ�����
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");

	// 3.�������ݿ�										// �˺�	  ����         ���ݿ���
	MYSQL* ret = mysql_real_connect(&mysql, "127.0.0.1", "root", "123456", "testc", 3306, NULL, 0);
	if (ret == NULL) {
		printf("���ݿ�����ʧ�ܣ�ʧ��ԭ��%s\n", mysql_error(&mysql));
		return false;
	}

	printf("�������ӳɹ���\n");

	// ѡ�����ݿ⣬�ɹ�����0��ʧ�ܷ��ط�0
	int res = mysql_select_db(&mysql, "testc");
	if (res) {
		printf("ѡ�����ݿ�ʧ�ܣ�ʧ��ԭ��%s\n", mysql_error(&mysql));
		return false;
	}

	printf("���ݿ�ѡ��ɹ���\n");

	return true;
}

bool addTableData_yao(char* faceid, char* yao, char* tabelname) {
	MYSQL mysql;			// ���ݿ���
	char sql[SQL_MAX];		// �洢sql���
	timetest();
	// �������ݿ�
	if (!connectDB(mysql)) {
		return false;
	}
	// C�����ַ������
	snprintf(sql, SQL_MAX, "INSERT INTO %s VALUES(%s, \"%s\", \"%s\");", tabelname,faceid, yao, (char*)buffertime);
	printf("����sql��䣺%s\n", sql);
	// ���id�ֶ�����auto_increment�������Σ���ô����ʹ���������ݿ���䣺
	// snprintf(sql, SQL_MAX, "INSERT INTO student(name, age, sex) VALUES('%s', %d, '%s');", name, age, sex);

	// ִ��sql��䣬�ɹ�����0
	//int ret = mysql_query(&mysql, sql);		
	int ret = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("ִ�в�����䣬���뷵�ؽ����%d\n", ret);

	if (ret) {
		printf("���������ʧ�ܣ�ʧ��ԭ��%s\n", mysql_error(&mysql));
		return false;
	}
	printf("��������ݳɹ���\n");

	// �ر����ݿ�
	mysql_close(&mysql);

	return true;
}

bool addTableData(char* faceid, char* tiwen, char* xueyang, char* xinlv, char* tabelname) {
	MYSQL mysql;			// ���ݿ���
	char sql[SQL_MAX];		// �洢sql���
	timetest();
	// �������ݿ�
	if (!connectDB(mysql)) {
		return false;
	}
	// C�����ַ������
	snprintf(sql, SQL_MAX, "INSERT INTO %s VALUES(%s, %s, %s, %s, \"%s\");", tabelname,faceid, tiwen, xueyang, xinlv, (char*)buffertime);
	printf("����sql��䣺%s\n", sql);
	// ���id�ֶ�����auto_increment�������Σ���ô����ʹ���������ݿ���䣺
	// snprintf(sql, SQL_MAX, "INSERT INTO student(name, age, sex) VALUES('%s', %d, '%s');", name, age, sex);

	// ִ��sql��䣬�ɹ�����0
	//int ret = mysql_query(&mysql, sql);		
	int ret = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("ִ�в�����䣬���뷵�ؽ����%d\n", ret);

	if (ret) {
		printf("���������ʧ�ܣ�ʧ��ԭ��%s\n", mysql_error(&mysql));
		return false;
	}
	printf("��������ݳɹ���\n");

	// �ر����ݿ�
	mysql_close(&mysql);

	return true;
}

bool queTableData(char* faceid, char* tablename) {
	MYSQL mysql;		// ���ݿ���
	MYSQL_RES* res;		// ��ѯ�����
	MYSQL_ROW row;		// ��¼�ṹ��
	char sql[SQL_MAX];	// SQL���

	// �������ݿ�
	if (!connectDB(mysql)) {
		return false;
	}
		// C��������ַ���
	snprintf(sql, SQL_MAX, "SELECT xingming,xingbie,nianling,zhengduan,yijian,yao FROM %s WHERE faceid= %s;", tablename, faceid);
	printf("��ѯsql��䣺%s\n", sql);
	// ��ѯ����
	int ret = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("ִ�в�ѯ��䣬��ѯ���ؽ����%d\n", ret);

	if (ret) {
		printf("���ݲ�ѯʧ�ܣ�ʧ��ԭ��%s\n", mysql_error(&mysql));
		return false;
	}
	printf("���ݲ�ѯ�ɹ���\n");
	// ��ȡ�����
	res = mysql_store_result(&mysql);
	// ��ȡ��ѯ����һ������
	// ��row��ֵ���ж�row�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ���ݡ�
    while (row = mysql_fetch_row(res)) 
    {
        sprintf(mqtt_message1, "xingming:%s,xingbie:%s,nianling:%s,zhengduan:%s,yijian:%s,yao:%s;", row[0], row[1], row[2], row[3],row[4], row[5]);//�¶�
        //��������
        strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
        //MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);
    }
	// �ͷŽ����
	mysql_free_result(res);
	// �ر����ݿ�
	mysql_close(&mysql);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!connectDB(mysql)) {
		return false;
	}
	// C��������ַ���
	snprintf(sql, SQL_MAX, "SELECT tiwen,xueyang,xinlv,time FROM %s WHERE faceid= %s;", (char*)"information", faceid);
	printf("��ѯsql��䣺%s\n", sql);
	// ��ѯ����
	int ret2 = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("ִ�в�ѯ��䣬��ѯ���ؽ����%d\n", ret2);
	if (ret2) {
		printf("���ݲ�ѯʧ�ܣ�ʧ��ԭ��%s\n", mysql_error(&mysql));
		return false;
	}
	printf("���ݲ�ѯ�ɹ���\n");
	// ��ȡ�����
	res = mysql_store_result(&mysql);
	// ��ȡ��ѯ����һ������
	// ��row��ֵ���ж�row�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ���ݡ�
	int i = 0;
	while (row = mysql_fetch_row(res)) 
	{
		sprintf(mqtt_message1, "tiwen:%s,xueyang:%s,xinlv:%s,time:%s;", row[0], row[1], row[2], row[3]);//�¶�
		//��������
		 strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
		//  i++;
		//  if (i > 2) break;
		//MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);
	}


    // �ͷŽ����
	mysql_free_result(res);
	// �ر����ݿ�
	mysql_close(&mysql);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!connectDB(mysql)) {
		return false;
	}
	// C��������ַ���
	snprintf(sql, SQL_MAX, "SELECT yao,time FROM %s WHERE faceid= %s;", (char*)"yao", faceid);
	printf("��ѯsql��䣺%s\n", sql);
	// ��ѯ����
	ret2 = mysql_real_query(&mysql, sql, (unsigned long)strlen(sql));
	printf("ִ�в�ѯ��䣬��ѯ���ؽ����%d\n", ret2);
	if (ret2) {
		printf("���ݲ�ѯʧ�ܣ�ʧ��ԭ��%s\n", mysql_error(&mysql));
		return false;
	}
	printf("���ݲ�ѯ�ɹ���\n");
	// ��ȡ�����
	res = mysql_store_result(&mysql);
	// ��ȡ��ѯ����һ������
	// ��row��ֵ���ж�row�Ƿ�Ϊ�գ���Ϊ�վʹ�ӡ���ݡ�
	while (row = mysql_fetch_row(res)) 
	{
		sprintf(mqtt_message1, "yao:%s,time:%s;", row[0], row[1]);//�¶�
		strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
	}

	//strncat(mqtt_message, mqtt_message1, strlen(mqtt_message1));
	MQTT_PublishData((char*)POST_TOPIC, mqtt_message, 0);

	// �ͷŽ����
	mysql_free_result(res);
	// �ر����ݿ�
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


