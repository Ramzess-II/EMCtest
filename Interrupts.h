#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "main.h"

class Batton {                    // класс кнопки. обьявлен в н файле для того чтобк нему можно было получить доступ из других файлов
	private:
	public:
	uint8_t schet_do_yskoreniya;
	uint8_t obratniy_otschet;
	uint8_t schetchik_nazhatiy;
	bool podtverzhdenie;
	bool opystit_flag;
	bool yderzhanie;
	void obrabotka_klavish (void){          // функция класса 
		
		if (schetchik_nazhatiy > 5 && schetchik_nazhatiy < 15 && !opystit_flag)      // если счетчик нажатий больше 5 и меньше 15 а так же сброшен флаг одноразового входа 
		{
			podtverzhdenie = true;                                                   // подтверждаем нажатие
			opystit_flag = true;                                                     // сбрасываем флаг чтоб сюда больше не заходить 
		}
		if (schetchik_nazhatiy > 60 && schetchik_nazhatiy < 120 && obratniy_otschet == 0)      // для длительного нажатия если счетчик больше 100 и меньше 150 а так же обратный отсчет равен нулю 
		{
			yderzhanie = true;                                                                 // поднимаем флаг удержания
			if (schet_do_yskoreniya > 5)                                                       // если счет до ускорения больше 5
			{
				obratniy_otschet = 5;                                                          // обратный отсчет уменьшаем до 5
				schet_do_yskoreniya  = 10;                                                     // счет до ускорения делаем 10 как защита от переполнения
			}
			
			else obratniy_otschet = 10;                                                        // если счет до ускорения еще не достиг 5 см выше то обратный отсчет у нас дольше. это влияет на скорость приращения переменной
			schet_do_yskoreniya ++;                                                            // увеличиваем счет до ускорения
		}
	}
};


void klavishi (void);
void deysyviya_nad_vremenem (void);
void peremeshenie_menu (void);

#endif 
