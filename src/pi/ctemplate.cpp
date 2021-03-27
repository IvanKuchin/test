#include <string>
#include <sstream>

#include "ctemplate.h"
#include "cvars.h"
#include "cexception.h"
#include "clog.h"

CTemplate::CTemplate() : fileName(""), vars(NULL)
{
}

CTemplate::CTemplate(string file) : fileName(file), vars(NULL)
{
}

CTemplate::CTemplate(string file, CVars *v) : fileName(file), vars(v)
{
}

void CTemplate::SetFile(string file)
{
	fileName = file;
}

void CTemplate::SetVars(CVars *v)
{
	vars = v;
}

string CTemplate::RecvLine(FILE *f)
{
    int i = 0;
    char ch;
    string result;

    result = "";

    for (; (ch = fgetc(f)) != EOF; i++)
    {
        result += ch;

        if (result[i] == '\n') break;
    }

    return result;
}

string CTemplate::Render()
{
    string		fLine;
    ostringstream	ost;
    FILE		*templateFile;
    string		result;

    if(vars == NULL)
    {
		MESSAGE_ERROR("", "", "cvars for mail is missed");

		CExceptionHTML("vars array");
    }

    templateFile = fopen(fileName.c_str(), "r");
    if(templateFile == NULL)
    {
		MESSAGE_ERROR("", "", "template for mail is missed");

		throw CExceptionHTML("mail template");
    }

    fLine = RecvLine(templateFile);

    while(fLine.length() > 0)
    {
		string::size_type	bPos;
		string			resultTmp;
		bool			flag = true;

		while(flag)
		{
			resultTmp = "";
			
			if(fLine.find("<<vars:") != string::npos)
			{
				string::size_type	ePos;

				bPos = fLine.find("<<vars:");
				ePos = fLine.find(">>", bPos);
				
				if((ePos != string::npos) && (ePos > bPos))
				{
					string		varName = fLine.substr(bPos + 7, ePos - bPos - 7);
					string		varValue = vars->Get(varName);

					string		before = fLine.substr(0, bPos);
					string		after = fLine.substr(ePos + 2, fLine.length() - ePos - 2);

					resultTmp += before;
					if(varValue.length() > 0)
					{
						resultTmp += varValue;
					}
					else
					{
						MESSAGE_DEBUG("", "", "value of the variable " + varName + " is empty.");
					}
					resultTmp += after;
				}
				else
				{
					resultTmp += fLine;
					flag = false;
				}
			}
			else if(fLine.find("<<template:") != string::npos)
			{
				string::size_type	ePos;

				bPos = fLine.find("<<template:");
				ePos = fLine.find(">>", bPos);
				if(ePos != string::npos)
				{
					string	templateName = fLine.substr(bPos + 11, ePos - bPos - 11);
					CTemplate	nextTempl(templateName, vars);

					string	before = fLine.substr(0, bPos);
					string	after = fLine.substr(ePos + 2, fLine.length() - ePos - 2);

					resultTmp += before;
					resultTmp += nextTempl.Render();
					resultTmp += after;
				}
				else
				{
					resultTmp += fLine;
					flag = false;
				}
			}
			else 
			{
				resultTmp += fLine;
				flag = false;
			}
			fLine = resultTmp;
		}
		result += resultTmp;
		fLine = RecvLine(templateFile);
    }

    return result;
}
