PrinterManager/Ŀ¼����PrinterManager.dll������Դ��
PrinterManagerDllTest/Ŀ¼����C++ʹ��dll�ļ������ӣ����������ӡ���������json�ַ������ɴ���

PrinterManager.dll�Ķ�̬����ʹ��˵��
0������ʱ��
2016��8��1��

1���ļ�˵��
PrinterManager.dll    ��ȫ�㱨ģ��
PrinterManager.h      ���ýӿڵ�C�����ӿ�ͷ�ļ��������ο��ã�����ʱ���𸽴�

2�����Žӿڣ�
�ӿ�Լ������ stdcall ��ʽ
��int  initPrinterManager(const char * url);
��void  setRecvDataCallback(_onMessage_func func);
��int  sendMessage(const char *message);
��int  closePrinterManager();

�ӿڢ٣�int  initPrinterManager(const char * url);
���ܣ���ʼ��websocket����������IP�Ͷ˿ڣ��Զ����Ӵ�ӡ�ͻ���
���������const char *url����ӡ������ĵ�ַ�Ͷ˿ڣ�����Ϊ��Ĭ�ϡ�ws://127.0.0.1:13528��
����ֵ�����ͣ��ɹ�����0������-1��ʾ��ʼ��ʧ�ܣ�����-2��ʾ�ظ���ʼ������
ע�⣺ÿ����һ��dll��ֻ����һ��initPrinterManager()���ظ����÷���-1.

�ӿڢڣ�void  setRecvDataCallback(_onMessage_func func);
���ܣ��趨�ص��������˻ص��������ã�������յ���������ûص�����
����ֵ����
���������_onMessage_func func������ָ�룬����Ϊtypedef void(*_onMessage_func)(const char* message)��
            C++�ж�Ӧ�ص��������ӣ�
			void handle_message(const char* message)
			{
				printf(">>> %s\n", message);
			}
�ӿڢۣ�int  sendMessage(const char *message);
���ܣ��������ݸ���ӡ�ͻ���
����ֵ�����ͣ��ɹ�����0������-1��ʾwebsocketδ����
���������const char *message:�ַ�������(������ַ�������ȷ���Ѿ���UTF-8����)

�ӿڢܣ�int closePrinterManager()
���ܣ��ر�websocket
����ֵ�����ͣ��ɹ�����0������-1��ʾwebsocket�ڵ���ǰ�ѹرջ�δ��ʼ��
ע�⣺��Ҫ�ظ��رգ����򷵻�-1.

