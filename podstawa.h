#pragma once

#include "pomocne_funkcje.h"

typedef fp_t punkt_czasu;

struct punkt_przestrzeni{
	fp_t x = FP_ZERO;
	fp_t y = FP_ZERO;
	fp_t z = FP_ZERO;

	void operator+=(const punkt_przestrzeni& drugi){
		x += drugi.x;
		y += drugi.y;
		z += drugi.z;
	}

	static friend punkt_przestrzeni operator-(const punkt_przestrzeni& P1, const punkt_przestrzeni& P2){
		return punkt_przestrzeni(P1.x - P2.x, P1.y - P2.y, P1.z - P2.z);
	}

	static friend punkt_przestrzeni operator+(const punkt_przestrzeni& P1, const punkt_przestrzeni& P2) {
		return punkt_przestrzeni(P1.x + P2.x, P1.y + P2.y, P1.z + P2.z);
	}

	static friend punkt_przestrzeni operator*(const punkt_przestrzeni& P, const fp_t& s) {
		return punkt_przestrzeni(P.x * s, P.y * s, P.z * s);
	}

	static friend punkt_przestrzeni operator/(const punkt_przestrzeni& P, const fp_t& s) {
		return punkt_przestrzeni(P.x / s, P.y / s, P.z / s);
	}

	fp_t norma() const {
		return sqrt(x*x+y*y+z*z);
	}
};

fp_t dystans(const punkt_przestrzeni& P1, const punkt_przestrzeni& P2){
	return (P1-P2).norma();
}

typedef punkt_przestrzeni vec_predkosci;
typedef punkt_przestrzeni vec_sily;

struct punkt_historyczny : punkt_przestrzeni{
	punkt_czasu t = FP_ZERO;

	static friend vec_predkosci predkosc(const punkt_historyczny P1, const punkt_historyczny P2){
		ASSERT_Z_ERROR_MSG(P2.t > P1.t, "P2 powinien byc pozniej niz P1\n");

		fp_t dt = P2.t - P1.t;
		return (P2 - P1)/dt;
	}
};

struct obiekt{
	std::vector<punkt_historyczny> przebyta_trasa;

	punkt_historyczny gdzie_byl(punkt_czasu t, bool* czy_poprawny = nullptr) const {
		if(t < gdzie_zaczol().t){ //zapytanie o nieznana przeszlosc
			if(czy_poprawny != nullptr) *czy_poprawny &= false;
			return punkt_historyczny();
		}

		if(t > gdzie_jest().t) { //zapytanie o przyszlosc
			if(czy_poprawny != nullptr) *czy_poprawny &= false;
			return punkt_historyczny();
		}

		//uint32_t idx_punktu_tuz_przed = przebyta_trasa.size() - 1;
		//while(!(t >= przebyta_trasa[idx_punktu_tuz_przed].t)) idx_punktu_tuz_przed--;

		//if(idx_punktu_tuz_przed == (przebyta_trasa.size() - 1)){
		//	return gdzie_jest();
		//}

		int32_t idx_punktu_tuz_przed = przebyta_trasa.size() - 1;
		int32_t idx_punktu_tuz_po = przebyta_trasa.size() - 2;

		const punkt_historyczny* tuz_przed = &(przebyta_trasa[idx_punktu_tuz_przed]);
		const punkt_historyczny* tuz_po = &(przebyta_trasa[idx_punktu_tuz_po]);

		fp_t roznica_czasow = tuz_po->t - tuz_przed->t;
		fp_t ile_po_przed = t - tuz_przed->t;
		fp_t jak_bardzo_po = ile_po_przed / roznica_czasow;

		while(!((t >= tuz_przed->t) && (t <= tuz_po->t))){
			// roznica_czasow * (idx - idx_punktu_tuz_przed) + tuz_przed->t = t
			// (t - tuz_przed->t) / roznica_czasow = idx - idx_punktu_tuz_przed
			// idx = (t - tuz_przed->t) / roznica_czasow + idx_punktu_tuz_przed

			fp_t estymowany_indeks = jak_bardzo_po + idx_punktu_tuz_przed;

			idx_punktu_tuz_przed = int32_t(floor(estymowany_indeks));
			idx_punktu_tuz_po = idx_punktu_tuz_przed + 1;
			
			if(idx_punktu_tuz_po >= (int32_t)przebyta_trasa.size()){
				idx_punktu_tuz_po = przebyta_trasa.size() - 1;
				idx_punktu_tuz_przed = idx_punktu_tuz_po - 1;
			}

			if (idx_punktu_tuz_przed < 0) {
				idx_punktu_tuz_przed = 0;
				idx_punktu_tuz_po = 1;
			}

			tuz_przed = &(przebyta_trasa[idx_punktu_tuz_przed]);
			tuz_po = &(przebyta_trasa[idx_punktu_tuz_po]);

			roznica_czasow = tuz_po->t - tuz_przed->t; // / 1
			ile_po_przed = t - tuz_przed->t;
			jak_bardzo_po = ile_po_przed / roznica_czasow;

			// bo ma³e b³êdy zmiennoprzecinkowe
			if ((ile_po_przed >= roznica_czasow * -0.001f) && ile_po_przed < 0.0f) {
				idx_punktu_tuz_przed--;
				idx_punktu_tuz_po = idx_punktu_tuz_przed + 1;

				tuz_przed = &(przebyta_trasa[idx_punktu_tuz_przed]);
				tuz_po = &(przebyta_trasa[idx_punktu_tuz_po]);

				roznica_czasow = tuz_po->t - tuz_przed->t; // / 1
				ile_po_przed = t - tuz_przed->t;
				jak_bardzo_po = ile_po_przed / roznica_czasow;
			}
		}

		// interpolacja miêdzy punktami danych
		return punkt_historyczny(*tuz_po * (jak_bardzo_po) + *tuz_przed * (FP_JEDEN - jak_bardzo_po), t);
	}

	punkt_historyczny gdzie_zaczol() const {
		return przebyta_trasa.front();
	}

	punkt_historyczny gdzie_jest() const {
		return przebyta_trasa.back();
	}

	punkt_historyczny gdzie_byl_przed_chwila() const {
		return przebyta_trasa[przebyta_trasa.size() - 2];
	}

	vec_predkosci predkosc_teraz() const {
		return predkosc(gdzie_byl_przed_chwila(), gdzie_jest());
	}

	bool czy_istnieje(){
		return !przebyta_trasa.empty();
	}

	void nastepna_lokalizacja(const punkt_historyczny P) {
		if(czy_istnieje()){
			ASSERT_Z_ERROR_MSG(P.t > gdzie_jest().t, "Nie tak dziala czas\n");
			ASSERT_Z_ERROR_MSG((predkosc(gdzie_jest(), P).norma() <= c), "Szybsze niz swiatlo?\n");
		}
		przebyta_trasa.push_back(P);
	}
	
	void wyczysc(){
		przebyta_trasa.resize(0);
	}
};

punkt_czasu w_jakim_czasie_widzi(const punkt_historyczny& lokalizacja_obserwujaca, const obiekt& obserwowany, bool* czy_poprawne = nullptr) {
	// czas_obserwawany taki ¿e:
	// c * (czas_obserwujacy - czas_obserwowany) = dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany))
	//--------------------------------------------------------------------------
	// c * czas_obserwujacy - c * czas_obserwowany = dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany))
	//						- c * czas_obserwowany = dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany)) - c * czas_obserwujacy
	//							  czas_obserwowany = -1/c * dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany)) + czas_obserwujacy
	//									^																					^
	//									|																					|
	//					   To samo      +-----------------------------------------------------------------------------------+


	
	// Czy to ma sens? W sumie nie wiem. Wydaje siê dzia³aæ. To nie ma sensu.
	
	#if 0	
	punkt_czasu czas_obserwowany = lokalizacja_obserwujaca.t - (FP_JEDEN/c) * dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(lokalizacja_obserwujaca.t, czy_poprawne));

	for(uint32_t i = 0; i < 100; i++){
		czas_obserwowany = lokalizacja_obserwujaca.t - (FP_JEDEN / c) * dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany, czy_poprawne));
	}

	return czas_obserwowany;
	#endif

	// Podejœcie 2

	fp_t odleglosc_przestrzena = dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(lokalizacja_obserwujaca.t, czy_poprawne));
	
	punkt_czasu opoznienie_czasu = odleglosc_przestrzena / c;

	fp_t odleglosc_w_czasie = c * opoznienie_czasu;
	
	punkt_czasu czas_obserwowany = lokalizacja_obserwujaca.t - opoznienie_czasu;

	odleglosc_przestrzena = dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany, czy_poprawne));

	while( (abs(odleglosc_przestrzena - odleglosc_w_czasie) > 0.0001f)
			&& *czy_poprawne ){

		opoznienie_czasu = odleglosc_przestrzena / c;

		odleglosc_w_czasie = c * opoznienie_czasu;

		czas_obserwowany = lokalizacja_obserwujaca.t - opoznienie_czasu;

		odleglosc_przestrzena = dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany, czy_poprawne));
	}

	//odleglosc_w_czasie == dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany, czy_poprawne))

	//for (uint32_t i = 0; i < 100; i++) {
	//	czas_obserwowany = lokalizacja_obserwujaca.t - (FP_JEDEN / c) * dystans(lokalizacja_obserwujaca, obserwowany.gdzie_byl(czas_obserwowany, czy_poprawne));
	//}

	return czas_obserwowany;

}

punkt_historyczny gdzie_widzi(const punkt_historyczny& lokalizacja_obserwujaca, const obiekt& obserwowany, bool* czy_poprawne = nullptr){
	punkt_historyczny ret = obserwowany.gdzie_byl(w_jakim_czasie_widzi(lokalizacja_obserwujaca, obserwowany, czy_poprawne));
	ret.t = lokalizacja_obserwujaca.t;
	if(!(*czy_poprawne)){
		ret = punkt_historyczny(punkt_przestrzeni(FP_NAN, FP_NAN, FP_NAN), FP_NAN);
	}
	return ret;
}

obiekt jak_widzi_obserwator_statyczny(const obiekt& obserwowany, const punkt_przestrzeni& obserwator, fp_t dt){
	obiekt zaaobserwowany;

	punkt_historyczny odbiornik(obserwator, obserwowany.gdzie_zaczol().t);

	while (true) {
		bool czy_poprawne = true;
		punkt_historyczny obserwacja = gdzie_widzi(odbiornik, obserwowany, &czy_poprawne);
		odbiornik.t += dt;

		if (czy_poprawne) {
			zaaobserwowany.nastepna_lokalizacja(obserwacja);
			break;
		}
	}

	while(true){
		bool czy_poprawne = true;
		punkt_historyczny obserwacja = gdzie_widzi(odbiornik, obserwowany, &czy_poprawne);
		odbiornik.t += dt;

		if(czy_poprawne){
			zaaobserwowany.nastepna_lokalizacja(obserwacja);
		} else {
			break;
		}
	}

	return zaaobserwowany;	
}

obiekt jak_widzi_obserwator_ruchomy(const obiekt& obserwowany, const obiekt& obserwator) {
	obiekt zaaobserwowany;

	for(auto& odbiornik : obserwator.przebyta_trasa){
		bool czy_poprawne = true;
		punkt_historyczny obserwacja = gdzie_widzi(odbiornik, obserwowany, &czy_poprawne);
		if(czy_poprawne) zaaobserwowany.nastepna_lokalizacja(obserwacja);
	}

	return zaaobserwowany;
}

typedef punkt_historyczny wartosc_pola;

struct pole : punkt_historyczny{
	wartosc_pola wartosc;
};

typedef std::vector<pole> fala;

fala jak_widzi_przestrzen(const obiekt& obserwowany, punkt_przestrzeni P_start, punkt_przestrzeni P_koniec, fp_t dP, punkt_czasu t){
	fala obserwacje;

	for(fp_t x = P_start.x; x <= P_koniec.x; x += dP){
		for (fp_t y = P_start.y; y <= P_koniec.y; y += dP) {
			for (fp_t z = P_start.z; z <= P_koniec.z; z += dP) {
				bool czy_poprawny = true;
				
				punkt_historyczny lokalizacja_obserwujaca = punkt_historyczny(x, y, z, t);
				pole obserwacja = pole(lokalizacja_obserwujaca, gdzie_widzi(lokalizacja_obserwujaca, obserwowany, &czy_poprawny));
		
				obserwacje.push_back(obserwacja);
			}
		}
	}

	return obserwacje;	
}

struct obiekt_kinetyczny : obiekt{
	vec_predkosci v;

	void krok_kinematyki(const fp_t dt){
		punkt_historyczny nastepny = gdzie_jest();
		nastepny.x += v.x * dt;
		nastepny.y += v.y * dt;
		nastepny.z += v.z * dt;
		nastepny.t += dt;

		nastepna_lokalizacja(nastepny);

	}
};


namespace stale_fizyczne{
	fp_t wsp_sprez = 0.01f;

}

struct atrybuty_fizyczne{
	fp_t masa = FP_JEDEN;
	fp_t ladunek = FP_ZERO;
};

struct obiekt_fizyczny : obiekt_kinetyczny, atrybuty_fizyczne {
	void zadzialaj_sila(const vec_sily sila, const fp_t dt) {
		fp_t temp = dt / masa;
		v.x += sila.x * temp;
		v.y += sila.y * temp;
		v.z += sila.z * temp;
	}
};

vec_sily sila(
		const punkt_przestrzeni pozycja_obiektu_1, const atrybuty_fizyczne atrybuty_obiektu_1,
		const punkt_przestrzeni pozycja_obiektu_2, const atrybuty_fizyczne atrybuty_obiektu_2){
	// si³a dzia³aj¹ca na obiekt 1

	vec_sily ret = (pozycja_obiektu_2 - pozycja_obiektu_1) * stale_fizyczne::wsp_sprez;

	return ret;
}

vec_sily sila_zewnetrzna(
	const punkt_przestrzeni pozycja_obiektu, const atrybuty_fizyczne atrybuty_obiektu) {
	// si³a dzia³aj¹ca na obiekt 1

	vec_sily ret;

	return ret;
}

struct kinematyka {
	std::vector<obiekt_fizyczny> obiekty;

	punkt_czasu dt = 0.01f;	
	
	kinematyka(uint8_t liczba_obiektow)
	:obiekty(liczba_obiektow){}
	
	void symuluj(punkt_czasu delta_t){
		punkt_czasu t = obiekty[0].gdzie_jest().t;
		punkt_czasu t_docelowe = t + delta_t;

		std::vector<vec_sily> dzialajace_sily(obiekty.size());
		
		while(t < t_docelowe){
	
			// liczenie dzia³aj¹cych si³
			for(uint32_t o1 = 0; o1 < obiekty.size(); o1++){

				const obiekt_fizyczny& obiekt1 = obiekty[o1];
				const punkt_historyczny lokalizacja_obserwujaca = obiekt1.gdzie_jest();

				vec_sily& dzialajaca_sila = dzialajace_sily[o1];
				dzialajaca_sila = sila_zewnetrzna(obiekt1.gdzie_jest(), obiekt1);

				for (uint32_t o2 = 0; o2 < obiekty.size(); o2++) {
					const obiekt_fizyczny& obiekt2 = obiekty[o2];

					bool czy_poprawne = true;
					punkt_historyczny obserwowana_lokalizacja = gdzie_widzi(lokalizacja_obserwujaca, obiekt2, &czy_poprawne);
					
					ASSERT_Z_ERROR_MSG(czy_poprawne, "Nie da sie policzyc przyszlosci\n");

					dzialajaca_sila += sila(obiekt1.gdzie_jest(), obiekt1, obserwowana_lokalizacja, obiekt2);

				}
			}

			// u¿ycie dzia³aj¹cych si³
			for (uint32_t o1 = 0; o1 < obiekty.size(); o1++) {

				obiekt_fizyczny& obiekt1 = obiekty[o1];
				obiekt1.zadzialaj_sila(dzialajace_sily[o1], dt);
			}
			
			// kroki kinematyki
			for (uint32_t o1 = 0; o1 < obiekty.size(); o1++) {

				obiekt_fizyczny& obiekt1 = obiekty[o1];
				obiekt1.krok_kinematyki(dt);
			}

			t += dt;
		}
	}

	void init(){
		for (uint32_t o1 = 0; o1 < obiekty.size(); o1++) {

			obiekt_fizyczny& obiekt1 = obiekty[o1];
			obiekt1.v = obiekt1.predkosc_teraz();
		}
	}

};

