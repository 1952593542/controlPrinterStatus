﻿// 控制打印机状态.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<stdio.h>
using namespace std;

#include <windows.h>
#include <string.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")// 功能：暂停打印任务

/**
* args：[in]bStop TRUE-暂停 FALSE-恢复打印
* 返回：成功返回0，其他失败
*/
int StopPrintJob(BOOL bStop)
{
	// 获取当前默认打印机
	DWORD dwNameLen = MAX_PATH;
	TCHAR szPrinterName[MAX_PATH] = { 0 };

	 GetDefaultPrinter(szPrinterName, &dwNameLen);
	 cout << "默认打印机名为：" << szPrinterName << endl;
	// OpenPrinter一定要加上这个设置权限，不然后面会SetPrinter设置失败，GetLastError()返回5-拒绝访问；
	// 另外，使用Unicode字符集声明PRINTER_DEFAULTSW，使用多字节字符集需要声明PRINTER_DEFAULTSA，我这边用的是Unicode字符集
	PRINTER_DEFAULTSW defaultSw = { NULL,  NULL, PRINTER_ACCESS_ADMINISTER };
	// 根据打印机名称打开打印机
	int iRet = 0;
	HANDLE hPrinter = NULL;
	BOOL bRet = OpenPrinter(szPrinterName, &hPrinter, &defaultSw); // open device
	if (!bRet){
		// 打开打印机失败
		cout << "打开打印机失败" << endl;
		return -1;
	}
	// 设置操作打印机指令，默认为暂停打印
	DWORD dwCommand = JOB_CONTROL_PAUSE;
	if (!bStop){
		// 如果不是暂停，那就是恢复打印
		dwCommand = JOB_CONTROL_RESUME;
	}
	// 暂停|恢复打印机
	DWORD dwLastError = 0;
	try {
		bRet = SetPrinter(hPrinter, 0, 0, dwCommand);
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	if (!bRet){// 暂停|恢复打印机失败
		iRet = -2;
		cout << "操作打印机失败" << endl;
		dwLastError = GetLastError();
	}
	// 关闭打印机
	bRet = ClosePrinter(hPrinter);
	if (!bRet){// 关闭打印机失败
		cout << "关闭打印机失败" << endl;
		iRet = -3;
	}return iRet;
}

int main()
{
	cout << "请输入操作：" << endl << "1.stop printer." << endl << "2.start printer" << endl;
	int operatorFlag = -1;
	cin >> operatorFlag;
	if (operatorFlag == 1) {
		StopPrintJob(TRUE);
	}
	else if (operatorFlag == 2) {
		StopPrintJob(FALSE);
	}
	else {
		cout << "不能识别的操作" << endl;
	}

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
