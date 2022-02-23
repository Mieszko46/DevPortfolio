//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "mainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMain *formMain;

Etap etapy[] = {  {4,1}, {3,2}, {2,3}, {1,4}, {0,0} };

//---------------------------------------------------------------------------
__fastcall TformMain::TformMain(TComponent* Owner, bool bTryb)
	: TForm(Owner)
{
	m_bTryb = bTryb;
	pboxSiatka->Width = COL_MAX * imagePuste->Width;
	pboxSiatka->Height = ROW_MAX * imagePuste->Height;
	panelSiatka->Width = pboxSiatka->Width+8;
	panelSiatka->Height = pboxSiatka->Height+8;

	timerStart->Enabled = true;
}
//---------------------------------------------------------------------------

void TformMain::rysujSiatke()
{
	for (int x = 0; x < COL_MAX; x++)
		for (int y = 0; y < ROW_MAX; y++)
		{
			TImage *pimg = getImage(x, y, m_oSiatka);
			pboxSiatka->Canvas->Draw(x*pimg->Width, y*pimg->Height, pimg->Picture->Graphic);
		}
}
//---------------------------------------------------------------------------

void TformMain::rysujStatek(int x, int y)
{
	TImage *pimg = getImage(STAN_STATEK);
	pboxSiatka->Canvas->Draw(x*pimg->Width, y*pimg->Height, pimg->Picture->Graphic);
}
//---------------------------------------------------------------------------

void TformMain::rysujPuste(int x, int y)
{
	TImage *pimg = getImage(STAN_PUSTE);
	pboxSiatka->Canvas->Draw(x*pimg->Width, y*pimg->Height, pimg->Picture->Graphic);
}
//---------------------------------------------------------------------------

void TformMain::rozmiescStatki()
{
	m_bTrybRozmieszczania = true;
	m_iEtap = 0;
	Etap xBiezacyEtap = etapy[m_iEtap];
	m_iMaszt = xBiezacyEtap.iWielkoscStatku;
	m_iStatek = xBiezacyEtap.iLiczbaStatkow;
	btDalej->Enabled = false;

	pokazInfo();
	m_RoboczyStatek.clear();
}
//---------------------------------------------------------------------------

void __fastcall TformMain::timerStartTimer(TObject *Sender)
{
	timerStart->Enabled = false;
	rysujSiatke();
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

void __fastcall TformMain::pboxSiatkaMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if (!m_bTrybRozmieszczania)
		return;

	int x = X / szerokosc();
	int y = Y / wysokosc();

	labelX->Caption = IntToStr(x);
	labelY->Caption = IntToStr(y);

	if (czyKomorkaPoprawna(x, y))
	{
		if (m_RoboczyStatek.utworzModul(x, y))
			{
			rysujStatek(x, y);
			nastepnyMaszt();
			}
		}
}
//---------------------------------------------------------------------------

void TformMain::showWarning(AnsiString sKomunikat)
{
	labelWarning->Caption = sKomunikat;
	labelWarning->Visible = true;
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
		showWarning("Nowy maszt za daleko!");
		return false;
		}

return true;
}
//---------------------------------------------------------------------------

void TformMain::pokazInfo()
{
	labelKto->Caption = "Rozmieœæ statek nr " + IntToStr(m_iStatek) + " o rozmiarze:" + IntToStr(etapy[m_iEtap].iWielkoscStatku);
	labelLiczbaMasztow->Caption = IntToStr(m_iMaszt);
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
			rozpocznijGre();
			bPokazInfo = false;
			}
	}

	if (bPokazInfo)
		pokazInfo();
}
//---------------------------------------------------------------------------

void TformMain::rozpocznijGre()
{
	m_bTrybRozmieszczania = false;
	labelKto->Caption = "GRA SIÊ ROZPOCZYNA!";
	btDalej->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TformMain::Button2Click(TObject *Sender)
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
	//jeœli statek roboczy jest jeszcze niezdefiniowany, to pobieram ostatni statek z listy
	if (m_RoboczyStatek.getRozmiar()==0 && m_vStatki.size()>0)
		{
		m_RoboczyStatek = m_vStatki[m_vStatki.size()-1];
		//m_RoboczyStatek = *(m_vStatki.end()-1);
		m_vStatki.pop_back();
		usunStatekZSiatki();

		//ustawiæ etap, rozmiar statku i iloœæ masztów
		//czy cofn¹æ etap
		if (etapy[m_iEtap].iLiczbaStatkow == m_iStatek)
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
		rysujPuste(xUndoModul.x, xUndoModul.y);

		//przywróæ liczbê masztów
		m_iMaszt++;

		pokazInfo();
		showWarning("Usuniêto maszt");
	}
}
//---------------------------------------------------------------------------

