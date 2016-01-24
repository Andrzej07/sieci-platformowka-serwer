*****
WYMAGANE BIBLIOTEKI
*****

Środowisko Qt >4.0

sudo apt-get install libglm-dev

*****
BUDOWANIE SERWERA
*****

(opcjonalnie)qmake platformowka-serwer.pro
make clean
make
Umieścić folder Levels(ten sam co u klienta) razem z plikiem wykonywalnym
Powinno działać


*****
UŻYWANIE
*****

1. Wybieramy poziom z comboboxa.
2. Przycisk start.
3. Zwiesza się do podłączenia klientów.
4. Czekamy na zakończenie rozgrywki lub przerywamy ją przyciskiem stop.

Przycisk restart odpowiada kliknięciu (Stop -> Start), mogą się dziać dziwne rzeczy.


*****
PROTOKÓŁ
*****

Używany protokół inet: UDP 
protokół przesyłania danych klient-serwer naszej aplikacji:
brak zmiany (1. a 2.) oznacza brak blokowania; asynchroniczne zdarzenia o dowolnej kolejności

	1. uruchomienie serwera, nasłuchiwanie na porcie 1234
	2. klienci przesyłają prośbę połączenia (pojedynczy znak 'c')
	3. serwer odsyła klientom ich numery ('1' i '2')
	4. klient 2 przechodzi na port 1235
	5. serwer wysyła klientom nazwę poziomu
	6. klienci odsyłają sygnał gotowości ('r')
	7. serwer odyła sygnał startu ('g')
	8. loop
		{
		klienci wysyłają dane o wciśniętych klawiszach
		serwer odbiera dane o wciśniętych klawiszach
		serwer wysyła dane o aktualizacji pozycji (zapobiega desynchronizacji)
		klienci otrzymują dane o pozycjach i aktualizują je
		}
	   until (klient chce się rozłączyć OR
		jednen z graczy wygrał)
