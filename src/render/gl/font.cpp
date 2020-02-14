/*
 * font.cpp - OpenGL-based Font package using FreeType library
 *
 *  Created on: Feb 13, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/font.h"

FT_Library TextureFont::font = nullptr;

TextureFont::~TextureFont()
{
	if (face != nullptr)
		FT_Done_Face(face);
	if (glyph != nullptr)
		delete [] glyph;
}

void TextureFont::ginit()
{
	if (font != nullptr)
		return;

	if (FT_Init_FreeType(&font)) {
		cerr << "TTF: Can't initialize freetype library - aborted." << endl;
	}
}

void TextureFont::gexit()
{
	if (font != nullptr)
		FT_Done_FreeType(font);
}

void TextureFont::computeTextureSize()
{
//	FT_GlyphSlot slot = face->glyph;
//
//	int roww = 0;
//	int rowh = 0;
//	int w = 0;
//	int h = 0;
//
//	for (int gidx = 0; gidx < face->num_glyphs; gidx++) {
//		if (glyph[gidx].ch == 0)
//			continue;
//		if (roww + glyph[gidx].bw + 1 >= maxTextureSize) {
//			w = max(w, roww);
//			h += rowh;
//			roww = 0;
//			rowh = 0;
//		}
//		roww += glyph[gidx].bw + 1;
//		rowh = max(rowh, (int)glyph[gidx].bh);
//	}
//
//	w = max(w, roww);
//	h += rowh;
//
//	texWidth = w;
//	texHeight = h;
//
//	cout << fmt::sprintf("Texture size: (%d,%d)\n", texWidth, texHeight);
}

void TextureFont::initGlyphs()
{
	FT_GlyphSlot slot = face->glyph;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glyph = new Glyph[face->num_glyphs];
	for (int gidx = 0; gidx < face->num_glyphs; gidx++) {
		if (FT_Load_Glyph(face, gidx, FT_LOAD_RENDER))
			continue;

		GLuint name;
		glGenTextures(1, &name);
		glBindTexture(GL_TEXTURE_2D, name);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
			slot->bitmap.width, slot->bitmap.rows,
			0, GL_ALPHA, GL_UNSIGNED_BYTE,
			slot->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glyph[gidx].glName = name;
		glyph[gidx].ax = slot->advance.x >> 6;
		glyph[gidx].ay = slot->advance.y >> 6;
		glyph[gidx].bw = slot->bitmap.width;
		glyph[gidx].bh = slot->bitmap.rows;
		glyph[gidx].bl = slot->bitmap_left;
		glyph[gidx].bt = slot->bitmap_top;
	}

	// Assign UNICODE code to glyph table
	uint32_t gidx;
	char32_t ch = FT_Get_First_Char(face, &gidx);
	while ( gidx != 0) {
//		cout << fmt::sprintf("Glyph index %d: %08X\n", gidx, uint32_t(ch));
		glyph[gidx].ch = ch;
		ch = FT_Get_Next_Char(face, ch, &gidx);
	}

}

void TextureFont::buildAtlas()
{
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
//	cout << fmt::sprintf("Maximum texture size: %d\n", maxTextureSize);

//	ShaderManager &smgr = scene.getShaderManager();
//	pgm = smgr.createShader("text");

	initGlyphs();


}

TextureFont *TextureFont::load(Context &gl, const fs::path &path, int size, int dpi)
{
	FT_Face face;

	if (FT_New_Face(font, path.string().c_str(), 0, &face) != 0) {
		fmt::fprintf(cerr, "TTF: Can't open font %s: %s\n",
			path, strerror(errno));
		return nullptr;
	}

	if (!FT_IS_SCALABLE(face)) {
		fmt::fprintf(cerr, "TTF: Font %s is not scalable\n",
			path);
		FT_Done_Face(face);
		return nullptr;
	}

	if (FT_Set_Char_Size(face, 0, size << 6, dpi, dpi) != 0) {
		fmt::fprintf(cerr, "TTF: Can't set %i on font %s\n",
			size, path);
		FT_Done_Face(face);
		return nullptr;
	}

	if (FT_Select_Charmap(face, FT_ENCODING_UNICODE) != 0) {
		fmt::fprintf(cerr, "TTF: Unicode not supported on font %s\n",
			path);
		FT_Done_Face(face);
		return nullptr;
	}

	fmt::fprintf(cout, "TTF: Loading %s (%s) %d glyphs...\n",
		face->family_name, face->style_name, face->num_glyphs);


	TextureFont *font = new TextureFont(gl);

	// Initializing glyph database from font files
	font->face = face;
	font->initGlyphs();

	return font;
}
