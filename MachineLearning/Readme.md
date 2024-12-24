

Download a Bluetooth Serial monitor and connect to ESP32 to view the type of motion classified by the machine learning model:
 Android: https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal&hl=en_IN --(Tested -Is OK)
 Windows: https://apps.microsoft.com/detail/9NRS6V7H0N9B?hl=en-us&gl=IN&ocid=pdpshare --(Might not work properly)

Install the MotionClassifierMpu6050_inferencing.h header file through following steps:
 1.Open Arduino IDE
 2.Head to Sketch->Include library -> Add .ZIP library
 3.Navigate to ei-motionclassifiermpu6050-arduino-1.0.1.zip
After installing the library, include it via Sketch->Include library.

Burn the Mpu6050Inferencing.ino code into ESP32 and connect to any device which supports bluetooth serial monitor to view the classification.