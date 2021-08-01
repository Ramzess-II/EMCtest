#include "main.h"

//------------------------------------------------------------------------------
// объявляем переменные
//------------------------------------------------------------------------------
extern uint16_t millis;
extern uint8_t sekond, minut,dop_vremya, menu ;
extern bool pauza;
extern Batton knop1,knop2,knop3,knop4,knop5;
uint8_t pod_menu;
bool podmenu, zero_sekond,ocistka,init;

int main(void)
{
	inicializacia ();      // первоначальная инициализация портов и переферии
	nastroika_max7219();   // настраиваем индикаторы


	for (;;)
	{
		//		hod_chasov();

		klavishi();                                  // опрос клавиш
			
			if (!podmenu) {                          // если мы не в подменю 
				  peremeshenie_menu ();     // включаем функцию слежения перемещения по меню       
				switch (menu){              // свитч перемещения по меню
					case 0:                 
					if (ocistka) pogasit_simvol (0x0F);      // если мы были в другом меню, то необходимо очистить индикатор от предыдущих символов
					ocistka = false;                         // сбрасываем очистку, второй раз она не нужна
					send_symbol (0x00, 'P');                 // отображаем символ параметр в нулевом элементе 
					send_dgit_poz (0x01, menu + 1);          // отображаем в каком именно пункте меню мы находимся
					
					if (knop3.podtverzhdenie ){              // отслеживаем кнопку вправо
						knop3.podtverzhdenie  = false;       // сбрасываем флаг нажатой кнопки 
						podmenu = true;                      // и переходим в подменю
						pod_menu = 0;                        // в нулевой элемент
						pauza = true;                        // ставим паузу, чтоб не начинать сразу отсчет времени
						zero_sekond = true;                  // поднимаем флаг нулевой секунды чтоб не показывать 59 сек
					}
					

					break;
					case 1:
					send_symbol (0x00, 'P');
					send_dgit_poz (0x01, menu + 1);

					
					break;
					case 2:
					send_symbol (0x00, 'P');
					send_dgit_poz (0x01, menu + 1);
					
					break;
					case 3:
					send_symbol (0x00, 'P');
					send_dgit_poz (0x01, menu + 1);
					
					break;
					case 4:
					send_symbol (0x00, 'H');
					send_dgit_poz (0x01, 1);
					
					break;
					case 5:
					send_symbol (0x00, 'H');
					send_dgit_poz (0x01, 2);
					
					break;
				}
				init = true;                                  // поднимаем флаг что нужно инициализировать переменные в подменю. 
			}
			
		if (podmenu){                                         // разветвление подменю
			
			
			if (init) {                                       // если поднят флаг инициализации
				zadat_vremya (10);                            // устанавливаем 10 минут
				init = false;                                 // сбрасывем флаг инициализации
			}
			
			switch (pod_menu) {                               // кейсы подменю
				case 0:
				hod_chasov();                                 // показываем ход часов
				deysyviya_nad_vremenem ();                    // отслеживаем прибавление или убавление времени а так же паузу
				
				if (knop1.podtverzhdenie){                    // если увидели что нажата кнопка влево
					podmenu = false;                          // покидаем подменю
					knop1.podtverzhdenie  = false;            // сбрасываем подтверждение нажатия
					ocistka = true;                           // поднимаем флаг очистить символы
				}
				
				break;
				
			}
			
		}
	}
}
void inicializacia (void) {
	
	//--инициализируем порты-- //
	DDRB = 0x2F;            // Ножки SPI на выход
	PORTB = 0x04;           // низкий уровень
	
	DDRC = 0x20;            // knopki
	PORTC = 0x1F;           // podtyashka

	DDRD = 0xF1;
	PORTD = 0x08;
	
	//--инициализируем прерывания -- //
	EICRA = 0x08;            // прерывания по спаду
	EIMSK = 0x02;	         // разрешить прерывания INT1
	
	//--инициализируем таймер 0 -- //
	TIMSK0=0x01;            // включаем прерывания по переполнению
	TCCR0B=0x05;            // делитель тактовой частоты на 64
	
	/*	//--инициализируем таймер 1-- //
	TCCR0A = 0x23;            // включаем прерывания по переполнению
	TCCR0B = 0x03;            // делитель тактовой частоты на 64*/
	
	//--инициализируем таймер 2-- //
	TIMSK2 = 0x01;            // включаем прерывания по переполнению
	TCCR2B = 0x07;            // делитель тактовой частоты на 1024
	
	//--инициализируем SPI-- //

	SPCR = ((1<<SPE)|(1<<MSTR)|(1<<SPR0) );  //Включим шину, объявим ведущим, делитель на 16
	
	sei ();
	
}
