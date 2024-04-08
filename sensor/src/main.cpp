#define ROSSERIAL_ARDUINO_TCP

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <ros.h>
#include <sensor_msgs/Imu.h>
#include <GY521.h>


const char* ssid     = "4809-2.4G";
const char* password = "20030327";
// Set the rosserial socket server IP address
IPAddress server(10,0,0,143);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;

GY521 sensor(0x68);

ros::NodeHandle  nh;
sensor_msgs::Imu imu;
ros::Publisher imu_pub("imu", &imu);

void setupWiFi()
{  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500);Serial.print("."); }
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP:   ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  setupWiFi();

  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.advertise(imu_pub);

  Wire.begin();
  sensor.setAccelSensitivity(0);  //  2g
  sensor.setGyroSensitivity(0);   //  250 degrees/s
  sensor.setThrottle();

  sensor.axe = 6.176758324727416e-05;
  sensor.aye = -1.8798829842125997e-05;
  sensor.aze = -0.000981201184913516;
  sensor.gxe = 0.0018702291417866945;
  sensor.gye = 0.0012900764122605324;
  sensor.gze = 0.0011374045861884952;

  imu.header.frame_id = "world";
}

void loop()
{
  sensor.read();
  imu.header.stamp = nh.now();

  imu.linear_acceleration.x = sensor.getAccelX();
  imu.linear_acceleration.y = sensor.getAccelY();
  imu.linear_acceleration.z = sensor.getAccelZ();
  imu.angular_velocity.x = sensor.getAngleX();
  imu.angular_velocity.y = sensor.getAngleY();
  imu.angular_velocity.z = sensor.getAngleZ();

  imu_pub.publish(&imu);
  nh.spinOnce();
  delay(20);
}
