#include "edui/helper.hpp"
#include <vector>

Rectangle paddingRect(Rectangle& rect, float padding) {
	Rectangle res = rect;
	res.x += padding;
	res.y += padding;
	res.height -= (padding * 2);
	res.width -= (padding * 2);

	return res;
}

void loadCodepointsBlock(std::vector<int>& vec, CharsBlock block) {
	for (int i = block.start; i <= block.end; i++) {
		vec.push_back(i);
	}
};

std::vector<int> loadFontCodepoints() {
	std::vector<int> codepoints;
	codepoints.reserve(40000);

	loadCodepointsBlock(codepoints, {32, 32 + 96}); //Basic Latin
	loadCodepointsBlock(codepoints, {0xA1, 0xFF}); //Latin-1 Supplement
	loadCodepointsBlock(codepoints, {0x100, 0x17F}); //Latin Extended A
	loadCodepointsBlock(codepoints, {0x370, 0x3FF}); //Greek and Coptic
	loadCodepointsBlock(codepoints, {0x400, 0x4FF}); //Cyrillic
	loadCodepointsBlock(codepoints, {0x1E00, 0x1EFF}); //Latin Extended Additional
	loadCodepointsBlock(codepoints, {0x3000, 0x303F}); //CJK Symbols and Punctuation
	loadCodepointsBlock(codepoints, {0x3040, 0x309F}); //Hiragana
	loadCodepointsBlock(codepoints, {0x30A0, 0x30FF}); //Katakana
	loadCodepointsBlock(codepoints, {0x3200, 0x32FF}); //Enclosed CJK Letters and Months
	loadCodepointsBlock(codepoints, {0x3300, 0x33FF}); //CJK Compatibility
	loadCodepointsBlock(codepoints, {0x4E00, 0x9FFF}); //CJK Unified Ideographs
	loadCodepointsBlock(codepoints, {0xF900, 0xFAFF}); //CJK Compatibility Ideographs

	return codepoints;
};

bool drawOverflownText(Rectangle rect, Font* font, float fontSize, float spacing, const std::string& text, std::string* shownText) {
	bool overflown = false;

	float totalFontSize = font->baseSize * fontSize;
	Vector2 textSize = MeasureTextEx(*font, text.c_str(), totalFontSize, spacing);

	std::string copiedStr = text;
	char *textPtr = copiedStr.data();
	auto codepointCount = GetCodepointCount(text.data());

	int codepointsTotal = 0;

	if (textSize.x > rect.width) {
		int result = text.size() - 1;
		for (int i = 0; i < codepointCount; i++) {
			int codepointSize = 0;
			GetCodepointNext(textPtr, &codepointSize);
			codepointsTotal += codepointSize;

			std::string subStr = TextSubtext(text.c_str(), 0, codepointsTotal);
			Vector2 testTextSize = MeasureTextEx(*font, subStr.c_str(), totalFontSize, spacing);
			if (testTextSize.x > (rect.width - 16)) {
				result = codepointsTotal - 1;

				break;
			}
		}

		*shownText = TextSubtext(text.c_str(), 0, result);
		*shownText = shownText->append("...");

		overflown = true;
	} else {
		*shownText = text;

		overflown = false;
	}

	return overflown;
}
