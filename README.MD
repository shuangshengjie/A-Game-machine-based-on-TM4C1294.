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
|        SCL        |       PB2        |
|        VCC        |        5V        |
|        GND        |       GND        |

--------

### FPGA通信引脚

--------
| Pin on the FPGA | Pin on the board |
| :-------------: | :--------------: |
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

|

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
| a or A |           Normal Mode           |
| b or B |    control the steeper motor    |
| c or C | return the data sensor has read |
| s or S |       stop send the data        |

--------

2. 发送的信号

--------
| Single | Meaning |
| :----: | :-----: |
|   w    |   win   |
|   l    |  lose   |

