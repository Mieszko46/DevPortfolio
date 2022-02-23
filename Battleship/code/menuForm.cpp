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
void __fastcall TformMenu::FormCreate(TObject *Sender)
{
	formMenu->Height = 900;
	formMenu->Width = 1200;
}
//---------------------------------------------------------------------------
void TformMenu::stworz(TRYB eTryb)
{
	TformMain *pForm = new TformMain(this, eTryb);
	try
	{
		formMenu->Visible = false;
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
	stworz(TRYB_KOMPUTER);

}
//---------------------------------------------------------------------------
void __fastcall TformMenu::PgraczClick(TObject *Sender)
{
	stworz(TRYB_GRACZ);
}
//---------------------------------------------------------------------------

void __fastcall TformMenu::PstartClick(TObject *Sender)
{
	Pstart->Visible = false;
	Pkomputer->Visible = true;
	Pgracz->Visible = true;
    Pwyjdz->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TformMenu::PwyjdzClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

