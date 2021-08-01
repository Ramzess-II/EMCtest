#include "Max7219.h"
// еще нужно создать свои символы и загрузить их в таблицу, и паралельно выключить режим декодирования
// а так же нужно добавить мигание символов при выборе какого нибудь пункта меню 
uint8_t razr1,minut;
extern bool dvoetochie,pauza;
volatile bool bolshe_99;
extern uint8_t sekond;
extern bool zero_sekond;

void send_data (uint8_t adress, uint8_t byte){
	SSOFF;                     // чип селект в ноль
	SPDR = adress;	           // загрузили данные
	while(!(SPSR & (1<<SPIF)));//подождем пока данные передадутся
	SPDR = byte;               // загрузили следующие данные
	while(!(SPSR & (1<<SPIF)));//подождем пока данные передадутся
	SSON;                      // чип селект в единицу
}

void nastroika_max7219 (void){
	asm("nop");
	send_data ( 0x0F,  0x00);        // регистр тестирования индикатора
	send_data ( 0x0C,  0x01);        // регистр спящего режима
	send_data ( 0x0A,  0x07);        // настройка яркости свечения
	send_data ( 0x09,  0x0F);        // регистр включения декодирования данных
	send_data ( 0x0B,  0x03);        // настройка количества активных элементов
	pogasit_simvol (0b00001111);
}

void tupo_pogasit (void)
{
	send_data ( 0x01,  0x0F);
	send_data ( 0x02,  0x0F);
	send_data ( 0x03,  0x0F);
	send_data ( 0x04,  0x0F);
	
}

void pogasit_simvol (uint8_t nomera)  // тут выбираем какой именно погасить. номер бита = погашеный символ. можно не один
{
	static uint8_t c ;                // создаем переменную которая видна только в єтой функции
	c = 0x01;                         // делаем присвоение 1 адреса
	for (int i = 1; i < 5; i ++)      // создаем цикл итераций символов
	{
		if   (c & nomera)             // если у переменной с и у переменной "номер" выбранный бит совпадает
		send_data ( i,  0x0F);        // отправляем его в микросхему с номером регистра i и отправляем пустой символ
		c = c << 1;                   // сдвигаем единичку в переменной с чтоб при следующем сравнении сравнивать второй бит
	}
}

void send_symbol (uint8_t poziciya, char symbol){
	
	static char simvol;
	
	switch (symbol) {
		case 'P':
		simvol = 0x0E;
		break;
		case 'L':
		simvol = 0x0D;
		break;
		case 'H':
		simvol = 0x0C;
		break;
		case 'E':
		simvol = 0x0B;
		break;
		case '-':
		simvol = 0x0A;
		break;
	}
	send_data ( poziciya + 1,  simvol );
}

void send_digit (uint16_t digit) {         // символы у нас с лева на право нумеруються 1234, а для отображения нужно наоборот 4321 по этому такая путаница с битами
	
	static uint8_t razr0;                  // переменные локальные
	static uint8_t razr2;
	static uint8_t razr3;
	
	if (digit > 99) bolshe_99 = true;      // управление флагом
	if (digit < 100) bolshe_99 = false;
	
	razr0 = digit / 1000;          // тысячи
	razr1 = digit % 1000 / 100;    // сотни
	razr2 = digit % 100 / 10;      // дес¤тки
	razr3 = digit % 10;            // единицы
	
	if (digit > 999) send_data ( 0x01,  razr0);     // этот блок позволяет не зажигать символы если они не используются
	else pogasit_simvol (0x02);
	if (digit > 99)  send_data ( 0x02,  razr1);
	else pogasit_simvol (0x03);
	if (digit > 9)   send_data ( 0x03,  razr2);
	else pogasit_simvol (0x04);
	send_data ( 0x04,  razr3);
}

void send_tochka (bool indicator){                   // отображение точки
	
	if (indicator){                                           // если индикатор труе
		static uint8_t sravnenie;                             // создаем переменную
		if (bolshe_99) sravnenie = 0x80 | razr1;              // если число больше чем 99 то мы добавляем к значению разряда точку
		else  sravnenie = 0x8F;                               // если число меньше 99 то мы просто отправляем точку, а все символы разряда гасим
		send_data ( 0x02,  sravnenie);
	}
	if (!indicator){
		
		if (bolshe_99) 	send_data ( 0x02,  razr1);         //if (bolshe_99) sravnenie1 = 0x00 | razr1;  если число больше 99 то мы просто отправляем разряд, так как он уже содержит ноль в конце
		else  	send_data ( 0x02,  0x0F);                      // если число меньше 99 то мы отправляем единицы что соответствует выключеному состоянию и отправляем ноль для точки

	}
}

void hod_chasov (void){
	
	static uint8_t razr0;                        // переменные локальные
	static uint8_t razr1;
	static uint8_t razr2;
	static uint8_t razr3; 
	if (minut == 0 && sekond == 0 )pauza = true;      // если и секунд и минут 0 то становимся на паузу, чтоб при увеличении времени не запускался сразу отсчет
	if (!zero_sekond){                           // если опущен флаг ноль секунд, чтоб секунды не стали 59 при паузе и приращении 
		if (minut > 0){                          // работаем с временем. если время больше 0
			if (sekond == 0) {                   // а секунды на 0
				minut --;                        // отнимаем одну минуту, и добавляем секунд
				sekond = 59;
			}
		}
	}
	razr0 = minut / 10;              // дес¤тки
	razr1 = minut % 10;              // единицы
	razr2 = sekond / 10;             // дес¤тки
	razr3 = sekond % 10;             // единицы
	
	send_data ( 0x01,  razr0);
	if (dvoetochie || pauza){                       // если поднят флаг двоеточия, добавляем его в посылку для микросхемы
		send_data ( 0x02,   0x80 | razr1);
	}
	if (!dvoetochie){                      // если флаг опущен то просто отправляем символ, так как он уже содержит в конце нули
		send_data ( 0x02,    razr1);
	}
	send_data ( 0x03,  razr2);
	send_data ( 0x04,  razr3);
	
}

void send_dgit_poz (uint8_t poziciya, char cifra){
	
	send_data ( poziciya +1,  cifra);
	
}

void zadat_vremya (uint8_t vremya){
	
	minut = vremya;
}
