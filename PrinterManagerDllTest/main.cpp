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

//获得获得打印机列表命令
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

//获得配置打印机命令，弹窗配置
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

//获得配置打印机命令，非弹窗配置
string JSON_PrinterConfigNoPoP()
{
	cJSON *root;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "cmd", "setPrinterConfig");
	cJSON_AddStringToObject(root, "requestID", "12345678901234567890");
	cJSON_AddStringToObject(root, "version", "1.0");
	cJSON *sub_root = cJSON_CreateObject();
	cJSON_AddStringToObject(sub_root, "name", "发送至 OneNote 2013");
	cJSON_AddTrueToObject(sub_root, "needTopLogo");
	cJSON_AddFalseToObject(sub_root, "needBottomLogo");
	cJSON_AddItemToObject(root, "printer", sub_root);
	string str = string(cJSON_Print(root));
	cJSON_Delete(root);
	//string str = "{\"requestID\":\"12345678901234567890\",\"version\":\"1.0\",\"cmd\":\"setPrinterConfig\",\"printer\":{\"name\":\"发送至 OneNote 2013\",\"needTopLogo\":true,\"needBottomLogo\":false}}";
	return str;
}

//获得"根据taskID查询打印任务"的命令
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

//获得"根据面单号查询打印任务"的命令
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
	string data = "{\"data\":{\"recipient\":{\"address\":{\"city\":\"北京市\",\"detail\":\"花家地社区卫生服务站三层楼我也不知道是哪儿了\",\"district\":\"朝阳区\",\"province\":\"北京011666\",\"town\":\"望京街道\"},\"mobile\":\"1326443654\",\"name\":\"秦疏\",\"phone\":\"057123222\"},\"routingInfo\":{\"consolidation\":{\"name\":\"杭州\",\"code\":\"hangzhou\"},\"origin\":{\"code\":\"POSTB\"},\"sortation\":{\"name\":\"杭州\"},\"routeCode\":\"380D-56-04\"},\"sender\":{\"address\":{\"city\":\"北京市\",\"detail\":\"花家地社区卫生服务站二层楼我也不知道是哪儿了\",\"district\":\"朝阳区\",\"province\":\"北京\",\"town\":\"望京街道\"},\"mobile\":\"1326443654\",\"name\":\"秦疏\",\"phone\":\"057123222\"},\"shippingOption\":{\"code\":\"COD\",\"services\":{\"SVC-COD\":{\"value\":\"200\"}},\"title\":\"代收货款\"},\"waybillCode\":\"9890000160004\"},\"signature\":\"19d6f7759487e556ddcdd3d499af087080403277b7deed1a951cc3d9a93c42a7e22ccba94ff609976c5d3ceb069b641f541bc9906098438d362cae002dfd823a8654b2b4f655e96317d7f60eef1372bb983a4e3174cc8d321668c49068071eaea873071ed683dd24810e51afc0bc925b7a2445fdbc2034cdffb12cb4719ca6b7\",\"templateURL\":\"https://cloudprint.cainiao.com/template/standard/1010\"}";

	cJSON_AddItemToArray(contents, cJSON_Parse(data.c_str()));
	cJSON_AddItemToObject(document, "contents", contents);
	cJSON_AddItemToArray(documents, document);
	cJSON_AddItemToObject(task, "documents", documents);
	cJSON_AddItemToObject(root, "task", task);
	string str = string(cJSON_PrintUnformatted(root));
	cJSON_Delete(root);
	//string data = "{\"cmd\":\"print\",\"requestID\":\"123458976\",\"version\":\"1.0\",\"task\":{\"taskID\":\"1467777171211\",\"preview\":false,\"printer\":\"\",\"documents\":[{\"documentID\":\"9890000106027\",\"contents\":[{\"data\":{\"recipient\":{\"address\":{\"city\":\"北京市\",\"detail\":\"花家地社区卫生服务站三层楼我也不知道是哪儿了\",\"district\":\"朝阳区\",\"province\":\"北京011666\",\"town\":\"望京街道\"},\"mobile\":\"1326443654\",\"name\":\"秦疏\",\"phone\":\"057123222\"},\"routingInfo\":{\"consolidation\":{\"name\":\"杭州\",\"code\":\"hangzhou\"},\"origin\":{\"code\":\"POSTB\"},\"sortation\":{\"name\":\"杭州\"},\"routeCode\":\"380D-56-04\"},\"sender\":{\"address\":{\"city\":\"北京市\",\"detail\":\"花家地社区卫生服务站二层楼我也不知道是哪儿了\",\"district\":\"朝阳区\",\"province\":\"北京\",\"town\":\"望京街道\"},\"mobile\":\"1326443654\",\"name\":\"秦疏\",\"phone\":\"057123222\"},\"shippingOption\":{\"code\":\"COD\",\"services\":{\"SVC-COD\":{\"value\":\"200\"}},\"title\":\"代收货款\"},\"waybillCode\":\"9890000160004\"},\"signature\":\"19d6f7759487e556ddcdd3d499af087080403277b7deed1a951cc3d9a93c42a7e22ccba94ff609976c5d3ceb069b641f541bc9906098438d362cae002dfd823a8654b2b4f655e96317d7f60eef1372bb983a4e3174cc8d321668c49068071eaea873071ed683dd24810e51afc0bc925b7a2445fdbc2034cdffb12cb4719ca6b7\",\"templateURL\":\"file:///D:/tools/wamp/www/template_6.xml\"}]}]}}";
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


//回调函数由子线程触发，故如使用注意避免多线程对同一数据实时操作
extern "C" void __stdcall onMessage(const char* message)
{
	printf("Recv: %s\n", UTF8ToGBK(string(message)).c_str());
}

int main()
{
	int res = 0;
	string url = "ws://127.0.0.1:13528";
	//动态加态DLL
	hdll = LoadLibrary(L"PrinterManager.dll");
	if (hdll == NULL)
	{
		printf("loadlibrary failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}
	//初始化PrinterManager,其中内部建立websocket连接
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
	//设定回调函数
	setRecvDataCallback = (Dllfun2)GetProcAddress(hdll, "setRecvDataCallback");
	if (setRecvDataCallback == NULL)
	{
		printf("load setRecvDataCallback failed! ErrorNuber:%d\n" + errno);
		Sleep(2000);
		exit(1);
	}
	setRecvDataCallback(onMessage);

	///获得JSON命令
	//string str = JSON_PrinterList();//获取打印机列表
	//string str = JSON_PrinterConfigPoP();//弹窗配置打印机
	//string str = JSON_PrinterConfigNoPoP();//非弹窗配置打印机
	//string str = JSON_TaskID();//根据TaskID查询打印任务
	//string str = JSON_DocumentID();//根据面单号查询打印任务
	string str = JSON_PrintMission();//打印任务
	printf("The JSON message : %s\n", str.c_str());
	//转成UTF8编码
	str = GBKToUTF8(str);

	//发送数据
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

	//关闭PrinterManager,内部断开websocket连接
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

	//释放动态链接库
	FreeLibrary(hdll);

	system("pause");
	return 0;
}
