/*
 Name:		FinalProject.ino
 Created:	4/27/2019 9:17:20 PM
 Author:	Gaoxz
*/
 /*Androidͨ�Ŵ���:
PA_4:RX;    PA_5:TX;
FPGAͨ�Ŷ˿�:
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
	//��Android��λ��ͨ�ŵĴ��ڳ�ʼ��
	Serial3.begin(115200);
	Serial3.flush();
	//�������ͨ�ŵĴ��������м���
	Serial.begin(115200);
	Serial.flush();
	//��ɫ�������;��봫�����ĳ�ʼ��
	rgb.Init();
	distance.Init();
	//������봫��������������
	pinMode(PE_4, INPUT);
	//��FPGA���в��д���ͨ�ŵĶ˿� 
	pinMode(PQ_3, OUTPUT);
	pinMode(PQ_2, OUTPUT);
	//FPGA�����
	pinMode(PP_3, OUTPUT);

	//��ȡ�û�ID
	pinMode(PL_0,INPUT);//HIGH
	pinMode(PL_1,INPUT);
	pinMode(PL_2, INPUT);
	pinMode(PL_3, INPUT);//LOW
}
// ��ѭ��
void loop() {
	JudgeId();
	if (Serial3.available() > 0) {
		Scanner();
	}
}
//��ȡ�û�ID
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
//�������ݶ�ȡ����
void Scanner() {
	buffer = Serial.read();
	chooseMode(buffer);
}
//���ݶ�ȡ�Ĵ������ݽ���ģʽ�ķ���
void chooseMode(char data) {
	//��ͨ��Ϸģʽ
	if (data == 'a' || data == 'A') {
		NormalMode();
		Serial3.flush();
		Scanner();//back to check wether the value has changed.
	}
	//����ģʽ1
	if (data == 'b' || data == 'B') {
		debugMode1();
		Serial3.flush();
		Scanner();//back to check wether the value has changed.
	}
	//����ģʽ2
	if (data == 'c' || data == 'C') {
		debugMode2();
		Serial3.flush();
		Scanner();//back to check wether the value has changed.
	}
}
//��FPGA���в��д���ͨ�ŵ����ֻ���ģʽ
//10�����ɫ
void Setgreen() {
	digitalWrite(PQ_3, HIGH);
	digitalWrite(PQ_2, LOW);
}
//01�����ɫ
void Setblue() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_2, HIGH);
}
//00�����ɫ
void Setred() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_2, LOW);
}
//11ȫ��ֹͣ || ��λ��
void turnOff() {
	digitalWrite(PQ_3, HIGH);
	digitalWrite(PQ_2, HIGH);
}
//��ͨ��Ϸģʽ			δ��
void NormalMode() {
	ColorFinalCheck();
	//�ж��������������
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
//��ɫʶ�����			δ��
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
//����ʶ��ģ��
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
//ʤ���жϲ���
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
//����ģʽ1�����źŷ��͸���λ��ʹ��������õ����
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
//����ģʽ2:����������ȡ�������ݴ������λ��
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