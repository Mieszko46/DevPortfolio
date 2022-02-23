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
	std::vector<Modul> m_vModul;
public:
	TStatki() {};
	~TStatki() {};

	enum KIERUNEK { BRAK, PIONOWO, POZIOMO };

	bool czyNowyModulOK(int x, int y);
	bool utworzModul(int x, int y);

	bool czyTrafienie(int x, int y);
	bool czyZatopiony();

	int getRozmiar();
	Modul getModul(int i);
	void clear();

	bool pobierzOstatni(Modul &xModul);

	KIERUNEK getKierunek();
	int getLastX();
	int getLastY();

	bool getGraniceStatku(std::vector<Modul> &vGraniceStatku);
};
//---------------------------------------------------------------------------
#endif
