#include <iostream>
#include <clocale>
#include <windows.h>
#include <errno.h>
#include "cJSON\cJSON.h"

using namespace std;

typedef int(*Dllfun)(const char*);
typedef void(__stdcall *_onMessage_func)(const char* message);
typedef void(*Dllfun2)(_onMessage_func);
typedef int(*Dllfun3)(const char*);
typedef int(*Dllfun4)(void);
HINSTANCE hdll;
Dllfun initPrinterManager;
Dllfun2 setRecvDataCallback;
Dllfun3 sendMessage;
Dllfun4 closePrinterManager;

//��û�ô�ӡ���б�����
string JSON_PrinterList()
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "cmd", "getPrinters");
	cJSON_AddStringToObject(root, "requestID", "123458976");
	cJSON_AddStringToObject(root, "version", "1.0");
	string str = string(cJSON_Print(root));
	cJSON_Delete(root);
	//string str = "{ \"cmd\":\"getPrinters\",\"requestID\":\"123458976\",\"version\":\"1.0\"}";
	return str;
}

//������ô�ӡ�������������
string JSON_PrinterConfigPoP()
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "cmd", "printerConfig");
	cJSON_AddStringToObject(root, "requestID", "12345678901234567890");
	cJSON_AddStringToObject(root, "version", "1.0");
	string str = string(cJSON_PrintUnformatted(root));
	cJSON_Delete(root);
	//string str = "{\"requestID\":\"12345678901234567890\",\"version\":\"1.0\",\"cmd\":\"printerConfig\"}";
	return str;
}

//������ô�ӡ������ǵ�������
string JSON_PrinterConfigNoPoP()
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "cmd", "setPrinterConfig");
	cJSON_AddStringToObject(root, "requestID", "12345678901234567890");
	cJSON_AddStringToObject(root, "version", "1.0");
	cJSON *sub_root = cJSON_CreateObject();
	cJSON_AddStringToObject(sub_root, "name", "������ OneNote 2013");
	cJSON_AddTrueToObject(sub_root, "needTopLogo");
	cJSON_AddFalseToObject(sub_root, "needBottomLogo");
	cJSON_AddItemToObject(root, "printer", sub_root);
	string str = string(cJSON_Print(root));
	cJSON_Delete(root);
	//string str = "{\"requestID\":\"12345678901234567890\",\"version\":\"1.0\",\"cmd\":\"setPrinterConfig\",\"printer\":{\"name\":\"������ OneNote 2013\",\"needTopLogo\":true,\"needBottomLogo\":false}}";
	return str;
}

//���"����taskID��ѯ��ӡ����"������
string JSON_TaskID()
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "cmd", "getTaskStatus");
	cJSON_AddStringToObject(root, "requestID", "12345678");
	cJSON_AddStringToObject(root, "version", "1.0");
	cJSON *sub_root = cJSON_CreateArray();
	cJSON_AddItemToArray(sub_root, cJSON_CreateString("12311"));
	cJSON_AddItemToArray(sub_root, cJSON_CreateString("12312"));
	cJSON_AddItemToObject(root, "taskID", sub_root);
	string str = string(cJSON_Print(root));
	cJSON_Delete(root);
	//string str = "{\"cmd\":\"getTaskStatus\",\"requestID\":\"12345678\",\"version\":\"1.0\",\"taskID\":[\"12311\",\"12312\"]}";
	return str;
}

//���"�����浥�Ų�ѯ��ӡ����"������
string JSON_DocumentID()
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "cmd", "getDocumentStatus");
	cJSON_AddStringToObject(root, "requestID", "12345678");
	cJSON_AddStringToObject(root, "version", "1.0");
	cJSON *sub_root = cJSON_CreateArray();
	cJSON_AddItemToArray(sub_root, cJSON_CreateString("12311"));
	cJSON_AddItemToArray(sub_root, cJSON_CreateString("12312"));
	cJSON_AddItemToObject(root, "documentIDs", sub_root);
	string str = string(cJSON_Print(root));
	cJSON_Delete(root);
	//string str = "{\"cmd\":\"getDocumentStatus\",\"requestID\":\"12345678\",\"version\":\"1.0\",\"documentIDs\":[\"12311\",\"12312\"]}";
	return str;
}

string JSON_PrintMission()
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "cmd", "print");
	cJSON_AddStringToObject(root, "requestID", "123458976");
	cJSON_AddStringToObject(root, "version", "1.0");

	cJSON *task = cJSON_CreateObject();
	cJSON_AddStringToObject(task, "taskID", "1467777171225");
	cJSON_AddTrueToObject(task, "preview");
	cJSON_AddStringToObject(task, "printer", "Microsoft XPS Document Writer");

	cJSON *documents = cJSON_CreateArray();
	cJSON *document = cJSON_CreateObject();
	cJSON_AddStringToObject(document, "documentID", "9890000106027");
	cJSON *contents = cJSON_CreateArray();
	string data = "{\"data\":{\"recipient\":{\"address\":{\"city\":\"������\",\"detail\":\"���ҵ�������������վ����¥��Ҳ��֪�����Ķ���\",\"district\":\"������\",\"province\":\"����011666\",\"town\":\"�����ֵ�\"},\"mobile\":\"1326443654\",\"name\":\"����\",\"phone\":\"057123222\"},\"routingInfo\":{\"consolidation\":{\"name\":\"����\",\"code\":\"hangzhou\"},\"origin\":{\"code\":\"POSTB\"},\"sortation\":{\"name\":\"����\"},\"routeCode\":\"380D-56-04\"},\"sender\":{\"address\":{\"city\":\"������\",\"detail\":\"���ҵ�������������վ����¥��Ҳ��֪�����Ķ���\",\"district\":\"������\",\"province\":\"����\",\"town\":\"�����ֵ�\"},\"mobile\":\"1326443654\",\"name\":\"����\",\"phone\":\"057123222\"},\"shippingOption\":{\"code\":\"COD\",\"services\":{\"SVC-COD\":{\"value\":\"200\"}},\"title\":\"���ջ���\"},\"waybillCode\":\"9890000160004\"},\"signature\":\"19d6f7759487e556ddcdd3d499af087080403277b7deed1a951cc3d9a93c42a7e22ccba94ff609976c5d3ceb069b641f541bc9906098438d362cae002dfd823a8654b2b4f655e96317d7f60eef1372bb983a4e3174cc8d321668c49068071eaea873071ed683dd24810e51afc0bc925b7a2445fdbc2034cdffb12cb4719ca6b7\",\"templateURL\":\"https://cloudprint.cainiao.com/template/standard/1010\"}";

	cJSON_AddItemToArray(contents, cJSON_Parse(data.c_str()));
	cJSON_AddItemToObject(document, "contents", contents);
	cJSON_AddItemToArray(documents, document);
	cJSON_AddItemToObject(task, "documents", documents);
	cJSON_AddItemToObject(root, "task", task);
	string str = string(cJSON_PrintUnformatted(root));
	cJSON_Delete(root);
	//string data = "{\"cmd\":\"print\",\"requestID\":\"123458976\",\"version\":\"1.0\",\"task\":{\"taskID\":\"1467777171211\",\"preview\":false,\"printer\":\"\",\"documents\":[{\"documentID\":\"9890000106027\",\"contents\":[{\"data\":{\"recipient\":{\"address\":{\"city\":\"������\",\"detail\":\"���ҵ�������������վ����¥��Ҳ��֪�����Ķ���\",\"district\":\"������\",\"province\":\"����011666\",\"town\":\"�����ֵ�\"},\"mobile\":\"1326443654\",\"name\":\"����\",\"phone\":\"057123222\"},\"routingInfo\":{\"consolidation\":{\"name\":\"����\",\"code\":\"hangzhou\"},\"origin\":{\"code\":\"POSTB\"},\"sortation\":{\"name\":\"����\"},\"routeCode\":\"380D-56-04\"},\"sender\":{\"address\":{\"city\":\"������\",\"detail\":\"���ҵ�������������վ����¥��Ҳ��֪�����Ķ���\",\"district\":\"������\",\"province\":\"����\",\"town\":\"�����ֵ�\"},\"mobile\":\"1326443654\",\"name\":\"����\",\"phone\":\"057123222\"},\"shippingOption\":{\"code\":\"COD\",\"services\":{\"SVC-COD\":{\"value\":\"200\"}},\"title\":\"���ջ���\"},\"waybillCode\":\"9890000160004\"},\"signature\":\"19d6f7759487e556ddcdd3d499af087080403277b7deed1a951cc3d9a93c42a7e22ccba94ff609976c5d3ceb069b641f541bc9906098438d362cae002dfd823a8654b2b4f655e96317d7f60eef1372bb983a4e3174cc8d321668c49068071eaea873071ed683dd24810e51afc0bc925b7a2445fdbc2034cdffb12cb4719ca6b7\",\"templateURL\":\"file:///D:/tools/wamp/www/template_6.xml\"}]}]}}";
	return str;
}

string GBKToUTF8(const std::string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

string UTF8ToGBK(string &strUtf8)
{
	string strOutGBK = "";
	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
	WCHAR *wszGBK = new WCHAR[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char *pszGBK = new char[len + 1];
	memset(pszGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, pszGBK, len, NULL, NULL);
	strOutGBK = pszGBK;
	delete[]pszGBK;
	delete[]wszGBK;
	return strOutGBK;
}


//�ص����������̴߳���������ʹ��ע�������̶߳�ͬһ����ʵʱ����
extern "C" void __stdcall onMessage(const char* message)
{
	printf("Recv: %s\n", UTF8ToGBK(string(message)).c_str());
}

int main()
{
	int res = 0;
	string url = "ws://127.0.0.1:13528";
	//��̬��̬DLL
	hdll = LoadLibrary(L"PrinterManager.dll");
	if (hdll == NULL)
	{
		printf("loadlibrary failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}
	//��ʼ��PrinterManager,�����ڲ�����websocket����
	initPrinterManager = (Dllfun)GetProcAddress(hdll, "initPrinterManager");
	if (initPrinterManager == NULL)
	{
		printf("load initPrinterManager failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}
	res = initPrinterManager(url.c_str());
	if (res < 0)
	{
		printf("initPrinterManager failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}
	//�趨�ص�����
	setRecvDataCallback = (Dllfun2)GetProcAddress(hdll, "setRecvDataCallback");
	if (setRecvDataCallback == NULL)
	{
		printf("load setRecvDataCallback failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}
	setRecvDataCallback(onMessage);

	///���JSON����
	//string str = JSON_PrinterList();//��ȡ��ӡ���б�
	//string str = JSON_PrinterConfigPoP();//�������ô�ӡ��
	//string str = JSON_PrinterConfigNoPoP();//�ǵ������ô�ӡ��
	//string str = JSON_TaskID();//����TaskID��ѯ��ӡ����
	//string str = JSON_DocumentID();//�����浥�Ų�ѯ��ӡ����
	string str = JSON_PrintMission();//��ӡ����
	printf("The JSON message : %s\n", str.c_str());
	//ת��UTF8����
	str = GBKToUTF8(str);

	//��������
	sendMessage = (Dllfun3)GetProcAddress(hdll, "sendMessage");
	if (sendMessage == NULL)
	{
		printf("load sendMessage failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}
	res = sendMessage(str.c_str());
	if (res < 0)
	{
		printf("sendMessage failed! ErrorNuber:%d\n", errno);
		Sleep(2000);
		exit(1);
	}
	Sleep(1000);

	//�ر�PrinterManager,�ڲ��Ͽ�websocket����
	closePrinterManager = (Dllfun4)GetProcAddress(hdll, "closePrinterManager");
	if (closePrinterManager == NULL)
	{
		printf("load closePrinterManager failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}

	res = closePrinterManager();
	if (res < 0)
	{
		printf("closePrinterManager failed! ErrorNuber:%d\n", errno);
		Sleep(2000);
		exit(1);
	}

	//�ͷŶ�̬���ӿ�
	FreeLibrary(hdll);

	system("pause");
	return 0;
}
