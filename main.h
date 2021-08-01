#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "Max7219.h"
#include "DigiPot.h"
#include "Interrupts.h"

//------------------------------------------------------------------------------
// дефайним порты
//------------------------------------------------------------------------------
#define BUZERON         PORTC |= 0x20
#define BUZEROFF        PORTC &= ~0x20

#define LEDON           PORTD |= 0x01
#define LEDOFF          PORTD &= ~0x01

#define KNOPKA1       ~ PINC  &  0x01
#define KNOPKA2       ~ PINC  &  0x02
#define KNOPKA3       ~ PINC  &  0x04
#define KNOPKA4       ~ PINC  &  0x08
#define KNOPKA5       ~ PINC  &  0x10

#define SSON            PORTB |= 0x04
#define SSOFF           PORTB &= ~0x04


#define PUNKT_MENU      6

#define GOOPWMNOGA          PORTB |= 0x02
#define STOPPWMNOGA         PORTB &= ~0x02
#define GOOPWM         TCCR1A  |= 0x80
#define STOPPWM        TCCR1A  &= ~ 0x80
#define KULERON        PORTD |= 0x08
#define KULEROFF       PORTD &= ~ 0x08
#define FILTR 4000             // фильтр значений ацп
#define FILTRTEMP 3000         // температурный фильтр
#define SWERHTOK 50000         // когда акум разряжен и включается нагрев, чтоб прибор не выключился моментально
#define TEMPERATURAVKL 55      // при каких показаниях включить кулер
#define TEMPERATURAVIKL 50     // при каких показаниях выключить кулер
#define ODIN 1             // фильтр значений ацп
#define PYAT 5             // фильтр значений ацп


//------------------------------------------------------------------------------
// дефайним порты
//------------------------------------------------------------------------------


void inicializacia ();



#endif /* MAIN_H_ */
