/*
 * font.cpp - OpenGL-based Font package using FreeType library
 *
 *  Created on: Feb 13, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/context.h"
#include "render/gl/font.h"

FT_Library TextureFont::font = nullptr;

TextureFont::~TextureFont()
{
	for (int gidx = 0; gidx < face->num_glyphs; gidx++) {
		if (glyph[gidx].glName != 0)
			glDeleteTextures(1, &glyph[gidx].glName);
	}
	if (glyph != nullptr)
		delete [] glyph;
	if (face != nullptr)
		FT_Done_Face(face);
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

void TextureFont::render(const string &text, float x, float y, const Color &color)
{
	if (pgm == nullptr)
		return;
	pgm->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	mat4f_t proj = glm::ortho(0.0f, float(gl.getWidth()), 0.0f, float(gl.getHeight()));

	uint32_t colorLoc = glGetUniformLocation(pgm->getID(), "textColor");
    glUniform3f(colorLoc, color.getRed(), color.getGreen(), color.getBlue());
	uint32_t mvpLoc = glGetUniformLocation(pgm->getID(), "proj");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(proj));

	string::const_iterator ch;
	for (ch = text.begin(); ch != text.end(); ch++) {
		int gidx = FT_Get_Char_Index(face, *ch);

		float xpos = x + glyph[gidx].bx;
		float ypos = y - (glyph[gidx].sy - glyph[gidx].by);
		float w = glyph[gidx].sx;
		float h = glyph[gidx].sy;
		float vtx[6][4] = {
				{ xpos,   ypos+h, 0.0, 0.0 },
				{ xpos,   ypos,   0.0, 1.0 },
				{ xpos+w, ypos,   1.0, 1.0 },

				{ xpos,   ypos+h, 0.0, 0.0 },
				{ xpos+w, ypos,   1.0, 1.0 },
				{ xpos+w, ypos+h, 1.0, 0.0 },
		};

//		cout << fmt::sprintf("Render '%lc' => Glyph index %d (%f,%f) at (%f,%f) Advance (%f,%f)\n",
//			*ch, gidx, w, h, xpos, ypos, glyph[gidx].ax, glyph[gidx].ay);

		glBindTexture(GL_TEXTURE_2D, glyph[gidx].glName);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtx), vtx);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		x += glyph[gidx].ax;
	}

	glDisable(GL_BLEND);
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			slot->bitmap.width, slot->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			slot->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glyph[gidx].glName = name;
		glyph[gidx].ax = slot->advance.x >> 6;
		glyph[gidx].ay = slot->advance.y >> 6;
		glyph[gidx].sx = slot->bitmap.width;
		glyph[gidx].sy = slot->bitmap.rows;
		glyph[gidx].bx = slot->bitmap_left;
		glyph[gidx].by = slot->bitmap_top;
	}

	// Assign UNICODE code to glyph table
	uint32_t gidx;
	char32_t ch = FT_Get_First_Char(face, &gidx);
	while ( gidx != 0) {
//		cout << fmt::sprintf("Glyph index %d: %08X\n", gidx, uint32_t(ch));
		glyph[gidx].ch = ch;
		ch = FT_Get_Next_Char(face, ch, &gidx);
	}

	ShaderManager &smgr = gl.getShaderManager();
	pgm = smgr.createShader("text");

	// Initialize OpenGL buffers
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
