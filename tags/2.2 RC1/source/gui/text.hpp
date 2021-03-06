
#ifndef __TEXT_HPP
#define __TEXT_HPP

#include "safe_vector.hpp"
#include <string>

#include "wstringEx.hpp"
#include "FreeTypeGX.h"
#include "video.hpp"

#include "smartptr.hpp"

struct SFont
{
	SmartBuf data;
	size_t dataSize;
	SmartPtr<FreeTypeGX> font;
	u32 lineSpacing;
public:
	bool fromBuffer(const u8 *buffer, u32 bufferSize, u32 size, u32 lspacing);
	bool fromFile(const char *filename, u32 size, u32 lspacing);
	bool newSize(u32 size, u32 lspacing);
	SFont(void) : dataSize(0), lineSpacing(0) { }
};

class CText
{
public:
	void setText(SFont font, const wstringEx &t);
	void setText(SFont font, const wstringEx &t, u32 startline);
	void setColor(const CColor &c);
	void setFrame(float width, u16 style, bool ignoreNewlines = false, bool instant = false);
	void tick(void);
	void draw(void);
	int getTotalHeight();
private:
	
	struct SWord
	{
		wstringEx text;
		Vector3D pos;
		Vector3D targetPos;
	};
private:
	typedef safe_vector<SWord> CLine;
	safe_vector<CLine> m_lines;
	SFont m_font;
	CColor m_color;
	u32 firstLine;
	u32 totalHeight;
};

// Nothing to do with CText. Q&D helpers for string formating.
const char *fmt(const char *format, ...);
std::string sfmt(const char *format, ...);
wstringEx wfmt(const wstringEx &format, ...);
bool checkFmt(const wstringEx &ref, const wstringEx &format);
wstringEx vectorToString(const safe_vector<wstringEx> &vect, char sep);
safe_vector<wstringEx> stringToVector(const wstringEx &text, char sep);
safe_vector<std::string> stringToVector(const std::string &text, char sep);
std::string upperCase(std::string text);
std::string lowerCase(std::string text);
std::string ltrim(std::string s);
std::string rtrim(std::string s);

#endif // !defined(__TEXT_HPP)
