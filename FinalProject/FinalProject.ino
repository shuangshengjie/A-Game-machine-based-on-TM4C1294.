/*
 Name:		FinalProject.ino
 Created:	4/27/2019 9:17:20 PM
 Author:	Gaoxz
*/
 /*Androidͨ�Ŵ���:
PA_4:RX;    PA_5:TX;
FPGAͨ�Ŷ˿�:
PP_3:red;   PQ_2:green;     PQ_3:blue;
*/

//��ɫ������ͷ�ļ�
#include "TCS34725.h"
//���봫����ͷ�ļ�
#include "VL6180X.h"
//����λ�����ܵ�������
char buffer[18];
//��������
VL6180X distance(1);
//��ɫ����
TCS34725 rgb(0);

void setup() {
	//��Android��λ��ͨ�ŵĴ��ڳ�ʼ��
	Serial3.begin(115200);
	Serial3.flush();
	//�������ͨ�ŵĴ��������м���
	Serial.begin(9600);
	Serial.flush();
	//��ɫ�������;��봫�����ĳ�ʼ��
	rgb.Init();
	distance.Init();

	//��FPGA���в��д���ͨ�ŵĶ˿� 
	pinMode(PQ_2, OUTPUT);//��ɫ
	pinMode(PQ_3, OUTPUT);//��ɫ
	pinMode(PP_3, OUTPUT);//��ɫ
	pinMode(PQ_1, OUTPUT);//�ܿؿ���
}

// ��ѭ��
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
//���ڶ�ȡ�����ݴ������
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
//���ݶ�ȡ�Ĵ������ݽ���ģʽ�ķ���
void chooseMode(char * data) {
	//��ͨ��Ϸģʽ
	if (data[0] == 'a' || data[0] == 'A') {
		NormalMode();
	}
	//����ģʽ1
	if (data[0] == 'b' || data[0] == 'B') {
		while (1) { 
			debugMode1();
		}
	}
	//����ģʽ2
	if (data[0] == 'c' || data[0] == 'C') {
		debugMode2();
	}
}
//��ͨ��Ϸģʽ
void NormalMode() {

}
//����ģʽ1
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
//����ģʽ2
void debugMode2() {

}

//��ɫʶ�����
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

//��FPGA���в��д���ͨ�ŵ�����ģʽ
//1001�����ɫ
void Setgreen() {
	digitalWrite(PQ_2, HIGH);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_1, HIGH);
}
//0011�����ɫ
void Setblue() {
	digitalWrite(PQ_2, LOW);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_3, HIGH);
	digitalWrite(PQ_1, HIGH);
}
//0101�����ɫ
void Setred() {
	digitalWrite(PQ_2, LOW);
	digitalWrite(PP_3, HIGH);
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_1, HIGH);
}
//0000ȫ��ֹͣ
void turnOff() {
	digitalWrite(PQ_2, LOW);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_3, LOW);
	digitalWrite(PQ_1, LOW);
}