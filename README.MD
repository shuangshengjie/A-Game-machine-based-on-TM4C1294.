# 项目简介
该项目是西安电子科技大学通信工程学院大二下学期项目管理课程要求涉及项目。
该文件夹是我所在的组的微控制器部分的程序，上传到Github上做一个纪念？（笑）

# 所用到的引脚和信号的定义

## 最终程序使用到的所有引脚定义
--------
### TCS34725颜色传感器


| Pin on the sensor | Pin on the board |
| :---------------: | :--------------: |
|        SDA        |       PB3        |
|        SCL        |       PB2        |
|        3V3        |        3V        |
|        GND        |       GND        |

--------

### VL6180X距离传感器
因为最终发现两个传感器同时使用的时候原先配置的头文件会让两个传感器的数据均出现
较大的误差，因此最终并没有让VL6180X连接在TM4C1294上，而是用Arduino作为VL6180X的
读取，由Arduino对当前的距离数据进行判断来决定是否向Android传输数据，提醒用户注意距离。
但在两个传感器单独使用的时候工作是正常的。

<h4>对于这个问题我也衷心地希望哪位大佬能告诉我这个情况出现的原因！如能赐教，不胜感激！</h4>

| Pin on the sensor | Pin on the board |
| :---------------: | :--------------: |
|        SDA        |       PN4        |
|        SCL        |       PN5        |
|        VCC        |        5V        |
|        GND        |       GND        |

--------

### 红外距离传感器


| Pin on the sensor | Pin on the board |
| :---------------: | :--------------: |
|        VCC        |        3V        |
|        GND        |       GND        |
|        OUT        |       PE_4       |

--------

### FPGA通信引脚


| Pin on the FPGA | Pin on the board |
| :-------------: | :--------------: |
|      PP_3       |  Give the Ball   |
|      PQ_3       |       HIGH       |
|      PQ_2       |       LOW        |
|       GND       |       GND        |

--------

### 与Android通信引脚定义


| Pin on the Android | Pin on the board |
| :----------------: | :--------------: |
|        PA_4        |        RX        |
|        PA_5        |        TX        |
|        GND         |       GND        |

--------

### 用户ID读取引脚


| Pin on the biard | define |
| :--------------: | :----: |
|       PL_0       |   *8   |
|       PL_1       |   *4   |
|       PL_2       |   *2   |
|       PL_3       |   *1   |

--------

## 所用到的信号定义

--------

### 与FPGA通信的通信信号定义

|        Single        |      Meaning      |
| :------------------: | :---------------: |
| PQ_3:0 PP_3:0 PQ_2:0 |       RESET       |
| PQ_3:X PP_3:1 PQ_2:1 | The ball is blue  |
| PQ_3:X PP_3:1 PQ_2:0 | The ball is green |
| PQ_3:X PP_3:0 PQ_2:1 |  The ball is red  |
| PQ_3:1 PP_3:0 PQ_2:0 |     Position      |

--------

### 与Android通信的信号定义

1. 接收到的信号


    | Single |             Meaning             |
    | :----: | :-----------------------------: |
    |   d    |           Normal Mode           |
    |   b    |    control the steeper motor    |
    |   c    | return the data sensor has read |
    |   s    |       stop send the data        |
    |   e    |              exit               |
    |   t    |       begin counting time       |
    |   f    |      too far or too close       |
    |   n    | user is in the normal distance  |

    --------

2. 发送的信号


    | Single |  Meaning   |
    | :----: | :--------: |
    |  A~O   | User group |
    --------