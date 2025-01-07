// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__WIFI_CLOUD

#include <WiFi.h>

// Include MPU6050Handler for mouse and car functioning
#include "MPU6050Handler.h"

// Includes for car NRF Module
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define pins for the RF24 module
#define CE_PIN 27
#define CSN_PIN 26

RF24 radio(CE_PIN, CSN_PIN);     // CE and CSN pins
const byte address[6] = "00001"; // Communication address
MPU6050Handler mpuHandler;

// Persistent `prev` variable
char prev = 'S';

// Mouse Control
// Define input pins
#define INPUT_PIN1 4 // D4
#define INPUT_PIN2 5 // D5

// Variables to store the previous button states
bool prevButtonStateLeft = HIGH;
bool prevButtonStateRight = HIGH;

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "stars"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "82a46bad2718eb9194c6c517c6425214"
#define REMOTEXY_ACCESS_PASSWORD "12345"

#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 47 bytes
  { 255,1,0,0,0,40,0,19,0,0,0,71,117,101,115,116,117,114,101,32,
  67,111,110,116,114,111,108,108,101,114,0,113,1,106,200,1,1,1,0,3,
  23,24,62,17,132,2,26 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t Modes; // from 0 to 4

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

#define MODE_IDLE 0 // Idle Mode
#define USECASE1 1  // UseCase1
#define USECASE2 2  // UseCase2
#define USECASE3 3  // UseCase3

// Variable to keep track of the current mode
int currentMode = MODE_IDLE;
bool modeChanged = false;

//////////////////////////////////////////
//         IDLE Definitions            //
//////////////////////////////////////////

// Function Prototypes for UseCase1
bool initializeIdle();
void handleIdle();
void cleanupIdle();

//////////////////////////////////////////
//         UseCase1 Definitions         //
//////////////////////////////////////////

// Function Prototypes for UseCase1
bool initializeUseCase1();
void handleUseCase1();
void cleanupUseCase1();

//////////////////////////////////////////
//         UseCase2 Definitions         //
//////////////////////////////////////////

// Function Prototypes for UseCase2
bool initializeUseCase2();
void handleUseCase2();
void cleanupUseCase2();

//////////////////////////////////////////
//         UseCase3 Definitions         //
//////////////////////////////////////////

// Function Prototypes for UseCase3
bool initializeUseCase3();
void handleUseCase3();
void cleanupUseCase3();

//////////////////////////////////////////
//        Mode Switching Functions      //
//////////////////////////////////////////

bool switchMode(int mode)
{
  switch (mode)
  {
  case MODE_IDLE:
    Serial.println("Switched to MODE_Idle");
    return initializeIdle();
  case USECASE1:
    Serial.println("Switched to UseCase1");
    return initializeUseCase1();
  case USECASE2:
    Serial.println("Switched to UseCase2");
    return initializeUseCase2();
  case USECASE3:
    Serial.println("Switched to UseCase3");
    return initializeUseCase3();
  default:
    Serial.println("Switched to Unknown Mode");
    return 0;
  }
  return 0;
}

void cleanupMode(int mode)
{
  switch (mode)
  {
  case MODE_IDLE:
    cleanupIdle();
    break;
  case USECASE1:
    cleanupUseCase1();
    break;
  case USECASE2:
    cleanupUseCase2();
    break;
  case USECASE3:
    cleanupUseCase3();
    break;
  default:
    // Handle unknown mode if necessary
    break;
  }
}

//////////////////////////////////////////
//           Setup Function             //
//////////////////////////////////////////

void setup()
{
  // Initialize Serial for debugging
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize RemoteXY
  RemoteXY_Init();

  // Initialize hardware for the default mode (UseCase1)
  initializeIdle();
}

//////////////////////////////////////////
//           Loop Function              //
//////////////////////////////////////////

void loop()
{
  // Handle RemoteXY communication
  RemoteXY_Handler();

  // Check if the mode selection has changed
  if (RemoteXY.Modes != currentMode)
  {
    // Cleanup the previous mode
    cleanupMode(currentMode);

    // Update the current mode
    currentMode = RemoteXY.Modes;
    modeChanged = true;
    Serial.print("Mode changed to: ");
    Serial.println(currentMode);
  }

  // Handle mode change if detected
  if (modeChanged)
  {
    if (!switchMode(currentMode))
    {
      digitalWrite(LED_BUILTIN, HIGH);
      return;
    }

    digitalWrite(LED_BUILTIN, LOW);
    modeChanged = false;
  }

  // Execute the handler function corresponding to the current mode
  switch (currentMode)
  {
  case MODE_IDLE:
    handleIdle();
    break;
  case USECASE1:
    handleUseCase1();
    break;
  case USECASE2:
    handleUseCase2();
    break;
  case USECASE3:
    handleUseCase3();
    break;
  default:
    // Default action or error handling
    Serial.println("Unknown mode!");
    break;
  }

}

//////////////////////////////////////////
//        IDLE Functions                //
//////////////////////////////////////////

bool initializeIdle()
{
  Serial.println("Initializing MODE_Idle (Deep Sleep)...");
  // TODO: Add initialization code for UseCase1 here
  return 1;
}

void handleIdle()
{
  // TODO: Add handling code for UseCase1 here
}

void cleanupIdle()
{
  Serial.println("Cleaning up Idle (Wake up)...");
  // TODO: Add cleanup code for UseCase1 here
}

//////////////////////////////////////////
//        UseCase1 Functions            //
//////////////////////////////////////////

bool initializeUseCase1()
{
  // Initialize RF24 Radio with retries
  int retryCount = 0;
  const int maxRetries = 7;

  while (!mpuHandler.initMPU())
  {
    retryCount++;
    if (retryCount >= maxRetries)
    {
      return 0; // Exit if initialization fails after retries
    }
    delay(500); // Delay before retrying
  }
  // Initialize MPU6050 Handler with retries
  retryCount = 0;
  while (!radio.begin())
  {
    retryCount++;
    if (retryCount >= maxRetries)
    {
      return 0; // Exit if initialization fails after retries
    }
    delay(500); // Delay before retrying
  }

  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setChannel(108);
  radio.openWritingPipe(address);
  radio.stopListening();

  return 1;
}

void handleUseCase1()
{
  float yaw, pitch, roll;

  if (mpuHandler.readYPR(yaw, pitch, roll))
  {
    char payload = 'S'; // Default state

    if (roll > 45)
    {
      payload = 'B'; // Backward
    }
    else if (roll < -45)
    {
      payload = 'F'; // Forward
    }
    else if (pitch > 45)
    {
      payload = 'L'; // Left
    }
    else if (pitch < -45)
    {
      payload = 'R'; // Right
    }

    // Only send if the payload has changed
    if (prev != payload)
    {
      Serial.println(payload);
      prev = payload; // Update the previous state

      if (radio.write(&payload, sizeof(payload)))
      {
        Serial.println("Sent: " + String(payload));
      }
    }
  }
  delay(10); // Adjust delay for stability
}

void cleanupUseCase1()
{
  Serial.println("Cleaning up UseCase1...");
  mpuHandler.cleanupMPU();

  // Stop RF24 Radio and power it down
  radio.stopListening();
  radio.powerDown();

  // Reset state variables
  prev = 'S';

  Serial.println("UseCase1 cleanup completed.");
}

//////////////////////////////////////////
//        UseCase2 Functions            //
//////////////////////////////////////////

bool initializeUseCase2()
{
  pinMode(INPUT_PIN1, INPUT_PULLUP);
  pinMode(INPUT_PIN2, INPUT_PULLUP);
  prevButtonStateLeft = HIGH;
  prevButtonStateRight = HIGH;
  
  int retryCount = 0;
  const int maxRetries = 7;
  
  while (!mpuHandler.initMPU())
  {
    retryCount++;
    if (retryCount >= maxRetries)
    {
      return 0;
    }
    delay(500);
  }

  return 1;
}

void handleUseCase2()
{
  // TODO: Add handling code for UseCase2 here
  float yaw, pitch, roll;

  // Read current button states
  int buttonStateRight = digitalRead(INPUT_PIN1);
  int buttonStateLeft = digitalRead(INPUT_PIN2);
  
  char btnLeft = '0';
  char btnRight = '0';

  // Detect a button press (transition from HIGH to LOW)
  if (buttonStateLeft == LOW && prevButtonStateLeft == HIGH) {
    btnLeft = '1';  // Register a single click
  }
  if (buttonStateRight == LOW && prevButtonStateRight == HIGH) {
    btnRight = '1';  // Register a single click
  }

  // Update the previous button states
  prevButtonStateLeft = buttonStateLeft;
  prevButtonStateRight = buttonStateRight;

  // Read data from MPU6050
  if (mpuHandler.readYPR(yaw, pitch, roll)) {
      // Send sensor data over Bluetooth
      Serial.print(yaw);
      Serial.print(",");
      Serial.print(pitch);
      Serial.print(",");
      Serial.print(roll);
      Serial.print(",");
      Serial.print(btnLeft);
      Serial.print(",");
      Serial.println(btnRight);
  }

  delay(10); // Small delay to avoid excessive CPU usage
}

void cleanupUseCase2()
{
  Serial.println("Cleaning up UseCase2...");
  mpuHandler.cleanupMPU();

  pinMode(INPUT_PIN1, INPUT);
  pinMode(INPUT_PIN2, INPUT);
  
  Serial.println("UseCase2 cleanup completed.");
}

//////////////////////////////////////////
//        UseCase3 Functions            //
//////////////////////////////////////////

bool initializeUseCase3()
{
  Serial.println("Initializing UseCase3...");
  // TODO: Add initialization code for UseCase3 here
  return 1;
}

void handleUseCase3()
{
  // TODO: Add handling code for UseCase3 here
}

void cleanupUseCase3()
{
  Serial.println("Cleaning up UseCase3...");
  // TODO: Add cleanup code for UseCase3 here
}
