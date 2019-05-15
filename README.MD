# 所用到的引脚和信号的定义

## 最终程序使用到的所有引脚定义

### TCS34725颜色传感器

--------
| Pin on the sensor | Pin on the board |
| :---------------: | :--------------: |
|        SDA        |       PB3        |
|        SCL        |       PB2        |
|        3V3        |        3V        |
|        GND        |       GND        |

--------

### VL6180X距离传感器

--------
| Pin on the sensor | Pin on the board |
| :---------------: | :--------------: |
|        SDA        |       PN4        |
|        SCL        |       PN5        |
|        VCC        |        5V        |
|        GND        |       GND        |

--------

### 红外距离传感器

--------
| Pin on the sensor | Pin on the board |
| :---------------: | :--------------: |
|        VCC        |        3V        |
|        GND        |       GND        |
|        OUT        |       PE_4       |

### FPGA通信引脚

--------
| Pin on the FPGA | Pin on the board |
| :-------------: | :--------------: |
|      PP_3       |  Give the Ball   |
|      PQ_3       |       HIGH       |
|      PQ_2       |       LOW        |
|       GND       |       GND        |

--------

### Android通信引脚定义

--------
| Pin on the Android | Pin on the board |
| :----------------: | :--------------: |
|        PA_4        |        RX        |
|        PA_5        |        TX        |
|        GND         |       GND        |

--------

### 用户ID读取引脚

--------
| Pin on the biard | define |
| :--------------: | :----: |
|       PL_0       |   *8   |
|       PL_1       |   *4   |
|       PL_2       |   *2   |
|       PL_3       |   *1   |

--------

## 所用到的信号定义

### 与FPGA通信的通信信号定义

--------
|    Single     |      Meaning      |
| :-----------: | :---------------: |
| PQ_3:0 PQ_2:0 |  The ball is red  |
| PQ_3:0 PQ_2:1 | The ball is blue  |
| PQ_3:1 PQ_2:0 | The ball is green |
| PQ_3:1 PQ_2:1 |       RESET       |
|    PP_3:1     |   Give the Ball   |
|    PP_3:0     |  Close the door   |

--------

### 与Android通信的信号定义

1. 接收到的信号

    --------
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

    --------
    | Single |  Meaning   |
    | :----: | :--------: |
    |  A~O   | User group |
    --------