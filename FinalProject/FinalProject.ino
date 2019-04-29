/*
 Name:		FinalProject.ino
 Created:	4/27/2019 9:17:20 PM
 Author:	Gaoxz
*/
 /*Androidͨ�Ŵ���:
PA_4:RX;    PA_5:TX;
FPGAͨ�Ŷ˿�:
PP_3:signal_High;   PQ_2:signal_Low;
*/

//��ɫ������ͷ�ļ�
#include "TCS34725.h"
//���봫����ͷ�ļ�
#include "VL6180X.h"

char buffer[18];	//���������յ�����
VL6180X distance(1);//���봫������ȡ������
TCS34725 rgb(0);	//��ɫ��������ȡ������
int colorJudged[4];	//���ն�ȡ����ɫ����
int distanceJudged;	//��ȡ�ı�׼����
int index = 0;		//�������������ݵ�
//��ʼ��
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
	pinMode(PP_3, OUTPUT);
	pinMode(PQ_2, OUTPUT);
}
// ��ѭ��
void loop() {
	if (Serial3.available() > 0) {
		Scanner();
	}
}
//�������ݶ�ȡ����
void Scanner() {
	delay(1);
	int numChar = Serial3.available();
	if (numChar > 15) {
		numChar = 15;
	}
	while (numChar--) {
		buffer[index++] = Serial3.read();
	}
	index = 0;
	splitString(buffer);
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
		Scanner();//back to check wether the value has changed.
	}
	//����ģʽ1
	if (data[0] == 'b' || data[0] == 'B') {
		debugMode1();
		Scanner();//back to check wether the value has changed.
	}
	//����ģʽ2
	if (data[0] == 'c' || data[0] == 'C') {
		debugMode2();
		Scanner();//back to check wether the value has changed.
	}
}
//��FPGA���в��д���ͨ�ŵ����ֻ���ģʽ
//10�����ɫ
void Setgreen() {
	digitalWrite(PP_3, HIGH);
	digitalWrite(PQ_2, LOW);
}
//01�����ɫ
void Setblue() {
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_2, HIGH);
}
//00�����ɫ
void Setred() {
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_2, LOW);
}
//11ȫ��ֹͣ || ��λ��
void turnOff() {
	digitalWrite(PP_3, HIGH);
	digitalWrite(PQ_2, HIGH);
}
//��ͨ��Ϸģʽ			δ��
void NormalMode() {
	ColorJudgement();
	//�ж��������������
	if (colorJudged[0] && colorJudged[1] && colorJudged[2] && colorJudged[3]) {

	}
}
//��ɫʶ�����			δ��
void ColorJudgement() {
	ColorFinalCheck();
	if (colorJudged[0] && colorJudged[1] && colorJudged[2] && colorJudged[3]) {

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
int distanceGet() {
	int distanceget = distance.readRangeSingle();
	delay(1);
	return distanceget;
}
//ʤ���жϲ���			δ��
int winORlose() {
	int distancedata;
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
	delay(1);
}
