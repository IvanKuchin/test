#include "utilities.h"

auto	GetSHA512(const string &src) -> string
{
	string			result = "";
    unsigned char	digest[SHA512_DIGEST_LENGTH];

	MESSAGE_DEBUG("", "", "start");

    SHA512_CTX ctx;
    SHA512_Init(&ctx);
    SHA512_Update(&ctx, src.c_str(), src.length());
    SHA512_Final(digest, &ctx);

    char mdString[SHA512_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
		sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    result = mdString;

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + " symbols)");

	return result;
}

/*
Copyright (c) <YEAR>, <OWNER>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n)
{
		MESSAGE_DEBUG("", "", "start");

		unsigned int i = 0;
		unsigned int j = 0;
		unsigned int k = 0;

		typedef struct ConvLetter {
				char	win1251;
				int	 unicode;
		} Letter;

		Letter  g_letters[] = {
		{(char)0x82, 0x201A}, // SINGLE LOW-9 QUOTATION MARK
		{(char)0x83, 0x0453}, // CYRILLIC SMALL LETTER GJE
		{(char)0x84, 0x201E}, // DOUBLE LOW-9 QUOTATION MARK
		{(char)0x85, 0x2026}, // HORIZONTAL ELLIPSIS
		{(char)0x86, 0x2020}, // DAGGER
		{(char)0x87, 0x2021}, // DOUBLE DAGGER
		{(char)0x88, 0x20AC}, // EURO SIGN
		{(char)0x89, 0x2030}, // PER MILLE SIGN
		{(char)0x8A, 0x0409}, // CYRILLIC CAPITAL LETTER LJE
		{(char)0x8B, 0x2039}, // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
		{(char)0x8C, 0x040A}, // CYRILLIC CAPITAL LETTER NJE
		{(char)0x8D, 0x040C}, // CYRILLIC CAPITAL LETTER KJE
		{(char)0x8E, 0x040B}, // CYRILLIC CAPITAL LETTER TSHE
		{(char)0x8F, 0x040F}, // CYRILLIC CAPITAL LETTER DZHE
		{(char)0x90, 0x0452}, // CYRILLIC SMALL LETTER DJE
		{(char)0x91, 0x2018}, // LEFT SINGLE QUOTATION MARK
		{(char)0x92, 0x2019}, // RIGHT SINGLE QUOTATION MARK
		{(char)0x93, 0x201C}, // LEFT DOUBLE QUOTATION MARK
		{(char)0x94, 0x201D}, // RIGHT DOUBLE QUOTATION MARK
		{(char)0x95, 0x2022}, // BULLET
		{(char)0x96, 0x2013}, // EN DASH
		{(char)0x97, 0x2014}, // EM DASH
		{(char)0x99, 0x2122}, // TRADE MARK SIGN
		{(char)0x9A, 0x0459}, // CYRILLIC SMALL LETTER LJE
		{(char)0x9B, 0x203A}, // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
		{(char)0x9C, 0x045A}, // CYRILLIC SMALL LETTER NJE
		{(char)0x9D, 0x045C}, // CYRILLIC SMALL LETTER KJE
		{(char)0x9E, 0x045B}, // CYRILLIC SMALL LETTER TSHE
		{(char)0x9F, 0x045F}, // CYRILLIC SMALL LETTER DZHE
		{(char)0xA0, 0x00A0}, // NO-BREAK SPACE
		{(char)0xA1, 0x040E}, // CYRILLIC CAPITAL LETTER SHORT U
		{(char)0xA2, 0x045E}, // CYRILLIC SMALL LETTER SHORT U
		{(char)0xA3, 0x0408}, // CYRILLIC CAPITAL LETTER JE
		{(char)0xA4, 0x00A4}, // CURRENCY SIGN
		{(char)0xA5, 0x0490}, // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
		{(char)0xA6, 0x00A6}, // BROKEN BAR
		{(char)0xA7, 0x00A7}, // SECTION SIGN
		{(char)0xA8, 0x0401}, // CYRILLIC CAPITAL LETTER IO
		{(char)0xA9, 0x00A9}, // COPYRIGHT SIGN
		{(char)0xAA, 0x0404}, // CYRILLIC CAPITAL LETTER UKRAINIAN IE
		{(char)0xAB, 0x00AB}, // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
		{(char)0xAC, 0x00AC}, // NOT SIGN
		{(char)0xAD, 0x00AD}, // SOFT HYPHEN
		{(char)0xAE, 0x00AE}, // REGISTERED SIGN
		{(char)0xAF, 0x0407}, // CYRILLIC CAPITAL LETTER YI
		{(char)0xB0, 0x00B0}, // DEGREE SIGN
		{(char)0xB1, 0x00B1}, // PLUS-MINUS SIGN
		{(char)0xB2, 0x0406}, // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
		{(char)0xB3, 0x0456}, // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
		{(char)0xB4, 0x0491}, // CYRILLIC SMALL LETTER GHE WITH UPTURN
		{(char)0xB5, 0x00B5}, // MICRO SIGN
		{(char)0xB6, 0x00B6}, // PILCROW SIGN
		{(char)0xB7, 0x00B7}, // MIDDLE DOT
		{(char)0xB8, 0x0451}, // CYRILLIC SMALL LETTER IO
		{(char)0xB9, 0x2116}, // NUMERO SIGN
		{(char)0xBA, 0x0454}, // CYRILLIC SMALL LETTER UKRAINIAN IE
		{(char)0xBB, 0x00BB}, // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
		{(char)0xBC, 0x0458}, // CYRILLIC SMALL LETTER JE
		{(char)0xBD, 0x0405}, // CYRILLIC CAPITAL LETTER DZE
		{(char)0xBE, 0x0455}, // CYRILLIC SMALL LETTER DZE
		{(char)0xBF, 0x0457} // CYRILLIC SMALL LETTER YI
	};

		typedef struct ConvLetter3Bytes {
				char	win1251;
				char	unicode1;
				char	unicode2;
				char	unicode3;
		} Letter3Bytes;
		Letter3Bytes g_lettersGeneralPunctuation[] = {
			{'-', (char)0xe2, (char)0x80, (char)0x94}, // EM DASH
			{'-', (char)0xe2, (char)0x80, (char)0x93}, // EN DASH
			{'-', (char)0xe2, (char)0x80, (char)0x90}, // HYPHEN
			{'-', (char)0xe2, (char)0x80, (char)0x91}, // NON-BREAKING HYPHEN
			{'-', (char)0xe2, (char)0x80, (char)0x92}, // FIGURE DASH
			{'-', (char)0xe2, (char)0x80, (char)0x93}, // EN DASH
			{'-', (char)0xe2, (char)0x80, (char)0x94}, // EM DASH
			{'-', (char)0xe2, (char)0x80, (char)0x95}, // HORIZONTAL BAR
			{'|', (char)0xe2, (char)0x80, (char)0x96}, // DOUBLE VERTICAL LINE
			{'_', (char)0xe2, (char)0x80, (char)0x97}, // DOUBLE LOW LINE
			{',', (char)0xe2, (char)0x80, (char)0x9a}, // SINGLE LOW QUOTATION MARK
			{'.', (char)0xe2, (char)0x80, (char)0xa4}, // ONE DOT LEADER
			{'.', (char)0xe2, (char)0x80, (char)0x5}, // TWO DOT LEADER
			{'.', (char)0xe2, (char)0x80, (char)0x6}, // HORIZONTAL ELLIPSIS
			{'<', (char)0xe2, (char)0x80, (char)0xb9}, // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
			{'>', (char)0xe2, (char)0x80, (char)0xba}, // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
			{'!', (char)0xe2, (char)0x80, (char)0xbc}, // DOUBLE EXCLAMNATION MARK
			{'-', (char)0xe2, (char)0x81, (char)0x83}, // HYPHEN BULLET
			{'/', (char)0xe2, (char)0x81, (char)0x84}, // FRACTION SLASH
			{'?', (char)0xe2, (char)0x81, (char)0x87}, // DOUBLE QUESTION MARK
			{'*', (char)0xe2, (char)0x81, (char)0x8e}, // LOW ASTERISK
			{'|', (char)0xe2, (char)0x83, (char)0x92}, // COMBINING LONG VERTICAL LINE OVERLAY
			{'\\', (char)0xe2, (char)0x83, (char)0xa5}, //COMBINING REVERSE SOLIDUS OVERLAY
			{'|', (char)0xe2, (char)0x83, (char)0xa6}, // COMBINING DOUBLE VERTICAL STROKE OVERLAY
			{'/', (char)0xe2, (char)0x83, (char)0xab}, // COMBINING LONG DOUBLE SOLIDUS OVERLAY
			{'\'', (char)0xe2, (char)0x80, (char)0x9b}, // SINGLE HIGN-REVERSED-9 QUOTATION MARK
			{'\\', (char)0xe2, (char)0x80, (char)0xb2}, // PRIME
			{'\\', (char)0xe2, (char)0x80, (char)0xb5}, // REVERSED PRIME
			{'"', (char)0xe2, (char)0x80, (char)0x9c}, // LEFT DOUBLE QUOTATION MARK
			{'"', (char)0xe2, (char)0x80, (char)0xb3}, // DOUBLE PRIME
			{'"', (char)0xe2, (char)0x80, (char)0xb6}, // REVERSED DOUBLE PRIME
			{'"', (char)0xe2, (char)0x80, (char)0x9c}, // RIGHT DOUBLE QUOTATION MARK
			{'"', (char)0xe2, (char)0x80, (char)0x9e}, // DOUBLE LOW-9 QUOTATION MARK
			{'"', (char)0xe2, (char)0x80, (char)0x9f}, // DOUBLE HIGH-REVERSED-9 QUOTATION MARK
			{'*', (char)0xe2, (char)0x80, (char)0xA2}, //  BULLET
			{'%', (char)0xe2, (char)0x80, (char)0xb0}, // PER MILLE SIGN
			{'%', (char)0xe2, (char)0x80, (char)0xb1}, // PER TEN THOUSAND SIGN
			{'*', (char)0xe2, (char)0x80, (char)0xa7}, // HYPHENATION POINT
			{'~', (char)0xe2, (char)0x81, (char)0x93}, // SWUNG DASH
			{'*', (char)0xe2, (char)0x81, (char)0x95}, // FLOWER PUNCTION MARK
			{':', (char)0xe2, (char)0x81, (char)0x9a}, // TWO DOT PUNCTION		};
			{'|', (char)0xe2, (char)0x83, (char)0x92}, // COMBINING LONG VERTICAL LINE OVERLAY
			{'\\', (char)0xe2, (char)0x83, (char)0xa5}, // COMBINING REVERSE SOLIDUS OVERLAY
			{'|', (char)0xe2, (char)0x83, (char)0xa6}, // COMBINING DOUBLE VERTICAL STROKE OVERLAY
			{'/', (char)0xe2, (char)0x83, (char)0xab}, // COMBINING LONG DOUBLE SOLIDUS OVERLAY
			{'*', (char)0xe2, (char)0x83, (char)0xb0}, // COMBINING ASTERISK ABOVE
			{(char)0xB9, (char)0xe2, (char)0x84, (char)0x96}, // number sign
			{'1', (char)0xe2, (char)0x85, (char)0xa0}, // ROMAN NUMERAL ONE
			{'2', (char)0xe2, (char)0x85, (char)0xa1}, // ROMAN NUMERAL TWO
			{'3', (char)0xe2, (char)0x85, (char)0xa2}, // ROMAN NUMERAL THREE
			{'4', (char)0xe2, (char)0x85, (char)0xa3}, // ROMAN NUMERAL FOUR
			{'5', (char)0xe2, (char)0x85, (char)0xa4}, // ROMAN NUMERAL FIVE
			{'6', (char)0xe2, (char)0x85, (char)0xa5}, // ROMAN NUMERAL SIX
			{'7', (char)0xe2, (char)0x85, (char)0xa6}, // ROMAN NUMERAL SEVEN
			{'8', (char)0xe2, (char)0x85, (char)0xa7}, // ROMAN NUMERAL EIGHT
			{'9', (char)0xe2, (char)0x85, (char)0xa8}, // ROMAN NUMERAL NINE
			{'X', (char)0xe2, (char)0x85, (char)0xa9}, // ROMAN NUMERAL TEN
			{'L', (char)0xe2, (char)0x85, (char)0xac}, // ROMAN NUMERAL FIFTY
			{'C', (char)0xe2, (char)0x85, (char)0xad}, // ROMAN NUMERAL ONE HUNDRED
			{'D', (char)0xe2, (char)0x85, (char)0xae}, // ROMAN NUMERAL FIVE HUNDRED
			{'M', (char)0xe2, (char)0x85, (char)0xaf} // ROMAN NUMERAL ONE THOUSAND
		};

		for(; i < (unsigned int)n && utf8[i] != 0; ++i) {
				char prefix = utf8[i];
				char suffix = utf8[i+1];
				if ((prefix & 0x80) == 0)
				{
						windows1251[j] = (char)prefix;
						++j;
				}
				else if ((~prefix) & 0x20)
				{
						int first5bit = prefix & 0x1F;
						first5bit <<= 6;
						int sec6bit = suffix & 0x3F;
						int unicode_char = first5bit + sec6bit;


						if ( unicode_char >= 0x410 && unicode_char <= 0x44F ) {
								windows1251[j] = (char)(unicode_char - 0x350);
						} else if (unicode_char >= 0x80 && unicode_char <= 0xFF) {
								windows1251[j] = (char)(unicode_char);
						} else if (unicode_char >= 0x402 && unicode_char <= 0x403) {
								windows1251[j] = (char)(unicode_char - 0x382);
						} else {
								unsigned int count = sizeof(g_letters) / sizeof(Letter);
								for (k = 0; k < count; ++k) {
										if (unicode_char == g_letters[k].unicode) {
												windows1251[j] = g_letters[k].win1251;
												goto NEXT_LETTER;
										}
								}
								// can't convert this char
								{
									CLog	log;
									ostringstream	ost;

									ost.str("");
									ost << __func__ << "[" << __LINE__ << "]:" << ": symbol at position " << i << " [" << hex << (prefix & 0xFF) << " " << (suffix & 0xFF) << "] doesn't belongs to UTF-8 cyrillic range (U+0400 ... U+04FF)";
									log.Write(DEBUG, ost.str());
								}
								return 0;
						}
NEXT_LETTER:
						++i;
						++j;
				}
				else if((prefix == static_cast<char>(0xf0)) and (suffix == static_cast<char>(0x9f)))
				{
					// --- emoji part (f0 9f 98 80 - f0 9f 9b b3) or (U+1F600 - U+1F6FF)
					// --- 4 bytes long
					if((i + 3) < n)
					{
						{
							CLog	log;
							ostringstream   ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]:" << ": emojy detected [" << hex << (utf8[i] & 0xFF) << " " << (utf8[i+1] & 0xFF) << " " << (utf8[i+2] & 0xFF) << " " << (utf8[i+3] & 0xFF) << "]";
							log.Write(DEBUG, ost.str());
						}
						i += 3;
					}
					else
					{
						MESSAGE_ERROR("", "", ": ERROR: emojy detected but dst string not lengthy enough, dst buffer size(" + to_string(n) + ") < current position (" + to_string(i) + "+3)");
						return 0;
					}
				}
				else if((prefix == static_cast<char>(0xe2)) && ((suffix & 0x80) == 0x80))
				{
					// --- letterlike symbols (e2 80 80 - e2 82 bf) or (U+2000 - U+20FF)
					// --- https://utf8-chartable.de/unicode-utf8-table.pl?start=8448&names=2
					// --- 3 bytes long
					if((i + 2) < n)
					{
						unsigned int	count = sizeof(g_lettersGeneralPunctuation) / sizeof(Letter3Bytes);
						bool			isFound = false;

						for (k = 0; ((k < count) && !isFound); ++k)
						{
								char currentSymb1 = prefix;
								char currentSymb2 = suffix;
								char currentSymb3 = utf8[i+2];

								if(
									(currentSymb1 == g_lettersGeneralPunctuation[k].unicode1) &&
									(currentSymb2 == g_lettersGeneralPunctuation[k].unicode2) &&
									(currentSymb3 == g_lettersGeneralPunctuation[k].unicode3)
								  )
								{
										windows1251[j] = g_lettersGeneralPunctuation[k].win1251;
										isFound = true;
										i += 2;
										j++;
								}
						}
						if(!isFound)
						{
							// --- unknown symbol
							// --- replace it to space
							windows1251[j] = ' ';
							// --- this is 3 bytes length symbol
							i += 2;
							j++;

							// can't convert this char
							CLog	log;
							ostringstream   ost;

							ost.str("");
							ost << __func__ << "[" << __LINE__ << "]:" << ": symbol at position " << (i - 2) << " [" << hex << (prefix & 0xFF) << " " << (suffix & 0xFF) << " " << (utf8[i+2 - 2] & 0xFF) << "] not found in mapping table Punctuation range (U+0400 ... U+04FF)";
							log.Write(DEBUG, ost.str());
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "general punctuation detected but dst string not lengthy enough, dst buffer size(" + to_string(n) + ") < current position (" + to_string(i) + "+2)");
						return 0;
					}

				}
				else
				{
					// can't convert this chars
					{
						CLog	log;
						ostringstream   ost;

						ost.str("");
						ost << __func__ << "[" << __LINE__ << "]:ERROR: can't convert this " << i << "-rd/th char " << hex << (+prefix & 0xFF) << " " << (+suffix & 0xFF);
						log.Write(ERROR, ost.str());
					}
					return 0;
				}
		}
		windows1251[j] = 0;

		MESSAGE_DEBUG("", "", "finish");

		return 1;
}

auto	utf8_to_cp1251(const string &src) -> string
{
	char	convertBuffer[100 * 1024];
	auto	result = 0;
	
	MESSAGE_DEBUG("", "", "start");

	memset(convertBuffer, 0, sizeof(convertBuffer));

	result = convert_utf8_to_windows1251(src.c_str(), convertBuffer, sizeof(convertBuffer) - 1);

	MESSAGE_DEBUG("", "", "finish (conversation result is " + to_string(result) + ")");

	return convertBuffer;
}


bool convert_cp1251_to_utf8(const char *in, char *out, int size)
{
	MESSAGE_DEBUG("", "", "start");

	const char table[129*3] = {
		"\320\202 \320\203 \342\200\232\321\223 \342\200\236\342\200\246\342\200\240\342\200\241"
		"\342\202\254\342\200\260\320\211 \342\200\271\320\212 \320\214 \320\213 \320\217 "
		"\321\222 \342\200\230\342\200\231\342\200\234\342\200\235\342\200\242\342\200\223\342\200\224"
		"   \342\204\242\321\231 \342\200\272\321\232 \321\234 \321\233 \321\237 "
		"\302\240 \320\216 \321\236 \320\210 \302\244 \322\220 \302\246 \302\247 "
		"\320\201 \302\251 \320\204 \302\253 \302\254 \302\255 \302\256 \320\207 "
		"\302\260 \302\261 \320\206 \321\226 \322\221 \302\265 \302\266 \302\267 "
		"\321\221 \342\204\226\321\224 \302\273 \321\230 \320\205 \321\225 \321\227 "
		"\320\220 \320\221 \320\222 \320\223 \320\224 \320\225 \320\226 \320\227 "
		"\320\230 \320\231 \320\232 \320\233 \320\234 \320\235 \320\236 \320\237 "
		"\320\240 \320\241 \320\242 \320\243 \320\244 \320\245 \320\246 \320\247 "
		"\320\250 \320\251 \320\252 \320\253 \320\254 \320\255 \320\256 \320\257 "
		"\320\260 \320\261 \320\262 \320\263 \320\264 \320\265 \320\266 \320\267 "
		"\320\270 \320\271 \320\272 \320\273 \320\274 \320\275 \320\276 \320\277 "
		"\321\200 \321\201 \321\202 \321\203 \321\204 \321\205 \321\206 \321\207 "
		"\321\210 \321\211 \321\212 \321\213 \321\214 \321\215 \321\216 \321\217 "
	};
	int	counter = 0;
	bool result = true;

	while (*in)
		if (*in & 0x80) {
			const char *p = &table[3 * (0x7f & *in++)];
			if (*p == ' ')
				continue;
			counter += 2;
			if(counter > size)
			{
				result = false;
				break;
			}
			*out++ = *p++;
			*out++ = *p++;

			if (*p == ' ')
				continue;

			counter++;
			if(counter > size)
			{
				result = false;
				break;
			}
			*out++ = *p++;
		}
		else
		{
			counter++;
			if(counter > size)
			{
				result = false;
				break;
			}
			*out++ = *in++;
		}
	*out = 0;

	if(!result)
	{
		MESSAGE_ERROR("", "", "buffer size is not enough");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

// --- basic string operators

// --- Quote Words: split string into vector<string> words
// --- input: 
//        	src string, 
//        	reference to dst vector
// --- output: success status
//			1 - success
//			0 - fail
int	qw(const string src, vector<string> &dst)
{
	istringstream iss(src);

    do
    {
        string word;

        iss >> word;
        if(word.length()) dst.push_back(word);
    } while (iss);

	return 1;
}

vector<string> split(const string& s, const char& c)
{
	string buff{""};
	vector<string> v;

	for(auto n:s)
	{
		if(n != c) buff+=n;
		else if(n == c && buff != "")
		{
			v.push_back(buff);
			buff = "";
		}
	}
	if(buff.length()) v.push_back(buff);

	return v;
}

string join(const vector<string>& vec, string separator)
{
	string result{""};

	if(separator.empty()) separator = ",";

	for(auto &item:vec)
	{
		if(result.length()) result += separator;
		result += item;
	}

	return result;
}

auto ReplaceWstringAccordingToMap(const wstring &src, const map<wstring, wstring> &replacements) -> wstring
{
	auto	result(src);
	auto	pos = result.find(L"1"); // --- fake find to deduct type

	MESSAGE_DEBUG("", "", "start");

	for(auto &replacement : replacements)
	{
		pos = 0;

		while((pos = result.find(replacement.first, pos)) != string::npos)
		{
			result.replace(pos, replacement.first.length(), replacement.second);
		}
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

bool AmIMessageOwner(string messageID, CUser *user, CMysql *db)
{
	auto		result = false;
	
	MESSAGE_DEBUG("", "", "start (messageID: " + messageID + ")");

	if(messageID.length() && (messageID != "0"))
	{
		if(db->Query("SELECT `userId`,`srcType` FROM `feed` WHERE `actionTypeId`='11' AND `actionId`=\"" + messageID + "\";"))
		{
			string		messageOwnerID = db->Get(0, "userId");
			string		messageOwnerType = db->Get(0, "srcType");

			if((messageOwnerType == "user") && (messageOwnerID == user->GetID()))
			{
				result = true;
			}
			else if(messageOwnerType == "company")
			{
				if(db->Query("SELECT `id` FROM `company` WHERE `id`=\"" + messageOwnerID + "\" AND `admin_userID`=\"" + user->GetID() + "\";"))
				{
					result = true;
				}
				else
				{
					MESSAGE_ERROR("", "", "user(id:" + user->GetID() + ") not owning company(id:" + messageOwnerID + ")");
				}
			}
			else if(messageOwnerType == "group")
			{
				if(db->Query("SELECT `id` FROM `groups` WHERE `id`=\"" + messageOwnerID + "\" AND `owner_id`=\"" + user->GetID() + "\";"))
				{
					result = true;
				}
				else
				{
					MESSAGE_ERROR("", "", "user(id:" + user->GetID() + ") not owning group(id:" + messageOwnerID + ")");
				}
			}
			else
			{
				{
					MESSAGE_ERROR("", "", "unknown message type (id:" + messageID + ", type:" + messageOwnerType + ")");
				}
			}
		}
		else
		{
			{
				MESSAGE_ERROR("", "", "message.id(" + messageID + ") not found");
			}
		}
	}
	else
	{
		{
			MESSAGE_DEBUG("", "", "message.id(" + messageID + ") empty or 0");
		}
	}

	MESSAGE_DEBUG("", "", "end (returning value is " + (result ? "true" : "false") + ")");

	return result;	
}

pair<string, string> GetMessageOwner(string messageID, CUser *user, CMysql *db)
{
	string		messageOwnerID = "";
	string		messageOwnerType = "";
	
	{
		MESSAGE_DEBUG("", "", "start (messageID: " + messageID + ")");
	}

	if(db->Query("SELECT `userId`,`srcType` FROM `feed` WHERE `actionTypeId`='11' AND `actionId`=\"" + messageID + "\";"))
	{
		messageOwnerID = db->Get(0, "userId");
		messageOwnerType = db->Get(0, "srcType");
	}
	else
	{
		{
			MESSAGE_ERROR("", "", "message.id(" + messageID + ") not found");
		}
	}

	{
		MESSAGE_DEBUG("", "", "end (returning value pair<messageOwnerType, messageOwnerID>(" + messageOwnerType + ", " + messageOwnerID + ")");
	}

	return make_pair(messageOwnerType, messageOwnerID);
}

string	GetUserSubscriptionsInJSONFormat(string sqlQuery, CMysql *db)
{
	int		affected;
	string	result;

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query(sqlQuery);
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			if(result.length()) result += ",";
			result +=	"{";
			result +=	"\"id\":\"" + string(db->Get(i, "id")) + "\",";
			result +=	"\"entity_id\":\"" + string(db->Get(i, "entity_id")) + "\",";
			result +=	"\"entity_type\":\"" + string(db->Get(i, "entity_type")) + "\",";
			result +=	"\"eventTimestamp\":\"" + string(db->Get(i, "eventTimestamp")) + "\"";
			result +=	"}";
		}
	}
	else
	{
		{
			MESSAGE_DEBUG("", "", "user have no active subscriptions to any company or group");
		}
	}

	{
		MESSAGE_DEBUG("", "", "end (returning result length(" + to_string(result.length()) + ")");
	}

	return result;
}

string	SubscribeToCompany(string companyID, CUser *user, CMysql *db)
{
	ostringstream	ostResult;

	MESSAGE_DEBUG("", "", "start");

	ostResult.str("");

	if(user && companyID.length())
	{
		if(db->Query("SELECT * FROM `users_subscriptions` WHERE `user_id`=\"" + user->GetID() + "\" AND `entity_type`=\"company\" AND `entity_id`=\"" + companyID + "\";"))
		{
			MESSAGE_ERROR("", "", "already subscribed [companyID: " + companyID + ", userID: " + user->GetID() + "]");

			ostResult << "\"result\": \"success\", \"description\": \""s + gettext("you are already subscribed") + "\"";
		}
		else
		{
			if(db->InsertQuery("INSERT INTO `users_subscriptions` SET "
								"`user_id`=\"" + user->GetID() + "\","
								"`entity_type`=\"company\","
								"`entity_id`=\"" + companyID + "\","
								"`eventTimestamp`=UNIX_TIMESTAMP();"))
			{
				ostResult << "\"result\": \"success\"";

				// --- insert notification into feed
				if(db->Query("SELECT `id` FROM `feed` WHERE `userId`=\"" + user->GetID() + "\" AND `srcType`=\"user\" AND `actionTypeId`=\"63\" AND `actionId`=\"" + companyID + "\";"))
				{
					MESSAGE_DEBUG("", "", "already subscribed [companyID: " + companyID + ", userID: " + user->GetID() + "]");
				}
				else
				{
					if(db->InsertQuery("INSERT INTO `feed` (`userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"" + user->GetID() + "\", \"63\", \"" + companyID + "\", NOW())"))
					{
					}
					else
					{
						MESSAGE_ERROR("", "", "inserting into `feed` table");
					}
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "error inserting into users subscription [companyID: " + companyID + ", userID: " + user->GetID() + "]");

				ostResult << "\"result\": \"error\",\"description\": \""s + gettext("SQL syntax error") + "\"";
			}
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "(companyID is empty) || (user == NULL) [companyID: " + companyID + ", userID: " + (user ? "not-NULL" : "NULL") + "]");

		ostResult << "\"result\": \"error\",\"description\": \"Компания не найдена или пользователь неопределен\"";
	}


	MESSAGE_DEBUG("", "", "end (returning result length(" + to_string(ostResult.str().length()) + ")");

	return ostResult.str();
}

string	UnsubscribeFromCompany(string companyID, CUser *user, CMysql *db)
{
	ostringstream	ostResult;

	MESSAGE_DEBUG("", "", "start");

	ostResult.str("");

	if(user && companyID.length())
	{
		if(db->Query("SELECT `id` FROM `users_subscriptions` WHERE `user_id`=\"" + user->GetID() + "\" AND `entity_type`=\"company\" AND `entity_id`=\"" + companyID + "\";"))
		{
			string		subscriptionID = db->Get(0, "id");

			if(db->Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + companyID + "\" AND `admin_userID`=\"" + user->GetID() + "\";"))
			{
				{
					MESSAGE_DEBUG("", "", "user can't unsubscribe from own company [companyID: " + companyID + ", userID: " + user->GetID() + "]");
				}

				ostResult << "\"result\": \"error\",\"description\": \"Вы не можете отписаться от собственной компании\"";
			}
			else
			{
				db->Query("DELETE FROM `users_subscriptions` WHERE `id`=\"" + subscriptionID + "\";");
				ostResult << "\"result\": \"success\"";

				// --- remove notification from feed
				if(db->Query("SELECT `id` FROM `feed` WHERE `userId`=\"" + user->GetID() + "\" AND `srcType`=\"user\" AND `actionTypeId`=\"63\" AND `actionId`=\"" + companyID + "\";"))
				{
					db->Query("DELETE FROM `feed` WHERE `userId`=\"" + user->GetID() + "\" AND `actionTypeId`=\"63\" AND `actionId`=\"" + companyID + "\";");
					if(db->isError())
					{
						MESSAGE_ERROR("", "", "removing form `feed` table");
					}
				}
				else
				{
					MESSAGE_DEBUG("", "", "feed subscription missed [companyID: " + companyID + ", userID: " + user->GetID() + "]");
				}

			}
		}
		else
		{
			{
				MESSAGE_ERROR("", "", "user not subscribed to company [companyID: " + companyID + ", userID: " + user->GetID() + "]");
			}

			ostResult << "\"result\": \"success\", \"description\": \"Вы не подписаны\"";
		}
	}
	else
	{
		{
			MESSAGE_ERROR("", "", "(companyID is empty) || (user == NULL) [companyID: " + companyID + ", userID: " + (user ? "not-NULL" : "NULL") + "]");
		}

		ostResult << "\"result\": \"error\",\"description\": \"Компания не найдена или пользователь неопределен\"";
	}


	{
		MESSAGE_DEBUG("", "", "end (returning result length(" + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

string	SubscribeToGroup(string groupID, CUser *user, CMysql *db)
{
	ostringstream	ostResult;

	MESSAGE_DEBUG("", "", "start");

	ostResult.str("");

	if(user && groupID.length())
	{
		if(db->Query("SELECT * FROM `users_subscriptions` WHERE `user_id`=\"" + user->GetID() + "\" AND `entity_type`=\"group\" AND `entity_id`=\"" + groupID + "\";"))
		{
			{
				MESSAGE_ERROR("", "", "already subscribed [groupID: " + groupID + ", userID: " + user->GetID() + "]");
			}

			ostResult << "\"result\": \"success\", \"description\": \"вы уже подписаны на новости этой группы\"";
		}
		else
		{
			if(db->InsertQuery("INSERT INTO `users_subscriptions` SET "
								"`user_id`=\"" + user->GetID() + "\","
								"`entity_type`=\"group\","
								"`entity_id`=\"" + groupID + "\","
								"`eventTimestamp`=UNIX_TIMESTAMP();"))
			{
				ostResult << "\"result\": \"success\"";

				// --- insert notification into feed
				if(db->Query("SELECT `id` FROM `feed` WHERE `userId`=\"" + user->GetID() + "\" AND `srcType`=\"user\" AND `actionTypeId`=\"64\" AND `actionId`=\"" + groupID + "\";"))
				{
					MESSAGE_DEBUG("", "", "already subscribed [groupID: " + groupID + ", userID: " + user->GetID() + "]");
				}
				else
				{
					if(db->InsertQuery("INSERT INTO `feed` (`userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"" + user->GetID() + "\", \"64\", \"" + groupID + "\", NOW())"))
					{
					}
					else
					{
						MESSAGE_ERROR("", "", "inserting into `feed` table");
					}
				}
			}
			else
			{
				{
					MESSAGE_ERROR("", "", "error inserting into users subscription [groupID: " + groupID + ", userID: " + user->GetID() + "]");
				}

				ostResult << "\"result\": \"error\",\"description\": \"Ошибка БД\"";
			}
		}
	}
	else
	{
		{
			MESSAGE_ERROR("", "", "(groupID is empty) || (user == NULL) [groupID: " + groupID + ", userID: " + (user ? "not-NULL" : "NULL") + "]");
		}

		ostResult << "\"result\": \"error\",\"description\": \"Компания не найдена или пользователь неопределен\"";
	}


	{
		MESSAGE_DEBUG("", "", "end (returning result length(" + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

string	UnsubscribeFromGroup(string groupID, CUser *user, CMysql *db)
{
	ostringstream	ostResult;

	MESSAGE_DEBUG("", "", "start");

	ostResult.str("");

	if(user && groupID.length())
	{
		if(db->Query("SELECT `id` FROM `users_subscriptions` WHERE `user_id`=\"" + user->GetID() + "\" AND `entity_type`=\"group\" AND `entity_id`=\"" + groupID + "\";"))
		{
			string		subscriptionID = db->Get(0, "id");

			if(db->Query("SELECT `owner_id` FROM `groups` WHERE `id`=\"" + groupID + "\" AND `owner_id`=\"" + user->GetID() + "\";"))
			{
				{
					MESSAGE_DEBUG("", "", "user can't unsubscribe from own group [groupID: " + groupID + ", userID: " + user->GetID() + "]");
				}

				ostResult << "\"result\": \"error\",\"description\": \"Вы не можете отписаться от собственной группы\"";
			}
			else
			{
				db->Query("DELETE FROM `users_subscriptions` WHERE `id`=\"" + subscriptionID + "\";");
				ostResult << "\"result\": \"success\"";
				
				// --- remove notification from feed
				if(db->Query("SELECT `id` FROM `feed` WHERE `userId`=\"" + user->GetID() + "\" AND `srcType`=\"user\" AND `actionTypeId`=\"64\" AND `actionId`=\"" + groupID + "\";"))
				{
					db->Query("DELETE FROM `feed` WHERE `userId`=\"" + user->GetID() + "\" AND `actionTypeId`=\"64\" AND `actionId`=\"" + groupID + "\";");
					if(db->isError())
					{
						MESSAGE_ERROR("", "", "removing form `feed` table");
					}
				}
				else
				{
					MESSAGE_DEBUG("", "", "feed subscription missed [groupID: " + groupID + ", userID: " + user->GetID() + "]");
				}
			}
		}
		else
		{
			{
				MESSAGE_ERROR("", "", "user not subscribed to group [groupID: " + groupID + ", userID: " + user->GetID() + "]");
			}

			ostResult << "\"result\": \"success\", \"description\": \"Вы не подписаны\"";
		}
	}
	else
	{
		{
			MESSAGE_ERROR("", "", "(groupID is empty) || (user == NULL) [groupID: " + groupID + ", userID: " + (user ? "not-NULL" : "NULL") + "]");
		}

		ostResult << "\"result\": \"error\",\"description\": \"Компания не найдена или пользователь неопределен\"";
	}


	{
		MESSAGE_DEBUG("", "", "end (returning result length(" + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

bool isBotIP(string ip)
{
	bool			result = false;
	vector<string>	botsIPs = {
		// --- Google bots
		string("66.249.64."),
		string("66.249.65."),
		string("66.249.66."),
		string("66.249.67."),
		string("66.249.68."),
		string("66.249.69."),
		string("66.249.71."),
		string("66.249.72."),
		string("66.249.73."),
		string("66.249.74."),
		string("66.249.75."),
		string("66.249.76."),
		string("66.249.77."),
		string("66.249.78."),
		string("66.249.79."),
		string("66.249.80."),
		string("66.249.81."),
		string("66.249.82."),
		string("66.249.83."),
		string("66.249.84."),
		string("66.249.85."),
		string("66.249.86."),
		string("66.249.87."),
		string("66.249.88."),
		string("66.249.89."),
		string("66.249.90."),
		string("66.249.91."),
		string("66.249.92."),
		string("66.249.93."),
		string("66.249.94."),
		string("66.249.95.")
		};

	MESSAGE_DEBUG("", "", "start");

	if(ip.length())
	{
		for(string &tmp : botsIPs)
		{
			if(ip.find(tmp) != string::npos)
			{
				result = true;
				break;
			}
		}
	}
	else
	{
		{
			MESSAGE_DEBUG("", "", "IP is empty");
		}
	}

	{
		MESSAGE_DEBUG("", "", "end (result  = " + (result ? "true" : "false") + ")");
	}

	return result;
}


bool isAdverseWordsHere(string text, CMysql *db)
{
	bool			result = false;
	int				affected;

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("SELECT * FROM `dictionary_adverse`;");
	if(affected)
	{
		text = toLower(text);

		for(int i = 0; i < affected; i++)
		{
			string		checkingWord = db->Get(i, "word");

			if(text.find(checkingWord) != string::npos)
			{
				{
					MESSAGE_DEBUG("", "", "adverse word.id[" + db->Get(i, "id") + "]");
				}

				result = true;
				break;
			}
		}
	}
	else
	{
		{
			MESSAGE_DEBUG("", "", "adverse_word table is empty");
		}

	}

	
	{
		MESSAGE_DEBUG("", "", "end (result  = " + (result ? "true" : "false") + ")");
	}
	
	return result;
}

string ParseGPSLongitude(const string longitudeStr)
{
	string  result = "";
	smatch  cm;
	regex   format1("[-+]?[[:digit:]]+(\\.[[:digit:]]+)?");
	regex   format2("([EW])\\:[[:space:]]*([[:digit:]]+)((\\/)([[:digit:]]+))?\\,[[:space:]]*([[:digit:]]+)((\\/)([[:digit:]]+))?\\,[[:space:]]*([[:digit:]]+)((\\/)([[:digit:]]+))?", regex_constants::ECMAScript | regex_constants::icase);
	regex   format3(".*unknown.*");
	regex   format4(".[[:space:]]*");

	MESSAGE_DEBUG("", "", "start(" + longitudeStr + ")");

	// --- format: +74.56 or 74.56 or 74
	if(regex_match(longitudeStr, format1))
	{
		result = longitudeStr;
	}
	else if(regex_match(longitudeStr, cm, format2))
	{
		string	ref = cm[1];
		string  degreeNumerator = cm[2];
		string  degreeDivisor = cm[5];
		string  minutesNumerator = cm[6];
		string  minutesDivisor = cm[9];
		string  secondsNumerator = cm[10];
		string  secondsDivisor = cm[13];
		float	temp = 0;

		if(!degreeDivisor.length() || !stof(degreeDivisor)) degreeDivisor = "1";
		if(!minutesDivisor.length() || !stof(minutesDivisor)) minutesDivisor = "1";
		if(!secondsDivisor.length() || !stof(secondsDivisor)) secondsDivisor = "1";

		if(stof(minutesNumerator)/stof(minutesDivisor) > 60)
		{
			// --- minutes couldn't be greater than 60
			{
				MESSAGE_ERROR("", "", " " + minutesNumerator + "/" + minutesDivisor + " minutes can't be greater 60");
			}
		}
		else if(stof(secondsNumerator)/stof(secondsDivisor) > 60)
		{
			// --- seconds couldn't be greater than 60
			{
				MESSAGE_ERROR("", "", " " + secondsNumerator + "/" + secondsDivisor + " seconds can't be greater 60");
			}
		}
		else
		{
			temp = stof(degreeNumerator)/stof(degreeDivisor) + stof(minutesNumerator)/stof(minutesDivisor)/60 + stof(secondsNumerator)/stof(secondsDivisor)/3600;
			result = (ref.length() && (toupper(ref[0]) == 'W') ? "-" : "+") + to_string(temp);
		}

	}
	else if(regex_match(longitudeStr, format3))
	{
	}
	else if(regex_match(longitudeStr, format4))
	{
	}
	else if(longitudeStr.length() == 0)
	{
	}
	else
	{
		{
			MESSAGE_ERROR("", "", " longitude(" + longitudeStr + ") didn't match any pattern");
		}
	}

	MESSAGE_DEBUG("", "", "finish (" + result + ")");

	return result;
}

string ParseGPSLatitude(const string latitudeStr)
{
	string  result = "";
	smatch  cm;
	regex   format1("[-+]?[[:digit:]]+(\\.[[:digit:]]+)?");
	regex   format2("([NS])\\:[[:space:]]*([[:digit:]]+)((\\/)([[:digit:]]+))?\\,[[:space:]]*([[:digit:]]+)((\\/)([[:digit:]]+))?\\,[[:space:]]*([[:digit:]]+)((\\/)([[:digit:]]+))?", regex_constants::ECMAScript | regex_constants::icase);
	regex   format3(".*unknown.*");
	regex   format4(".[[:space:]]*");

	{
		MESSAGE_DEBUG("", "", "start(" + latitudeStr + ")");
	}
	
	// --- format: +74.56 or 74.56 or 74
	if(regex_match(latitudeStr, regex(format1)))
	{
		result = latitudeStr;
	}
	else if(regex_match(latitudeStr, cm, format2))
	{
		string	ref = cm[1];
		string  degreeNumerator = cm[2];
		string  degreeDivisor = cm[5];
		string  minutesNumerator = cm[6];
		string  minutesDivisor = cm[9];
		string  secondsNumerator = cm[10];
		string  secondsDivisor = cm[13];
		float	temp = 0;

		if(!degreeDivisor.length() || !stof(degreeDivisor)) degreeDivisor = "1";
		if(!minutesDivisor.length() || !stof(minutesDivisor)) minutesDivisor = "1";
		if(!secondsDivisor.length() || !stof(secondsDivisor)) secondsDivisor = "1";

		if(stof(minutesNumerator)/stof(minutesDivisor) > 60)
		{
			// --- minutes couldn't be greater than 60
			{
				MESSAGE_ERROR("", "", " " + minutesNumerator + "/" + minutesDivisor + " minutes can't be greater 60");
			}
		}
		else if(stof(secondsNumerator)/stof(secondsDivisor) > 60)
		{
			// --- seconds couldn't be greater than 60
			{
				MESSAGE_ERROR("", "", " " + secondsNumerator + "/" + secondsDivisor + " seconds can't be greater 60");
			}
		}
		else
		{
			temp = stof(degreeNumerator)/stof(degreeDivisor) + stof(minutesNumerator)/stof(minutesDivisor)/60 + stof(secondsNumerator)/stof(secondsDivisor)/3600;
			result = (ref.length() && (toupper(ref[0]) == 'S') ? "-" : "+") + to_string(temp);
		}

	}
	else if(regex_match(latitudeStr, format3))
	{
	}
	else if(regex_match(latitudeStr, format4))
	{
	}
	else if(latitudeStr.length() == 0)
	{
	}

	else
	{
			{
				MESSAGE_ERROR("", "", " latitude(" + latitudeStr + ") didn't match any pattern");
			}
	}

	{
		MESSAGE_DEBUG("", "", "finish (" + result + ")");
	}

	return result;
}

string ParseGPSAltitude(const string altitudeStr)
{
	string  result = "";
	smatch  cm;
	regex   format1("[-+]?[[:digit:]]+(\\.[[:digit:]]+)?");
	regex   format2(".*\\:[[:space:]]*([[:digit:]]+)(\\/)([[:digit:]]+).*");
	regex   format3(".*unknown.*");
	regex   format4(".[[:space:]]*");
	regex   format5("([[:digit:]]+)(\\/)([[:digit:]]+)\n.*");
	regex   format6("[^[:digit:]]*([[:digit:]]+)(\\/)([[:digit:]]+)[^[:digit:]]*");

	MESSAGE_DEBUG("", "", "start(" + altitudeStr + ")");
	
	// --- format: +74.56 or 74.56 or 74
	if(regex_match(altitudeStr, regex(format1)))
	{
		result = altitudeStr;
	}
	else if(regex_match(altitudeStr, cm, format2) || regex_match(altitudeStr, cm, format5) || regex_match(altitudeStr, cm, format6))
	{
		string		altitudeNumerator = cm[1];
		string		altitudeDivisor = cm[3];
		float		temp = 0;

		if(!altitudeDivisor.length() || !stof(altitudeDivisor)) altitudeDivisor = "1";

		temp = stof(altitudeNumerator)/stof(altitudeDivisor);
		result = (temp > 0 ? "+" : "") + to_string(temp);
	}
	else if(regex_match(altitudeStr, format3))
	{
	}
	else if(regex_match(altitudeStr, format4))
	{
	}
	else if(altitudeStr.length() == 0)
	{
	}
	else
	{
		MESSAGE_ERROR("", "", "altitude(" + altitudeStr + ") didn't match any pattern");
	}

	MESSAGE_DEBUG("", "", "finish (" + result + ")");

	return result;
}

string ParseGPSSpeed(const string speedStr)
{
	string  result = "";
	smatch  cm;
	regex   format1("[-+]?[[:digit:]]+(\\.[[:digit:]]+)?");
	regex   format2("K\\:[[:space:]]*([[:digit:]]+)(\\/)([[:digit:]]+)");
	regex   format3(".*unknown.*");
	regex   format4(".[[:space:]]*");

	{
		MESSAGE_DEBUG("", "", "start(" + speedStr + ")");
	}

	// --- format: +74.56 or 74.56 or 74
	if(regex_match(speedStr, regex(format1)))
	{
		result = speedStr;
	}
	else if(regex_match(speedStr, cm, format2))
	{
		string	speedNumerator = cm[1];
		string	speedDivisor = cm[3];
		float		temp = 0;

		if(!speedDivisor.length() || !stof(speedDivisor)) speedDivisor = "1";

		temp = stof(speedNumerator)/stof(speedDivisor);
		result = (temp > 0 ? "+" : "") + to_string(temp);
	}
	else if(regex_match(speedStr, format3))
	{
	}
	else if(regex_match(speedStr, format4))
	{
	}
	else if(speedStr.length() == 0)
	{
	}
	else
	{
			{
				MESSAGE_ERROR("", "", " speed(" + speedStr + ") didn't match any pattern");
			}
	}

	{
		MESSAGE_DEBUG("", "", "finish (" + result + ")");
	}

	return result;
}

string	GetMySQLDateInJSONFormat(string dateString)
{
	string	result = "{}";
	regex   format1("([[:digit:]]+)\\-([[:digit:]]+)\\-([[:digit:]]+)");
	smatch  cm;

	MESSAGE_DEBUG("", "", "start");

	if(regex_match(dateString, cm, format1))
	{
		string	dateYear = cm[1];
		string	dateMonth = cm[2];
		string	dateDate = cm[3];

		result = "{"
				"\"date\":" + to_string(stol(dateDate)) + ", "
				"\"month\":" + to_string(stol(dateMonth)) + ", "
				"\"year\":" + dateYear + ""
				"}";
	}
	else
	{
		MESSAGE_ERROR("", "", "date format taken from DB have to be in format YYYY-MM-DD, but gotten(" + dateString + ")");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

string GetCompanyPositionsInJSONFormat(string dbQuery, CMysql *db, CUser *user)
{
	struct ItemClass
	{
		string	id;
		string	title;
		string	area;
	};

	ostringstream			ost, ostFinal;
	string					sessid, lookForKey;
	int						affected;
	vector<ItemClass>		eventsList;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	ostFinal.str("");

	if((affected = db->Query(dbQuery)) > 0)
	{
		int						eventCounter = affected;

		eventsList.reserve(eventCounter);  // --- reserving allows avoid moving vector in memory
											// --- to fit vector into continuous memory piece

		for(int i = 0; i < affected; i++)
		{
			ItemClass	event;

			event.id = db->Get(i, "id");
			event.title = db->Get(i, "title");
			event.area = db->Get(i, "area");

			eventsList.push_back(event);
		}

		for(int i = 0; i < eventCounter; i++)
		{
				if(ostFinal.str().length()) ostFinal << ", ";

				ostFinal << "{";
				ostFinal << "\"id\": \""				  	<< eventsList[i].id << "\",";
				ostFinal << "\"title\": \""					<< eventsList[i].title << "\",";
				ostFinal << "\"area\": \""					<< eventsList[i].area << "\"";
				ostFinal << "}";
		} // --- for loop through event list
	} // --- if sql-query on event selection success
	else
	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: there are no themes returned by request [", dbQuery, "]");
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: end (result length = " + to_string(ostFinal.str().length()) + ")");
	}

	return ostFinal.str();
}

string GetSiteThemesInJSONFormat(string dbQuery, CMysql *db, CUser *user)
{
	struct ItemClass
	{
		string	id;
		string	name;
		string	path;
	};

	ostringstream			ost, ostFinal;
	string					sessid, lookForKey;
	int						affected;
	vector<ItemClass>		eventsList;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	ostFinal.str("");

	if((affected = db->Query(dbQuery)) > 0)
	{
		int						eventCounter = affected;

		eventsList.reserve(eventCounter);  // --- reserving allows avoid moving vector in memory
											// --- to fit vector into continuous memory piece

		for(int i = 0; i < affected; i++)
		{
			ItemClass	event;

			event.id = db->Get(i, "id");
			event.name = db->Get(i, "name");
			event.path = db->Get(i, "path");

			eventsList.push_back(event);
		}

		for(int i = 0; i < eventCounter; i++)
		{
				if(ostFinal.str().length()) ostFinal << ", ";

				ostFinal << "{";
				ostFinal << "\"id\": \""				  	<< eventsList[i].id << "\",";
				ostFinal << "\"name\": \""					<< eventsList[i].name << "\",";
				ostFinal << "\"path\": \""					<< eventsList[i].path << "\"";
				ostFinal << "}";
		} // --- for loop through event list
	} // --- if sql-query on event selection success
	else
	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: there are no themes returned by request [", dbQuery, "]");
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: end (result length = " + to_string(ostFinal.str().length()) + ")");
	}

	return ostFinal.str();
}

// --- this function different from other "InJSONFormat"-functions (zip_id vs sql_query)
// --- because sql doing INNER JOIN
string GetZipInJSONFormat(string zip_id, CMysql *db, CUser *user)
{
	ostringstream			ostFinal;
	string					dbQuery;

	MESSAGE_DEBUG("", "", "start");

	ostFinal.str("");

	dbQuery = 
				"SELECT  "
					"`geo_zip`.`id` AS `geo_zip_id`, "
					"`geo_locality_id`, "
					"`geo_region_id`, "
					"`geo_country_id`, "
					"`zip`, "
					"`geo_locality`.`title` AS `geo_locality_title`, "
					"`geo_region`.`title` AS `geo_region_title`, "
					"`geo_country`.`title` AS `geo_country_title` "
				"FROM `geo_zip` "
				"INNER JOIN `geo_locality` ON `geo_locality`.`id`=`geo_zip`.`geo_locality_id` "
				"INNER JOIN `geo_region` ON `geo_region`.`id`=`geo_locality`.`geo_region_id` "
				"INNER JOIN `geo_country` ON `geo_country`.`id`=`geo_region`.`geo_country_id` "
				"WHERE `geo_zip`.`id`=\"" + zip_id + "\";";

	if(db->Query(dbQuery))
	{
		ostFinal<< "{"
				<< 	"\"id\": \""	<< db->Get(0, "geo_zip_id") << "\","
				<< 	"\"zip\": \""	<< db->Get(0, "zip") << "\","
				<< 	"\"locality\": {"
				<< 		"\"id\": \""	<< db->Get(0, "geo_locality_id") << "\","
				<< 		"\"title\": \""	<< db->Get(0, "geo_locality_title") << "\","
				<<			"\"region\": {"
				<<				"\"id\": \""	<< db->Get(0, "geo_region_id") << "\","
				<<				"\"title\": \""	<< db->Get(0, "geo_region_title") << "\","
				<<				"\"country\": {"
				<<					"\"id\": \""	<< db->Get(0, "geo_country_id") << "\","
				<<					"\"title\": \""	<< db->Get(0, "geo_country_title") << "\""
				<<				"}"
				<<			"}"
				<< 	"}"
				<< "}";
	} // --- if sql-query on event selection success
	else
	{
		MESSAGE_DEBUG("", "", "zip.id(" + zip_id + ") not found or failed structure");
	}

	MESSAGE_DEBUG("", "", "finish (result length = " + to_string(ostFinal.str().length()) + ")");

	return ostFinal.str();
}

string GetBankInJSONFormat(string sqlQuery, CMysql *db, CUser *user)
{
	struct ItemClass
	{
		string	id;
		string	title;
		string	bik;
		string	account;
		string	geo_zip_id;
		string	address;
		string	phone;
		string	okato;
		string	okpo;
		string	regnum;
		string	srok;
	};

	vector<ItemClass>		items;
	ostringstream			ostFinal;
	int						affected;

	MESSAGE_DEBUG("", "", "start");

	ostFinal.str("");

	affected = db->Query(sqlQuery);
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			ItemClass	item;

			item.id			= db->Get(i, "id");
			item.title		= db->Get(i, "title");
			item.bik		= db->Get(i, "bik");
			item.account	= db->Get(i, "account");
			item.geo_zip_id	= db->Get(i, "geo_zip_id");
			item.address	= db->Get(i, "address");
			item.phone		= db->Get(i, "phone");
			item.okato		= db->Get(i, "okato");
			item.okpo		= db->Get(i, "okpo");
			item.regnum		= db->Get(i, "regnum");
			item.srok		= db->Get(i, "srok");

			items.push_back(item);
		}

		for(auto i = 0u; i < items.size(); ++i)
		{
			if(i) ostFinal << ",";

			ostFinal<< "{"
					<<	"\"id\":\""			<< items[i].id << "\","
					<<	"\"title\":\""		<< items[i].title << "\","
					<<	"\"bik\":\""		<< items[i].bik << "\","
					<<	"\"account\":\""	<< items[i].account << "\","
					<<	"\"geo_zip_id\":["	<< GetZipInJSONFormat(items[i].geo_zip_id, db, user) << "],"
					<<	"\"geo_zip\":\""	<< items[i].geo_zip_id << "\","
					<<	"\"address\":\""	<< items[i].address << "\","
					<<	"\"phone\":\""		<< items[i].phone << "\","
					<<	"\"okato\":\""		<< items[i].okato << "\","
					<<	"\"okpo\":\""		<< items[i].okpo << "\","
					<<	"\"regnum\":\""		<< items[i].regnum << "\","
					<<	"\"srok\":\""		<< items[i].srok << "\""
					<< "}";
		}
	} // --- if sql-query on event selection success
	else
	{
		MESSAGE_DEBUG("", "", "no banks found from SQL-request");
	}

	MESSAGE_DEBUG("", "", "finish (result length = " + to_string(ostFinal.str().length()) + ")");

	return ostFinal.str();
}

string	GetSpelledZIPByID(string id, CMysql *db)
{
	string	result = "";

	MESSAGE_DEBUG("", "", "start");

	if(db->Query(
				"SELECT  "
					"`geo_zip`.`id` AS `geo_zip_id`, "
					"`geo_locality_id`, "
					"`geo_region_id`, "
					"`geo_country_id`, "
					"`zip`, "
					"`geo_locality`.`title` AS `geo_locality_title`, "
					"`geo_region`.`title` AS `geo_region_title`, "
					"`geo_country`.`title` AS `geo_country_title` "
				"FROM `geo_zip` "
				"INNER JOIN `geo_locality` ON `geo_locality`.`id`=`geo_zip`.`geo_locality_id` "
				"INNER JOIN `geo_region` ON `geo_region`.`id`=`geo_locality`.`geo_region_id` "
				"INNER JOIN `geo_country` ON `geo_country`.`id`=`geo_region`.`geo_country_id` "
				"WHERE `geo_zip`.`id`=\"" + id + "\";"
	))
	{
		result = string(db->Get(0, "zip")) + " " + db->Get(0, "geo_country_title") + ", " + db->Get(0, "geo_region_title") + ", " + db->Get(0, "geo_locality_title");
	}
	else
	{
		MESSAGE_ERROR("", "", "geo_zip.id(" + id + ") not found");
	}
	
	MESSAGE_DEBUG("", "", "finish (result.length is " + to_string(result.length()) + ")");

	return result;	
}

string	GetSpelledBankByID(string id, CMysql *db)
{
	string	result = "";

	MESSAGE_DEBUG("", "", "start");

	if(db->Query("SELECT * FROM `banks` WHERE `id`=\"" + id + "\";"))
	{
		string	bank_title = db->Get(0, "title");
		string	bank_bik = db->Get(0, "bik");
		string	bank_geo_zip_id = db->Get(0, "geo_zip_id");
		string	bank_address = db->Get(0, "address");

		result = bank_title + " (БИК: " + bank_bik + ") " + GetSpelledZIPByID(bank_geo_zip_id, db) + ", " + bank_address;
	}
	else
	{
		MESSAGE_ERROR("", "", "bank_id(" + id + ") not found");
	}
	
	MESSAGE_DEBUG("", "", "finish (result.length is " + to_string(result.length()) + ")");

	return result;	
}

auto	stod_noexcept(const string &num) noexcept -> double
{
	auto	result = 0.0;
	auto	decimal_point = (localeconv()->decimal_point ? localeconv()->decimal_point[0] : ',');
	auto	lc_original = ""s;
	auto	lc_en = "en_US.utf8"s;
	auto	space_pos = string::npos;	// --- type casting

	if(decimal_point != '.')
	{
		lc_original = setlocale(LC_NUMERIC, NULL);
		setlocale(LC_NUMERIC, lc_en.c_str());
		// --- commented due to performance
		// MESSAGE_DEBUG("", "", "temporary switch locale from " + lc_original + " to " + lc_en)
	}

	try
	{
		if((space_pos = num.find(' ')) != string::npos)
		{
			auto	temp = num;
			// --- remove any spaces
			do
			{
				temp.erase(space_pos, 1);
			} while((space_pos = temp.find(' ', space_pos)) != string::npos);

			if(temp.length()) result = stod(temp);
		}
		else
		{
			if(num.length()) result = stod(num);
		}
	}
	catch(...)
	{
		MESSAGE_ERROR("", "", "can't convert " + num + " to double");
	}

	if(decimal_point != '.')
	{
		setlocale(LC_NUMERIC, lc_original.c_str());
		// --- commented due to performance
		// MESSAGE_DEBUG("", "", "switch locale back to " + lc_original)
	}

	return result;
}

auto	MaskSymbols(string src, int first_pos, int last_pos) -> string
{
	MESSAGE_DEBUG("", "", "start");

	for(auto i = first_pos; i < last_pos; ++i)
		src[i] = '.';

	MESSAGE_DEBUG("", "", "finish");

	return src;
}

auto ConvertHTMLToText(const wstring &src) -> wstring
{

	auto					result = src;
	map<wstring, wstring>	map_replacement_1 = {
		{L"&quot;", L"\""},
		{L"&gt;", L">"},
		{L"&lt;", L"<"}
	};

	MESSAGE_DEBUG("", "", "start");

	result = ReplaceWstringAccordingToMap(result, map_replacement_1);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto ConvertHTMLToText(const string &src) -> string
{
	return(wide_to_multibyte(ConvertHTMLToText(multibyte_to_wide(src))));
}

// Convert UTF-8 byte string to wstring
auto multibyte_to_wide(std::string const& s) -> wstring
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.from_bytes(s);
}

// Convert wstring to UTF-8 byte string
auto wide_to_multibyte(std::wstring const& s) -> string
{
	std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	return conv.to_bytes(s);
}

auto CutTrailingZeroes(string number) -> string
{
    size_t   dec_point_place = number.find(".");
    
    if(dec_point_place != string::npos)
    {
        bool   domore = true;
        int    symbols_to_cut = 0;

        for(string::reverse_iterator rit = number.rbegin(); domore && (rit != number.rend()); ++rit)
        {
            if(*rit == '0') ++symbols_to_cut;
            else if(*rit == '.') { ++symbols_to_cut; domore = false;}
            else domore = false;
        }

        number.erase(number.length() - symbols_to_cut);
    }
    
    return number;
}

auto DateInPast(string date_to_check) -> bool
{
	MESSAGE_DEBUG("", "", "start");

	auto	result = false;
	auto	date_to_check_obj = GetTMObject(date_to_check);

	time_t	rawtime;
	struct	tm *timeinfo_temp, timeinfo_now;

	time(&rawtime);
	timeinfo_temp = localtime (&rawtime);
	timeinfo_now = *timeinfo_temp;
	
	timeinfo_now.tm_sec	= 0;   // seconds of minutes from 0 to 61
	timeinfo_now.tm_min	= 0;   // minutes of hour from 0 to 59
	timeinfo_now.tm_hour = 0;  // hours of day from 0 to 24

	if(date_to_check_obj < timeinfo_now) result = true;

	MESSAGE_DEBUG("", "", "finish (result is " + to_string(result) + ")");

	return result;
}

auto	GetHelpDeskTicketsInJSONFormat(string sqlQuery, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	int		affected;
	string	result;


	struct ItemClass
	{
		string	id;
		string	customer_user_id;
		string	severity;
		string	title;
	};
	vector<ItemClass>		itemsList;


	affected = db->Query(sqlQuery);
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			ItemClass	item;

			item.id = db->Get(i, "id");
			item.customer_user_id = db->Get(i, "customer_user_id");
			item.title = db->Get(i, "title");

			itemsList.push_back(item);
		}

		for (const auto& item : itemsList)
		{
			if(result.length()) result += ",";
			result +=	"{";


			result += "\"id\":\"" + item.id + "\",";
			result += "\"customer_user_id\":\"" + item.customer_user_id + "\",";
			result += "\"users\":[" + GetHelpdeskBaseUserInfoInJSONFormat("SELECT * FROM `users` WHERE `id`=" + quoted(item.customer_user_id) + ";", db, user) + "],";
			result += "\"title\":\"" + item.title + "\",";
			result += "\"history\":[" + GetHelpDeskTicketHistoryInJSONFormat("SELECT * FROM `helpdesk_ticket_history` WHERE `helpdesk_ticket_id`=\"" + item.id + "\";", db, user) + "]";
			result +=	"}";
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "no ticket found");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto	GetHelpDeskTicketHistoryInJSONFormat(string sqlQuery, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	int		affected;
	string	result;


	struct ItemClass
	{
		string	id;
		string	customer_user_id;
		string	user_id;
		string	state;
		string	severity;
		string	description;
		string	eventTimestamp;
	};
	vector<ItemClass>		itemsList;


	affected = db->Query(sqlQuery);
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			ItemClass	item;

			item.id = db->Get(i, "id");
			item.user_id = db->Get(i, "user_id");
			item.state = db->Get(i, "state");
			item.severity = db->Get(i, "severity");
			item.description = db->Get(i, "description");
			item.eventTimestamp = db->Get(i, "eventTimestamp");

			itemsList.push_back(item);
		}

		for (const auto& item : itemsList)
		{
			if(result.length()) result += ",";
			result +=	"{";


			result += "\"id\":\"" + item.id + "\",";
			result += "\"users\":[" + GetHelpdeskBaseUserInfoInJSONFormat("SELECT * FROM `users` WHERE `id`=" + quoted(item.user_id) + ";", db, user) + "],";
			result += "\"files\":[" + GetHelpDeskTicketAttachInJSONFormat("SELECT * FROM `helpdesk_ticket_attaches` WHERE `helpdesk_ticket_history_id`=" + quoted(item.id) + ";", db, user) + "],";
			result += "\"state\":\"" + item.state + "\",";
			result += "\"severity\":\"" + item.severity + "\",";
			result += "\"description\":\"" + item.description + "\",";
			result += "\"eventTimestamp\":\"" + item.eventTimestamp + "\"";
			result +=	"}";
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "no history found for helpdesk_ticket (" + sqlQuery + ")");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto	GetHelpDeskTicketAttachInJSONFormat(string sqlQuery, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	int		affected;
	string	result;


	struct ItemClass
	{
		string	id;
		string	helpdesk_ticket_history_id;
		string	filename;
	};
	vector<ItemClass>		itemsList;


	affected = db->Query(sqlQuery);
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			ItemClass	item;

			item.id = db->Get(i, "id");
			item.helpdesk_ticket_history_id = db->Get(i, "helpdesk_ticket_history_id");
			item.filename = db->Get(i, "filename");

			itemsList.push_back(item);
		}

		for (const auto& item : itemsList)
		{
			if(result.length()) result += ",";
			result +=	"{";


			result += "\"id\":\"" + item.id + "\",";
			result += "\"helpdesk_ticket_history_id\":\"" + item.helpdesk_ticket_history_id + "\",";
			result += "\"filename\":\"" + item.filename + "\"";
			result +=	"}";
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "no attaches to the helpdesk_ticket_history");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto CleanupFilename(string filename) -> string
{
	MESSAGE_DEBUG("", "", "start (" + filename + ")");

	auto found = filename.find_first_not_of("."); // --- type derivation;

	while((found = filename.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890_-().")) != string::npos)
	{
		filename.replace(found, 1, "_");
	}

	MESSAGE_DEBUG("", "", "finish (" + filename + ")");

	return filename;
}

auto	isHelpdeskTicketOwner(string ticket_id, string user_id, CMysql *db, CUser *user) -> bool
{
	MESSAGE_DEBUG("", "", "start");

	auto	result = false;

	if(db->Query("SELECT `id` FROM `helpdesk_tickets` WHERE `id`=" + quoted(ticket_id) + " AND `customer_user_id`=" + quoted(user_id) + ";"))
	{
		result = true;
	}
	else
	{
		MESSAGE_DEBUG("", "", "helpdesk_ticket.id(" + ticket_id + ") not owned by user.id(" + user_id + ")");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

auto	GetFAQInJSONFormat(string sqlQuery, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	int		affected;
	string	result;


	struct ItemClass
	{
		string	id;
		string	title;
		string	description;
	};
	vector<ItemClass>		itemsList;


	affected = db->Query(sqlQuery);
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			ItemClass	item;

			item.id = db->Get(i, "id");
			item.title = db->Get(i, "title");
			item.description = db->Get(i, "description");

			itemsList.push_back(item);
		}

		for (const auto& item : itemsList)
		{
			if(result.length()) result += ",";
			result +=	"{";


			result += "\"id\":\"" + item.id + "\",";
			result += "\"title\":\"" + item.title + "\",";
			result += "\"description\":\"" + item.description + "\"";
			result +=	"}";
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "no attaches to the helpdesk_ticket_history");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}


auto	GetValueFromDB(string sql, CMysql *db) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	result = ""s;

	if(db->Query(sql)) result = db->Get(0, 0);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto	GetValuesFromDB(string sql, CMysql *db) -> vector<string>
{
	MESSAGE_DEBUG("", "", "start");

	vector<string>	result;
	auto			affected = db->Query(sql);

	for(auto i = 0; i < affected; ++i) result.push_back(db->Get(i, 0));

	MESSAGE_DEBUG("", "", "finish (result size is " + to_string(result.size()) + ")");

	return result;
}

static bool CheckImageFileInTempFolder(string src, string dst, string f_type)
{
	bool		result = false;

	MESSAGE_DEBUG("", "", "start (src: " + src + ", dst: " + dst + ")");

#ifndef IMAGEMAGICK_DISABLE
	// Construct the image object. Separating image construction from the
	// the read operation ensures that a failure to read the image file
	// doesn't render the image object useless.
	try {
		Magick::Image		   image;
		Magick::OrientationType imageOrientation;
		Magick::Geometry		imageGeometry;

		// Read a file into image object
		image.read( src );

		imageGeometry = image.size();
		imageOrientation = image.orientation();

		MESSAGE_DEBUG("", "", "imageOrientation = " + to_string(imageOrientation) + ", xRes = " + to_string(imageGeometry.width()) + ", yRes = " + to_string(imageGeometry.height()))

		if(imageOrientation == Magick::TopRightOrientation) image.flop();
		if(imageOrientation == Magick::BottomRightOrientation) image.rotate(180);
		if(imageOrientation == Magick::BottomLeftOrientation) { image.flop(); image.rotate(180); }
		if(imageOrientation == Magick::LeftTopOrientation) { image.flop(); image.rotate(-90); }
		if(imageOrientation == Magick::RightTopOrientation) image.rotate(90);
		if(imageOrientation == Magick::RightBottomOrientation) { image.flop(); image.rotate(90); }
		if(imageOrientation == Magick::LeftBottomOrientation) image.rotate(-90);

		if((imageGeometry.width() > GetSpecificData_GetMaxWidth(f_type)) || (imageGeometry.height() > GetSpecificData_GetMaxHeight(f_type)))
		{
			int   newHeight, newWidth;
			if(imageGeometry.width() >= imageGeometry.height())
			{
				newWidth = GetSpecificData_GetMaxWidth(f_type);
				newHeight = newWidth * imageGeometry.height() / imageGeometry.width();
			}
			else
			{
				newHeight = GetSpecificData_GetMaxHeight(f_type);
				newWidth = newHeight * imageGeometry.width() / imageGeometry.height();
			}

			image.resize(Magick::Geometry(newWidth, newHeight, 0, 0));
		}

		// --- strip EXIF info
		image.strip();

		// Write the image to a file
		image.write( dst );
		result = true;
	}
	catch( Magick::Exception &error_ )
	{
		MESSAGE_DEBUG("", "", "ImageMagick read/write thrown exception [" + error_.what() + "]");
	}
#else

	MESSAGE_DEBUG("", "", "simple file coping, because ImageMagick++ is not activated");

	CopyFile(src, dst);
	result = true;

#endif

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

static string SaveOrCheckFileFromHandler(string f_name, string f_type, CFiles *files, string file_extension, bool is_check_only)
{
	MESSAGE_DEBUG("", "", "start (f_name = " + f_name + ", f_type = " + f_type + ", check_only = " + to_string(is_check_only) + ")");

	auto		result = ""s;
	auto		error_message = ""s;
	auto		finalFilename = ""s;
	auto		originalFilename = ""s;
	auto		preFinalFilename = ""s;
	auto		fileName = ""s;
	auto		fileExtension = ""s;
	auto		filePrefix = ""s;
	auto		folderID = 0;
	
	if(f_name.length())
	{
		if(f_type.length())
		{
			if(files->GetSize(f_name) && (files->GetSize(f_name) <= GetSpecificData_GetMaxFileSize(f_type)))
			{
				FILE	*f;

				//--- check logo file existing
				do
				{
					std::size_t  	foundPos;

					folderID = (int)(rand()/(RAND_MAX + 1.0) * GetSpecificData_GetNumberOfFolders(f_type)) + 1;
					filePrefix = GetRandom(20);

					if((foundPos = f_name.rfind(".")) != string::npos) 
						fileExtension = f_name.substr(foundPos, f_name.length() - foundPos);
					else
					{
						MESSAGE_ERROR("", "", "fileExtension MUST be predefined, if workflow gets here then filename doesn't contains extension which is wrong. Require to check subcontractor.cpp:AJAX_sumitBT part");
						fileExtension = ".jpg";
					}

					originalFilename = "/tmp/tmp_" + filePrefix + fileExtension;
					preFinalFilename = "/tmp/" + filePrefix + fileExtension;
					finalFilename = GetSpecificData_GetBaseDirectory(f_type) + "/" + to_string(folderID) + "/" + filePrefix + fileExtension;
				} while(isFileExists(finalFilename) || isFileExists(originalFilename) || isFileExists(preFinalFilename));

				MESSAGE_DEBUG("", "", "Save file to /tmp for checking of image validity [" + originalFilename + "]");

				// --- Save file to "/tmp/" for checking of image validity
				f = fopen(originalFilename.c_str(), "w");
				if(f)
				{
					fwrite(files->Get(f_name), files->GetSize(f_name), 1, f);
					fclose(f);

					if(file_extension == ".jpg")
					{
						if(CheckImageFileInTempFolder(originalFilename, preFinalFilename, f_type)) {}
						else
						{
							error_message = gettext("incorrect image file") + ", "s + gettext("try to upload as pdf");
							MESSAGE_ERROR("", "", error_message + " (" + f_name + ") ");
						}
					}
					else if(file_extension == ".pdf")
					{
						CopyFile(originalFilename, preFinalFilename);
					}
					else
					{
						error_message = gettext("unknown file type");
						MESSAGE_ERROR("", "", error_message);
					}
				}
				else
				{
					error_message = "fail writing to temp file " + originalFilename;
					MESSAGE_ERROR("", "", error_message);
				}
			}
			else
			{
				error_message = f_name + " size(" + to_string(GetSpecificData_GetMaxFileSize(f_type)) + ") is beyond the limit (1, " + to_string(GetSpecificData_GetMaxFileSize(f_type)) + ")";
				MESSAGE_ERROR("", "", error_message);
			}
		}
		else
		{
			error_message = "f_type is empty";
			MESSAGE_ERROR("", "", error_message);
		}
	}
	else
	{
		error_message = "f_name is empty";
		MESSAGE_ERROR("", "", error_message);
	}

	if(is_check_only)
	{
		if(preFinalFilename.length()) unlink(preFinalFilename.c_str());

		if(error_message.empty())
		{
			unlink(originalFilename.c_str());
		}
		else
		{
			MESSAGE_ERROR("", "", "keep file copy that failed sanity check (" + originalFilename + ")")
			result = error_message;
		}
	}
	else
	{
		// --- copy checked file from temp folder to final place 
		if(error_message.empty())
		{
			unlink(originalFilename.c_str());

			CopyFile(preFinalFilename, finalFilename);

			unlink(preFinalFilename.c_str());

			result = to_string(folderID) + "/" + filePrefix + fileExtension;
		}
		else
		{
			MESSAGE_ERROR("", "", "file(" + originalFilename + ") neither image, nor pdf");
		}
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string	CheckFileFromHandler(string f_name, string f_type, CFiles *files, string file_extension) // --- f_type required to check file size specific to file type
{
	return(SaveOrCheckFileFromHandler(f_name, f_type, files, file_extension, true));
}

string	SaveFileFromHandler(string f_name, string f_type, CFiles *files, string file_extension)
{
	return(SaveOrCheckFileFromHandler(f_name, f_type, files, file_extension, false));
}

bool isFileExists(const std::string& name)
{
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

bool isDirExists(const std::string& name)
{
	struct stat buffer;
	auto		result = false;

	MESSAGE_DEBUG("", "", "start");

	if(stat(name.c_str(), &buffer) != 0)
	{
		MESSAGE_DEBUG("", "", "can't access " + name);
	}
	else if(buffer.st_mode & S_IFDIR)
	{
		result = true;
	}
	else
	{
		MESSAGE_DEBUG("", "", name + " isn't directory");
	}

	MESSAGE_DEBUG("", "", "finish (file " + name + " " + (result ? "" : "not") + " exists)");

	return result;
}

bool CreateDir(const string &dir)
{
	auto		result = false;
	struct stat buffer;

	MESSAGE_DEBUG("", "", "start (" + dir + ")");

	if(stat(dir.c_str(), &buffer) != 0)
	{
		if ((mkdir(dir.c_str(), 0750) != 0) && (errno != EEXIST))
		{
			MESSAGE_ERROR("", "", "can't create directory(" + dir + ")");
		}
		else
			result = true;
	}
	else
	{
		MESSAGE_ERROR("", "", "dir(" + dir + ") already exists")
	}

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool RmDirRecursive(const char *dirname)
{
	DIR		*dir;
	struct	dirent *entry;
	char	path[4096];

	MESSAGE_DEBUG("", "", "start (" + dirname + ")");
	
	dir = opendir(dirname);

	if(dir == NULL) 
	{
		MESSAGE_ERROR("", "", "fail to open dir("s + dirname + ")")
		return false;
	}
	
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) 
		{
			snprintf(path, (size_t) 4095, "%s%s", dirname, entry->d_name);

			if (entry->d_type == DT_DIR)
			{
				RmDirRecursive((string(path) + "/").c_str());
			} 
			else
			{

				MESSAGE_DEBUG("", "", "remove file " + path);
				unlink(path);
			}
		}
	}
	
	closedir(dir);
	
	MESSAGE_DEBUG("", "", "remove dir " + dirname);
	rmdir(dirname);
	
	MESSAGE_DEBUG("", "", "finish (" + dirname + ")");

	return true;
}

auto	GetGeoCountryListInJSONFormat(string dbQuery, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	struct ItemClass
	{
		string	id;
		string	title;
	};
	vector<ItemClass>		itemsList;
	auto					affected = db->Query(dbQuery);
	auto					result = ""s;

	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			ItemClass	item;

			item.id = db->Get(i, "id");
			item.title = db->Get(i, "title");

			itemsList.push_back(item);
		}

		for (const auto& item : itemsList)
		{
			if(result.length()) result += ",";
			result +=	"{";

			result += "\"id\":\"" + item.id + "\",";
			result += "\"title\":\"" + item.title + "\"";
			result +=	"}";
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "country list is empty");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto GetHelpdeskBaseUserInfoInJSONFormat(string dbQuery, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto							result = ""s;
	unordered_set<unsigned long>	setOfUserID;

	struct	ItemClass
	{
		string	userID;
		string	userLogin;
		string	userName;
		string	userNameLast;
		string	first_name_en;
		string	last_name_en;
		string	middle_name_en;
		string	country_code;
		string	phone;
		string	email;
		string	userType;
		string	userSex;
		string	userBirthday;
		string	userBirthdayAccess;
		string	userCurrentCityID;
		string	passport_series;
		string	passport_number;
		string	passport_issue_date;
		string	passport_issue_authority;
		string	foreign_passport_number;
		string	foreign_passport_expiration_date;
		string	citizenship_code;
		string	site_theme_id;
		string	userLastOnline;
		string	userLastOnlineSecondSinceY2k;
		string	helpdesk_subscription_S1_sms;
		string	helpdesk_subscription_S2_sms;
		string	helpdesk_subscription_S3_sms;
		string	helpdesk_subscription_S4_sms;
		string	helpdesk_subscription_S1_email;
		string	helpdesk_subscription_S2_email;
		string	helpdesk_subscription_S3_email;
		string	helpdesk_subscription_S4_email;
	};
	vector<ItemClass>		itemsList;
	auto					itemsCount = 0;



	if((itemsCount = db->Query(dbQuery)) > 0)
	{
		for(auto i = 0; i < itemsCount; ++i)
		{
			ItemClass	item;
			item.userID								= db->Get(i, "id");
			item.userLogin							= db->Get(i, "login");
			item.userName							= db->Get(i, "name");
			item.userNameLast						= db->Get(i, "nameLast");
			item.country_code						= db->Get(i, "country_code");
			item.phone								= db->Get(i, "phone");
			item.email								= db->Get(i, "email");
			item.userSex							= db->Get(i, "sex");
			item.userType							= db->Get(i, "type");
			item.site_theme_id						= db->Get(i, "site_theme_id");
			item.userLastOnline						= db->Get(i, "last_online");
			item.userLastOnlineSecondSinceY2k		= db->Get(i, "last_onlineSecondsSinceY2k");
			item.helpdesk_subscription_S1_email		= db->Get(i, "helpdesk_subscription_S1_email");
			item.helpdesk_subscription_S2_email		= db->Get(i, "helpdesk_subscription_S2_email");
			item.helpdesk_subscription_S3_email		= db->Get(i, "helpdesk_subscription_S3_email");
			item.helpdesk_subscription_S4_email		= db->Get(i, "helpdesk_subscription_S4_email");
			item.helpdesk_subscription_S1_sms		= db->Get(i, "helpdesk_subscription_S1_sms");
			item.helpdesk_subscription_S2_sms		= db->Get(i, "helpdesk_subscription_S2_sms");
			item.helpdesk_subscription_S3_sms		= db->Get(i, "helpdesk_subscription_S3_sms");
			item.helpdesk_subscription_S4_sms		= db->Get(i, "helpdesk_subscription_S4_sms");

			itemsList.push_back(item);
		}

		for(auto i = 0; i < itemsCount; i++)
		{
			// --- if user_list have duplicates(1, 2, 1), avoid duplications
			if(setOfUserID.find(stol(itemsList[i].userID)) == setOfUserID.end())
			{
				auto				userID					= itemsList[i].userID;
				auto				userBirthday			= itemsList[i].userBirthday;
				auto				userCurrentCityID		= itemsList[i].userCurrentCityID;
				auto				userCurrentCity			= ""s;
				auto				avatarPath				= ""s;
				auto				userLastOnline			= itemsList[i].userLastOnline;
				auto				userLastOnlineSecondSinceY2k = itemsList[i].userLastOnlineSecondSinceY2k;
				ostringstream		ost1;

				setOfUserID.insert(stol(userID));

				// --- Get user avatars
				avatarPath = "empty";
				if(db->Query("SELECT * from `users_avatars` WHERE `userid`=" + quoted(userID) + " AND `isActive`=\"1\";"))
				{
					avatarPath = "/images/avatars/avatars" + db->Get(0, "folder") + "/" + db->Get(0, "filename");
				}

				if(userCurrentCityID.length() && db->Query("SELECT `title` FROM `geo_locality` WHERE `id`=\"" + userCurrentCityID + "\";"))
					userCurrentCity = db->Get(0, "title");

				if(result.length()) result += ", ";

				if(user && (userID == user->GetID()))
				{
					// --- user have to be able to see his own bday
				}
				else
				{
					if(itemsList[i].userBirthdayAccess == "private") userBirthday = "";
				}

				result += "{"
						"\"id\": \""							+ itemsList[i].userID + "\", "
						"\"name\": \""							+ itemsList[i].userName + "\", "
						"\"nameLast\": \""						+ itemsList[i].userNameLast + "\","
						"\"userSex\": \""						+ itemsList[i].userSex + "\","
						"\"userType\": \""						+ itemsList[i].userType + "\","
						"\"last_online\": \""					+ itemsList[i].userLastOnline + "\","
						"\"last_online_diff\": \""				+ to_string(GetTimeDifferenceFromNow(userLastOnline)) + "\","
						"\"last_onlineSecondsSinceY2k\": \""	+ userLastOnlineSecondSinceY2k + "\","
						"\"avatar\": \""						+ avatarPath + "\","
						"\"site_theme_id\": \""					+ itemsList[i].site_theme_id + "\","
						"\"themes\": ["							+ GetSiteThemesInJSONFormat("SELECT * FROM `site_themes`", db, user) + "],"
						"\"country_code\": \""					+ ((user && (userID == user->GetID())) ? itemsList[i].country_code : "") + "\","
						"\"phone\": \""							+ ((user && (userID == user->GetID())) ? itemsList[i].phone : "") + "\","
						"\"email\": \""							+ ((user && (userID == user->GetID())) ? itemsList[i].email : "") + "\","
						"\"helpdesk_subscriptions_sms\": ["		+ ((user && (userID == user->GetID())) ? quoted(itemsList[i].helpdesk_subscription_S1_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S2_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S3_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S4_sms)  : "") + "],"
						"\"helpdesk_subscriptions_email\": ["	+ ((user && (userID == user->GetID())) ? quoted(itemsList[i].helpdesk_subscription_S1_email) + "," + quoted(itemsList[i].helpdesk_subscription_S2_email) + "," + quoted(itemsList[i].helpdesk_subscription_S3_email) + "," + quoted(itemsList[i].helpdesk_subscription_S4_email)  : "") + "],"
						"\"isMe\": \""							+ ((user && (userID == user->GetID())) ? "yes" : "no") + "\""
						"}";
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "there are users returned by request [" + dbQuery + "]");
	}

	MESSAGE_DEBUG("", "", "finish [length" + to_string(result.length()) + "]");

	return result;
}

auto SendPhoneConfirmationCode(const string &country_code, const string &phone_number, const string &session, CMysql *db, CUser *user) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message = ""s;
	auto	confirmation_code = GetRandom(4);
	c_smsc	smsc(db);

	if(country_code.length() && phone_number.length() && session.length())
	{
		// --- don't move it behind InsertQuery, 
		// --- first clean-up, then insert new token
		RemovePhoneConfirmationCodes(session, db);

		auto	phone_confirmation_id = db->InsertQuery("INSERT INTO `phone_confirmation` (`session`, `confirmation_code`, `country_code`, `phone_number`, `eventTimestamp`)"
														" VALUES ("
															"\"" + session + "\","
															"\"" + confirmation_code + "\","
															"\"" + country_code + "\","
															"\"" + phone_number + "\","
															"UNIX_TIMESTAMP()"
														")"
														);

		if(phone_confirmation_id)
		{
			error_message = smsc.send_sms(country_code + phone_number, "Code " + confirmation_code, 0, "", 0, SMSC_DELIVERY_MODE, SMSC_SENDER_NAME, "", "");

			if(error_message.length())
			{
				MESSAGE_ERROR("", "", error_message);
			}
		}
		else
		{
			error_message = gettext("SQL syntax error");
			MESSAGE_ERROR("", "", error_message);
		}
	}
	else
	{
		error_message = gettext("mandatory parameter missed");
		MESSAGE_ERROR("", "", error_message)
	}

	return error_message;
}

auto CheckPhoneConfirmationCode(const string &confirmation_code, const string &session, CMysql *db, CUser *user) -> vector<pair<string, string>>
{
	MESSAGE_DEBUG("", "", "start");

	vector<pair<string, string>>	error_message;

	if(confirmation_code.length() && session.length())
	{
		auto affected = db->Query("SELECT `id` FROM `phone_confirmation` WHERE "
									"`confirmation_code`=\"" + confirmation_code + "\" AND "
									"`session`=\"" + session + "\" AND "
									"`attempt`<=\"3\";");
		if(affected == 1)
		{
			// --- good2go
			RemovePhoneConfirmationCodes(session, db);
		}
		else if(affected == 0)
		{
			db->Query("UPDATE `phone_confirmation` SET `attempt`=`attempt` + 1 WHERE `session`=\"" + session + "\";");

			if(db->Query("SELECT `attempt` FROM `phone_confirmation` WHERE `session`=\"" + session + "\";"))
			{
				auto	attempts = db->Get(0, "attempt");

				if(stoi(attempts) >= 3) RemovePhoneConfirmationCodes(session, db);

				error_message.push_back(make_pair("attempt", attempts));
			}
			else
			{
				MESSAGE_ERROR("", "", "fail to select data");
			}
			error_message.push_back(make_pair("description", gettext("incorrect confirmation code")));
			MESSAGE_DEBUG("", "", error_message[error_message.size() - 1].second);
		}
		else
		{
			RemovePhoneConfirmationCodes(session, db);
			error_message.push_back(make_pair("description", gettext("there was created more token than allowed") + ", "s + gettext("please try again")));
			MESSAGE_ERROR("", "", error_message[error_message.size() - 1].second);
		}
	}
	else
	{
		error_message.push_back(make_pair("description", gettext("mandatory parameter missed")));
		MESSAGE_ERROR("", "", error_message[error_message.size() - 1].second)
	}

	MESSAGE_DEBUG("", "", "result (" + to_string(error_message.size()) + ")");

	return error_message;
}

auto RemovePhoneConfirmationCodes(string sessid, CMysql *db) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message = ""s;

	db->Query("DELETE FROM `phone_confirmation` WHERE `session`=\"" + sessid + "\";");

	MESSAGE_DEBUG("", "", "finish");

	return error_message;
}

auto	GetCountryCodeAndPhoneNumberBySMSCode(const string &confirmation_code, const string &session, CMysql *db) -> tuple<string, string, string>
{
	auto	country_code = ""s;
	auto	phone_number = ""s;
	auto	password = ""s;

	MESSAGE_DEBUG("", "", "start (" + confirmation_code + ")");

	if(db->Query ("SELECT * FROM `phone_confirmation` WHERE `confirmation_code`=" + quoted(confirmation_code) + " AND `session` = " + quoted(session) + ";"))
	{
		country_code = db->Get(0, "country_code");
		phone_number = db->Get(0, "phone_number");

		if(db->Query("SELECT `passwd` FROM `users_passwd` WHERE "
						"`isActive`=\"true\" "
						"AND "
						"`userID`=(SELECT `id` FROM `users` WHERE `country_code`=" + quoted(country_code) + " AND `phone`=" + quoted(phone_number) + ")"
						";")
			)
		{
			password = db->Get(0, "passwd");
		}
		else
		{
			MESSAGE_ERROR("", "", "workflow not suppose to be here (either two or more users got the same phone number or no active password)");
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "confirmation_code and session");
	}

	MESSAGE_DEBUG("", "", "finish (" + country_code + "," + phone_number + ")");

	return make_tuple(country_code, phone_number, password);
}

auto GetNumberOfCntrl(const wstring &src) -> unsigned int
{
	MESSAGE_DEBUG("", "", "start (" + to_string(src.length()) + ")");

	auto	result = 0u;

	for(auto i = 0u; i < src.length(); ++i)
	{
		result += iswcntrl(src[i]) ? 1 : 0;
	}

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

auto GetNumberOfCntrl(const string &src) -> unsigned int
{
	return GetNumberOfCntrl(multibyte_to_wide(src));
}

auto GetNumberOfPunct(const wstring &src) -> unsigned int
{
	MESSAGE_DEBUG("", "", "start (" + to_string(src.length()) + ")");

	auto	result = 0u;

	for(auto i = 0u; i < src.length(); ++i)
	{
		result += iswpunct(src[i]) ? 1 : 0;
	}

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

auto GetNumberOfPunct(const string &src) -> unsigned int
{
	return GetNumberOfPunct(multibyte_to_wide(src));
}

auto GetNumberOfSpaces(const wstring &src) -> unsigned int
{
	MESSAGE_DEBUG("", "", "start (" + to_string(src.length()) + ")");

	auto	result = 0u;

	for(auto i = 0u; i < src.length(); ++i)
	{
		result += iswspace(src[i]) ? 1 : 0;
	}

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

auto GetNumberOfSpaces(const string &src) -> unsigned int
{
	return GetNumberOfSpaces(multibyte_to_wide(src));
}

auto GetNumberOfDigits(const wstring &src) -> unsigned int
{
	MESSAGE_DEBUG("", "", "start (" + to_string(src.length()) + ")");

	auto	result = 0u;

	for(auto i = 0u; i < src.length(); ++i)
	{
		result += iswdigit(src[i]) ? 1 : 0;
	}

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

auto GetNumberOfDigits(const string &src) -> unsigned int
{
	return GetNumberOfDigits(multibyte_to_wide(src));
}

auto GetNumberOfLetters(const wstring &src) -> unsigned int
{
	MESSAGE_DEBUG("", "", "start (" + to_string(src.length()) + ")");

	auto	src_len = src.length();
	auto	result = src_len - GetNumberOfDigits(src) - GetNumberOfSpaces(src) - GetNumberOfPunct(src) - GetNumberOfCntrl(src);

	MESSAGE_DEBUG("", "", "finish (" + to_string(result) + ")");

	return result;
}

auto GetNumberOfLetters(const string &src) -> unsigned int
{
	return GetNumberOfLetters(multibyte_to_wide(src));
}

auto isDemoDomain() -> bool
{
	auto	result = false;
	string	domain_name = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(getenv("SERVER_NAME")) domain_name = getenv("SERVER_NAME");

	if(domain_name.find("demo") != string::npos) result = true;

	MESSAGE_DEBUG("", "", "result (" + to_string(result) + ")");

	return result;
}

// return mydomain.org instead www.mydomain.org
auto GetDomain() -> string
{
	auto	fqdn		= string(DOMAIN_NAME);
    auto    first_dot	= fqdn.rfind(".");
    auto    second_dot	= first_dot  == string::npos ? string::npos : fqdn.rfind(".", first_dot - 1);
    auto	result		= second_dot == string::npos ? "" : fqdn.substr(second_dot + 1);

    return result;
}

