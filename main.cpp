#include <iostream>
#include <fstream>

// Definicje kolorów dla ANSI escape codes
#define RED_TEXT "\033[1;31m"
#define GREEN_TEXT "\033[1;32m"
#define BLUE_TEXT "\033[34m"
#define MAGENTA_TEXT "\033[35m"
#define RESET_COLOR "\033[0m"


void wczytywanie_danych(int rok[][1000], int miesiac[][1000], int dzien[][1000], float open[][1000], float high[][1000], float low[][1000], float close[][1000], float volume[][1000], int& rozszerzenie, int& indeks, float zaok_open[7][1000], float zaok_high[7][1000], float zaok_low[7][1000], float zaok_close[7][1000]) {
	char wiersz[300];
	int indeks_przecinka[5], indeks_kropki[5];
	int korekta_kropki = 0;

	std::ifstream plik("intc_us_data.csv");

	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 7; j++) {
			rok[j][i] = 0;
			miesiac[j][i] = 0;
			dzien[j][i] = 0;
			open[j][i] = 0;
			high[j][i] = 0;
			low[j][i] = 0;
			close[j][i] = 0;
			volume[j][i] = 0;
		}
	}


	if (plik.is_open())
	{
		while (plik.getline(wiersz, 300)) {
			int kolumna = 10;
			int licznik_przecinkow = 0;
			int licznik_kropek = 0;
			int miejsca_przed_kropka, miejsca_za_kropka;
			int indeks_n = -1;

			int korekta_kropki = 0;

			indeks_kropki[4] = 500;
			indeks_kropki[3] = 499;
			indeks_kropki[2] = 498;
			indeks_kropki[1] = 497;
			indeks_kropki[0] = 496;

			for (int i = 0; i < 5; i++) {
				indeks_przecinka[i] = 0;
			}

			indeks += 1;

			if (wiersz[0] != 'D') {

				if (indeks == 1000) {
					rozszerzenie += 1;
					indeks = 0;
				}

				rok[rozszerzenie][indeks] = ((wiersz[0] - '0') * 1000) + ((wiersz[1] - '0') * 100) + ((wiersz[2] - '0') * 10) + (wiersz[3] - '0');
				miesiac[rozszerzenie][indeks] = (wiersz[5] - '0') * 10 + (wiersz[6] - '0');
				dzien[rozszerzenie][indeks] = (wiersz[8] - '0') * 10 + (wiersz[9] - '0');

				//wyszukiwanie miejsc w ktorych sa , i .

				while (true) {
					if (wiersz[kolumna] == ',') {
						indeks_przecinka[licznik_przecinkow] = kolumna;
						licznik_przecinkow += 1;
					}

					else if (wiersz[kolumna] == '.') {
						indeks_kropki[licznik_kropek] = kolumna;
						licznik_kropek += 1;
					}

					else if (!isdigit(wiersz[kolumna])) {
						indeks_n = kolumna;
						break;
					}

					kolumna += 1;
				}


				//przypisywanie wartosci dla open

				if (indeks_przecinka[1] > indeks_kropki[0]) {

					miejsca_przed_kropka = indeks_kropki[0] - 11;

					for (int i = 11; i < indeks_kropki[0]; i++) {
						miejsca_przed_kropka -= 1;
						open[rozszerzenie][indeks] = open[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
					}

					miejsca_za_kropka = 0;

					for (int i = indeks_kropki[0] + 1; i < indeks_przecinka[1]; i++) {
						miejsca_za_kropka += 1;
						open[rozszerzenie][indeks] = open[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(0.1, miejsca_za_kropka));
					}
				}
				else {
					miejsca_przed_kropka = indeks_przecinka[1] - 11;

					for (int i = 11; i < indeks_przecinka[1]; i++) {
						miejsca_przed_kropka -= 1;
						open[rozszerzenie][indeks] = open[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
					}

					korekta_kropki += 1;
				}


				//przypisywanie wartosci dla high

				if (indeks_przecinka[2] > indeks_kropki[1 - korekta_kropki]) {

					miejsca_przed_kropka = indeks_kropki[1 - korekta_kropki] - (indeks_przecinka[1] + 1);

					for (int i = (indeks_przecinka[1] + 1); i < indeks_kropki[1 - korekta_kropki]; i++) {
						miejsca_przed_kropka -= 1;
						high[rozszerzenie][indeks] = high[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
					}

					miejsca_za_kropka = 0;

					for (int i = indeks_kropki[1 - korekta_kropki] + 1; i < indeks_przecinka[2]; i++) {
						miejsca_za_kropka += 1;
						high[rozszerzenie][indeks] = high[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(0.1, miejsca_za_kropka));
					}
				}
				else {

					miejsca_przed_kropka = indeks_przecinka[2] - (indeks_przecinka[1] + 1);

					for (int i = (indeks_przecinka[1] + 1); i < indeks_przecinka[2]; i++) {
						miejsca_przed_kropka -= 1;
						high[rozszerzenie][indeks] = high[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
					}

					korekta_kropki += 1;
				}


				//przypisywanie wartosci dla Low

				if (indeks_przecinka[3] > indeks_kropki[2 - korekta_kropki]) {

					miejsca_przed_kropka = indeks_kropki[2 - korekta_kropki] - (indeks_przecinka[2] + 1);

					for (int i = (indeks_przecinka[2] + 1); i < indeks_kropki[2 - korekta_kropki]; i++) {
						miejsca_przed_kropka -= 1;
						low[rozszerzenie][indeks] = low[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
					}

					miejsca_za_kropka = 0;

					for (int i = indeks_kropki[2 - korekta_kropki] + 1; i < indeks_przecinka[3]; i++) {
						miejsca_za_kropka += 1;
						low[rozszerzenie][indeks] = low[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(0.1, miejsca_za_kropka));
					}
				}
				else {
					miejsca_przed_kropka = indeks_przecinka[3] - (indeks_przecinka[2] + 1);

					for (int i = (indeks_przecinka[2] + 1); i < indeks_przecinka[3]; i++) {
						miejsca_przed_kropka -= 1;
						low[rozszerzenie][indeks] = low[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
					}

					korekta_kropki += 1;
				}


				//przypisywanie wartosci dla close

				if (indeks_przecinka[4] != 0) {
					if (indeks_przecinka[4] > indeks_kropki[3 - korekta_kropki]) {

						miejsca_przed_kropka = indeks_kropki[3 - korekta_kropki] - (indeks_przecinka[3] + 1);

						for (int i = (indeks_przecinka[3] + 1); i < indeks_kropki[3 - korekta_kropki]; i++) {
							miejsca_przed_kropka -= 1;
							close[rozszerzenie][indeks] = close[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
						}

						miejsca_za_kropka = 0;

						for (int i = indeks_kropki[3 - korekta_kropki] + 1; i < indeks_przecinka[4]; i++) {
							miejsca_za_kropka += 1;
							close[rozszerzenie][indeks] = close[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(0.1, miejsca_za_kropka));
						}
					}
					else {
						miejsca_przed_kropka = indeks_przecinka[4] - (indeks_przecinka[3] + 1);

						for (int i = (indeks_przecinka[3] + 1); i < indeks_przecinka[4]; i++) {
							miejsca_przed_kropka -= 1;
							close[rozszerzenie][indeks] = close[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
						}

						korekta_kropki += 1;
					}
				}

				else {

					if (indeks_n > indeks_kropki[3 - korekta_kropki]) {

						miejsca_przed_kropka = indeks_kropki[3 - korekta_kropki] - (indeks_przecinka[3] + 1);

						for (int i = (indeks_przecinka[3] + 1); i < indeks_kropki[3 - korekta_kropki]; i++) {
							miejsca_przed_kropka -= 1;
							close[rozszerzenie][indeks] = close[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
						}

						miejsca_za_kropka = 0;

						for (int i = indeks_kropki[3 - korekta_kropki] + 1; i < indeks_n; i++) {
							miejsca_za_kropka += 1;
							close[rozszerzenie][indeks] = close[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(0.1, miejsca_za_kropka));
						}
					}
					else {
						miejsca_przed_kropka = indeks_n - (indeks_przecinka[3] + 1);

						for (int i = (indeks_przecinka[3] + 1); i < indeks_n; i++) {
							miejsca_przed_kropka -= 1;
							close[rozszerzenie][indeks] = close[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
						}

						korekta_kropki += 1;
					}
				}


				//przypisywanie wartosci dla volume

				if (indeks_przecinka[4] != 0) {

					if (indeks_n > indeks_kropki[4 - korekta_kropki]) {

						miejsca_przed_kropka = indeks_kropki[4 - korekta_kropki] - (indeks_przecinka[4] + 1);

						for (int i = (indeks_przecinka[4] + 1); i < indeks_kropki[4 - korekta_kropki]; i++) {
							miejsca_przed_kropka -= 1;
							volume[rozszerzenie][indeks] = volume[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
						}

						miejsca_za_kropka = 0;

						for (int i = indeks_kropki[4 - korekta_kropki] + 1; i < indeks_n; i++) {
							miejsca_za_kropka += 1;
							volume[rozszerzenie][indeks] = volume[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(0.1, miejsca_za_kropka));
						}
					}
					else {
						miejsca_przed_kropka = indeks_n - (indeks_przecinka[4] + 1);

						for (int i = (indeks_przecinka[4] + 1); i < indeks_n; i++) {
							miejsca_przed_kropka -= 1;
							volume[rozszerzenie][indeks] = volume[rozszerzenie][indeks] + ((wiersz[i] - '0') * pow(10, miejsca_przed_kropka));
						}
					}
				}


				zaok_open[rozszerzenie][indeks] = round(open[rozszerzenie][indeks] * 10) / 10;
				zaok_high[rozszerzenie][indeks] = round(high[rozszerzenie][indeks] * 10) / 10;
				zaok_low[rozszerzenie][indeks] = round(low[rozszerzenie][indeks] * 10) / 10;
				zaok_close[rozszerzenie][indeks] = round(close[rozszerzenie][indeks] * 10) / 10;
			}
		}
	}
	else std::cout << "nie udalo sie otworzyc pliku";

	plik.close();
}

void rysowanie_wykresu(char obiekty[20][50][100], bool kolory[200][200], bool zielony[200][200], int wysokosc, int szerokosc, char os_wartosci[10][100], int nr_swiecy, float podzialka, float najwiekszy_high, float najmniejszy_low, bool kolory_os_wartosci[100], bool niebieski_wartosci[100], int dzien_os_czasu[2], int miesiac_os_czasu[2], int rok_os_czasu[2], int odczyty) {

	std::ofstream zapis("chart.txt");

	float napis_wartosci = najmniejszy_low + podzialka * wysokosc;

	for (int y = 0; y <= wysokosc + 1; y++) {

		napis_wartosci = round(napis_wartosci * 10) / 10;

		int proba_kontrolna = round(napis_wartosci * 10);

		if (kolory_os_wartosci[y] == true) {
			std::cout << char(186);

			if (napis_wartosci < 10) {
				std::cout << "     ";
			}
			else if (napis_wartosci < 100) {
				std::cout << "    ";
			}
			else if (napis_wartosci < 1000) {
				std::cout << "   ";
			}
			else if (napis_wartosci < 10000) {
				std::cout << "  ";
			}
			else if (napis_wartosci < 100000) {
				std::cout << " ";
			}

			if (niebieski_wartosci[y] == true) {
				std::cout << BLUE_TEXT << napis_wartosci << RESET_COLOR;
				if (proba_kontrolna % 10 == 0) {
					std::cout << BLUE_TEXT << ".0" << RESET_COLOR;
				}
			}
			else {
				std::cout << MAGENTA_TEXT << napis_wartosci << RESET_COLOR;
				if (proba_kontrolna % 10 == 0) {
					std::cout << MAGENTA_TEXT << ".0" << RESET_COLOR;
				}
			}

			napis_wartosci -= podzialka;
		}
		else if (y > 0 && y <= wysokosc) {
			std::cout << char(186);

			if (napis_wartosci < 10) {
				std::cout << "     ";
			}
			else if (napis_wartosci < 100) {
				std::cout << "    ";
			}
			else if (napis_wartosci < 1000) {
				std::cout << "   ";
			}
			else if (napis_wartosci < 10000) {
				std::cout << "  ";
			}
			else if (napis_wartosci < 100000) {
				std::cout << " ";
			}

			std::cout << napis_wartosci;

			if (proba_kontrolna % 10 == 0) {
				std::cout << ".0";
			}

			napis_wartosci -= podzialka;
		}
		else {
			for (int x = 0; x <= 8; x++) {
				std::cout << os_wartosci[x][y];
			}
		}

		for (int x = 0; x <= szerokosc + 1; x++) {
			if (kolory[x][y] == true) {

				if (nr_swiecy == x) {
					if (x < 50) {
						if (zielony[x][y] == true) {
							std::cout << BLUE_TEXT << obiekty[0][x][y] << RESET_COLOR;

							if (obiekty[0][x][y] == char(475)) {
								zapis << "O";
							}
							else {
								zapis << "|";
							}
						}
						else {
							std::cout << MAGENTA_TEXT << obiekty[0][x][y] << RESET_COLOR;

							if (obiekty[0][x][y] == char(475)) {
								zapis << "#";
							}
							else {
								zapis << "|";
							}
						}
					}

					else if (x < 100) {
						if (zielony[x][y] == true) {
							std::cout << BLUE_TEXT << obiekty[1][x - 50][y] << RESET_COLOR;

							if (obiekty[1][x - 50][y] == char(475)) {
								zapis << "O";
							}
							else {
								zapis << "|";
							}
						}
						else {
							std::cout << MAGENTA_TEXT << obiekty[1][x - 50][y] << RESET_COLOR;

							if (obiekty[1][x - 50][y] == char(475)) {
								zapis << "#";
							}
							else {
								zapis << "|";
							}
						}
					}

					else if (x < 150) {
						if (zielony[x][y] == true) {
							std::cout << BLUE_TEXT << obiekty[2][x - 100][y] << RESET_COLOR;

							if (obiekty[2][x - 100][y] == char(475)) {
								zapis << "O";
							}
							else {
								zapis << "|";
							}
						}
						else {
							std::cout << MAGENTA_TEXT << obiekty[2][x - 100][y] << RESET_COLOR;

							if (obiekty[2][x - 100][y] == char(475)) {
								zapis << "#";
							}
							else {
								zapis << "|";
							}
						}
					}
				}

				else if (x < 50) {
					if (zielony[x][y] == true) {
						std::cout << GREEN_TEXT << obiekty[0][x][y] << RESET_COLOR;

						if (obiekty[0][x][y] == char(475)) {
							zapis << "O";
						}
						else {
							zapis << "|";
						}
					}
					else {
						std::cout << RED_TEXT << obiekty[0][x][y] << RESET_COLOR;

						if (obiekty[0][x][y] == char(475)) {
							zapis << "#";
						}
						else {
							zapis << "|";
						}
					}
				}

				else if (x < 100) {
					if (zielony[x][y] == true) {
						std::cout << GREEN_TEXT << obiekty[1][x - 50][y] << RESET_COLOR;

						if (obiekty[1][x - 50][y] == char(475)) {
							zapis << "O";
						}
						else {
							zapis << "|";
						}
					}
					else {
						std::cout << RED_TEXT << obiekty[1][x - 50][y] << RESET_COLOR;

						if (obiekty[1][x - 50][y] == char(475)) {
							zapis << "#";
						}
						else {
							zapis << "|";
						}
					}
				}

				else if (x < 150) {
					if (zielony[x][y] == true) {
						std::cout << GREEN_TEXT << obiekty[2][x - 100][y] << RESET_COLOR;

						if (obiekty[2][x - 100][y] == char(475)) {
							zapis << "O";
						}
						else {
							zapis << "|";
						}
					}
					else {
						std::cout << RED_TEXT << obiekty[2][x - 100][y] << RESET_COLOR;

						if (obiekty[2][x - 100][y] == char(475)) {
							zapis << "#";
						}
						else {
							zapis << "|";
						}
					}
				}
			}

			else {
				if (x < 50) {
					std::cout << obiekty[0][x][y];
					zapis << ' ';
				}

				else if (x < 100) {
					std::cout << obiekty[1][x - 50][y];
					zapis << ' ';
				}

				else if (x < 150) {
					std::cout << obiekty[2][x - 100][y];
					zapis << ' ';
				}
			}
		}
		std::cout << std::endl;
		zapis << std::endl;
	}

	std::cout << char(201);
	for (int i = 1; i <= szerokosc + 9; i++) {
		std::cout << char(205);
	}
	std::cout << char(187) << std::endl;

	std::cout << char(186);
	for (int i = 1; i < nr_swiecy; i++) {
		std::cout << ' ';
	}
	std::cout << rok_os_czasu[0] << '-';
	if (miesiac_os_czasu[0] < 10) {
		std::cout << '0';
	}
	std::cout << miesiac_os_czasu[0] << '-';
	if (dzien_os_czasu[0] < 10) {
		std::cout << '0';
	}
	std::cout << dzien_os_czasu[0];
	for (int i = szerokosc + 9; i > 9 + nr_swiecy; i--) {
		std::cout << ' ';
	}
	std::cout << char(186) << std::endl;

	if (odczyty > 1) {
		std::cout << char(186);
		for (int i = 1; i < nr_swiecy; i++) {
			std::cout << ' ';
		}
		std::cout << rok_os_czasu[1] << '-';
		if (miesiac_os_czasu[1] < 10) {
			std::cout << '0';
		}
		std::cout << miesiac_os_czasu[1] << '-';
		if (dzien_os_czasu[1] < 10) {
			std::cout << '0';
		}
		std::cout << dzien_os_czasu[1];
		for (int i = szerokosc + 9; i > 9 + nr_swiecy; i--) {
			std::cout << ' ';
		}
		std::cout << char(186) << std::endl;
	}

	std::cout << char(200);
	for (int i = 1; i <= szerokosc + 9; i++) {
		std::cout << char(205);
	}
	std::cout << char(188);




	zapis.close();
}

void spacje(int odstep) {
	for (int i = 0; i < odstep; i++) {
		std::cout << " ";
	}
}

void menu(char& ruch_uzytkownika) {

	std::cout << char(201);
	for (int i = 0; i < 50; i++) {
		std::cout << char(205);
	}

	std::cout << char(187) << std::endl;
	std::cout << char(186) << "  By wczytac dane i wygenerowac wykres wpisz 'g'  " << char(186) << std::endl;
	std::cout << char(186) << "  By zakonczyc dzialanie programu wcisnij 'q'     " << char(186) << std::endl;
	std::cout << char(186) << "  By ponownie przywolac menu wcisnij 'm'          " << char(186) << std::endl;
	std::cout << char(200);

	for (int i = 0; i < 50; i++) {
		std::cout << char(205);
	}
	std::cout << char(188) << std::endl;

	std::cin >> ruch_uzytkownika;

	while (true) {
		if (ruch_uzytkownika == 'm' || ruch_uzytkownika == 'M') {
			std::cout << "Juz jestes w menu, wybierz inna opcje. " << std::endl;
			std::cin >> ruch_uzytkownika;
		}
		else if (ruch_uzytkownika != 'q' && ruch_uzytkownika != 'Q' && ruch_uzytkownika != 'g' && ruch_uzytkownika != 'G') {
			std::cout << "Wybierz inna opcje. " << std::endl;
			std::cin >> ruch_uzytkownika;
		}
		else {
			break;
		}
	}

	system("cls");
}

void tworzenie_swiecy(float& robocze_low, float& robocze_high, float& robocze_close, float& robocze_open, int wysokosc, float podzialka, int nr_indeksu, char obiekty[20][50][100], bool kolory[200][200], bool zielony[200][200], int nr_swiecy, bool kolory_os_wartosci[100], bool niebieski_wartosci[100], bool czerwona) {


	int Y = wysokosc;
	int ostatni_Y_ciala = -1;
	bool koniec = false;
	bool poczatek = true;

	while (koniec == false) {

		if (robocze_low >= 0 && robocze_low < podzialka) {

			while (koniec == false) {

				if ((robocze_open >= 0 && robocze_open < podzialka && czerwona == false) || (robocze_close >= 0 && robocze_close < podzialka && czerwona == true)) {

					while (koniec == false) {

						ostatni_Y_ciala = Y;

						if ((robocze_close >= 0 && robocze_close < podzialka && czerwona == false) || (robocze_open >= 0 && robocze_open < podzialka && czerwona == true)) {

							while (koniec == false) {

								if (robocze_high >= 0 && robocze_high < podzialka) {

									if (nr_indeksu < 50) {
										obiekty[0][nr_indeksu][Y] = char(435);
									}
									else if (nr_indeksu < 100) {
										obiekty[1][nr_indeksu - 50][Y] = char(435);
									}
									else if (nr_indeksu < 150) {
										obiekty[2][nr_indeksu - 100][Y] = char(435);
									}

									if (nr_swiecy == nr_indeksu) {
										kolory_os_wartosci[Y] = true;
										if (czerwona == false) {
											niebieski_wartosci[Y] = true;
										}
										else {
											niebieski_wartosci[Y] = false;
										}
									}

									kolory[nr_indeksu][Y] = true;
									if (czerwona == false) {
										zielony[nr_indeksu][Y] = true;
									}
									else {
										zielony[nr_indeksu][Y] = false;
									}

									koniec = true;
								}

								if (koniec == false) {

									if (nr_indeksu < 50) {
										obiekty[0][nr_indeksu][Y] = char(435);
									}
									else if (nr_indeksu < 100) {
										obiekty[1][nr_indeksu - 50][Y] = char(435);
									}
									else if (nr_indeksu < 150) {
										obiekty[2][nr_indeksu - 100][Y] = char(435);
									}

									if (nr_swiecy == nr_indeksu) {
										kolory_os_wartosci[Y] = true;
										if (czerwona == false) {
											niebieski_wartosci[Y] = true;
										}
										else {
											niebieski_wartosci[Y] = false;
										}
									}

									kolory[nr_indeksu][Y] = true;
									if (czerwona == false) {
										zielony[nr_indeksu][Y] = true;
									}
									else {
										zielony[nr_indeksu][Y] = false;
									}

									robocze_low = robocze_low - podzialka;
									robocze_close = robocze_close - podzialka;
									robocze_open = robocze_open - podzialka;
									robocze_high = robocze_high - podzialka;

									robocze_low = round(robocze_low * 10) / 10;
									robocze_close = round(robocze_close * 10) / 10;
									robocze_open = round(robocze_open * 10) / 10;
									robocze_high = round(robocze_high * 10) / 10;
									Y -= 1;
								}
							}
						}

						if (koniec == false) {

							if (nr_indeksu < 50) {
								obiekty[0][nr_indeksu][Y] = char(475);
							}
							else if (nr_indeksu < 100) {
								obiekty[1][nr_indeksu - 50][Y] = char(475);
							}
							else if (nr_indeksu < 150) {
								obiekty[2][nr_indeksu - 100][Y] = char(475);
							}

							if (nr_swiecy == nr_indeksu) {
								kolory_os_wartosci[Y] = true;
								if (czerwona == false) {
									niebieski_wartosci[Y] = true;
								}
								else {
									niebieski_wartosci[Y] = false;
								}
							}

							kolory[nr_indeksu][Y] = true;
							if (czerwona == false) {
								zielony[nr_indeksu][Y] = true;
							}
							else {
								zielony[nr_indeksu][Y] = false;
							}

							robocze_low = robocze_low - podzialka;
							robocze_close = robocze_close - podzialka;
							robocze_open = robocze_open - podzialka;
							robocze_high = robocze_high - podzialka;

							robocze_low = round(robocze_low * 10) / 10;
							robocze_close = round(robocze_close * 10) / 10;
							robocze_open = round(robocze_open * 10) / 10;
							robocze_high = round(robocze_high * 10) / 10;

							Y -= 1;
						}

						if (koniec == true && ostatni_Y_ciala != -1) {

							if (nr_indeksu < 50) {
								obiekty[0][nr_indeksu][ostatni_Y_ciala] = char(475);
							}
							else if (nr_indeksu < 100) {
								obiekty[1][nr_indeksu - 50][ostatni_Y_ciala] = char(475);
							}
							else if (nr_indeksu < 150) {
								obiekty[2][nr_indeksu - 100][ostatni_Y_ciala] = char(475);
							}
						}
					}
				}

				if (koniec == false) {

					if (nr_indeksu < 50) {
						obiekty[0][nr_indeksu][Y] = char(435);
					}
					else if (nr_indeksu < 100) {
						obiekty[1][nr_indeksu - 50][Y] = char(435);
					}
					else if (nr_indeksu < 150) {
						obiekty[2][nr_indeksu - 100][Y] = char(435);
					}

					if (nr_swiecy == nr_indeksu) {
						kolory_os_wartosci[Y] = true;
						if (czerwona == false) {
							niebieski_wartosci[Y] = true;
						}
						else {
							niebieski_wartosci[Y] = false;
						}
					}

					kolory[nr_indeksu][Y] = true;
					if (czerwona == false) {
						zielony[nr_indeksu][Y] = true;
					}
					else {
						zielony[nr_indeksu][Y] = false;
					}

					robocze_low = robocze_low - podzialka;
					robocze_close = robocze_close - podzialka;
					robocze_open = robocze_open - podzialka;
					robocze_high = robocze_high - podzialka;

					robocze_low = round(robocze_low * 10) / 10;
					robocze_close = round(robocze_close * 10) / 10;
					robocze_open = round(robocze_open * 10) / 10;
					robocze_high = round(robocze_high * 10) / 10;
					Y -= 1;
				}
			}
		}

		if (koniec == false) {

			robocze_low = robocze_low - podzialka;
			robocze_close = robocze_close - podzialka;
			robocze_open = robocze_open - podzialka;
			robocze_high = robocze_high - podzialka;

			robocze_low = round(robocze_low * 10) / 10;
			robocze_close = round(robocze_close * 10) / 10;
			robocze_open = round(robocze_open * 10) / 10;
			robocze_high = round(robocze_high * 10) / 10;
			Y -= 1;
		}
	}
}


void uzupelnianie_wykresu(float open[7][1000], float high[7][1000], float low[7][1000], float close[7][1000], float volume[7][1000], float zaok_open[7][1000], float zaok_high[7][1000], float zaok_low[7][1000], float zaok_close[7][1000], bool zielony[200][200], bool kolory[200][200], char obiekty[20][50][100], int wysokosc, int szerokosc, int odczyty, int indeksy_konca_poczatku[2], int rozszerzenia_konca_poczatku[2], char os_wartosci[10][100], bool wartosci[100], int nr_swiecy, float& podzialka, bool kolory_os_wartosci[100], bool niebieski_wartosci[100], float& najwiekszy_high, float& najmniejszy_low, int dzien_os_czasu[2], int miesiac_os_czasu[2], int rok_os_czasu[2], int rok[7][1000], int miesiac[7][1000], int dzien[7][1000], int& odstep) {

	najwiekszy_high = high[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0]];
	najmniejszy_low = low[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0]];

	int dodatkowy_indeks = round(szerokosc / 50) + 1;

	int rozszerzenie = rozszerzenia_konca_poczatku[0];
	int pozycja = indeksy_konca_poczatku[0];
	int licznik = 0;

	bool czerwona = true;

	for (int i = 0; i <= szerokosc * odczyty; i++) {

		if (pozycja == 1000) {
			rozszerzenie += 1;
			pozycja = 0;
		}

		if (najmniejszy_low > zaok_low[rozszerzenie][pozycja]) {
			najmniejszy_low = zaok_low[rozszerzenie][pozycja];
		}
		if (najwiekszy_high < zaok_high[rozszerzenie][pozycja]) {
			najwiekszy_high = zaok_high[rozszerzenie][pozycja];
		}

		pozycja += 1;
		licznik += 1;
	}

	najmniejszy_low = round(najmniejszy_low * 10) / 10;

	podzialka = (najwiekszy_high - najmniejszy_low) / wysokosc;

	podzialka = round(podzialka * 10) / 10;

	if ((najwiekszy_high - najmniejszy_low) / podzialka > wysokosc) {
		podzialka += 0.1;
	}

	//puste pola

	for (int y = 0; y <= wysokosc + 2; y++) {
		for (int x = 0; x <= szerokosc + 2; x++) {
			for (int z = 0; z < dodatkowy_indeks; z++) {
				obiekty[z][x][y] = ' ';
			}
		}
	}

	//domyslne kolory

	for (int y = 0; y < wysokosc + 2; y++) {
		for (int x = 0; x < szerokosc + 2; x++) {
			kolory[x][y] = false;
			zielony[x][y] = false;
		}
	}

	for (int i = 0; i < 95; i++) {
		kolory_os_wartosci[i] = false;
		niebieski_wartosci[i] = false;
	}

	//czyszczenie osi wartosci

	for (int i = 0; i <= wysokosc; i++) {
		wartosci[i] = false;
	}


	//przyporzadkowywanie kordow swiecom

	rozszerzenie = 0;
	pozycja = 0;

	for (int nr_indeksu = 1; nr_indeksu <= szerokosc; nr_indeksu++) {

		int nr_indeksu_backup = nr_indeksu * odczyty + indeksy_konca_poczatku[0];

		float robocze_open = 0;
		float robocze_high = 0;
		float robocze_low = 100000;
		float robocze_close = 0;

		if (nr_indeksu_backup < 1000) {
			rozszerzenie = rozszerzenia_konca_poczatku[0];
		}
		else if (nr_indeksu_backup < 2000) {
			nr_indeksu_backup = nr_indeksu_backup - 1000;
			rozszerzenie = rozszerzenia_konca_poczatku[0] + 1;
		}
		else if (nr_indeksu_backup < 3000) {
			nr_indeksu_backup = nr_indeksu_backup - 2000;
			rozszerzenie = rozszerzenia_konca_poczatku[0] + 2;
		}
		else if (nr_indeksu_backup < 4000) {
			nr_indeksu_backup = nr_indeksu_backup - 3000;
			rozszerzenie = rozszerzenia_konca_poczatku[0] + 3;
		}
		else if (nr_indeksu_backup < 5000) {
			nr_indeksu_backup = nr_indeksu_backup - 4000;
			rozszerzenie = rozszerzenia_konca_poczatku[0] + 4;
		}
		else if (nr_indeksu_backup < 6000) {
			nr_indeksu_backup = nr_indeksu_backup - 5000;
			rozszerzenie = rozszerzenia_konca_poczatku[0] + 5;
		}
		else if (nr_indeksu_backup < 7000) {
			nr_indeksu_backup = nr_indeksu_backup - 6000;
			rozszerzenie = rozszerzenia_konca_poczatku[0] + 6;
		}
		else if (nr_indeksu_backup < 8000) {
			nr_indeksu_backup = nr_indeksu_backup - 7000;
			rozszerzenie = rozszerzenia_konca_poczatku[0] + 7;
		}


		if (nr_indeksu_backup - odczyty + 1 < 0) {
			robocze_open = zaok_open[rozszerzenie - 1][1000 + (nr_indeksu_backup - odczyty + 1)] - najmniejszy_low;
			if (nr_indeksu == nr_swiecy) {
				dzien_os_czasu[0] = dzien[rozszerzenie - 1][1000 + (nr_indeksu_backup - odczyty + 1)];
				miesiac_os_czasu[0] = miesiac[rozszerzenie - 1][1000 + (nr_indeksu_backup - odczyty + 1)];
				rok_os_czasu[0] = rok[rozszerzenie - 1][1000 + (nr_indeksu_backup - odczyty + 1)];
			}
		}
		else {
			robocze_open = zaok_open[rozszerzenie][nr_indeksu_backup - odczyty + 1] - najmniejszy_low;
			if (nr_indeksu == nr_swiecy) {
				dzien_os_czasu[0] = dzien[rozszerzenie][nr_indeksu_backup - odczyty + 1];
				miesiac_os_czasu[0] = miesiac[rozszerzenie][nr_indeksu_backup - odczyty + 1];
				rok_os_czasu[0] = rok[rozszerzenie][nr_indeksu_backup - odczyty + 1];
			}
		}

		robocze_close = zaok_close[rozszerzenie][nr_indeksu_backup] - najmniejszy_low;
		if (nr_indeksu == nr_swiecy) {
			dzien_os_czasu[1] = dzien[rozszerzenie][nr_indeksu_backup];
			miesiac_os_czasu[1] = miesiac[rozszerzenie][nr_indeksu_backup];
			rok_os_czasu[1] = rok[rozszerzenie][nr_indeksu_backup];
		}


		for (int i = odczyty - 1; i >= 0; i--) {

			if (nr_indeksu_backup - i < 0) {

				if (zaok_low[rozszerzenie - 1][1000 + (nr_indeksu_backup - i)] - najmniejszy_low < robocze_low) {
					robocze_low = zaok_low[rozszerzenie - 1][1000 + (nr_indeksu_backup - i)] - najmniejszy_low;
				}
				if (zaok_high[rozszerzenie - 1][1000 + (nr_indeksu_backup - i)] - najmniejszy_low > robocze_high) {
					robocze_high = zaok_high[rozszerzenie - 1][1000 + (nr_indeksu_backup - i)] - najmniejszy_low;
				}
			}

			else {

				if (zaok_low[rozszerzenie][nr_indeksu_backup - i] - najmniejszy_low < robocze_low) {
					robocze_low = zaok_low[rozszerzenie][nr_indeksu_backup - i] - najmniejszy_low;
				}
				if (zaok_high[rozszerzenie][nr_indeksu_backup - i] - najmniejszy_low > robocze_high) {
					robocze_high = zaok_high[rozszerzenie][nr_indeksu_backup - i] - najmniejszy_low;
				}
			}
		}


		if (nr_indeksu_backup - odczyty + 1 < 0) {

			if (open[rozszerzenie - 1][1000 + (nr_indeksu_backup - odczyty + 1)] < close[rozszerzenie][nr_indeksu_backup]) {
				tworzenie_swiecy(robocze_low, robocze_high, robocze_close, robocze_open, wysokosc, podzialka, nr_indeksu, obiekty, kolory, zielony, nr_swiecy, kolory_os_wartosci, niebieski_wartosci, false);
			}
			else {
				tworzenie_swiecy(robocze_low, robocze_high, robocze_close, robocze_open, wysokosc, podzialka, nr_indeksu, obiekty, kolory, zielony, nr_swiecy, kolory_os_wartosci, niebieski_wartosci, true);
			}
		}

		else {

			if (open[rozszerzenie][nr_indeksu_backup - odczyty + 1] < close[rozszerzenie][nr_indeksu_backup]) {

				tworzenie_swiecy(robocze_low, robocze_high, robocze_close, robocze_open, wysokosc, podzialka, nr_indeksu, obiekty, kolory, zielony, nr_swiecy, kolory_os_wartosci, niebieski_wartosci, false);
			}
			else {
				tworzenie_swiecy(robocze_low, robocze_high, robocze_close, robocze_open, wysokosc, podzialka, nr_indeksu, obiekty, kolory, zielony, nr_swiecy, kolory_os_wartosci, niebieski_wartosci, true);
			}
		}
	}

	//tworzenie ramki

	obiekty[0][0][0] = char(203);
	obiekty[0][0][wysokosc + 1] = char(202);

	if (szerokosc < 50) {
		obiekty[0][szerokosc + 1][wysokosc + 1] = char(188);
	}
	else if (szerokosc < 100) {
		obiekty[1][szerokosc + 1 - 50][wysokosc + 1] = char(188);
	}
	else if (szerokosc < 150) {
		obiekty[2][szerokosc + 1 - 100][wysokosc + 1] = char(188);
	}

	if (szerokosc < 50) {
		obiekty[0][szerokosc + 1][0] = char(187);
	}
	else if (szerokosc < 100) {
		obiekty[1][szerokosc + 1 - 50][0] = char(187);
	}
	else if (szerokosc < 150) {
		obiekty[2][szerokosc + 1 - 100][0] = char(187);
	}


	for (int y = 1; y <= wysokosc; y++) {
		obiekty[0][0][y] = char(186);
	}

	for (int y = 1; y <= wysokosc; y++) {

		if (szerokosc < 50) {
			obiekty[0][szerokosc + 1][y] = char(186);
		}
		else if (szerokosc < 100) {
			obiekty[1][szerokosc + 1 - 50][y] = char(186);
		}
		else if (szerokosc < 150) {
			obiekty[2][szerokosc + 1 - 100][y] = char(186);
		}
	}

	for (int x = 1; x <= szerokosc; x++) {

		if (szerokosc < 50) {
			obiekty[0][x][wysokosc + 1] = char(205);
		}
		else if (szerokosc < 100) {
			obiekty[1][x - 50][wysokosc + 1] = char(205);
		}
		else if (szerokosc < 150) {
			obiekty[2][x - 100][wysokosc + 1] = char(205);
		}

		kolory[x][wysokosc + 1] = false;
	}

	for (int x = 1; x <= szerokosc; x++) {
		if (szerokosc < 50) {
			obiekty[0][x][0] = char(205);
		}
		else if (szerokosc < 100) {
			obiekty[1][x - 50][0] = char(205);
		}
		else if (szerokosc < 150) {
			obiekty[2][x - 100][0] = char(205);
		}

		kolory[x][0] = false;
	}

	// tworzenie osi

	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 100; y++) {
			os_wartosci[x][y] = ' ';
		}
	}

	os_wartosci[0][0] = char(201);
	os_wartosci[0][wysokosc + 1] = char(200);

	for (int Y = 1; Y <= wysokosc; Y++) {
		os_wartosci[0][Y] = char(186);
	}


	for (int i = 1; i <= 8; i++) {
		os_wartosci[i][0] = char(205);
		os_wartosci[i][wysokosc + 1] = char(205);
	}

	// odstep

	if (szerokosc - 90 <= 0) {
		odstep = 0;
	}
	else {
		odstep = round((szerokosc - 90) / 2);
	}
}

void powitanie() {

	std::cout << char(201);

	for (int i = 1; i < 118; i++) {
		std::cout << char(205);
	}
	std::cout << char(187) << std::endl;

	std::cout << char(186) << " ______             _______ __ __ __               _____         __           __                          __     __  " << char(186) << std::endl;
	std::cout << char(186) << "|   __ `.--.--.    |    ___|__|  |__|.-----.     _|     |.---.-.|  |--.--.--.|  |--.-----.--.--.--.-----.|  |--.|__| " << char(186) << std::endl;
	std::cout << char(186) << "|   __ <|  |  |    |    ___|  |  |  ||  _  |    |       ||  _  ||    <|  |  ||  _  |  _  |  |  |  |__ --||    < |  | " << char(186) << std::endl;
	std::cout << char(186) << "|______/|___  |    |___|   |__|__|__||   __|    |_______||___._||__|__|_____||_____|_____|________|_____||__|__||__| " << char(186) << std::endl;
	std::cout << char(186) << "        |_____|                      |__|                                                                            " << char(186) << std::endl;
	std::cout << char(186) << "                                                                                                                     " << char(186) << std::endl;
	std::cout << char(186) << "                                           _  ___ _____ __   _  _   _  _                                             " << char(186) << std::endl;
	std::cout << char(186) << "                                          / |/ _ `___  / /_ | || | | || |                                            " << char(186) << std::endl;
	std::cout << char(186) << "                                          | | (_) | / / '_ `| || |_| || |_                                           " << char(186) << std::endl;
	std::cout << char(186) << "                                          | |`__, |/ /| (_) |__   _|__   _|                                          " << char(186) << std::endl;
	std::cout << char(186) << "                                          |_|  /_//_/  `___/   |_|    |_|                                            " << char(186) << std::endl;
	std::cout << char(186) << "                                                                                                                     " << char(186) << std::endl;

	std::cout << char(200);

	for (int i = 1; i < 118; i++) {
		std::cout << char(205);
	}
	std::cout << char(188) << std::endl;

	char kontynuacja;

	std::cout << std::endl << "By kontynowac wpisz dowolny znak: ";
	std::cin >> kontynuacja;

	system("cls");
}


void napis_Gielda(int odstep) {

	spacje(odstep);
	std::cout << char(201);

	for (int i = 1; i < 100; i++) {
		std::cout << char(205);
	}
	std::cout << char(187) << std::endl;

	spacje(odstep);
	std::cout << char(186) << "                                                            ";                                                                                                        std::cout << GREEN_TEXT << "            $$$$$$$$";              std::cout << "                   " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "        $$$$$$$$$$$$$"; std::cout << RED_TEXT << "   $$$$   "; std::cout << GREEN_TEXT << "                     "; std::cout << RED_TEXT << "$$$$$$$ "; std::cout << GREEN_TEXT << "            $::::::$"; std::cout << RED_TEXT << "                   " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "     $$$::::::::::::$"; std::cout << RED_TEXT << "  $::::$  "; std::cout << GREEN_TEXT << "                     "; std::cout << RED_TEXT << "$:::::$ "; std::cout << GREEN_TEXT << "            $::::::$"; std::cout << RED_TEXT << "                   " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "   $$:::::::::::::::$"; std::cout << RED_TEXT << "   $$$$   "; std::cout << GREEN_TEXT << "                     "; std::cout << RED_TEXT << "$:::::$ "; std::cout << GREEN_TEXT << "            $::::::$"; std::cout << RED_TEXT << "                   " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "  $:::::$$$$$$$$::::$"; std::cout << RED_TEXT << "          "; std::cout << GREEN_TEXT << "                     "; std::cout << RED_TEXT << "$:::::$ "; std::cout << GREEN_TEXT << "            $:::::$ "; std::cout << RED_TEXT << "                   " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << " $:::::$       $$$$$$"; std::cout << RED_TEXT << " $$$$$$$  "; std::cout << GREEN_TEXT << "     $$$$$$$$$$$$    "; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << "    $$$$$$$$$:::::$ "; std::cout << RED_TEXT << "   $$$$$$$$$$$$$   " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "$:::::$              "; std::cout << RED_TEXT << " $:::::$  "; std::cout << GREEN_TEXT << "   $$::::::::::::$$  "; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << "  $$::::::::::::::$ "; std::cout << RED_TEXT << "   $::::::::::::$  " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "$:::::$              "; std::cout << RED_TEXT << "  $::::$  "; std::cout << GREEN_TEXT << "  $::::::$$$$$:::::$$"; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << " $::::::::::::::::$ "; std::cout << RED_TEXT << "   $$$$$$$$$:::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "$:::::$    $$$$$$$$$$"; std::cout << RED_TEXT << "  $::::$  "; std::cout << GREEN_TEXT << " $::::::$     $:::::$"; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << "$:::::::$$$$$:::::$ "; std::cout << RED_TEXT << "            $::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "$:::::$    $::::::::$"; std::cout << RED_TEXT << "  $::::$  "; std::cout << GREEN_TEXT << " $:::::::$$$$$::::::$"; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << "$::::::$    $:::::$ "; std::cout << RED_TEXT << "     $$$$$$$:::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "$:::::$    $$$$$::::$"; std::cout << RED_TEXT << "  $::::$  "; std::cout << GREEN_TEXT << " $:::::::::::::::::$ "; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << "$:::::$     $:::::$ "; std::cout << RED_TEXT << "   $$::::::::::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "$:::::$        $::::$"; std::cout << RED_TEXT << "  $::::$  "; std::cout << GREEN_TEXT << " $::::::$$$$$$$$$$$  "; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << "$:::::$     $:::::$ "; std::cout << RED_TEXT << "  $::::$$$$::::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << " $:::::$       $::::$"; std::cout << RED_TEXT << "  $::::$  "; std::cout << GREEN_TEXT << " $:::::::$           "; std::cout << RED_TEXT << " $::::$ "; std::cout << GREEN_TEXT << "$:::::$     $:::::$ "; std::cout << RED_TEXT << " $::::$    $:::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "  $:::::$$$$$$$$::::$"; std::cout << RED_TEXT << " $::::::$ "; std::cout << GREEN_TEXT << " $::::::::$          "; std::cout << RED_TEXT << "$::::::$"; std::cout << GREEN_TEXT << "$::::::$$$$$::::::$$"; std::cout << RED_TEXT << " $::::$    $:::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "   $$:::::::::::::::$"; std::cout << RED_TEXT << " $::::::$ "; std::cout << GREEN_TEXT << "  $::::::::$$$$$$$$  "; std::cout << RED_TEXT << "$::::::$"; std::cout << GREEN_TEXT << " $:::::::::::::::::$"; std::cout << RED_TEXT << " $:::::$$$$::::::$ " << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "     $$$::::::$$$:::$"; std::cout << RED_TEXT << " $::::::$ "; std::cout << GREEN_TEXT << "   $$:::::::::::::$  "; std::cout << RED_TEXT << "$::::::$"; std::cout << GREEN_TEXT << "  $:::::::::$$$::::$"; std::cout << RED_TEXT << "  $::::::::::$$:::$" << RESET_COLOR << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << GREEN_TEXT << "        $$$$$$   $$$$"; std::cout << RED_TEXT << " $$$$$$$$ "; std::cout << GREEN_TEXT << "     $$$$$$$$$$$$$$  "; std::cout << RED_TEXT << "$$$$$$$$"; std::cout << GREEN_TEXT << "   $$$$$$$$$   $$$$$"; std::cout << RED_TEXT << "   $$$$$$$$$$  $$$$" << RESET_COLOR << char(186) << std::endl;

	spacje(odstep);
	std::cout << char(200);

	for (int i = 1; i < 100; i++) {
		std::cout << char(205);
	}
	std::cout << char(188) << std::endl;
}

void konfiguracja(int& wysokosc, int& szerokosc, int& odczyty, int& pierwszy_rok, int& pierwszy_miesiac, int& pierwszy_dzien, int& ostatni_rok, int& ostatni_miesiac, int& ostatni_dzien, int rok[7][1000], int miesiac[7][1000], int dzien[7][1000], int indeksy_konca_poczatku[2], int rozszerzenia_konca_poczatku[2], bool& blad_konfiguracji) {

	char kontynuacja;

	std::cout << "Podaj wysokosc wykresu: ";
	while (true) {
		if (std::cin >> wysokosc) {
			break;
		}
		else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
		}
	}

	system("cls");


	while (wysokosc < 0 || wysokosc > 91) {
		std::cout << "Podaj wysokosc z zakresu od 1 do 90: ";
		while (true) {
			if (std::cin >> wysokosc) {
				break;
			}
			else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Podaj wysokosc z zakresu od 1 do 90: ";
			}
		}
	}

	system("cls");

	std::cout << "Podaj date startowa: " << std::endl;
	std::cout << "Rok: ";
	while (true) {
		if (std::cin >> pierwszy_rok) {
			break;
		}
		else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
		}
	}
	std::cout << "Miesiac: ";
	while (true) {
		if (std::cin >> pierwszy_miesiac) {
			break;
		}
		else {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
		}
	}

	std::cout << "Dostepne dni: ";

	int pozycja = 1;
	int roz = 0;
	int pierwsze_dni[31], indeksy_pierwszych_dni[31], rozszerzenia_pierwszych_dni[31];
	int ilosc_dostepnych_dni = 0;

	int indeksy_ostatnich_dni[31], rozszerzenia_ostatnich_dni[31];

	bool brak_miesiaca = true;
	bool brak_roku = true;
	bool poprawny_dzien = false;

	int zakres_dat = 0;

	while (rok[roz][pozycja] != 0) {

		if (rok[roz][pozycja] == pierwszy_rok) {

			brak_roku = false;

			if (miesiac[roz][pozycja] == pierwszy_miesiac) {
				std::cout << dzien[roz][pozycja] << " ";

				rozszerzenia_pierwszych_dni[ilosc_dostepnych_dni] = roz;
				indeksy_pierwszych_dni[ilosc_dostepnych_dni] = pozycja;

				brak_miesiaca = false;
				ilosc_dostepnych_dni += 1;
			}
		}

		if ((rok[roz][pozycja] != pierwszy_rok && miesiac[roz][pozycja] != pierwszy_miesiac && brak_miesiaca == false) || (rok[roz][pozycja] == pierwszy_rok && miesiac[roz][pozycja] != pierwszy_miesiac && brak_miesiaca == false)) {
			break;
		}

		pozycja += 1;

		if (pozycja == 1000) {
			roz += 1;
			pozycja = 1;
		}
	}

	if (brak_roku == true) {
		std::cout << "brak danych dla tego roku." << std::endl;
		blad_konfiguracji = true;
	}

	else if (brak_miesiaca == true) {
		std::cout << "Brak danych dla wybranego miesiaca. " << std::endl;
		blad_konfiguracji = true;
	}
	else {
		std::cout << std::endl << "Dzien: ";
		while (true) {
			if (std::cin >> pierwszy_dzien) {
				break;
			}
			else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
			}
		}

		for (int i = 0; i < ilosc_dostepnych_dni; i++) {
			if (pierwszy_dzien == dzien[rozszerzenia_pierwszych_dni[i]][indeksy_pierwszych_dni[i]]) {
				poprawny_dzien = true;
				break;
			}
		}

		if (poprawny_dzien == false) {
			pierwszy_dzien = dzien[rozszerzenia_pierwszych_dni[0]][indeksy_pierwszych_dni[0]];
		}
	}


	for (int i = 0; i < ilosc_dostepnych_dni; i++) {

		if (pierwszy_dzien == dzien[rozszerzenia_pierwszych_dni[i]][indeksy_pierwszych_dni[i]]) {
			rozszerzenia_konca_poczatku[0] = rozszerzenia_pierwszych_dni[i];
			indeksy_konca_poczatku[0] = indeksy_pierwszych_dni[i];
			break;
		}
	}

	if (blad_konfiguracji == false) {

		std::cout << "Podaj date koncowa: " << std::endl;
		std::cout << "Rok: ";
		while (true) {
			if (std::cin >> ostatni_rok) {
				break;
			}
			else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
			}
		}
		std::cout << "Miesiac: ";
		while (true) {
			if (std::cin >> ostatni_miesiac) {
				break;
			}
			else {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
			}
		}

		std::cout << "Dostepne dni: ";

		brak_roku = true;
		brak_miesiaca = true;
		ilosc_dostepnych_dni = 0;

		roz = rozszerzenia_pierwszych_dni[0];
		pozycja = indeksy_pierwszych_dni[0];

		while (rok[roz][pozycja] != 0) {

			if (rok[roz][pozycja] == ostatni_rok) {

				brak_roku = false;

				if (miesiac[roz][pozycja] == ostatni_miesiac) {
					std::cout << dzien[roz][pozycja] << " ";

					rozszerzenia_ostatnich_dni[ilosc_dostepnych_dni] = roz;
					indeksy_ostatnich_dni[ilosc_dostepnych_dni] = pozycja;

					brak_miesiaca = false;
					ilosc_dostepnych_dni += 1;
				}
			}

			if ((rok[roz][pozycja] != ostatni_rok && miesiac[roz][pozycja] != ostatni_miesiac && brak_miesiaca == false) || (rok[roz][pozycja] == ostatni_rok && miesiac[roz][pozycja] != ostatni_miesiac && brak_miesiaca == false)) {
				break;
			}

			pozycja += 1;

			if (pozycja == 1000) {
				roz += 1;
				pozycja = 0;
			}
		}

		if (brak_roku == true) {
			std::cout << "Brak dlanych dla tego roku, albo wpisano zly zakres dat." << std::endl;
			blad_konfiguracji = true;
		}

		else if (brak_miesiaca == true) {
			std::cout << "Brak danych dla tego miesiaca, albo wpisano zly zakres dat. " << std::endl;
			blad_konfiguracji = true;
		}
		else {
			std::cout << std::endl << "Dzien: ";
			while (true) {
				if (std::cin >> ostatni_dzien) {
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
				}
			}

			poprawny_dzien = false;

			for (int i = 0; i < ilosc_dostepnych_dni; i++) {
				if (ostatni_dzien == dzien[rozszerzenia_ostatnich_dni[i]][indeksy_ostatnich_dni[i]]) {
					poprawny_dzien = true;
					break;
				}
			}

			if (poprawny_dzien == false) {
				ostatni_dzien = dzien[rozszerzenia_ostatnich_dni[0]][indeksy_ostatnich_dni[0]];
			}
		}

		if (blad_konfiguracji != true) {

			for (int i = 0; i < ilosc_dostepnych_dni; i++) {
				if (ostatni_dzien == dzien[rozszerzenia_ostatnich_dni[i]][indeksy_ostatnich_dni[i]]) {
					rozszerzenia_konca_poczatku[1] = rozszerzenia_ostatnich_dni[i];
					indeksy_konca_poczatku[1] = indeksy_ostatnich_dni[i];
					break;
				}
			}


			roz = rozszerzenia_konca_poczatku[0];
			pozycja = indeksy_konca_poczatku[0];

			while (!(roz == rozszerzenia_konca_poczatku[1] && pozycja == indeksy_konca_poczatku[1])) {

				zakres_dat += 1;
				pozycja += 1;


				if (pozycja == 1000) {
					pozycja = 0;
					roz += 1;
				}
			}

			zakres_dat += 1;

			std::cout << "By kontynuowac wpisz dowolny znak: ";
			std::cin >> kontynuacja;

			system("cls");

			

			std::cout << "Podaj z ilu odczytow ma skladac sie swieca: ";

			while (true) {
				if (std::cin >> odczyty) {
					break;
				}
				else {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
				}
			}

			while (odczyty > zakres_dat) {
				std::cout << "Podane odczyty sa wieksze niz zakres danych, prosze podac odczyty mniejsze od: " << odczyty + 1 << std::endl;
				while (true) {
					if (std::cin >> odczyty) {
						break;
					}
					else {
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
					}
				}
			}


			if (zakres_dat % odczyty != 0) {
				std::cout << std::endl << "Zakres dat nie jest podzelny przez " << odczyty << ", czy jestes pewny tego wyboru?" << std::endl;
				std::cout << "Ostatnia swieca w tym wykresie moze skladac sie z mniejscej liczby odczytow." << std::endl;
				std::cout << "By potwierdzic wpisz ta sama wartosc, mozesz takze zmienic ilosc odczytow." << std::endl << std::endl;
				std::cout << "Podaj z ilu odczytow ma skladac sie swieca: ";
				while (true) {
					if (std::cin >> odczyty) {
						break;
					}
					else {
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						std::cout << "Nieprawidlowe dane. Wprowadz liczbe jeszcze raz." << std::endl;
					}
				}
			}


			if ( odczyty < 1 || odczyty > 31) {
				std::cout << "Liczba spoza zakresu 1-31, zmieniono liczbe odczytow na: 1" << std::endl;
				odczyty = 1;
			}

			szerokosc = round(zakres_dat / odczyty);

			if (szerokosc > 148) {
				std::cout << "Zakres dat byl zbyt duzy, zmieniony zakres dat to: " << std::endl;

				zakres_dat = 148 * odczyty;

				std::cout << pierwszy_rok << "-" << pierwszy_miesiac << "-" << pierwszy_dzien << std::endl;

				if (indeksy_konca_poczatku[0] + zakres_dat < 1000) {
					std::cout << rok[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0] + zakres_dat] << "-" << miesiac[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0] + zakres_dat] << "-" << dzien[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0] + zakres_dat] << std::endl;
					ostatni_rok = rok[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0] + zakres_dat];
					ostatni_miesiac = miesiac[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0] + zakres_dat];
					ostatni_dzien = dzien[rozszerzenia_konca_poczatku[0]][indeksy_konca_poczatku[0] + zakres_dat];
					indeksy_konca_poczatku[1] = zakres_dat;
				}
				else if (indeksy_konca_poczatku[0] + zakres_dat < 2000) {
					std::cout << rok[rozszerzenia_konca_poczatku[0] + 1][indeksy_konca_poczatku[0] + zakres_dat - 1000] << "-" << miesiac[rozszerzenia_konca_poczatku[0] + 1][indeksy_konca_poczatku[0] + zakres_dat - 1000] << "-" << dzien[rozszerzenia_konca_poczatku[0] + 1][indeksy_konca_poczatku[0] + zakres_dat - 1000] << std::endl;
					ostatni_rok = rok[rozszerzenia_konca_poczatku[0] + 1][indeksy_konca_poczatku[0] + zakres_dat - 1000];
					ostatni_miesiac = miesiac[rozszerzenia_konca_poczatku[0] + 1][indeksy_konca_poczatku[0] + zakres_dat - 1000];
					ostatni_dzien = dzien[rozszerzenia_konca_poczatku[0] + 1][indeksy_konca_poczatku[0] + zakres_dat - 1000];
					indeksy_konca_poczatku[1] = zakres_dat - 1000;
					rozszerzenia_konca_poczatku[1] = rozszerzenia_konca_poczatku[0] + 1;
				}
				else if (indeksy_konca_poczatku[0] + zakres_dat < 3000) {
					std::cout << rok[rozszerzenia_konca_poczatku[0] + 2][indeksy_konca_poczatku[0] + zakres_dat - 2000] << "-" << miesiac[rozszerzenia_konca_poczatku[0] + 2][indeksy_konca_poczatku[0] + zakres_dat - 2000] << "-" << dzien[rozszerzenia_konca_poczatku[0] + 2][indeksy_konca_poczatku[0] + zakres_dat - 2000] << std::endl;
					ostatni_rok = rok[rozszerzenia_konca_poczatku[0] + 2][indeksy_konca_poczatku[0] + zakres_dat - 2000];
					ostatni_miesiac = miesiac[rozszerzenia_konca_poczatku[0] + 2][indeksy_konca_poczatku[0] + zakres_dat - 2000];
					ostatni_dzien = dzien[rozszerzenia_konca_poczatku[0] + 2][indeksy_konca_poczatku[0] + zakres_dat - 2000];
					indeksy_konca_poczatku[1] = zakres_dat - 2000;
					rozszerzenia_konca_poczatku[1] = rozszerzenia_konca_poczatku[0] + 2;
				}
				else if (indeksy_konca_poczatku[0] + zakres_dat < 4000) {
					std::cout << rok[rozszerzenia_konca_poczatku[0] + 3][indeksy_konca_poczatku[0] + zakres_dat - 3000] << "-" << miesiac[rozszerzenia_konca_poczatku[0] + 3][indeksy_konca_poczatku[0] + zakres_dat - 3000] << "-" << dzien[rozszerzenia_konca_poczatku[0] + 3][indeksy_konca_poczatku[0] + zakres_dat - 3000] << std::endl;
					ostatni_rok = rok[rozszerzenia_konca_poczatku[0] + 3][indeksy_konca_poczatku[0] + zakres_dat - 3000];
					ostatni_miesiac = miesiac[rozszerzenia_konca_poczatku[0] + 3][indeksy_konca_poczatku[0] + zakres_dat - 3000];
					ostatni_dzien = dzien[rozszerzenia_konca_poczatku[0] + 3][indeksy_konca_poczatku[0] + zakres_dat - 3000];
					indeksy_konca_poczatku[1] = zakres_dat - 3000;
					rozszerzenia_konca_poczatku[1] = rozszerzenia_konca_poczatku[0] + 3;
				}
				else if (indeksy_konca_poczatku[0] + zakres_dat < 5000) {
					std::cout << rok[rozszerzenia_konca_poczatku[0] + 4][indeksy_konca_poczatku[0] + zakres_dat - 4000] << "-" << miesiac[rozszerzenia_konca_poczatku[0] + 4][indeksy_konca_poczatku[0] + zakres_dat - 4000] << "-" << dzien[rozszerzenia_konca_poczatku[0] + 4][indeksy_konca_poczatku[0] + zakres_dat - 4000] << std::endl;
					ostatni_rok = rok[rozszerzenia_konca_poczatku[0] + 4][indeksy_konca_poczatku[0] + zakres_dat - 4000];
					ostatni_miesiac = miesiac[rozszerzenia_konca_poczatku[0] + 4][indeksy_konca_poczatku[0] + zakres_dat - 4000];
					ostatni_dzien = dzien[rozszerzenia_konca_poczatku[0] + 4][indeksy_konca_poczatku[0] + zakres_dat - 4000];
					indeksy_konca_poczatku[1] = zakres_dat - 4000;
					rozszerzenia_konca_poczatku[1] = rozszerzenia_konca_poczatku[0] + 4;
				}
				else {
					std::cout << rok[rozszerzenia_konca_poczatku[0] + 5][indeksy_konca_poczatku[0] + zakres_dat - 5000] << "-" << miesiac[rozszerzenia_konca_poczatku[0] + 5][indeksy_konca_poczatku[0] + zakres_dat - 5000] << "-" << dzien[rozszerzenia_konca_poczatku[0] + 5][indeksy_konca_poczatku[0] + zakres_dat - 5000] << std::endl;
					ostatni_rok = rok[rozszerzenia_konca_poczatku[0] + 5][indeksy_konca_poczatku[0] + zakres_dat - 5000];
					ostatni_miesiac = miesiac[rozszerzenia_konca_poczatku[0] + 5][indeksy_konca_poczatku[0] + zakres_dat - 5000];
					ostatni_dzien = dzien[rozszerzenia_konca_poczatku[0] + 5][indeksy_konca_poczatku[0] + zakres_dat - 5000];
					indeksy_konca_poczatku[1] = zakres_dat - 5000;
					rozszerzenia_konca_poczatku[1] = rozszerzenia_konca_poczatku[0] + 5;
				}

				szerokosc = 148;
			}

			std::cout << std::endl << "By kontynowac wpisz dowolny znak: ";
			std::cin >> kontynuacja;


			system("cls");



			std::cout << char(201);
			for (int i = 0; i <= 30; i++) {
				std::cout << char(205);
			}
			std::cout << char(187) << std::endl;

			std::cout << char(186) << " Parametry wykresu:            " << char(186) << std::endl;

			std::cout << char(186) << "                               " << char(186) << std::endl;

			std::cout << char(186) << " Wysokosc: " << wysokosc;
			if (wysokosc / 10 < 1) {
				std::cout << ' ';
			}
			std::cout << "                  " << char(186) << std::endl;

			std::cout << char(186) << "                               " << char(186) << std::endl;

			std::cout << char(186) << " Szerokosc: " << szerokosc;
			if (szerokosc / 10 < 1) {
				std::cout << "  ";
			}
			else if (szerokosc / 100 < 1) {
				std::cout << ' ';
			}
			std::cout << "                " << char(186) << std::endl;

			std::cout << char(186) << "                               " << char(186) << std::endl;

			std::cout << char(186) << " Zakres czasowy:               " << char(186) << std::endl;

			std::cout << char(186) << " Od " << pierwszy_rok << "-" << pierwszy_miesiac << "-" << pierwszy_dzien << " do " << ostatni_rok << "-" << ostatni_miesiac << "-" << ostatni_dzien;
			if (pierwszy_miesiac / 10 < 1) {
				std::cout << ' ';
			}
			if (pierwszy_dzien / 10 < 1) {
				std::cout << ' ';
			}
			if (ostatni_miesiac / 10 < 1) {
				std::cout << ' ';
			}
			if (ostatni_dzien / 10 < 1) {
				std::cout << ' ';
			}
			std::cout << "   " << char(186) << std::endl;

			std::cout << char(186) << "                               " << char(186) << std::endl;

			std::cout << char(186) << " Odczyty tworzace 1 swiece: " << odczyty;
			if (odczyty / 10 < 1) {
				std::cout << ' ';
			}
			std::cout << " " << char(186) << std::endl;

			std::cout << char(200);
			for (int i = 0; i <= 30; i++) {
				std::cout << char(205);
			}
			std::cout << char(188) << std::endl;

			std::cout << "By kontynuowac wpisz dowolny znak: ";
			std::cin >> kontynuacja;

			system("cls");
		}
	}
}

void ostatnia_tabelka(char& ruch_uzytkownika, int szerokosc, int& odstep) {
	if (szerokosc - 41 <= 0) {
		odstep = 0;
	}
	else {
		odstep = round((szerokosc - 41) / 2);
	}

	std::cout << std::endl;

	spacje(odstep);

	std::cout << char(201);

	for (int i = 0; i < 50; i++) {
		std::cout << char(205);
	}

	std::cout << char(187) << std::endl;
	spacje(odstep);
	std::cout << char(186) << "      By przywolac menu wcisnij klawisz 'm'       " << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << "   By zakonczyc dzialanie programu wcisnij 'q'.   " << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << "  By zmienic wybrana swiece wcisnij 'a' lub 'd'   " << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << "  By wpisac indeks konkretnej swiecy wcisnij 'i'  " << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(186) << "   By kontynuowac wcisnij dowolny inny klawisz.   " << char(186) << std::endl;
	spacje(odstep);
	std::cout << char(200);

	for (int i = 0; i < 50; i++) {
		std::cout << char(205);
	}

	std::cout << char(188) << std::endl << std::endl;

	std::cin >> ruch_uzytkownika;

	if (ruch_uzytkownika == 'm' || ruch_uzytkownika == 'M') {
		system("cls");
		menu(ruch_uzytkownika);
	}
}

void program(char& ruch_uzytkownika) {

	int rok[7][1000], miesiac[7][1000], dzien[7][1000];
	float open[7][1000], high[7][1000], low[7][1000], close[7][1000], volume[7][1000];
	float zaok_open[7][1000], zaok_high[7][1000], zaok_low[7][1000], zaok_close[7][1000];

	int rozszerzenie = 0;
	int indeks = -1;

	float najwiekszy_high, najmniejszy_low;
	char obiekty[20][50][100];
	char os_wartosci[10][100];
	bool wartosci[100];
	bool zielony[200][200], kolory[200][200];
	bool kolory_os_wartosci[100], niebieski_wartosci[100];
	int nr_swiecy = 1;
	int dzien_os_czasu[2], miesiac_os_czasu[2], rok_os_czasu[2];

	int wysokosc, szerokosc = 0, odczyty, pierwszy_rok, pierwszy_miesiac, pierwszy_dzien, ostatni_rok, ostatni_miesiac, ostatni_dzien;
	int indeksy_konca_poczatku[2], rozszerzenia_konca_poczatku[2];
	bool blad_konfiguracji = false;

	int odstep = 9;
	float podzialka;

	napis_Gielda(odstep);

	powitanie();

	menu(ruch_uzytkownika);

	if (ruch_uzytkownika == 'g' || ruch_uzytkownika == 'G') {
		wczytywanie_danych(rok, miesiac, dzien, open, high, low, close, volume, rozszerzenie, indeks, zaok_open, zaok_high, zaok_low, zaok_close);

		konfiguracja(wysokosc, szerokosc, odczyty, pierwszy_rok, pierwszy_miesiac, pierwszy_dzien, ostatni_rok, ostatni_miesiac, ostatni_dzien, rok, miesiac, dzien, indeksy_konca_poczatku, rozszerzenia_konca_poczatku, blad_konfiguracji);

		if (blad_konfiguracji == false) {
			uzupelnianie_wykresu(open, high, low, close, volume, zaok_open, zaok_high, zaok_low, zaok_close, zielony, kolory, obiekty, wysokosc, szerokosc, odczyty, indeksy_konca_poczatku, rozszerzenia_konca_poczatku, os_wartosci, wartosci, nr_swiecy, podzialka, kolory_os_wartosci, niebieski_wartosci, najwiekszy_high, najmniejszy_low, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, rok, miesiac, dzien, odstep);
			napis_Gielda(odstep);
			rysowanie_wykresu(obiekty, kolory, zielony, wysokosc, szerokosc, os_wartosci, nr_swiecy, podzialka, najwiekszy_high, najmniejszy_low, kolory_os_wartosci, niebieski_wartosci, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, odczyty);

			ostatnia_tabelka(ruch_uzytkownika, szerokosc, odstep);

			while (ruch_uzytkownika == 'a' || ruch_uzytkownika == 'd' || ruch_uzytkownika == 'A' || ruch_uzytkownika == 'D' || ruch_uzytkownika == 'i' || ruch_uzytkownika == 'I') {
				if (ruch_uzytkownika == 'a' || ruch_uzytkownika == 'A') {
					if (nr_swiecy - 1 <= 0) {
						nr_swiecy = szerokosc;

						system("cls");
						uzupelnianie_wykresu(open, high, low, close, volume, zaok_open, zaok_high, zaok_low, zaok_close, zielony, kolory, obiekty, wysokosc, szerokosc, odczyty, indeksy_konca_poczatku, rozszerzenia_konca_poczatku, os_wartosci, wartosci, nr_swiecy, podzialka, kolory_os_wartosci, niebieski_wartosci, najwiekszy_high, najmniejszy_low, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, rok, miesiac, dzien, odstep);
						napis_Gielda(odstep);
						rysowanie_wykresu(obiekty, kolory, zielony, wysokosc, szerokosc, os_wartosci, nr_swiecy, podzialka, najwiekszy_high, najmniejszy_low, kolory_os_wartosci, niebieski_wartosci, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, odczyty);
						ostatnia_tabelka(ruch_uzytkownika, szerokosc, odstep);
					}
					else {
						nr_swiecy -= 1;

						system("cls");
						uzupelnianie_wykresu(open, high, low, close, volume, zaok_open, zaok_high, zaok_low, zaok_close, zielony, kolory, obiekty, wysokosc, szerokosc, odczyty, indeksy_konca_poczatku, rozszerzenia_konca_poczatku, os_wartosci, wartosci, nr_swiecy, podzialka, kolory_os_wartosci, niebieski_wartosci, najwiekszy_high, najmniejszy_low, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, rok, miesiac, dzien, odstep);
						napis_Gielda(odstep);
						rysowanie_wykresu(obiekty, kolory, zielony, wysokosc, szerokosc, os_wartosci, nr_swiecy, podzialka, najwiekszy_high, najmniejszy_low, kolory_os_wartosci, niebieski_wartosci, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, odczyty);
						ostatnia_tabelka(ruch_uzytkownika, szerokosc, odstep);
					}
				}
				else if (ruch_uzytkownika == 'd' || ruch_uzytkownika == 'D') {
					if (nr_swiecy + 1 > szerokosc) {
						nr_swiecy = 1;

						system("cls");
						uzupelnianie_wykresu(open, high, low, close, volume, zaok_open, zaok_high, zaok_low, zaok_close, zielony, kolory, obiekty, wysokosc, szerokosc, odczyty, indeksy_konca_poczatku, rozszerzenia_konca_poczatku, os_wartosci, wartosci, nr_swiecy, podzialka, kolory_os_wartosci, niebieski_wartosci, najwiekszy_high, najmniejszy_low, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, rok, miesiac, dzien, odstep);
						napis_Gielda(odstep);
						rysowanie_wykresu(obiekty, kolory, zielony, wysokosc, szerokosc, os_wartosci, nr_swiecy, podzialka, najwiekszy_high, najmniejszy_low, kolory_os_wartosci, niebieski_wartosci, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, odczyty);
						ostatnia_tabelka(ruch_uzytkownika, szerokosc, odstep);
					}
					else {
						nr_swiecy += 1;

						system("cls");
						uzupelnianie_wykresu(open, high, low, close, volume, zaok_open, zaok_high, zaok_low, zaok_close, zielony, kolory, obiekty, wysokosc, szerokosc, odczyty, indeksy_konca_poczatku, rozszerzenia_konca_poczatku, os_wartosci, wartosci, nr_swiecy, podzialka, kolory_os_wartosci, niebieski_wartosci, najwiekszy_high, najmniejszy_low, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, rok, miesiac, dzien, odstep);
						napis_Gielda(odstep);
						rysowanie_wykresu(obiekty, kolory, zielony, wysokosc, szerokosc, os_wartosci, nr_swiecy, podzialka, najwiekszy_high, najmniejszy_low, kolory_os_wartosci, niebieski_wartosci, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, odczyty);
						ostatnia_tabelka(ruch_uzytkownika, szerokosc, odstep);
					}
				}

				if (ruch_uzytkownika == 'i' || ruch_uzytkownika == 'I') {
					std::cout << "Wpisz indeks swiecy: " << std::endl;
					std::cin >> nr_swiecy;

					while (nr_swiecy < 1 || nr_swiecy > szerokosc) {
						std::cout << "Indeks swiecy musi byc liczba z przedzialu od 1 do " << szerokosc << std::endl;
						std::cin >> nr_swiecy;
					}

					system("cls");
					uzupelnianie_wykresu(open, high, low, close, volume, zaok_open, zaok_high, zaok_low, zaok_close, zielony, kolory, obiekty, wysokosc, szerokosc, odczyty, indeksy_konca_poczatku, rozszerzenia_konca_poczatku, os_wartosci, wartosci, nr_swiecy, podzialka, kolory_os_wartosci, niebieski_wartosci, najwiekszy_high, najmniejszy_low, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, rok, miesiac, dzien, odstep);
					napis_Gielda(odstep);
					rysowanie_wykresu(obiekty, kolory, zielony, wysokosc, szerokosc, os_wartosci, nr_swiecy, podzialka, najwiekszy_high, najmniejszy_low, kolory_os_wartosci, niebieski_wartosci, dzien_os_czasu, miesiac_os_czasu, rok_os_czasu, odczyty);
					ostatnia_tabelka(ruch_uzytkownika, szerokosc, odstep);
				}
			}
		}

		else {
			ostatnia_tabelka(ruch_uzytkownika, szerokosc, odstep);
		}
	}

	system("cls");
}



int main() {
	char ruch_uzytkownika = 'w';

	while (ruch_uzytkownika != 'q' && ruch_uzytkownika != 'Q') {
		program(ruch_uzytkownika);
	}

	return 0;
}