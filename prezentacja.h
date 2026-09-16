#pragma once

bool pokaz_punkty = false;
bool pokaz_x = true;
bool pokaz_y = false;
bool pokaz_z = false;

void pokaz_1d_wzgledem_czasu_pozycje(const obiekt& pokazywany, std::string nazwa = "obiekt") {

	if (pokaz_x) ImPlot::PlotLine((nazwa + " x").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].x), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
	if (pokaz_y) ImPlot::PlotLine((nazwa + " y").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].y), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
	if (pokaz_z) ImPlot::PlotLine((nazwa + " z").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].z), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));

	if (pokaz_punkty) {
		if (pokaz_x) ImPlot::PlotScatter((nazwa + " x").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].x), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
		if (pokaz_y) ImPlot::PlotScatter((nazwa + " y").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].y), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
		if (pokaz_z) ImPlot::PlotScatter((nazwa + " z").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].z), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
	}
}

void pokaz_1d_wzgledem_czasu_predkosci(const obiekt& pokazywany, std::string nazwa = "obiekt") {

	if (pokaz_x) ImPlot::PlotLine((nazwa + " x").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].v.x), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
	if (pokaz_y) ImPlot::PlotLine((nazwa + " y").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].v.y), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
	if (pokaz_z) ImPlot::PlotLine((nazwa + " z").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].v.z), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));

	if (pokaz_punkty) {
		if (pokaz_x) ImPlot::PlotScatter((nazwa + " x").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].v.x), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
		if (pokaz_y) ImPlot::PlotScatter((nazwa + " y").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].v.y), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
		if (pokaz_z) ImPlot::PlotScatter((nazwa + " z").c_str(), &(pokazywany.przebyta_trasa[0].t), &(pokazywany.przebyta_trasa[0].v.z), pokazywany.przebyta_trasa.size(), 0, 0, sizeof(punkt_historyczny));
	}
}

void pokaz_1d_wzgledem_przestrzeni(const fala& pokazywana, std::string nazwa = "obiekt", uint8_t os_badana = 0) {
	// oœ badana:
	// 0 - x 
	// 1 - y
	// 2 - z

	if (pokaz_x) ImPlot::PlotLine((nazwa + " x").c_str(), &(pokazywana[0].x) + os_badana, &(pokazywana[0].wartosc.x), pokazywana.size(), 0, 0, sizeof(pole));
	if (pokaz_y) ImPlot::PlotLine((nazwa + " y").c_str(), &(pokazywana[0].x) + os_badana, &(pokazywana[0].wartosc.y), pokazywana.size(), 0, 0, sizeof(pole));
	if (pokaz_z) ImPlot::PlotLine((nazwa + " z").c_str(), &(pokazywana[0].x) + os_badana, &(pokazywana[0].wartosc.z), pokazywana.size(), 0, 0, sizeof(pole));

	if (pokaz_punkty) {
		if (pokaz_x) ImPlot::PlotScatter((nazwa + " x").c_str(), &(pokazywana[0].x) + os_badana, &(pokazywana[0].wartosc.x), pokazywana.size(), 0, 0, sizeof(pole));
		if (pokaz_y) ImPlot::PlotScatter((nazwa + " y").c_str(), &(pokazywana[0].x) + os_badana, &(pokazywana[0].wartosc.y), pokazywana.size(), 0, 0, sizeof(pole));
		if (pokaz_z) ImPlot::PlotScatter((nazwa + " z").c_str(), &(pokazywana[0].x) + os_badana, &(pokazywana[0].wartosc.z), pokazywana.size(), 0, 0, sizeof(pole));
	}
}

struct pokaz_interaktywny{
	std::vector<punkt_czasu> punkty_czasu;

	punkt_czasu t_poczatek = fp_t(-30.0);
	punkt_czasu t_start = fp_t(10.0);


	kinematyka symulator;

	obiekt& obiekt1;
	obiekt& obiekt2;

	fp_t v_x1 = 0.0f;
	fp_t v_y1 = FP_ZERO;
	fp_t v_z1 = FP_ZERO;

	fp_t v_x2 = 0.0f;
	fp_t v_y2 = FP_ZERO;
	fp_t v_z2 = FP_ZERO;



	obiekt _1patrzyna2;
	obiekt _2patrzyna1;


	punkt_przestrzeni obserwator_statyczny = punkt_przestrzeni(FP_ZERO, FP_ZERO, FP_ZERO);

	obiekt obserwowany1;
	obiekt obserwowany2;



	punkt_czasu ogladany_czas = -25.0f;
	fp_t zasieg_ogladania = 40.0f;

	fala jak_widza_obiekt_1_pozycje_wzdluz_x;
	fala jak_widza_obiekt_1_pozycje_wzdluz_y;
	fala jak_widza_obiekt_1_pozycje_wzdluz_z;

	fala jak_widza_obiekt_2_pozycje_wzdluz_x;
	fala jak_widza_obiekt_2_pozycje_wzdluz_y;
	fala jak_widza_obiekt_2_pozycje_wzdluz_z;

	pokaz_interaktywny()
	: symulator(2), obiekt1(symulator.obiekty[0]), obiekt2(symulator.obiekty[1]) {
		policz_trase_stala();
	}

	void policz_trase_stala(){
		obiekt1.wyczysc();
		obiekt2.wyczysc();

		for (uint32_t i = 0; i <= 1000; i++) {
			punkt_czasu t = t_poczatek + (fp_t(i) / fp_t(1000)) * (t_start - t_poczatek);

			obiekt1.nastepna_lokalizacja(
				punkt_historyczny(sin(v_x1 * t), sin(v_y1 * t), sin(v_z1 * t), t)
				//punkt_historyczny(10.0f, 0.0f, 0.0f, t)
			);
		}

		for (uint32_t i = 0; i <= 1000; i++) {
			punkt_czasu t = t_poczatek + (fp_t(i) / fp_t(1000)) * (t_start - t_poczatek);

			obiekt2.nastepna_lokalizacja(
				punkt_historyczny(sin(v_x2 * t), sin(v_y2 * t), sin(v_z2 * t), t)
				//punkt_historyczny(-10.0f, 0.0f, 0.0f, t)
			);
		}
		symulator.init();
		policz_widok_obiektow();
		policz_widok_obserwatora_statycznego();
		policz_jak_widzi_przestrzen();
	}

	void policz_widok_obiektow(){
		_1patrzyna2 = jak_widzi_obserwator_ruchomy(obiekt2, obiekt1);
		_2patrzyna1 = jak_widzi_obserwator_ruchomy(obiekt1, obiekt2);
	}

	void policz_widok_obserwatora_statycznego(){
		obserwowany1 = jak_widzi_obserwator_statyczny(obiekt1, obserwator_statyczny, 0.1f);
		obserwowany2 = jak_widzi_obserwator_statyczny(obiekt2, obserwator_statyczny, 0.1f);
	}

	void policz_jak_widzi_przestrzen(){
		jak_widza_obiekt_1_pozycje_wzdluz_x = jak_widzi_przestrzen(obiekt1,
			punkt_przestrzeni(-zasieg_ogladania, 0.0f, 0.0f) + obserwator_statyczny,
			punkt_przestrzeni(zasieg_ogladania, 0.0f, 0.0f) + obserwator_statyczny,
			0.1f, ogladany_czas);

		jak_widza_obiekt_1_pozycje_wzdluz_y = jak_widzi_przestrzen(obiekt1,
			punkt_przestrzeni(0.0f, -zasieg_ogladania, 0.0f) + obserwator_statyczny,
			punkt_przestrzeni(0.0f, zasieg_ogladania, 0.0f) + obserwator_statyczny,
			0.1f, ogladany_czas);

		jak_widza_obiekt_1_pozycje_wzdluz_z = jak_widzi_przestrzen(obiekt1,
			punkt_przestrzeni(0.0f, 0.0f, -zasieg_ogladania) + obserwator_statyczny,
			punkt_przestrzeni(0.0f, 0.0f, zasieg_ogladania) + obserwator_statyczny,
			0.1f, ogladany_czas);



		jak_widza_obiekt_2_pozycje_wzdluz_x = jak_widzi_przestrzen(obiekt2,
			punkt_przestrzeni(-zasieg_ogladania, 0.0f, 0.0f) + obserwator_statyczny,
			punkt_przestrzeni(zasieg_ogladania, 0.0f, 0.0f) + obserwator_statyczny,
			0.1f, ogladany_czas);

		jak_widza_obiekt_2_pozycje_wzdluz_y = jak_widzi_przestrzen(obiekt2,
			punkt_przestrzeni(0.0f, -zasieg_ogladania, 0.0f) + obserwator_statyczny,
			punkt_przestrzeni(0.0f, zasieg_ogladania, 0.0f) + obserwator_statyczny,
			0.1f, ogladany_czas);

		jak_widza_obiekt_2_pozycje_wzdluz_z = jak_widzi_przestrzen(obiekt2,
			punkt_przestrzeni(0.0f, 0.0f, -zasieg_ogladania) + obserwator_statyczny,
			punkt_przestrzeni(0.0f, 0.0f, zasieg_ogladania) + obserwator_statyczny,
			0.1f, ogladany_czas);
	}

	void pokaz_okno(){
		ImGui::Begin("Pokaz interaktywny");

		ImGui::Checkbox("Pokaz punkty", &pokaz_punkty); ImGui::SameLine();
		ImGui::Checkbox("Pokaz X", &pokaz_x); ImGui::SameLine();
		ImGui::Checkbox("Pokaz Y", &pokaz_y); ImGui::SameLine();
		ImGui::Checkbox("Pokaz Z", &pokaz_z);

		if(ImGui::Button("Przyszlosc")){
			symulator.symuluj(3.0f);
			policz_widok_obiektow();
			policz_widok_obserwatora_statycznego();
			policz_jak_widzi_przestrzen();
		}


		bool odswiez_trase = false; 
		odswiez_trase |= ImGui::SliderFloat3("Predkosc obiekt 1", &v_x1, 0.0f, c);
		odswiez_trase |= ImGui::SliderFloat3("Predkosc obiekt 2", &v_x2, 0.0f, c);

		if(odswiez_trase) policz_trase_stala();


		bool odswiez_obserwatora_statycznego = false;
		odswiez_obserwatora_statycznego |= ImGui::DragFloat3("Lokalizacja obserwatora statycznego", &obserwator_statyczny.x, 0.1f, 10000.0f, -10000.0f);

		if(odswiez_obserwatora_statycznego){
			policz_widok_obserwatora_statycznego();
			policz_jak_widzi_przestrzen();
		}

		bool odswiez_widok_przestrzeni = false;
		odswiez_widok_przestrzeni |= ImGui::DragFloat("Ogladany czas", &ogladany_czas, 0.1f, 10000.0f, -10000.0f);
		odswiez_widok_przestrzeni |= ImGui::DragFloat("Zasieg ogladania", &zasieg_ogladania, 0.1f, 10000.0f, -10000.0f);

		if(odswiez_widok_przestrzeni) policz_jak_widzi_przestrzen();

		if (ImPlot::BeginPlot("Wykresy pozycji", ImVec2(2000.0f, 350.0f))) {

			ImPlot::SetupAxes("czas", "pozycja");

			pokaz_1d_wzgledem_czasu_pozycje(obiekt1, "Trasa obiekt 1");
			pokaz_1d_wzgledem_czasu_pozycje(obiekt2, "Trasa obiekt 2");

			pokaz_1d_wzgledem_czasu_pozycje(obserwowany1, "Obserwator statyczny obiekt 1");
			pokaz_1d_wzgledem_czasu_pozycje(obserwowany2, "Obserwator statyczny obiekt 2");

			pokaz_1d_wzgledem_czasu_pozycje(_2patrzyna1, "obiekt 2 patrzy na 1");
			pokaz_1d_wzgledem_czasu_pozycje(_1patrzyna2, "obiekt 1 patrzy na 2");

			ImPlot::EndPlot();
		}

		if (ImPlot::BeginPlot("Wykresy predkosci", ImVec2(2000.0f, 350.0f))) {

			ImPlot::SetupAxes("czas", "predkosc");

			pokaz_1d_wzgledem_czasu_predkosci(obiekt1, "Predkosci obiekt 1");
			pokaz_1d_wzgledem_czasu_predkosci(obiekt2, "Predkosci obiekt 2");

			pokaz_1d_wzgledem_czasu_predkosci(obserwowany1, "Obserwator statyczny obiekt 1");
			pokaz_1d_wzgledem_czasu_predkosci(obserwowany2, "Obserwator statyczny obiekt 2");

			pokaz_1d_wzgledem_czasu_predkosci(_2patrzyna1, "obiekt 2 patrzy na 1");
			pokaz_1d_wzgledem_czasu_predkosci(_1patrzyna2, "obiekt 1 patrzy na 2");

			ImPlot::EndPlot();
		}

		if (ImPlot::BeginPlot("Wykresy pola", ImVec2(2000.0f, 350.0f))) {

			ImPlot::SetupAxes("pozycja", "wartosc");

			pokaz_1d_wzgledem_przestrzeni(jak_widza_obiekt_1_pozycje_wzdluz_x, "wzdluz x obiekt 1 pozycja", 0);
			pokaz_1d_wzgledem_przestrzeni(jak_widza_obiekt_1_pozycje_wzdluz_y, "wzdluz y obiekt 1 pozycja", 1);
			pokaz_1d_wzgledem_przestrzeni(jak_widza_obiekt_1_pozycje_wzdluz_z, "wzdluz z obiekt 1 pozycja", 2);

			pokaz_1d_wzgledem_przestrzeni(jak_widza_obiekt_2_pozycje_wzdluz_x, "wzdluz x obiekt 2 pozycja", 0);
			pokaz_1d_wzgledem_przestrzeni(jak_widza_obiekt_2_pozycje_wzdluz_y, "wzdluz y obiekt 2 pozycja", 1);
			pokaz_1d_wzgledem_przestrzeni(jak_widza_obiekt_2_pozycje_wzdluz_z, "wzdluz z obiekt 2 pozycja", 2);

			ImPlot::EndPlot();
		}

		ImGui::End();
	}


};


