PrinterManager/目录下是PrinterManager.dll的生成源码
PrinterManagerDllTest/目录下是C++使用dll文件的例子，包括了与打印组件交互的json字符串生成代码

PrinterManager.dll的动态调用使用说明
0、发布时间
2016年8月1日

1、文件说明
PrinterManager.dll    安全汇报模块
PrinterManager.h      调用接口的C导出接口头文件，开发参考用，发布时请勿附带

2、开放接口：
接口约定采用 stdcall 方式
①int  initPrinterManager(const char * url);
②void  setRecvDataCallback(_onMessage_func func);
③int  sendMessage(const char *message);
④int  closePrinterManager();

接口①：int  initPrinterManager(const char * url);
功能：初始化websocket，无需输入IP和端口，自动连接打印客户端
传入参数：const char *url：打印机组件的地址和端口，参数为空默认“ws://127.0.0.1:13528”
返回值：整型，成功返回0，返回-1表示初始化失败，返回-2表示重复初始化错误
注意：每载入一次dll，只调用一次initPrinterManager()，重复调用返回-1.

接口②：void  setRecvDataCallback(_onMessage_func func);
功能：设定回调函数，此回调函数作用：如果接收到数据则调用回调函数
返回值：无
传入参数：_onMessage_func func：函数指针，类型为typedef void(*_onMessage_func)(const char* message)，
            C++中对应回调函数例子：
			void handle_message(const char* message)
			{
				printf(">>> %s\n", message);
			}
接口③：int  sendMessage(const char *message);
功能：发送数据给打印客户端
返回值：整型，成功返回0，返回-1表示websocket未连接
传入参数：const char *message:字符串类型(传入的字符串必须确保已经是UTF-8编码)

接口④：int closePrinterManager()
功能：关闭websocket
返回值：整型，成功返回0，返回-1表示websocket在调用前已关闭或未初始化
注意：不要重复关闭，否则返回-1.

