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
	TLabel *startGame;
	TPanel *Pkomputer;
	TPanel *Pgracz;
	TLabel *Tytul;
	void __fastcall startGameClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall PkomputerClick(TObject *Sender);
	void __fastcall PgraczClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TformMenu(TComponent* Owner);
	void stworz(bool bTryb);
};

//---------------------------------------------------------------------------
extern PACKAGE TformMenu *formMenu;
//---------------------------------------------------------------------------
#endif
