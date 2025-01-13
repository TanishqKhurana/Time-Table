#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display
RTC_DS3231 rtc;

struct Class {
  int startHour;
  int startMinute;
  int endHour;
  int endMinute;
  String subject;
};

// Define the class schedules for each weekday
Class schedule[5][6] = {
  {
    {9, 0, 11, 0, "PBL3 B2 5th"},  // Monday
    {9, 0, 10, 0, "English"},
    {10, 15, 11, 15, "Chemistry"},
    {11, 30, 12, 30, "Biology"},
    {13, 0, 14, 0, "PE"},
    {14, 15, 15, 15, "History"}
  },
  {
    {2, 0, 3, 0, "PBL2 IOT 4A"},  // Tuesday
    {9, 0, 10, 0, "Math"},
    {10, 15, 11, 15, "Geography"},
    {11, 30, 12, 30, "French"},
    {13, 0, 14, 0, "Art"},
    {14, 15, 15, 15, "Music"}
  },
  // Add Wednesday, Thursday, Friday schedules similarly
  {
    {8, 0, 9, 0, "Math"},  // Wednesday
    {9, 0, 10, 0, "PE"},
    {10, 15, 11, 15, "History"},
    {11, 30, 12, 30, "English"},
    {13, 0, 14, 0, "Biology"},
    {14, 15, 15, 15, "Physics"}
  },
  {
    {8, 0, 9, 0, "Art"},  // Thursday
    {9, 0, 10, 0, "Chemistry"},
    {10, 15, 11, 15, "Math"},
    {11, 30, 12, 30, "PE"},
    {13, 0, 14, 0, "Geography"},
    {14, 15, 15, 15, "French"}
  },
  {
    {8, 0, 9, 0, "Music"},  // Friday
    {9, 0, 10, 0, "Biology"},
    {10, 15, 11, 15, "Physics"},
    {11, 30, 12, 30, "Math"},
    {13, 0, 14, 0, "English"},
    {14, 15, 15, 15, "History"}
  }
};

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);

  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);  // Stop execution if RTC is not found
  }

  if (rtc.lostPower()) {
    lcd.print("RTC reset, set time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Corrected time setting
  }
}

void loop() {
  DateTime now = rtc.now();
  int dayOfWeek = now.dayOfTheWeek();  // 0 = Sunday, 6 = Saturday

  lcd.clear();
  lcd.setCursor(0, 0);
 
  printDigits(now.hour());
  lcd.print(":");
  printDigits(now.minute());
  lcd.print(" ");

  lcd.setCursor(6, 0);
  printDigits(now.day());
  lcd.print("/");
  printDigits(now.month());
  lcd.print("/");
  lcd.print(now.year());

  lcd.setCursor(0, 1);
  if(dayOfWeek==0){
    lcd.print("Sun");
  }

  else if(dayOfWeek==1){
    lcd.print("Mon");
  }

  else if(dayOfWeek==2){
    lcd.print("Tue");
  }

  else if(dayOfWeek==3){
    lcd.print("Wed");
  }

  else if(dayOfWeek==4){
    lcd.print("Thu");
  }

  else if(dayOfWeek==5){
    lcd.print("Fri");
  }
  else if(dayOfWeek==6){
    lcd.print("Sat");
  }

  lcd.setCursor(6, 1);

  if (dayOfWeek > 0 && dayOfWeek < 6) {  // Check if it's a weekday (Mon-Fri)
    lcd.print("Now: ");
    lcd.print(currentClass(dayOfWeek - 1, now.hour(), now.minute()));
  } else {
    lcd.print("No classes today");
  }

  delay(60000);  // Update the display every 2 seconds
}

void printDigits(int digits) {
  // Utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

String currentClass(int dayIndex, int hour, int minute) {
  for (Class cls : schedule[dayIndex]) {
    if ((hour > cls.startHour || (hour == cls.startHour && minute >= cls.startMinute)) &&
        (hour < cls.endHour || (hour == cls.endHour && minute < cls.endMinute))) {
      return cls.subject;
    }
  }
  return "Free";
}
