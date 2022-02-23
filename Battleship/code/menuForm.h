//---------------------------------------------------------------------------

#ifndef menuFormH
#define menuFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
//---------------------------------------------------------------------------

enum TRYB
{
	TRYB_KOMPUTER = 0,
	TRYB_GRACZ
};

class TformMenu : public TForm
{
__published:	// IDE-managed Components
	TImage *introImg;
	TPanel *Pkomputer;
	TPanel *Pgracz;
	TLabel *Tytul;
	TPanel *Pwyjdz;
	TPanel *Pstart;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall PkomputerClick(TObject *Sender);
	void __fastcall PgraczClick(TObject *Sender);
	void __fastcall PstartClick(TObject *Sender);
	void __fastcall PwyjdzClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TformMenu(TComponent* Owner);
	void stworz(TRYB eTryb);
};

//---------------------------------------------------------------------------
extern PACKAGE TformMenu *formMenu;
//---------------------------------------------------------------------------
#endif
