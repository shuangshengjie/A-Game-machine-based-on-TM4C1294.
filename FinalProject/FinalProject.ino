/*
 Name:		FinalProject.ino
 Created:	4/27/2019 9:17:20 PM
 Author:	Gaoxz
*/
 /*Android通信串口:
PA_4:RX;    PA_5:TX;
FPGA通信端口:
PP_3:red;   PQ_2:green;     PQ_3:blue;
*/

//颜色传感器头文件
#include "TCS34725.h"
//距离传感器头文件
#include "VL6180X.h"
//从上位机接受到的数据
char buffer[18];
//距离数据
VL6180X distance(1);
//颜色数据
TCS34725 rgb(0);

void setup() {
	//与Android上位机通信的串口初始化
	Serial3.begin(115200);
	Serial3.flush();
	//打开与电脑通信的串口来进行检验
	Serial.begin(9600);
	Serial.flush();
	//颜色传感器和距离传感器的初始化
	rgb.Init();
	distance.Init();

	//与FPGA进行并行串口通信的端口 
	pinMode(PQ_2, OUTPUT);//绿色
	pinMode(PQ_3, OUTPUT);//蓝色
	pinMode(PP_3, OUTPUT);//红色
	pinMode(PQ_1, OUTPUT);//总控开管
}

// 主循环
void loop() {
	if (Serial3.available() > 0) {
		int index = 0;
		delay(1);
		int numChar = Serial3.available();
		if (numChar > 15) {
			numChar = 15;
		}
		while (numChar--) {
			buffer[index++] = Serial3.read();
		}
		splitString(buffer);
	}
}
//串口读取的数据处理程序
void splitString(char * data) {
	char * parameter = strtok(data, " ,");
	while (parameter != NULL) {
		chooseMode(parameter);
		parameter = strtok(NULL, " ,");
	}
	for (int i = 0; i < 16; i++) {
		buffer[i] = '\0';
	}
	Serial3.flush();
}
//根据读取的串口数据进行模式的分流
void chooseMode(char * data) {
	//普通游戏模式
	if (data[0] == 'a' || data[0] == 'A') {
		NormalMode();
	}
	//调试模式1
	if (data[0] == 'b' || data[0] == 'B') {
		while (1) { 
			debugMode1();
		}
	}
	//调试模式2
	if (data[0] == 'c' || data[0] == 'C') {
		debugMode2();
	}
}
//普通游戏模式
void NormalMode() {

}
//调试模式1
void debugMode1() {
	Setgreen();
	delay(1000);
	Setblue();
	delay(1000);
	Setred();
	delay(1000);
	turnOff();
	delay(1000);
}
//调试模式2
void debugMode2() {

}

//颜色识别程序
int *getColor(int *color) {
	color[0] = rgb.getGreenData();
	color[1] = rgb.getBlueData();
	color[2] = rgb.getRedData();
	delay(10);
	return color;
}
int *average(int *color1, int *color2, int *color3, int *color4, int *color5) {
	int color[3];
	for (int i = 0; i < 3; i++) {
		color[i] = color1[i] + color2[i] + color3[i] + color4[i] + color5[i];
		color[i] /= 5;
	}
	return color;
}

//和FPGA进行并行串口通信的四种模式
//1001输出绿色
void Setgreen() {
	digitalWrite(PQ_2, HIGH);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_1, HIGH);
}
//0011输出蓝色
void Setblue() {
	digitalWrite(PQ_2, LOW);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_3, HIGH);
	digitalWrite(PQ_1, HIGH);
}
//0101输出红色
void Setred() {
	digitalWrite(PQ_2, LOW);
	digitalWrite(PP_3, HIGH);
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_1, HIGH);
}
//0000全部停止
void turnOff() {
	digitalWrite(PQ_2, LOW);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_1, LOW);
}