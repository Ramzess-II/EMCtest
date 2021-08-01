#include "Interrupts.h"

volatile uint8_t dop_vremya, millis, sekond,menu;
volatile bool dvoetochie,pauza;
extern uint8_t  minut;
extern bool zero_sekond;
Batton knop1,knop2,knop3,knop4,knop5;         // класс кнопок

//------------------------------------------------------------------------------
// инициализируем прерывания
//------------------------------------------------------------------------------
ISR (TIMER0_OVF_vect)
{
	millis ++;                                // счетчик
	if (millis > 65500) millis = 0;
	
	// опрос кнопок 
	if (KNOPKA1 ) {                           // опрос клавиши
		if (knop1.schetchik_nazhatiy < 110)   // если количество нажатий меньше 140
		knop1.schetchik_nazhatiy ++;          // инкрементируем переменную счетчика нажатий
	}
	else {                                    // если кнопка не нажата
		knop1.opystit_flag = false;           // опускаем флаг единичного попадания в функцию обработчика 
		knop1.podtverzhdenie = false;         // сбрасываем подтверждение
		knop1.yderzhanie = false;             // сбрасываем флаг удержания
		knop1.schetchik_nazhatiy = 0;         // обнуляем количество нажатий
		knop1.schet_do_yskoreniya = 0;        // сбрасываем параметр счета до ускорения
	}
	
	if (KNOPKA2) {                            // агалогично для всех кнопок
		if (knop2.schetchik_nazhatiy < 110)
		knop2.schetchik_nazhatiy ++;
	}
	else {
		knop2.opystit_flag = false;
		knop2.podtverzhdenie = false;
		knop2.yderzhanie = false;
		knop2.schetchik_nazhatiy = 0;
		knop2.schet_do_yskoreniya = 0;
	}
	
	if (KNOPKA3)  {
		if (knop3.schetchik_nazhatiy < 110)
		knop3.schetchik_nazhatiy ++;
	}
	else {
		knop3.opystit_flag = false;
		knop3.podtverzhdenie = false;
		knop3.yderzhanie = false;
		knop3.schetchik_nazhatiy = 0;
		knop3.schet_do_yskoreniya = 0;
	}
	
	if (KNOPKA4) {
		if (knop4.schetchik_nazhatiy < 110)
		knop4.schetchik_nazhatiy ++;
	}
	else {
		knop4.opystit_flag = false;
		knop4.podtverzhdenie = false;
		knop4.yderzhanie = false;
		knop4.schetchik_nazhatiy = 0;
		knop4.schet_do_yskoreniya = 0;
	}
	if (KNOPKA5) {
		if (knop5.schetchik_nazhatiy < 110)
		knop5.schetchik_nazhatiy ++;
	}
	else {
		knop5.opystit_flag = false;
		knop5.podtverzhdenie = false;
		knop5.yderzhanie = false;
		knop5.schetchik_nazhatiy = 0;
		knop5.schet_do_yskoreniya = 0;
	}
	
	
	if (knop1.obratniy_otschet > 0) knop1.obratniy_otschet --;                 // уменьшаем обратный отсчет если он больше нуля. это задержка для ускоренного приращение переменной
	if (knop2.obratniy_otschet > 0) knop2.obratniy_otschet --;
	if (knop3.obratniy_otschet > 0) knop3.obratniy_otschet --;
	if (knop4.obratniy_otschet > 0) knop4.obratniy_otschet --;
	if (knop5.obratniy_otschet > 0) knop5.obratniy_otschet --;
}

ISR (TIMER2_OVF_vect)              // прерывание для отсчета времени
{
	if (!pauza){                   // если мы не на паузе то считаем
		dop_vremya ++;
		if (dop_vremya == 62){     // это параметр для четкого отображения секунд. (16МГц / 1024) / 250 это таймер и получится для одной секунды нужно 62 переполнения таймера
			dop_vremya = 0;        // обнуляем доп счетчик
			if (sekond > 0){       // если секунд больше 0
				sekond --;         // декрементируем
				dvoetochie = !dvoetochie;  // показываем - не показываем двоеточие
			}
		}
	}
	if (pauza) dvoetochie = true;     // на паузе всегда показывать двоеточие
	if (minut == 0) dvoetochie = true; // по окончанию счета  показывать двоеточие
	if (zero_sekond) sekond = 0;      // сброс секунд в ноль для красивого отображения
	
	TCNT2 = 0x05;                     // записываем в таймер 5 для коректного счета времени 
}


void klavishi (void){                 // обработка клавиш нажатие, удержание через класс, очень удобно
	

	knop1.obrabotka_klavish();
	knop2.obrabotka_klavish();
    knop3.obrabotka_klavish();
	knop4.obrabotka_klavish();
	knop5.obrabotka_klavish();
	
}



void deysyviya_nad_vremenem (void){     // функция для прибавления - убавления времени
	
	if (knop4.podtverzhdenie || knop4.yderzhanie ){     // если произошло нажатие или удержание кнопки вниз
		if (pauza) zero_sekond = true;                  // если мы стоим на паузе и добавляем минуты сбросить секунды в ноль для красоты
		if (minut > 1 )minut --;                        // если минут больше 1 то отнимаем одну
		knop4.podtverzhdenie  = false;                  // сбрасываем нажатие кнопки
		knop4.yderzhanie = false;                       // сбрасываем удержание кнопки
	}
	
	if (knop2.podtverzhdenie || knop2.yderzhanie  ){     // аналогично для кнопки вверх
		if (minut < 30) minut ++;
		if (minut == 30  ) sekond = 0;
		if (pauza) zero_sekond = true;
		knop2.podtverzhdenie  = false;
		knop2.yderzhanie = false;
	}
	
	if (knop5.podtverzhdenie){                          // кнопка старт - стоп выполнения программы
		pauza = !pauza;                                 // ставим или снимаем паузу
		zero_sekond = false;                            // запрещаем сбрасывать секунды в ноль
		knop5.podtverzhdenie  = false;                  // сбрасываем подтверждение нажатия кнопки
	}
}

void peremeshenie_menu (void){                         // функция перемещения по меню
	
	if (knop2.podtverzhdenie ){                        // если кнопка вверх нажата
		if (menu < PUNKT_MENU)  menu ++;               // и переменная меню меньше пунктов меню плюсуем ее
		if (menu == PUNKT_MENU)  menu = 0;             // если переменная меню равна пунктам меню сьрасываем меню в ноль
		knop2.podtverzhdenie  = false;                 // сбрасываем подтверждение чтоб второй раз сюда не заходить 
	}
	if (knop4.podtverzhdenie ){                        // аналогично для кнопки вниз 
		if (menu == 0) menu = PUNKT_MENU;
		if (menu > 0) menu --;
		knop4.podtverzhdenie  = false;
	}
}
