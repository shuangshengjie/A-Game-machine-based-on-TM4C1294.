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

char buffer;	//�Ӵ����ж�ȡ��������
VL6180X distance(1);//����һ���������
TCS34725 rgb(0);	//����һ����ɫ����
int colorJudged[3];	//��ɫ��������ȡ������ɫ����
int DISTANCE_LOW = 10;	//�û����ڷ�Χ���������
int DISTANCE_HIGH = 90;	//�û����ڷ�Χ����Զ����
int userId = 0;
int TIME_LIMITED = 50;
int score;

char userGroup[15] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'
};

//��ʼ��
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
	//������봫��������������		ʤ���ж�����
	pinMode(PE_4, INPUT);
	//��FPGA���в��д���ͨ�ŵĶ˿� 
	pinMode(PQ_3, OUTPUT);//HIGH
	pinMode(PP_3, OUTPUT);
	pinMode(PQ_1, OUTPUT);//LOW

	//��ȡ�û�ID
	pinMode(PL_0,INPUT);//HIGH
	pinMode(PL_1,INPUT);
	pinMode(PL_2, INPUT);
	pinMode(PL_3, INPUT);//LOW

	pinMode(D1_LED, OUTPUT);
	RESETON();
}
// ��ѭ��
void loop() {
	JudgeId();//�ж��û�ID��ά��ģʽ
	//stayHere();//�ж��û�����
	delay(2000);
	NormalMode();
	delay(3000);//�ȴ��û�����Ƭȡ��
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
		
		if (Serial3.available() > 0) {
			if (Serial3.read() == 'b') {
				debugMode1();
			}
			else if (Serial3.read() == 'c') {
				debugMode2();
			}
		}
		Serial3.flush();

		button1 = digitalRead(PL_0);
		button2 = digitalRead(PL_1);
		button3 = digitalRead(PL_2);
		button4 = digitalRead(PL_3);

		userIdGet1 = button1 * 8 + button2 * 4 + button3 * 2 + button4 * 1;
		delay(300);

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
		if (Serial3.available() > 0) {
			if (Serial3.read() == 's') {
				break;
			}
		}
		Serial3.print(userGroup[userId-1]);
		Serial3.flush();
		delay(400);
	}
	blink();
}
//����ʶ��ģ��
void stayHere() {
	int distance;
	while (1) {
		Serial3.flush();
		distance = distanceGet();
		if (distance > DISTANCE_HIGH || distance < DISTANCE_LOW) {
			Serial3.print('n');
			Serial3.flush();
			delay(400);
		}
		else{
			Serial3.flush();
			break;
		}
	}
	blink();
}
int distanceGet() {
	int distanceget1 = distance.readRangeSingle();
	delay(200);
	int distanceget2 = distance.readRangeSingle();
	int distanceget = (distanceget1 + distanceget2) / 2;
	return distanceget;
}
//��������Ϸģʽ
void NormalMode() {
	delay(3000);//��������ʵ��������и���
	blink();
	colorFinalCheck();
	while (1) {
		if (Serial3.available() > 0) {
			if (Serial3.read() == 't') {
				break;
			}
		}
		delay(10);
		Serial3.flush();
	}
	blink();
	colorSendtoFPGA();	//��ɫ�ж�&&���ƶ���ģʽ
	sendDataToAndroid();//������Ӯ���ݸ���׿
	//������λ���ķ������ݾ����Ƿ������Ϸ
	while (1) {
		if (Serial3.available() > 0) {
			if (Serial3.read() == 'e') {
				break;
			}
		}
		delay(10);
		Serial3.flush();
	}
	resetAllData();
}

//��ɫʶ�����			δ��		ʵ�����ݲ���
void colorSendtoFPGA() {
	if (colorJudged[0] > 170) {
		setGreen();
	}
	if (colorJudged[2] > 170) {
		setRed();
	}
	if (colorJudged[1] > 100) {
		setBlue();
	}
	blink();
}
/*
��ɫ�жϲ��֣��ܹ����25�Σ��ܼƳ���2.5s
*/
void colorFinalCheck() {
	int* color1 = getColor2();
	int* color2 = getColor2();
	int* color3 = getColor2();
	int* color4 = getColor2();
	int* color5 = getColor2();
	int* coloraverage = average(color1, color2, color3, color4, color5);
	colorJudged[0] = coloraverage[0];
	colorJudged[1] = coloraverage[1];
	colorJudged[2] = coloraverage[2];
}
int* getColor2(){
	int* color1 = getColor1();
	int* color2 = getColor1();
	int* color3 = getColor1();
	int* color4 = getColor1();
	int* color5 = getColor1();
	int* coloraverage = average(color1, color2, color3, color4, color5);
	return coloraverage;
}
int* getColor1() {
	int color[3];
	color[0] = rgb.getGreenData();
	color[1] = rgb.getBlueData();
	color[2] = rgb.getRedData();
	rgb.getClearData();
	delay(100);
	return color;
}
//�Բɼ�������ɫ���ݽ���ƽ��������
int* average(int* color1, int* color2, int* color3, int* color4, int* color5) {
	int color[3];
	for (int i = 0; i < 3; i++) {
		color[i] = color1[i] + color2[i] + color3[i] + color4[i] + color5[i];
		color[i] /= 5;
	}
	return color;
}

//ʤ���жϲ���
void sendDataToAndroid() {
	int single;
	single = winORlose();
	while (1) {
		if (buffer == 's') {
			Serial3.flush();
			break;
		}
		Serial3.print(single);
		Serial3.flush();
		buffer = Serial3.read();
		delay(400);
	}
	blink();
}
//����Ϊ��������Ϸ		����������۵���Ϸʱ����е���
int winORlose() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 100; j++) {
			if (digitalRead(PE_4) == 0) {
				score = ((1000-i*j) / 1000) * 100;
				return score;
			}
			delay(10);
		}
	}
	score = 0;
	RESETON();
	return score;
}
void resetAllData() {
	Serial3.flush();
	Serial.flush();
	colorJudged[0] = '\0';
	colorJudged[1] = '\0';
	colorJudged[2] = '\0';
	buffer = '\0';
	userId = 0;
	RESETON();
	rgb.getClearData();
}
//����ģʽ1�����źŷ��͸���λ��ʹ��������õ����
void debugMode1() {
	RESETOFF();
	delay(2000);
	RESETON();
}
//����ģʽ2:����������ȡ�������ݴ������λ��
void debugMode2() {
	Serial3.print("R:");
	Serial3.print(rgb.getRedData());
	Serial3.print("\tG:");
	Serial3.print(rgb.getGreenData());
	Serial3.print("\tB:");
	Serial3.print(rgb.getBlueData());
	Serial3.print("\tC:");
	Serial3.print(rgb.getClearData());
	Serial3.print("Distance:");
	Serial3.println(distance.readRangeSingle());
	Serial3.flush();
	blink();
}
//�������ݳɹ���������ʾ
void blink() {
	digitalWrite(D1_LED, HIGH);
	delay(200);
	digitalWrite(D1_LED, LOW);
	delay(200);
}
//��FPGAͨ�ŵ�����ģʽ
//����FPGA�����ģʽΪ��ɫ
void setGreen() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PP_3, HIGH);
	digitalWrite(PQ_1, LOW);
}
//����FPGA�����ģʽΪ��ɫ
void setBlue() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PP_3, HIGH);
	digitalWrite(PQ_1, HIGH);
}
//����FPGA�����ģʽΪ��ɫ
void setRed() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_1, HIGH);
}
//��FPGA��Ϊ��ʼ��ģʽ
void RESETON() {
	digitalWrite(PQ_3, LOW);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_1, LOW);
}
//��FPGA��Ϊ����ģʽ�ж��ȫ��̧���״̬
void RESETOFF() {
	digitalWrite(PQ_3, HIGH);
	digitalWrite(PP_3, LOW);
	digitalWrite(PQ_1, LOW);
}