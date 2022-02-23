//---------------------------------------------------------------------------

#include <vcl.h>
#include <System.DateUtils.hpp>
#pragma hdrstop

#include "mainForm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;

Etap etapy[] = {  {4,1}, {3,2}, {2,3}, {1,4}, {0,0} };

//---------------------------------------------------------------------------
__fastcall TformMain::TformMain(TComponent* Owner, TRYB eTryb)
	: TForm(Owner)
{
	m_eTryb = eTryb;
	pboxSiatka1->Width = COL_MAX * imagePuste->Width;
	pboxSiatka1->Height = ROW_MAX * imagePuste->Height;
	panelSiatka1->Width = pboxSiatka1->Width+8;
	panelSiatka1->Height = pboxSiatka1->Height+8;

	pboxSiatka2->Width = COL_MAX * imagePuste->Width;
	pboxSiatka2->Height = ROW_MAX * imagePuste->Height;
	panelSiatka2->Width = pboxSiatka2->Width+8;
	panelSiatka2->Height = pboxSiatka2->Height+8;

	labelGracz1->Caption = "Gracz 1";
	if(eTryb == TRYB_GRACZ)
		labelGracz2->Caption = "Gracz 2";
	else
		labelGracz2->Caption = "Komputer";

	m_bTrybGry = m_bTuraGracz1 = false;
	m_bKoniecGry = false;
	m_bSITrafilo = false;

	timerStart->Enabled = true;
}
//---------------------------------------------------------------------------

void TformMain::rysujSiatke(bool bSiatka1)
{
	TPaintBox *p = bSiatka1 ? pboxSiatka1 : pboxSiatka2;
	for (int x = 0; x < COL_MAX; x++)
		for (int y = 0; y < ROW_MAX; y++)
		{
			TImage *pimg = getImage(x, y, m_oSiatka);
			p->Canvas->Draw(x*pimg->Width, y*pimg->Height, pimg->Picture->Graphic);
		}
}
//---------------------------------------------------------------------------

void TformMain::rysuj(int x, int y, bool bSiatka1, STAN eStan)
{
	TImage *pimg = getImage(eStan);
	TPaintBox *p = bSiatka1 ? pboxSiatka1 : pboxSiatka2;
	p->Canvas->Draw(x*pimg->Width, y*pimg->Height, pimg->Picture->Graphic);
}
//---------------------------------------------------------------------------

void TformMain::rysujStatek(int x, int y, bool bSiatka1)
{
	rysuj(x, y, bSiatka1, STAN_STATEK);
}
//---------------------------------------------------------------------------

void TformMain::rysujPuste(int x, int y, bool bSiatka1)
{
	rysuj(x, y, bSiatka1, STAN_PUSTE);
}
//---------------------------------------------------------------------------

void TformMain::rysujTrafienie(int x, int y, bool bSiatka1)
{
	rysuj(x, y, bSiatka1, STAN_TRAFIENIE);
}
//---------------------------------------------------------------------------

void TformMain::rysujPudlo(int x, int y, bool bSiatka1)
{
	rysuj(x, y, bSiatka1, STAN_PUDLO);
}
//---------------------------------------------------------------------------

void TformMain::rozmiescStatki()
{
	m_bTrybRozmieszczania = true;
	m_iEtap = 0;
	Etap xBiezacyEtap = etapy[m_iEtap];
	m_iMaszt = xBiezacyEtap.iWielkoscStatku;
	m_iStatek = xBiezacyEtap.iLiczbaStatkow;
	btDalej->Enabled = !m_bTrybRozmieszczania;
	m_RoboczyStatek.clear();
	m_vStatki.clear();
	pokazInfo();

	if (m_iTuraRozmieszczania==2 && m_eTryb==TRYB_KOMPUTER)
	{
		btLosuj->Enabled = false;
		btCofnij->Enabled = false;
		timerSIStart->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TformMain::timerStartTimer(TObject *Sender)
{
	timerStart->Enabled = false;
	m_iTuraRozmieszczania = 1;
	rysujSiatke(true);
	rysujSiatke(false);
	rozmiescStatki();
}
//---------------------------------------------------------------------------

TImage* TformMain::getImage(STAN eStanKomorki)
{
	switch (eStanKomorki) {
		case STAN_PUSTE:
			return imagePuste;
		case STAN_PUDLO:
			return imagePudlo;
		case STAN_STATEK:
			return imageStatek;
		case STAN_TRAFIENIE:
			return imageTrafienie;
		default:
			return imagePuste;
	}
}
//---------------------------------------------------------------------------

TImage* TformMain::getImage(int x, int y, TSiatka& xSiatka)
{
	return getImage(xSiatka.getStan(x, y));
}
//---------------------------------------------------------------------------

int TformMain::szerokosc()
{
	return imagePuste->Width;
}

int TformMain::wysokosc()
{
	return imagePuste->Height;
}

void __fastcall TformMain::pboxSiatka1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if (m_bTrybGry && !m_bTuraGracz1 && m_eTryb==TRYB_GRACZ)
		sprawdzRuch(X, Y, true);
	else
	if (m_bTrybRozmieszczania && m_iTuraRozmieszczania==1)
		sprawdzKlik(X, Y, true);
}
//---------------------------------------------------------------------------

void __fastcall TformMain::pboxSiatka2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (m_bTrybGry && m_bTuraGracz1)
		sprawdzRuch(X, Y, false);
	else
	if (m_bTrybRozmieszczania && m_iTuraRozmieszczania==2 && m_eTryb==TRYB_GRACZ)
		sprawdzKlik(X, Y, false);
}
//---------------------------------------------------------------------------

void TformMain::sprawdzKlik(int X, int Y, bool bSiatka1)
{
	int x = X / szerokosc();
	int y = Y / wysokosc();
	labelX->Caption = IntToStr(x);
	labelY->Caption = IntToStr(y);

	if (czyKomorkaPoprawna(x, y))
	{
		if (m_RoboczyStatek.utworzModul(x, y))
		{
			rysujStatek(x, y, bSiatka1);
			nastepnyMaszt();
		}
	}
}
//---------------------------------------------------------------------------

bool TformMain::czyTuraSI()
{
	return !m_bTuraGracz1 && m_eTryb==TRYB_KOMPUTER;
}
//---------------------------------------------------------------------------

void TformMain::sprawdzRuch(int X, int Y, bool bSiatka1)
{
	int x = X / szerokosc();
	int y = Y / wysokosc();
	labelX->Caption = IntToStr(x);
	labelY->Caption = IntToStr(y);

	//sprawdzmy poprawnosc ruchu
	if (czyStrzalPoprawny(x, y, bSiatka1))
	{
		//sprawdzamy czy trafiono
		if (czyTrafienie(x, y, bSiatka1))
		{
			if (czyTuraSI())
			{
				m_bSITrafilo = true;
				m_xSITrafienie.x = x;
				m_xSITrafienie.y = y;
				m_xSIStatekRoboczy.utworzModul(x, y);
			}

			showWarning("TRAFIENIE!!!");
			przetwarzajTrafienie(x, y, bSiatka1);

			m_bKoniecGry = sprawdzKoniecGry(bSiatka1);
		}
		else
		{
			showWarning("PUD£O!!!");
			przetwarzajPudlo(x, y, bSiatka1);
			m_bTuraGracz1 = !m_bTuraGracz1;
			pokazInfo();
		}
	}

	if (m_bKoniecGry)
		koniecGry();
	else
	//aktywacja SI
	if (czyTuraSI())
		timerSI->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TformMain::timerSITimer(TObject *Sender)
{
	timerSI->Enabled = false;

	bool bStrzal = false;
	int x = -1, y = -1;

	//czy ostatnio bylo trafienie?
	if (m_bSITrafilo)
	{
		//jesli bylo trafienie, to badamy czy znany jest kierunek statku
		TStatki::KIERUNEK eKierunekTrafionegoStatku = m_xSIStatekRoboczy.getKierunek();

		if (eKierunekTrafionegoStatku != TStatki::BRAK)
		{
			std::vector<Modul> vGraniceStatku;
			if (m_xSIStatekRoboczy.getGraniceStatku(vGraniceStatku))
			{
				for (std::vector<Modul>::iterator it = vGraniceStatku.begin(); it != vGraniceStatku.end(); )
				{
					STAN eStan = m_oMaskaSiatki1.getStan(it->x, it->y);
					if (eStan != STAN_PUSTE)
						vGraniceStatku.erase(it);
					else
						++it;
				}

				if (vGraniceStatku.size()>0)
				{
					int iLiczba = rand() % vGraniceStatku.size();
					Modul xStrzal = vGraniceStatku[iLiczba];
					x = xStrzal.x;
					y = xStrzal.y;
					bStrzal = true;
				}
			}
		}
		else
		{
			//losujemy w okolicach gdzie trafilo
			int iPromien = 1;
			int iIloscMozliwychStrzalow = m_oMaskaSiatki1.mozliweStrzaly(m_xSITrafienie.x, m_xSITrafienie.y, iPromien);
			if (iIloscMozliwychStrzalow > 0)
			{
				int iLiczba = rand() % iIloscMozliwychStrzalow;
				bStrzal = m_oMaskaSiatki1.getStrzal(iLiczba, m_xSITrafienie.x, m_xSITrafienie.y, iPromien, x, y);
			}
			else
				m_bSITrafilo = false;
		}
	}
	else
	{
		//komputer losuje punkt z calej planszy (tylko wolne pola)
		int iLiczba = rand() % m_oMaskaSiatki1.policz(STAN_PUSTE);
		bStrzal = m_oMaskaSiatki1.getKomorka(STAN_PUSTE, iLiczba, x, y);
	}


	if (bStrzal)
		sprawdzRuch(x * szerokosc(), y * wysokosc(), true);
	else
		timerSI->Enabled = true;
}
//---------------------------------------------------------------------------

bool TformMain::sprawdzKoniecGry(bool bSiatka1)
{
	//sprawdzamy czy wszystkie statki s¹ zatopione
	std::vector<TStatki> *pvStatki = bSiatka1 ? &m_vStatkiGracz1 : &m_vStatkiGracz2;
	for (std::vector<TStatki>::iterator it = pvStatki->begin(); it != pvStatki->end(); it++)
	{
		if (!it->czyZatopiony())
			return false;
	}

return true;
}
//---------------------------------------------------------------------------

void TformMain::koniecGry()
{
	m_bTrybGry = false;
	pokazInfo();
	showWarning("GRATULACJE!!!");
}
//---------------------------------------------------------------------------

void TformMain::showWarning(AnsiString sKomunikat)
{
	labelWarning->Caption = sKomunikat;
	labelWarning->Visible = true;
	if (timerWarning->Enabled)
		timerWarning->Enabled = false;
	timerWarning->Enabled = true;
}
//---------------------------------------------------------------------------

bool TformMain::czyKomorkaPoprawna(int x, int y)
{
	if (m_oSiatka.getStan(x, y)!=STAN_PUSTE)
	{
		showWarning("Komórka zajêta!");
		return false;
	}

	if (m_oSiatka.czyStatekObok(x, y))
	{
		showWarning("Za blisko innego statku!");
		return false;
	}

	if (!m_RoboczyStatek.czyNowyModulOK(x, y))
	{
		showWarning("B³êdne rozmieszczenie!");
		return false;
	}

return true;
}
//---------------------------------------------------------------------------

bool TformMain::czyStrzalPoprawny(int X, int Y, bool bSiatka1)
{
	TSiatka *p = bSiatka1 ? &m_oMaskaSiatki1 : &m_oMaskaSiatki2;

	//czytam stan danej komórki
	STAN eStan = p->getStan(X, Y);

	//czy nie bylo strzalu ju¿ w to miejsce
	if (eStan != STAN_PUSTE)
	{
		showWarning("B³êdny strza³!");
		return false;
	}

return true;
}
//---------------------------------------------------------------------------

bool TformMain::czyTrafienie(int X, int Y, bool bSiatka1)
{
	TSiatka *pSiatka = bSiatka1 ? &m_oSiatkaGracz1 : &m_oSiatkaGracz2;

	//czytam stan danej komórki z siatki
	STAN eStan = pSiatka->getStan(X, Y);

	//czy trafienie
	if (eStan == STAN_STATEK)
		return true;

return false;
}
//---------------------------------------------------------------------------

void TformMain::przetwarzajTrafienie(int X, int Y, bool bSiatka1)
{
	TSiatka *pMaska = bSiatka1 ? &m_oMaskaSiatki1 : &m_oMaskaSiatki2;
	TSiatka *pSiatka = bSiatka1 ? &m_oSiatkaGracz1 : &m_oSiatkaGracz2;

	//zaktualizowac maske
	pMaska->setStan(X, Y, STAN_TRAFIENIE);
	//pokazac trafienie
	rysujTrafienie(X, Y, bSiatka1);

	//zaktualizowac liste statkow
	std::vector<TStatki> *pvStatki = bSiatka1 ? &m_vStatkiGracz1 : &m_vStatkiGracz2;
	TStatki *pTrafionyStatek = NULL;
	for (std::vector<TStatki>::iterator it = pvStatki->begin(); it != pvStatki->end(); it++)
	{
		if (it->czyTrafienie(X, Y))
		{
			pTrafionyStatek = &(*it);
			break;
		}
	}

	//ale... sprawdzamy czy zatopiono statek
	if (pTrafionyStatek->czyZatopiony())
	{
		showWarning("STATEK TONIE!!!");
		przetwarzajOtoczenieStatku(pMaska, pTrafionyStatek, bSiatka1);

		if (czyTuraSI())
		{
			m_bSITrafilo = false;
			m_xSITrafienie.clear();
			m_xSIStatekRoboczy.clear();
		}
	}
	else
		playTrafienie();
}
//---------------------------------------------------------------------------

void TformMain::przetwarzajOtoczenieStatku(TSiatka *pMaska, TStatki *pTrafionyStatek, bool bSiatka1)
{
	if (!pTrafionyStatek)
		return;

	for (int i = 0; i < pTrafionyStatek->getRozmiar(); i++)
	{
		//bierzemy modul
		Modul xModul = pTrafionyStatek->getModul(i);

		//czyscimy obszar dooko³a modu³u
		for (int iRow = -1; iRow < 2; iRow++)
			for (int iCol = -1; iCol < 2; iCol++)
			{
				//wyznaczam wspó³rzêdne danej komórki obszaru
				int iX = xModul.x + iCol;
				int iY = xModul.y + iRow;

				//sprawdzam czy komorka jest w siatce
				if (iX < 0 || iX >= COL_MAX || iY < 0 || iY >= ROW_MAX)
					continue;

				//sprawdzam czy komórka jest pusta
				if (pMaska->getStan(iX, iY) == STAN_PUSTE)
				{
					rysujPudlo(iX, iY, bSiatka1);
					pMaska->setStan(iX, iY, STAN_PUDLO);
				}
			}
	}
}
//---------------------------------------------------------------------------

void TformMain::przetwarzajPudlo(int X, int Y, bool bSiatka1)
{
	TSiatka *pMaska = bSiatka1 ? &m_oMaskaSiatki1 : &m_oMaskaSiatki2;

	//zaktualizowac maske
	pMaska->setStan(X, Y, STAN_PUDLO);
	//pokazac pud³o
	rysujPudlo(X, Y, bSiatka1);
	playPudlo();
}
//---------------------------------------------------------------------------

void TformMain::pokazKto(const AnsiString& sTekst)
{
	labelKto->Caption = sTekst;
}
//---------------------------------------------------------------------------

void TformMain::pokazCo(const AnsiString& sTekst)
{
	labelCo->Caption = sTekst;
}
//---------------------------------------------------------------------------

void TformMain::wskazKto()
{
	if(m_bTuraGracz1)
	{
		labelGracz2->Font->Size = 10;
		labelGracz2->Font->Color = clBlack;
		imageRedArrow->Visible = false;
		labelGracz1->Font->Size = 15;
		labelGracz1->Font->Color = clSkyBlue;
		imageBlueArrow->Visible = true;
	}
	else
	{
		labelGracz1->Font->Size = 10;
		labelGracz1->Font->Color = clBlack;
		imageBlueArrow->Visible = false;
		labelGracz2->Font->Size = 15;
		labelGracz2->Font->Color = clRed;
		imageRedArrow->Visible = true;
	}

}
//---------------------------------------------------------------------------

void TformMain::pokazInfo()
{
	if (m_bKoniecGry)
	{
		if (m_bTuraGracz1)
			pokazKto("****** ZWYCIÊ¯A GRACZ 1 !!! ******");
		else
			pokazKto(AnsiString().sprintf("****** ZWYCIÊ¯A %s !!! ******", m_eTryb==TRYB_GRACZ ? "GRACZ 2" : "KOMPUTER"));
	}
	else if (m_bTrybGry)
	{
		if (m_bTuraGracz1)
			pokazKto("GRACZ 1 - TWÓJ RUCH");
		else
			pokazKto(AnsiString().sprintf("%s - TWÓJ RUCH", m_eTryb==TRYB_GRACZ ? "GRACZ 2" : "KOMPUTER"));
		wskazKto();
	}
	else
	{
		if (m_iTuraRozmieszczania==1)
			pokazKto("GRACZ 1 - ROZMIEŒÆ STATKI");
		else
			pokazKto(AnsiString().sprintf("%s - ROZMIEŒÆ STATKI", m_eTryb==TRYB_GRACZ ? "GRACZ 2" : "KOMPUTER"));

		pokazCo(AnsiString("Rozmieœæ statek nr " + IntToStr(m_iStatek) + " o rozmiarze: " + IntToStr(etapy[m_iEtap].iWielkoscStatku)));
		labelLiczbaMasztow->Caption = IntToStr(m_iMaszt);
	}
}
//---------------------------------------------------------------------------

void TformMain::nastepnyMaszt()
{
	bool bPokazInfo = true;

	//nastêpny maszt
	if (m_iMaszt>1)
		m_iMaszt--;
	else
	//nastêpny statek
	if (m_iStatek>1)
	{
		m_iStatek--;
		m_iMaszt = etapy[m_iEtap].iWielkoscStatku;
		zapamietajStatek();
	}
	else
	{
		zapamietajStatek();

		//dane nowego etapu
		m_iEtap++;
		Etap xBiezacyEtap = etapy[m_iEtap];
		if (xBiezacyEtap.iLiczbaStatkow > 0)
		{
			m_iMaszt = xBiezacyEtap.iWielkoscStatku;
			m_iStatek = xBiezacyEtap.iLiczbaStatkow;
		}
		else
		{
			AnsiString sKto;
			if (m_iTuraRozmieszczania==1)
				sKto = "GRACZ 1";
			else
				sKto = (m_eTryb==TRYB_GRACZ) ? "GRACZ 2" : "KOMPUTER";
			pokazKto(sKto + " - Statki rozmieszczone pomyœlnie");
			pokazCo("");
			bPokazInfo = false;
			m_bTrybRozmieszczania = false;
			btDalej->Enabled = !m_bTrybRozmieszczania;
		}
	}

	if (bPokazInfo)
		pokazInfo();
}
//---------------------------------------------------------------------------

void TformMain::rozpocznijGre()
{
	pokazKto("GRA SIÊ ROZPOCZYNA!");
	pokazCo("");
	labelLiczbaMasztow->Visible = false;

	btLosuj->Visible = false;
	btCofnij->Visible = false;
	btDalej->Visible = false;
	labelMaszty->Visible = false;
	labelLiczbaMasztow->Visible = false;

	mySleep(1000);

	m_bTrybGry = true;
	m_bTuraGracz1 = true;
	pokazInfo();

	m_oMaskaSiatki1.clear();
	m_oMaskaSiatki2.clear();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btAnulujClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::timerWarningTimer(TObject *Sender)
{
	timerWarning->Enabled = false;
	labelWarning->Visible = false;
}
//---------------------------------------------------------------------------

void TformMain::zapamietajStatek()
{
	for (int i = 0; i < m_RoboczyStatek.getRozmiar(); i++)
	{
		Modul xModul = m_RoboczyStatek.getModul(i);
		m_oSiatka.setStan(xModul.x, xModul.y, STAN_STATEK);
	}

	m_vStatki.push_back(m_RoboczyStatek);
	m_RoboczyStatek.clear();
}
//---------------------------------------------------------------------------

void TformMain::usunStatekZSiatki()
{
	for (int i = 0; i < m_RoboczyStatek.getRozmiar(); i++)
	{
		Modul xModul = m_RoboczyStatek.getModul(i);
		m_oSiatka.setStan(xModul.x, xModul.y, STAN_PUSTE);
	}
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btCofnijClick(TObject *Sender)
{
	if (!m_bTrybRozmieszczania)
	{
		m_bTrybRozmieszczania = true;
		btDalej->Enabled = !m_bTrybRozmieszczania;
	}

	//jeœli statek roboczy jest jeszcze niezdefiniowany, to pobieram ostatni statek z listy
	if (m_RoboczyStatek.getRozmiar()==0 && m_vStatki.size()>0)
	{
		m_RoboczyStatek = m_vStatki[m_vStatki.size()-1];
		m_vStatki.pop_back();
		usunStatekZSiatki();

		//ustawiæ etap, rozmiar statku i iloœæ masztów
		//czy cofn¹æ etap - albo jest to ju¿ koniec rozmieszczania lub pocz¹tek nowego etapu
		if (etapy[m_iEtap].iLiczbaStatkow==0 || etapy[m_iEtap].iLiczbaStatkow == m_iStatek)
		{
			m_iEtap--;
			m_iStatek = 1;
		}
		else
			m_iStatek++;

		m_iMaszt = 0;
	}

	Modul xUndoModul;
	//pobierz i usuñ ostatni modul ze statku roboczego
	if (m_RoboczyStatek.pobierzOstatni(xUndoModul))
	{
		//wizualizacja pustego elementu
		rysujPuste(xUndoModul.x, xUndoModul.y, m_iTuraRozmieszczania==1);

		//przywróæ liczbê masztów
		m_iMaszt++;

		pokazInfo();
		showWarning("Usuniêto maszt");
	}
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btDalejClick(TObject *Sender)
{
	//zapamiêtaj ustawienie dla gracza wg tury fazy rozmieszczania
	if (m_iTuraRozmieszczania==1)
	{
		m_oSiatkaGracz1 = m_oSiatka;
		m_vStatkiGracz1.clear();
		for (std::vector<TStatki>::iterator it = m_vStatki.begin(); it != m_vStatki.end(); it++)
			m_vStatkiGracz1.push_back(*it);
	}
	else
	{
		m_oSiatkaGracz2 = m_oSiatka;
		m_vStatkiGracz2.clear();
		for (std::vector<TStatki>::iterator it = m_vStatki.begin(); it != m_vStatki.end(); it++)
			m_vStatkiGracz2.push_back(*it);
	}
	m_oSiatka.clear();

	rysujSiatke(m_iTuraRozmieszczania==1);

	if (m_iTuraRozmieszczania==2)
		rozpocznijGre();
	else
	{
		m_iTuraRozmieszczania++;
		//jesli tryb z drugim graczem, to rozmieszczenie drugi gracz
		if (m_eTryb==TRYB_GRACZ)
		{
			rozmiescStatki();
		}
		//jeœli tryb z komputerem, to losowe rozmieszczenie
		else if (m_eTryb==TRYB_KOMPUTER)
		{
			rozmiescStatki();
		}
		else
			throw Exception(AnsiString().sprintf("Nieznany tryb %d!", (int)m_eTryb));
	}
}
//---------------------------------------------------------------------------

void __fastcall TformMain::timerSIStartTimer(TObject *Sender)
{
	timerSIStart->Enabled = false;
	pokazCo("Komputer: Rozmieszczam moje statki...");

	losujStatki(false);
	pokazCo("Komputer: Zaczynajmy!");
	mySleep(2000);
	pokazCo("");
}
//---------------------------------------------------------------------------

void TformMain::mySleep(__int64 iMiliSec)
{
	TDateTime dtCzasStart = Now();
	while (MilliSecondsBetween(Now(), dtCzasStart) <= iMiliSec)
	{
		Sleep(1);
		Application->ProcessMessages();
	}
}
//---------------------------------------------------------------------------

void TformMain::losujStatki(bool bSiatka1)
{
	if (etapy[m_iEtap].iLiczbaStatkow==0)
        return;

	int x, y;
	int iIleRazyStatek = 0;

	do
	{
		int iOffsetX = 0;
		int iOffsetY = 0;
		int iModuloX = COL_MAX;
		int iModuloY = ROW_MAX;

		//optymalizacja losowania pod k¹tem promienia
		if (m_RoboczyStatek.getRozmiar())
		{
			int iLastX = m_RoboczyStatek.getLastX();
			int iLastY = m_RoboczyStatek.getLastY();

			int iBiezacyStatekRozmiar = etapy[m_iEtap].iWielkoscStatku;

			iOffsetX = iLastX - iBiezacyStatekRozmiar;
			if (iOffsetX < 0)
				iOffsetX = 0;
			else
			if (iOffsetX > COL_MAX)
				iOffsetX = COL_MAX;

			iOffsetY = iLastY - iBiezacyStatekRozmiar;
			if (iOffsetY < 0)
				iOffsetY = 0;
			else
			if (iOffsetY > ROW_MAX)
				iOffsetY = ROW_MAX;

			iModuloX = 2*iBiezacyStatekRozmiar;
			iModuloY = 2*iBiezacyStatekRozmiar;
		}

		//optymalizacja losowania pod k¹tem analizy kierunku
		TStatki::KIERUNEK eKierunek = m_RoboczyStatek.getKierunek();
		if (eKierunek==TStatki::PIONOWO)
		{
			x = m_RoboczyStatek.getLastX();
			y = iOffsetY + rand()%iModuloY;
		}
		else
		if (eKierunek==TStatki::POZIOMO)
		{
			x = iOffsetX + rand()%iModuloX;
			y = m_RoboczyStatek.getLastY();
		}
		else
		{
			x = iOffsetX + rand()%iModuloX;
			y = iOffsetY + rand()%iModuloY;
		}

		if (iOffsetX > COL_MAX)
			iOffsetX = COL_MAX;
		if (iOffsetY > ROW_MAX)
			iOffsetY = ROW_MAX;

		iIleRazyStatek++;

		labelX->Caption = IntToStr(x);
		labelY->Caption = IntToStr(y);
		mySleep(0);

		if (czyKomorkaPoprawna(x, y))
		{
			if (m_RoboczyStatek.utworzModul(x, y))
			{
				if (m_iTuraRozmieszczania==2 && m_eTryb==TRYB_KOMPUTER)
					; //nie rysujemy statków, ktore rozmieszcza sobie komputer
				else
					rysujStatek(x, y, bSiatka1);

				nastepnyMaszt();
				//zaczêliœmy nowy statek
				if (m_RoboczyStatek.getRozmiar()==0)
					iIleRazyStatek = 0;
			}
		}

		//jeœli SI utknê³o, to cofamy ca³y statek
		if (iIleRazyStatek >= 10)
		{
			while (m_RoboczyStatek.getRozmiar()>0)
				btCofnijClick(this);

			iIleRazyStatek = 0;
		}

	}
	while (m_bTrybRozmieszczania);
}
//---------------------------------------------------------------------------

void __fastcall TformMain::btLosujClick(TObject *Sender)
{
	try
	{
		btLosuj->Enabled = false;
		btCofnij->Enabled = false;
		btAnuluj->Enabled = false;
		losujStatki(m_iTuraRozmieszczania==1);
	}
	__finally
	{
		btLosuj->Enabled = true;
		btCofnij->Enabled = true;
		btAnuluj->Enabled = true;
	}
}
//---------------------------------------------------------------------------

void TformMain::playPudlo()
{
	if (!checkboxSounds->Checked)
		return;

	mp->FileName = "snd/pudlo.mp3";
	mp->Open();
	mp->Play();
	timerSound->Enabled = true;
}
//---------------------------------------------------------------------------

void TformMain::playTrafienie()
{
	if (!checkboxSounds->Checked)
		return;

	mp->FileName = "snd/trafienie.mp3";
	mp->Open();
	mp->Play();
	timerSound->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TformMain::timerSoundTimer(TObject *Sender)
{
	timerSound->Enabled = false;
	mp->Stop();
//	mp->Close();
}
//---------------------------------------------------------------------------

