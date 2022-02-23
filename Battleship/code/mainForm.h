//---------------------------------------------------------------------------
#ifndef mainFormH
#define mainFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "siatka.h"
#include "statki.h"
#include "menuForm.h"
#include <Vcl.MPlayer.hpp>
//---------------------------------------------------------------------------

struct Etap
{
	int iWielkoscStatku;
	int iLiczbaStatkow;
};

struct Strzal
{
	int x;
	int y;

	void clear()
	{
		x = -1;
		y = -1;
	}

	Strzal()
	{
		clear();
	}
};

class TformMain : public TForm
{
__published:	// IDE-managed Components
	TImage *imagePuste;
	TPanel *panelSiatka1;
	TPaintBox *pboxSiatka1;
	TButton *btDalej;
	TTimer *timerStart;
	TImage *imagePudlo;
	TImage *imageStatek;
	TImage *imageTrafienie;
	TLabel *labelX;
	TLabel *labelY;
	TLabel *labelKto;
	TButton *btAnuluj;
	TLabel *labelMaszty;
	TLabel *labelLiczbaMasztow;
	TLabel *labelWarning;
	TTimer *timerWarning;
	TButton *btCofnij;
	TImage *imageTlo;
	TPanel *panelSiatka2;
	TLabel *labelCo;
	TPaintBox *pboxSiatka2;
	TTimer *timerSIStart;
	TButton *btLosuj;
	TLabel *labelGracz1;
	TLabel *labelGracz2;
	TImage *imageBlueArrow;
	TImage *imageRedArrow;
	TTimer *timerSI;
	TMediaPlayer *mp;
	TCheckBox *checkboxSounds;
	TTimer *timerSound;
	void __fastcall timerStartTimer(TObject *Sender);
	void __fastcall pboxSiatka1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall btAnulujClick(TObject *Sender);
	void __fastcall timerWarningTimer(TObject *Sender);
	void __fastcall btCofnijClick(TObject *Sender);
	void __fastcall btDalejClick(TObject *Sender);
	void __fastcall pboxSiatka2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall timerSIStartTimer(TObject *Sender);
	void __fastcall btLosujClick(TObject *Sender);
	void __fastcall timerSITimer(TObject *Sender);
	void __fastcall timerSoundTimer(TObject *Sender);
private:	// User declarations
	TRYB m_eTryb;
    bool m_bTrybRozmieszczania;
	int  m_iTuraRozmieszczania;

	bool m_bTrybGry;
	bool m_bTuraGracz1;

	bool m_bKoniecGry;

	bool m_bSITrafilo;
	Strzal  m_xSITrafienie;
	TStatki m_xSIStatekRoboczy;

	TSiatka m_oSiatka;
	TSiatka m_oSiatkaGracz1;
	TSiatka m_oSiatkaGracz2;
	TSiatka m_oMaskaSiatki1;
	TSiatka m_oMaskaSiatki2;

	TStatki m_RoboczyStatek;
	std::vector<TStatki> m_vStatki;
	std::vector<TStatki> m_vStatkiGracz1;
	std::vector<TStatki> m_vStatkiGracz2;

	TImage* getImage(int x, int y, TSiatka& xSiatka);
	TImage* getImage(STAN eStanKomorki);

	void showWarning(AnsiString sKomunikat);
	void rysujSiatke(bool bSiatka1);

	void rysuj(int x, int y, bool bSiatka1, STAN eStan);
	void rysujStatek(int x, int y, bool bSiatka1);
	void rysujPuste(int x, int y, bool bSiatka1);
	void rysujTrafienie(int x, int y, bool bSiatka1);
	void rysujPudlo(int x, int y, bool bSiatka1);

	void rozmiescStatki();
	void zapamietajStatek();
	void usunStatekZSiatki();

	int szerokosc();
	int wysokosc();

	bool czyKomorkaPoprawna(int x, int y);
	bool czyStrzalPoprawny(int X, int Y, bool bSiatka1);
	bool czyTrafienie(int X, int Y, bool bSiatka1);

	void nastepnyMaszt();
	void pokazInfo();
	void rozpocznijGre();

	void pokazKto(const AnsiString& sTekst);
	void pokazCo(const AnsiString& sTekst);
    void wskazKto();

	void sprawdzKlik(int X, int Y, bool bSiatka1);
	void sprawdzRuch(int X, int Y, bool bSiatka1);
	void mySleep(__int64 iMiliSec);
	void losujStatki(bool bSiatka1);

	void przetwarzajTrafienie(int X, int Y, bool bSiatka1);
	void przetwarzajPudlo(int X, int Y, bool bSiatka1);
	void przetwarzajOtoczenieStatku(TSiatka *pMaska, TStatki *pTrafionyStatek, bool bSiatka1);

	bool sprawdzKoniecGry(bool bSiatka1);
	void koniecGry();
	bool czyTuraSI();

	void playPudlo();
	void playTrafienie();


public:		// User declarations
	__fastcall TformMain(TComponent* Owner, TRYB eTryb);

	int m_iEtap;
	int m_iMaszt;
	int m_iStatek;
};
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif
