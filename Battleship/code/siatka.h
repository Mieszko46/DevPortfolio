//---------------------------------------------------------------------------

#ifndef siatkaH
#define siatkaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
enum STAN
{
	STAN_NULL = 0,
	STAN_PUSTE,
	STAN_PUDLO,
	STAN_STATEK,
	STAN_TRAFIENIE
};

#define COL_MAX 	10
#define ROW_MAX 	10
//const int COL_MAX_NUM = 10;

class TSiatka
{
private:	// User declarations
	STAN    m_xPlansza[COL_MAX][ROW_MAX];

public:		// User declarations

	STAN	getStan(int x, int y);
	void 	setStan(int x, int y, STAN eStan);
	bool	czyStatekObok(int x, int y);

	TSiatka();
	void clear();
};
//---------------------------------------------------------------------------
#endif
