/*本程序由Oreo编辑用于H.Studios的智能药盒项目*/
/*下面是程序初始化内容*/
#include <DS3231.h>
#include <SD.h>
#include <SPI.h>
#include <Wire.h>
DS3231 clock;
File myFile;
RTCDateTime dt;
String Date;
String Filetype;
String Year;
String Month;
String Day;
String Recordfile;
int SDread();
int SDwrite();
bool TimeAlert();
void Bee();
void LED();
/*程序执行的主函数*/
void setup()
{
    //定义针脚
    pinMode(2, INPUT); //检测盒子开关的针脚
    pinMode(3, INPUT); //检测是否需要读取的针脚
    //第4针脚用于CS
    pinMode(5, INPUT);  //暂时没用
    pinMode(6, OUTPUT); //LED控制针脚
    pinMode(7, OUTPUT);
    //初始化针脚
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    //初始化串口
    Serial.begin(9600);
    Filetype = ".txt"; //初始化要生成文件的后缀
    while (!Serial)
    {
        ; //等待串口
    }
    Serial.println("initializing......");
    if (!SD.begin(4))
    {
        Serial.println("initializing failed");
        while (1)
            ;
    }
    clock.begin(); //初始化DS3231时钟
    clock.setDateTime(__DATE__, __TIME__);
    dt = clock.getDateTime();
    Year = dt.year;
    Month = dt.month;
    Day = dt.day;
    Date = Year + "-" + Month + "-" + Day + " " + dt.hour + ":" + dt.minute + ":" + dt.second;
    myFile = SD.open("record.txt", FILE_WRITE); //开始生成记录文件
    myFile.println("system start at - " + Date);
    Serial.println("already recorded");
    Serial.println("initializion done");
    myFile.close();
}
void loop()
{
    if (digitalRead(2) == LOW) //盒子开关
    {
        Serial.println("the case is opened start to write to SD card....");
        LED(1);
        if (SDwrite() == 1)
        {
            Serial.println("data has been already writen");
        }
        else
        {
            Serial.println("error to wtrite to sd card");
        }
        while (digitalRead(2) == LOW)
        {
            ;
        }
        LED(0);
    }
    if (digitalRead(3) == LOW) //读取开关
    {
        Serial.println("start to read from SD card......");
        LED(1);
        if (SDread() == 1)
        {
            Serial.println("**********over***********");
            while (digitalRead(3) == LOW)
            {
                ;
            }
        }
        else
        {
            Serial.println("error to read from SD card");
            while (digitalRead(3) == LOW)
            {
                ;
            }
        }
        LED(0);
    }
}
/*接下来是各子程序*/

int SDread() //读取文件的函数
{
    myFile = SD.open(Month + Day + Filetype);
    Serial.println(Month + Day + Filetype + ":");
    Serial.println("**********HEAD**********");
    while (myFile.available())
    {
        Serial.write(myFile.read());
    }
    return 1;
    myFile.close();
}
int SDwrite() //写入文件的函数
{
    dt = clock.getDateTime();
    Year = dt.year;
    Month = dt.month;
    Day = dt.day;
    myFile = SD.open(Month + Day + Filetype, FILE_WRITE);
    myFile.print("the case open at:         ");
    myFile.print(dt.year);
    myFile.print("-");
    myFile.print(dt.month);
    myFile.print("-");
    myFile.print(dt.day);
    myFile.print("      ");
    myFile.print(dt.hour);
    myFile.print(":");
    myFile.print(dt.minute);
    myFile.print(":");
    myFile.print(dt.second);
    myFile.print("\n");
    myFile.close();
    return 1;
}
void Bee(int a) //蜂鸣函数
{
    if (a == 1)
    {
        digitalWrite(7, HIGH);
    }
    else
    {
        digitalWrite(7, LOW);
    }
}
bool TimeAlert() //判断是否报警的函数
{
}
void LED(int a)
{
    if (a == 1)
    {
        digitalWrite(6, HIGH);
    }
    else
    {
        digitalWrite(6, LOW);
    }
}
