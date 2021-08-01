#ifndef MAX7219_H_
#define MAX7219_H_

#include "main.h"


void send_data (uint8_t adress, uint8_t byte);
void nastroika_max7219 (void);
void pogasit_simvol (uint8_t nomera);
void send_symbol (uint8_t poziciya, char symbol);
void send_digit (uint16_t digit) ;
void send_tochka (bool indicator );
void hod_chasov (void);
void zadat_vremya (uint8_t minut);
void send_dgit_poz (uint8_t poziciya, char cifra);

void tupo_pogasit (void);



#endif /* MAX7219_H_ */
