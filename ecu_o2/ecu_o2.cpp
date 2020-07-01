// ecu_o2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <atlstr.h>

typedef struct
{
    short lambda;
    short vol;
}cali_data_t;

static cali_data_t cali_data[] = 
{
{80,525},           //0.525V
{82,658},			//0.658V 
{85,814},			//0.814V 
{90,1074},			//1.074V 
{95,1307},			//1.307V 
{97,1388},			//1.388V 
{99,1458},			//1.458V 
{100,1500},			//1.5V 
{101,1515},			//1.515V 
{105,1602},			//1.602V 
{110,1703},			//1.703V 
{113,1763},			//1.763V 
{118,1846},			//1.846V 
{142,2206},			//2.206V 
{170,2487},			//2.487V 
{199,2710},			//2.710V 
{243,2958},			//2.958V 
{341,3289},			//3.289V 
{539,3605},			//3.605V 
{750,3762},	    	//3.762V 
{1012,3868},		//3.868V 
};


short get_lambda_value_by_adc(short cur_adc)
{
	short adc;
	short calc_result;
	short prev_adc, next_adc;
	short i, prev, next;


	prev_adc = 0;
	next_adc = 1024;
	prev = 0xFF;
	next = 0xFF;



	for (i = 0; i < sizeof(cali_data) / sizeof(cali_data_t); i++)
	{
		adc = roundf(((float)cali_data[i].vol * 1024 / 1000) / 5);

		if (adc <= cur_adc)
		{
			if (adc >= prev_adc)
			{
				prev_adc = adc;
				prev = i;
			}
		}
		else if (adc >= cur_adc)
		{
			if (adc <= next_adc)
			{
				next_adc = adc;
				next = i;
			}
		}
	}

	return cali_data[prev].lambda + roundf((float)(cur_adc - prev_adc) * (float)(cali_data[next].lambda - cali_data[prev].lambda) / (next_adc - prev_adc));
}


int main()
{
    int i;
    int j;
    FILE* fd;
    short vol, adc, adc_max, lambda;

	CString lambda_data;// = _T("static const INT16S LambdaValue[] = \r\n{\r\n");
	CString tempStr;
	char temp[100];

    vol = cali_data[0].vol;

    adc = roundf(((float)cali_data[0].vol * 1024 / 1000) / 5);
	adc_max = roundf(((float)cali_data[sizeof(cali_data) / sizeof(cali_data_t) - 1].vol * 1024 / 1000) / 5);

	sprintf((char* const)temp, "#define O2AD_MIN	%d\n#define O2AD_MAX	%d\r\nstatic const INT16S LambdaValue[%d] = \n{\n", adc, adc_max, adc_max - adc + 1);
	tempStr = temp;
	lambda_data += tempStr;


    for (i = adc; i <= adc_max; i++)
    {
		lambda = get_lambda_value_by_adc(i);

		sprintf((char* const)temp, "%d,			//%.3fV\n", lambda, (float)i * 5 / 1024);
		tempStr = temp;
		lambda_data += tempStr;
	}

	lambda_data += "};";

    char* file_content;

    file_content = new char[lambda_data.GetLength()];
    WideCharToMultiByte(CP_OEMCP, NULL, lambda_data, -1, file_content, lambda_data.GetLength(), NULL, FALSE);

    fd = fopen("D:\\lambda.c", "wt+");
    fwrite(file_content, lambda_data.GetLength(), 1, fd);
    fclose(fd);

    delete[] file_content;


    std::cout << "Hello World!\n";
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
