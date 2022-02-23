//---------------------------------------------------------------------------

#ifndef statkiH
#define statkiH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <vector>
//---------------------------------------------------------------------------
class Modul
{
public:
	Modul();
	Modul(int x, int y);

	int x;
	int y;
	bool trafienie;

};

class TStatki
{
private:
	int m_iRozmiar;
	std::vector<Modul> m_vModul;
public:
	TStatki() {};
	~TStatki() {};
	void przypiszWartosciStartowe(int i);
	void przypiszWspolrzedne(int x, int y, bool b);
	void set(int x, int y, bool pion);

	bool czyNowyModulOK(int x, int y);
	bool utworzModul(int x, int y);

	int getRozmiar();
	Modul getModul(int i);
	void clear();

	bool pobierzOstatni(Modul &xModul);

};
//---------------------------------------------------------------------------
#endif
