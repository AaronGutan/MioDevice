// Видеообзоры и уроки работы с ARDUINO на YouTube-канале IOMOIO: https://www.youtube.com/channel/UCmNXABaTjX_iKH28TTJpiqA

#include <OLED_I2C.h>                               // Подключаем библиотеку OLED_I2C для работы со шрифтами и графикой
#include <HX711.h>                                  // Подключаем библиотеку HX711 для работы с АЦП hx711

HX711 scale(A1, A0);                                // Указываем в какие пины подключен АЦП hx711 (DT(DOUT)  - pin A1, SCK - pin A0)
OLED  myOLED(A4, A5, A4);                           // Определяем пины OLED дисплея: SDA - пин A4, SCL - пин A5
extern uint8_t RusFont[];                           // Подключаем русский шрифт
extern uint8_t BigNumbers[];                        // Подключаем цифровой шрифт

float myScale;                                      // Инициализируем переменную в которой будем хранить текущий вес

void setup() {
  myOLED.begin();                                   // Инициализируем дисплей, как устройство вывода
  myOLED.clrScr();                                  // Стираем все с экрана
  myOLED.setFont(RusFont);                          // Инициализируем русский шрифт
  myOLED.print("Initialization.", LEFT, 0);         // Выводим надпись "Идет калибровка"
  myOLED.update();                                  // Обновляем содержимое экрана
  
  scale.set_scale(3333.33);                          // Устанавливаем калибровочный коэффициент 
  scale.tare();                                     // Сбрасываем весы на 0
  myScale = round(scale.get_units(10));             // Запоминаем среднее значение без груза, но с учетом тары 
  
  myOLED.clrScr();                                  // Стираем все с экрана
  myOLED.setFont(BigNumbers);                       // Инициализируем шрифт BigNumbers
  myOLED.print("0.00", CENTER, 20);                 // Обнуляем значение веса на экране
  myOLED.update();                                  // Выводим на дисплей
}

void loop() {
  scale.power_up();                                 // Отключаем энергосберегающий режим hx711
  if (round(scale.get_units(10)) != myScale) {      // Условие выполняется, если округленный до граммов вес отличается от последнего
    myScale = scale.get_units(20);                  // Запоминаем текущий вес
    myOLED.clrScr();                                // Стираем все с экрана
    myOLED.setFont(BigNumbers);                     // Инициализируем шрифт вывода веса
   
    if (myScale > 0.5)                              // Если вес не ничтожно мал
      myOLED.print(String(myScale), CENTER, 20);    // то выводим его
    else                                            // иначе
      myOLED.print("0.00", CENTER, 20);             // выводим нули
   
    myOLED.update();                                // Обновляем информацию на дисплее
    myScale = round(myScale);                       // Запоминаем округленный текущий вес (для сравнения)
  }
  scale.power_down();                               // Включаем энергосберегающий режим hx711
  delay(300);                                       // Пауза на половину сек.
}
