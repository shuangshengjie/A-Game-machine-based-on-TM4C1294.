/*
 Name:		FinalProject.ino
 Created:	4/27/2019 9:17:20 PM
 Author:	Gaoxz
*/
 /*Android的通信引脚
PA_4:RX;    PA_5:TX;
FPGA的通信引脚
PQ_3:signal_High;   PQ_2:signal_Low;
和Arduino的通信引脚：
PK_0: RX;	PK_1: TX;
*/


#include "TCS34725.h"

char buffer;	//从上位机串口读取到的数据
TCS34725 rgb(0);	//设定一个颜色传感器的类
int userId = 0;		//用户ID
char color;		//判断的颜色
int score;		//最终的分数

char userGroup[15] = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'
};

//��ʼ��
void setup() {
	//与上位机的通信串口
	Serial3.begin(115200);
	Serial3.flush();
	//打开电脑的串口
	Serial.begin(115200);
	Serial.flush();
	//初始化两个传感器的读取引脚
	rgb.Init();
	Serial4.begin(115200);
	Serial4.flush();
	//胜负判断的引脚
	pinMode(PE_4, INPUT);
	//与FPGA的通信引脚
	pinMode(PQ_3, OUTPUT);//HIGH
	pinMode(PP_3, OUTPUT);
	pinMode(PQ_1, OUTPUT);//LOW

	//ID型号的读取引脚
	pinMode(PL_0,INPUT);//HIGH
	pinMode(PL_1,INPUT);
	pinMode(PL_2, INPUT);
	pinMode(PL_3, INPUT);//LOW

	pinMode(D1_LED, OUTPUT);
	RESETON();
}
//主要的循环
void loop() {
	JudgeId_And_Debug();//判断用户的ID && 维护模式的进行
	stayHere();//判断用户的距离并进入正常的游戏模式
	delay(3000);//等待用户将卡片取走
}
//判断用户ID
void JudgeId_And_Debug() {
	int button1;
	int button2;
	int button3;
	int button4;
	int userIdGet1;
	int userIdGet2;
	//Get the User ID
	while (1) {
		
		if (Serial3.available() > 0) {
			//维护模式1
			if (Serial3.read() == 'b') {
				blink();
				RESETOFF();
				delay(2000);
				RESETON();
				blink();
			}
			//维护模式2
			else if (Serial3.read() == 'c') {
				blink();
				Serial3.print("R ");
				Serial3.print(String(rgb.getRedData(), 10));
				Serial3.print(" G ");
				Serial3.print(String(rgb.getGreenData(), 10));
				Serial3.print(" B ");
				Serial3.print(String(rgb.getBlueData(), 10));
				Serial3.print(" C ");
				Serial3.println(String(rgb.getClearData(), 10));
				Serial3.print("Distance: ");
				Serial4.flush();
				if (Serial4.available() > 0){
					Serial3.write(Serial4.read());
				}
				else{
					Serial3.print("You are now within the allowable range");
				}
				Serial3.println();
				Serial4.flush();
				Serial3.flush();
				blink();
			}
		}
		Serial3.flush();

		button1 = digitalRead(PL_0);
		button2 = digitalRead(PL_1);
		button3 = digitalRead(PL_2);
		button4 = digitalRead(PL_3);

		userIdGet1 = button1 * 8 + button2 * 4 + button3 * 2 + button4 * 1;
		delay(1000);

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
	//发送读取到的用户数据
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
//判读那用户距离
void stayHere() {
	int distance;
	while (1) {
		if (Serial3.available() > 0) {
			if (Serial3.read() == 'd') {
				NormalMode();
				break;
			}
		}
		if (Serial4.available() > 0) {
			if (Serial4.read() == 'n') {
				Serial3.print('n');
				Serial3.flush();
				Serial4.flush();
			}
		}
		delay(400);
	}
	blink();
}
//正常的游戏模式
void NormalMode() {
	delay(3000);//延时三秒，等待用户取球
	blink();
	colorFinalCheck();

	blink();
	colorSendtoFPGA();	//将颜色信号发送给下位机
	sendDataToAndroid();//将输赢的数据发送给安卓
	//等待用户确认结果并点击退出
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
//颜色数据的判断
void colorFinalCheck(){
	while (1) {
		if (Serial3.available() > 0) {
			if (Serial3.read() == 't') {
				break;
			}
		}
		delay(10);
		if (rgb.getRedData() > 600) {
	  		color='r';
  		}
  		else if (rgb.getGreenData() > 600) {
	  		color='g';
  		}
  		else if (rgb.getBlueData() > 300) {
	  		color='b';
  		}
		Serial3.flush();
	}
}
//将颜色数据发送给下位机
void colorSendtoFPGA() {
	if (color == 'g') {
		setGreen();
	}
	else if (color == 'r') {
		setRed();
	}
	else if (color == 'b') {
		setBlue();
	}
	else{
	  setGreen();
	}
 blink();
}

//将输赢的数据发送给安卓
void sendDataToAndroid() {
	winORlose();
	while (1) {
		if (buffer == 's') {
			Serial3.flush();
			break;
		}
		Serial3.print(score);
		Serial3.flush();
		buffer = Serial3.read();
		delay(400);
	}
	blink();
}
//输赢判断方法
int winORlose() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 100; j++) {
			if (digitalRead(PE_4) == 1) {
				score = 1000-i*j;
        RESETON();
				return score;
			}
			delay(10);
		}
	}
	RESETON();
	return score;
}
//将所有信号重置
void resetAllData() {
	Serial3.flush();
	Serial.flush();
	color = '\0';
	buffer = '\0';
	userId = 0;
	RESETON();
	rgb.getClearData();
}
//闪灯进行流程确认
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
