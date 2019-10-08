/*
 * 8 bites p�rhuzamos LCD/TOUCH GPIO driver STM32F1-re
 * 5 vez�rl�l�b (CS, RS, WR, RD, RST) + 8 adatl�b + h�tt�rvil�git�s vez�rl�s

 * Figyelem: mivel azonos l�bakon van az Lcd �s a Touchscreen,
 * ez�rt ezek ki kell z�rni az Lcd �s a Touchscreen egyidej� haszn�lat�t!
 * T�bbsz�las/megszakit�sos k�rnyezetben igy gondoskodni kell az �sszeakad�sok megel�z�s�r�l!
 */

//=============================================================================
// Lcd vez�rl� l�bak hozz�rendel�se (A..M, 0..15)
#define LCD_CS            X, 0
#define LCD_RS            X, 0
#define LCD_WR            X, 0
#define LCD_RD            X, 0
#define LCD_RST           X, 0

// Lcd adat l�bak hozz�rendel�se (A..M, 0..15)
#define LCD_D0            X, 0
#define LCD_D1            X, 0
#define LCD_D2            X, 0
#define LCD_D3            X, 0
#define LCD_D4            X, 0
#define LCD_D5            X, 0
#define LCD_D6            X, 0
#define LCD_D7            X, 0

// H�tt�rvil�git�s vez�rl�s (opcion�lis, l�b hozz�rendel�s �s aktiv �llapot)
#define LCD_BL            X, 0
#define LCD_BLON          0

/*-----------------------------------------------------------------------------
Touch I/O l�bak �s A/D csatorn�k
A kijelz�n bel�l a k�vetkez� l�bak vannak p�rhuzamositva
 - TS_XM <- LCD_RS
 - TS_XP <- LCD_D6
 - TS_YM <- LCD_D7
 - TS_YP <- LCD_WR */

/* ADC konverter sz�ma (�rt�ke lehet 1, 2, 3, vagy 0 ha nem haszn�ljuk)
   - 0: analog touchscreen driver nem lesz haszn�lva
   - 1..3: a haszn�lni kiv�nt A/D konverter sz�ma
*/
#define TS_ADC            0

// Itt kell megadni, hogy melyik csatorn�t kiv�lasztva lehet az adott l�bat az AD bemenet�re kapcsolni
#define TS_XM_ADCCH       0
#define TS_YP_ADCCH       0

/* nsec nagys�grend� v�rakoz�s az LCD ir�si �s az olvas�si impulzusn�l �s a touchscreen AD �talakiton�l
   - kezd� �rt�knek �rdemes 10, 20 illetve 500-bol elindulni, azt�n lehet cs�kkenteni a sebess�g n�vel�se �rdek�ben
     (az �rt�kek f�ggnek a processzor orajel�t�l �s az LCD kijelz� sebess�g�t�l is)
*/
#define LCD_WRITE_DELAY  10
#define LCD_READ_DELAY   20
#define TS_AD_DELAY     500

/*=============================================================================
I/O csoport optimaliz�ci�, hogy ne bitenk�nt t�rt�njenek a GPIO m�veletek:
Megj: ha az adat l�bakat egy porton bel�l emelked� sorrendben defini�ljuk
      automatikusan optimaliz�lva fognak t�rt�nni a GPIO m�veletek akkor is, ha
      itt nem defini�ljuk az optimaliz�lt m�k�d�shez sz�ks�ges elj�r�sokat
A lenti p�lda a k�vetkez� l�bakhoz optimaliz�l:
      LCD_D0<-D14, LCD_D1<-D15, LCD_D2<-D0, LCD_D3<-D1
      LCD_D4<-E7,  LCD_D5<-E8,  LCD_D6<-E9, LCD_D7<-E10 */
#if 0
// datapins setting to output (data direction: STM32 -> LCD)
#define LCD_DIRWRITE { /* D0..D1, D14..D15, E7..E10 <- 0x3 */ \
GPIOD->CRH = (GPIOD->CRH & ~0xFF000000) | 0x33000000; \
GPIOD->CRL = (GPIOD->CRH & ~0x000000FF) | 0x00000033; \
GPIOE->CRL = (GPIOE->CRL & ~0xF0000000) | 0x30000000; \
GPIOE->CRH = (GPIOE->CRH & ~0x00000FFF) | 0x00000333; }
// datapins setting to input (data direction: STM32 <- LCD)
#define LCD_DIRREAD { /* D0..D1, D14..D15, E7..E10 <- 0x4 */ \
GPIOD->CRH = (GPIOD->CRH & ~0xFF000000) | 0x44000000; \
GPIOD->CRL = (GPIOD->CRH & ~0x000000FF) | 0x00000044; \
GPIOE->CRL = (GPIOE->CRL & ~0xF0000000) | 0x40000000; \
GPIOE->CRH = (GPIOE->CRH & ~0x00000FFF) | 0x00000444; }
// datapins write, STM32 -> LCD (write I/O pins from dt data)
#define LCD_WRITE(dt) { /* D14..15 <- dt0..1, D0..1 <- dt2..3, E7..10 <- dt4..7 */ \
GPIOD->ODR = (GPIOD->ODR & ~0b1100000000000011) | (((dt & 0b00000011) << 14) | ((dt & 0b00001100) >> 2)); \
GPIOE->ODR = (GPIOE->ODR & ~0b0000011110000000) | ((dt & 0b11110000) << 3); }
// datapins read, STM32 <- LCD (read from I/O pins and store to dt data)
#define LCD_READ(dt) { /* dt0..1 <- D14..15, dt2..3 <- D0..1, dt4..7 <- E7..10 */ \
dt = ((GPIOD->IDR & 0b1100000000000000) >> 14) | ((GPIOD->IDR & 0b0000000000000011) << 2) | \
     ((GPIOE->IDR & 0b0000011110000000) >> 3); }
#endif
