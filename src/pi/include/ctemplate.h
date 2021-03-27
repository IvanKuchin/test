#ifndef __CTEMPLATE__H__
#define __CTEMPLATE__H__

#include <string>
#include <sstream>
#include "localy.h"
#include "cvars.h"

using namespace std;

class CTemplate
{
	private:
		ostringstream	ost;
		string			fileName = "";
		CVars			*vars = NULL;

		string			RecvLine(FILE *f);
	public:
						CTemplate();
						CTemplate(string file);
						CTemplate(string file, CVars *v);

		void			SetFile(string file);
		void			SetVars(CVars *v);

		string			Render();
};

#endif
