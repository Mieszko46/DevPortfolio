//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "menuForm.h"
#include "mainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformMenu *formMenu;
//---------------------------------------------------------------------------
__fastcall TformMenu::TformMenu(TComponent* Owner)
	: TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TformMenu::startGameClick(TObject *Sender)
{
	startGame->Visible = false;
	Pkomputer->Visible = true;
	Pgracz->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TformMenu::FormCreate(TObject *Sender)
{
	formMenu->Height = 900;
	formMenu->Width = 1200;
}
//---------------------------------------------------------------------------
void TformMenu::stworz(bool bTryb)
{
	TformMain *pForm = new TformMain(this, bTryb);
	try
	{
		formMenu->Visible = false;
		pForm->Height = formMenu->Height;
		pForm->Width = formMenu->Width;
		pForm->ShowModal();
	}
	__finally
	{
	   delete pForm;
	   formMenu->Visible = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TformMenu::PkomputerClick(TObject *Sender)
{

	bool bTryb = TRYB_KOMPUTER;
	stworz(bTryb);

}
//---------------------------------------------------------------------------
void __fastcall TformMenu::PgraczClick(TObject *Sender)
{
	bool bTryb = TRYB_GRACZ;
	stworz(bTryb);
}
//---------------------------------------------------------------------------

