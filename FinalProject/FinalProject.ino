/*
 Name:		FinalProject.ino
 Created:	4/27/2019 9:17:20 PM
 Author:	Gaoxz
*/
 /*Android通信串口:
PA_4:RX;    PA_5:TX;
FPGA通信端口:
PQ_3:signal_High;   PQ_2:signal_Low;
*/


#include "TCS34725.h"
#include "VL6180X.h"

#define distanceSample 16

char buffer;	//data read from the Android board
VL6180X distance(1);//data which is read by the distance sensor
TCS34725 rgb(0);	//data which is read by the color sensor
int colorJudged[4];	//the color data
int DISTANCE_LOW = 16;	//the distance data	closest
int DISTANCE_HIGH = 25;	//the distance data farest
int userId = 0;
int jumper;	//The value to control the program transport the data for only one time
//Initial
void setup() {
	//与Android上位机通信的串口初始化
	Serial3.begin(115200);
	Serial3.flush();
	//打开与电脑通信的串口来进行检验
	Serial.begin(115200);
	Serial.flush();
	//颜色传感器和距离传感器的初始化
	rgb.Init();
	distance.Init();
	//红外距离传感器的输入引脚
	pinMode(PE_4, INPUT);
	//与FPGA进行并行串口通信的端口 
	pinMode(PQ_3, OUTPUT);
	pinMode(PQ_2, OUTPUT);
	//FPGA给球口
	pinMode(PP_3, OUTPUT);

	//读取用户ID
	pinMode(PL_0,INPUT);//HIGH
	pinMode(PL_1,INPUT);
	pinMode(PL_2, INPUT);
	pinMode(PL_3, INPUT);//LOW
}
// 主循环
void loop() {
	JudgeId();
	if (Serial3.available() > 0) {
		Scanner();
	}
}
//读取用户ID
void JudgeId() {
	int button1;
	int button2;
	int button3;
	int button4;
	int userIdGet1;
	int userIdGet2;
	//Get the User ID
	while (1) {
		button1 = digitalRead(PL_0);
		button2 = digitalRead(PL_1);
		button3 = digitalRead(PL_2);
		button4 = digitalRead(PL_3);

		userIdGet1 = button1 * 8 + button2 * 4 + button3 * 2 + button4 * 1;
		delay(100);

		button1 = digitalRead(PL_0);
		button2 = digitalRead(PL_1);
		button3 = digitalRead(PL_2);
		button4 = digitalRead(PL_3);

		userIdGet2 = button1 * 8 + button2 * 4 + button3 * 2 + button4 * 1;
		if (userIdGet1 == userIdGet2) {
			if (userIdGet1 != 0) {
				break;
			}
		}
	}
	userId = userIdGet1;
	//Send the User ID
	while (1) {
		Serial3.print(userId);
		delay(500);
		if (Serial3.available() > 0) {
			if (Serial3.read() == 's') {
				break;
			}
		}
	}
}
//串口数据读取程序
void Scanner() {
	buffer = Serial.read();
	chooseMode(buffer);
}
//根据读取的串口数据进行模式的分流
void chooseMode(char data) {
	//普通游戏模式
	if (data == 'a' || data == 'A') {
		NormalMode();
		Serial3.flush();
		Scanner();//back to check wether the value has changed.
	}
	//调试模式1
	if (data == 'b' || data == 'B') {
		debugMode1();
		Serial3.flush();
		Scanner();//back to check wether the value has changed.
	}
	//调试模式2
	if (data == 'c' || data == 'C') {
		debugMode2();
		Serial3.flush();
		Scanner();//back to check wether the value has changed.
	}
}
//和FPGA进行并行串口通信的四种基础模式
//10输出绿色
void Setgreen() {
	digitalWrite(PQ_3, HIGH);
	digitalWrite(PQ_2, LOW);
}
//01输出蓝色
void Setblue() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_2, HIGH);
}
//00输出红色
void Setred() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_2, LOW);
}
//11全部停止 || 复位？
void turnOff() {
	digitalWrite(PQ_3, HIGH);
	digitalWrite(PQ_2, HIGH);
}
//普通游戏模式			未完
void NormalMode() {
	ColorFinalCheck();
	//判断条件并决定输出
	GiveBall();
	ColorJudgement();
	stayHere();
	sendDataToAndroid();
}
void GiveBall() {
	digitalWrite(PP_3, HIGH);
	delay(1000);
	digitalWrite(PP_3, LOW);
}
//颜色识别程序			未完
void ColorJudgement() {
	if (colorJudged[0] && colorJudged[1] && colorJudged[2] && colorJudged[3]) {
		Setgreen();
	}
	else if (colorJudged[0] && colorJudged[1] && colorJudged[2] && colorJudged[3]) {
		Setred();
	}
	else if (colorJudged[0] && colorJudged[1] && colorJudged[2] && colorJudged[3]) {
		Setblue();
	}
}
void ColorFinalCheck() {
	int* color1 = getColor();
	int* color2 = getColor();
	int* color3 = getColor();
	int* color4 = getColor();
	int* color5 = getColor();
	int* coloraverage = average(color1, color2, color3, color4, color5);
	colorJudged[0] = coloraverage[0];
	colorJudged[1] = coloraverage[1];
	colorJudged[2] = coloraverage[2];
	colorJudged[3] = coloraverage[3];
}
int* getColor() {
	int color[4];
	color[0] = rgb.getGreenData();
	color[1] = rgb.getBlueData();
	color[2] = rgb.getRedData();
	color[3] = rgb.getClearData();
	delay(10);
	return color;
}
int* average(int* color1, int* color2, int* color3, int* color4, int* color5) {
	int color[4];
	for (int i = 0; i < 4; i++) {
		color[i] = color1[i] + color2[i] + color3[i] + color4[i] + color5[i];
		color[i] /= 5;
	}
	return color;
}
//距离识别模块
void stayHere() {
	int distance;
	while (1) {
		distance = distanceGet();
		if (distance > DISTANCE_HIGH) {
			Serial3.write('f');
		}
		else if (distance < DISTANCE_LOW) {
			Serial3.print('c');
		}
		else if (distance < DISTANCE_HIGH && distance > DISTANCE_LOW) {
			break;
		}
	}
}
int distanceGet() {
	int distanceget = distance.readRangeSingle();
	delay(1);
	return distanceget;
}
//胜负判断部分
void sendDataToAndroid() {
	char single;
	single = winORlose();
	while (1) {
		Serial3.write(single);
		Serial3.flush();
		buffer = Serial3.read();
		if (buffer == 's') {
			break;
		}
		delay(500);
	}
}
char winORlose() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 100; j++) {
			if (digitalRead(PE_4) == 0) {
				return 'w';
			}
			delay(10);
		}
	}
	return 'l';
}
//调试模式1：将信号发送给下位机使其逐个调用电机。
void debugMode1() {
	Setgreen();
	delay(1000);
	Setblue();
	delay(1000);
	Setred();
	delay(1000);
	turnOff();
	delay(5000);
}
//调试模式2:将传感器读取到的数据传输给上位机
void debugMode2() {
	Serial3.print("R:");
	Serial3.print(String(rgb.getRedData(), 10));
	Serial3.print("\tG:");
	Serial3.print(String(rgb.getGreenData(), 10));
	Serial3.print("\tB:");
	Serial3.print(String(rgb.getBlueData(), 10));
	Serial3.print("\tC:");
	Serial3.println(String(rgb.getClearData(), 10));
	Serial3.print("Distance:");
	Serial3.println(String(distance.readRangeSingle(), 10));
}