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
//---------------------------------------------------------------------------

struct Etap
{
	int iWielkoscStatku;
	int iLiczbaStatkow;
};

class TformMain : public TForm
{
__published:	// IDE-managed Components
	TImage *imagePuste;
	TPanel *panelSiatka;
	TPaintBox *pboxSiatka;
	TButton *btDalej;
	TTimer *timerStart;
	TImage *imagePudlo;
	TImage *imageStatek;
	TImage *imageTrafienie;
	TLabel *labelX;
	TLabel *labelY;
	TLabel *labelKto;
	TButton *Button2;
	TLabel *Label1;
	TLabel *labelLiczbaMasztow;
	TLabel *labelWarning;
	TTimer *timerWarning;
	TButton *btCofnij;
	void __fastcall timerStartTimer(TObject *Sender);
	void __fastcall pboxSiatkaMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall timerWarningTimer(TObject *Sender);
	void __fastcall btCofnijClick(TObject *Sender);
private:	// User declarations
	bool m_bTryb;
    bool m_bTrybRozmieszczania;
	TSiatka m_oSiatka;
	TStatki m_RoboczyStatek;
	std::vector<TStatki> m_vStatki;

	TImage* getImage(int x, int y, TSiatka& xSiatka);
	TImage* getImage(STAN eStanKomorki);

	void showWarning(AnsiString sKomunikat);
	void rysujSiatke();
	void rysujStatek(int x, int y);
	void rysujPuste(int x, int y);
	void rozmiescStatki();
	void zapamietajStatek();
	void usunStatekZSiatki();

	int szerokosc();
	int wysokosc();

	bool czyKomorkaPoprawna(int x, int y);
	void nastepnyMaszt();
	void pokazInfo();
	void rozpocznijGre();

public:		// User declarations
	__fastcall TformMain(TComponent* Owner,bool bTryb);

	int m_iEtap;
	int m_iMaszt;
	int m_iStatek;
};
//---------------------------------------------------------------------------
extern PACKAGE TformMain *formMain;
//---------------------------------------------------------------------------
#endif
