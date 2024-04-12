#define ROSSERIAL_ARDUINO_TCP
// #define R4WIFI

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
  Serial.begin(9600);
  setupWiFi();

  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.advertise(imu_pub);

  Wire.begin();
  sensor.setAccelSensitivity(0);  //  2g
  sensor.setGyroSensitivity(0);   //  250 degrees/s
  //sensor.calibrate(1000);
  sensor.setThrottle();

  sensor.axe = 0;
  sensor.aye = 0;
  sensor.aze = 0;
  sensor.gxe = 0;
  sensor.gye = 0;
  sensor.gze = 0;

  imu.header.frame_id = "world";
}

void loop()
{
  sensor.read();
  imu.header.stamp = nh.now();

  imu.linear_acceleration.x = - sensor.getAccelY() * 10;
  imu.linear_acceleration.y = sensor.getAccelX() * 10;
  imu.linear_acceleration.z = - sensor.getAccelZ() * 10;
  imu.angular_velocity.x = - sensor.getGyroY() * M_PI / 180;
  imu.angular_velocity.y = sensor.getGyroX() * M_PI / 180;
  imu.angular_velocity.z = sensor.getGyroZ() * M_PI / 180;

  imu_pub.publish(&imu);
  nh.spinOnce();
  delay(100);
}
