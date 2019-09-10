// example5.cpp

// This program is a simple example that prints a character's outline in the
// SVG format to stdout, demonstrating the usage of FT_Outline_Decompose().
//
// Developed by Static Jobs LLC and contributed to the FreeType project.
//
// Copyright (c) 2016-2018 Static Jobs LLC
//   IT and software engineering jobs in the US, Canada and the UK
//   https://www.staticjobs.com
//
// License: MIT (see below)
//
// The source code was reformatted by Werner Lemberg, also including a few
// minor code changes and comments for didactic purposes.
//
// On a Unix box like GNU/Linux or OS X, compile with
//
//    g++ -o example5 example5.cpp `pkg-config freetype2 --cflags --libs`
//
// or
//
//    g++ -o example5 example5.cpp `freetype-config --cflags --libs`
//
// on the command line.
//
// On other platforms that don't have the `freetype-config' shell script or
// the `pkg-config' tool, you have to pass the necessary compiler flags
// manually.


// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
// OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cctype>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_BBOX_H


using namespace std;


// A minimal wrapper for RAII (`resource acquisition is initialization').
class FreeTypeLibrary
{
public:
  FreeTypeLibrary();
  ~FreeTypeLibrary();

  operator FT_Library() const;

private:
  FreeTypeLibrary(const FreeTypeLibrary &);
  FreeTypeLibrary &operator =(const FreeTypeLibrary &);

private:
  FT_Library m_ftLibrary;
};


inline
FreeTypeLibrary::FreeTypeLibrary()
{
  FT_Error error = FT_Init_FreeType(&m_ftLibrary);

  if (error)
    throw runtime_error("Couldn't initialize the library:"
                        " FT_Init_FreeType() failed");
}


inline
FreeTypeLibrary::~FreeTypeLibrary()
{
  FT_Done_FreeType(m_ftLibrary);
}


inline
FreeTypeLibrary::operator FT_Library() const
{
  return m_ftLibrary;
}


// Another minimal wrapper for RAII.
class FreeTypeFace
{
public:
  FreeTypeFace(const FreeTypeLibrary &library,
               const char *filename);
  ~FreeTypeFace();

  operator FT_Face() const;

private:
  FreeTypeFace(const FreeTypeFace &);
  FreeTypeFace &operator =(const FreeTypeFace &);

private:
  FT_Face m_ftFace;
};


inline
FreeTypeFace::FreeTypeFace(const FreeTypeLibrary &library,
                           const char *filename)
{
  // For simplicity, always use the first face index.
  FT_Error error = FT_New_Face(library, filename, 0, &m_ftFace);

  if (error)
    throw runtime_error("Couldn't load the font file:"
                        " FT_New_Face() failed");
}


inline
FreeTypeFace::~FreeTypeFace()
{
  FT_Done_Face(m_ftFace);
}


inline
FreeTypeFace::operator FT_Face() const
{
  return m_ftFace;
}


class OutlinePrinter
{
public:
  OutlinePrinter(const char *filename);
  int Run(const char *symbol);

private:
  OutlinePrinter(const OutlinePrinter &);
  OutlinePrinter &operator =(const OutlinePrinter &);

private:
  void LoadGlyph(const char *symbol) const;
  bool OutlineExists() const;
  void FlipOutline() const;
  void ExtractOutline();
  void ComputeViewBox();
  void PrintSVG() const;

  static int MoveToFunction(const FT_Vector *to,
                            void *user);
  static int LineToFunction(const FT_Vector *to,
                            void *user);
  static int ConicToFunction(const FT_Vector *control,
                             const FT_Vector *to,
                             void *user);
  static int CubicToFunction(const FT_Vector *controlOne,
                             const FT_Vector *controlTwo,
                             const FT_Vector *to,
                             void *user);

private:
  // These two lines initialize the library and the face;
  // the order is important!
  FreeTypeLibrary m_library;
  FreeTypeFace m_face;

  ostringstream m_path;

  // These four variables are for the `viewBox' attribute.
  FT_Pos m_xMin;
  FT_Pos m_yMin;
  FT_Pos m_width;
  FT_Pos m_height;
};


inline
OutlinePrinter::OutlinePrinter(const char *filename)
: m_face(m_library, filename),
  m_xMin(0),
  m_yMin(0),
  m_width(0),
  m_height(0)
{
  // Empty body.
}


int
OutlinePrinter::Run(const char *symbol)
{
  LoadGlyph(symbol);

  // Check whether outline exists.
  bool outlineExists = OutlineExists();

  if (!outlineExists) // Outline doesn't exist.
    throw runtime_error("Outline check failed.\n"
                        "Please, inspect your font file or try another one,"
                        " for example LiberationSerif-Bold.ttf");

  FlipOutline();

  ExtractOutline();

  ComputeViewBox();

  PrintSVG();

  return 0;
}


void
OutlinePrinter::LoadGlyph(const char *symbol) const
{
  FT_ULong code = symbol[0];

  // For simplicity, use the charmap FreeType provides by default;
  // in most cases this means Unicode.
  FT_UInt index = FT_Get_Char_Index(m_face, code);

  FT_Error error = FT_Load_Glyph(m_face,
                                 index,
                                 FT_LOAD_NO_SCALE | FT_LOAD_NO_BITMAP);

  if (error)
    throw runtime_error("Couldn't load the glyph: FT_Load_Glyph() failed");
}


// While working on this example, we found fonts with no outlines for
// printable characters such as `A', i.e., `outline.n_contours' and
// `outline.n_points' were zero.  FT_Outline_Check() returned `true'.
// FT_Outline_Decompose() also returned `true' without walking the outline.
// That is, we had no way of knowing whether the outline existed and could
// be (or was) decomposed.  Therefore, we implemented this workaround to
// check whether the outline does exist and can be decomposed.
bool
OutlinePrinter::OutlineExists() const
{
  FT_Face face = m_face;
  FT_GlyphSlot slot = face->glyph;
  FT_Outline &outline = slot->outline;

  if (slot->format != FT_GLYPH_FORMAT_OUTLINE)
    return false; // Should never happen.  Just an extra check.

  if (outline.n_contours <= 0 || outline.n_points <= 0)
    return false; // Can happen for some font files.

  FT_Error error = FT_Outline_Check(&outline);

  return error == 0;
}


// This function flips outline around x-axis. We need it because
// FreeType and SVG use opposite vertical directions.
void
OutlinePrinter::FlipOutline() const
{
  const FT_Fixed multiplier = 65536L;

  FT_Matrix matrix;

  matrix.xx = 1L * multiplier;
  matrix.xy = 0L * multiplier;
  matrix.yx = 0L * multiplier;
  matrix.yy = -1L * multiplier;

  FT_Face face = m_face;
  FT_GlyphSlot slot = face->glyph;
  FT_Outline &outline = slot->outline;

  FT_Outline_Transform(&outline, &matrix);
}


void
OutlinePrinter::ExtractOutline()
{
  m_path << "  <path d='\n";

  FT_Outline_Funcs callbacks;

  callbacks.move_to = MoveToFunction;
  callbacks.line_to = LineToFunction;
  callbacks.conic_to = ConicToFunction;
  callbacks.cubic_to = CubicToFunction;

  callbacks.shift = 0;
  callbacks.delta = 0;

  FT_Face face = m_face;
  FT_GlyphSlot slot = face->glyph;
  FT_Outline &outline = slot->outline;

  FT_Error error = FT_Outline_Decompose(&outline, &callbacks, this);

  if (error)
    throw runtime_error("Couldn't extract the outline:"
                        " FT_Outline_Decompose() failed");

  m_path << "          '\n"
            "        fill='red'/>\n";
}


void
OutlinePrinter::ComputeViewBox()
{
  FT_Face face = m_face;
  FT_GlyphSlot slot = face->glyph;
  FT_Outline &outline = slot->outline;

  FT_BBox boundingBox;

  FT_Outline_Get_BBox(&outline, &boundingBox);

  FT_Pos xMin = boundingBox.xMin;
  FT_Pos yMin = boundingBox.yMin;
  FT_Pos xMax = boundingBox.xMax;
  FT_Pos yMax = boundingBox.yMax;

  m_xMin = xMin;
  m_yMin = yMin;
  m_width = xMax - xMin;
  m_height = yMax - yMin;
}


void
OutlinePrinter::PrintSVG() const
{
  cout << "<svg xmlns='http://www.w3.org/2000/svg'\n"
          "     xmlns:xlink='http://www.w3.org/1999/xlink'\n"
          "     viewBox='"
       << m_xMin << ' ' << m_yMin << ' ' << m_width << ' ' << m_height
       << "'>\n"
       << m_path.str()
       << "</svg>"
       << endl;
}


int
OutlinePrinter::MoveToFunction(const FT_Vector *to,
                               void *user)
{
  OutlinePrinter *self = static_cast<OutlinePrinter *>(user);

  FT_Pos x = to->x;
  FT_Pos y = to->y;

  self->m_path << "           "
                  "M " << x << ' ' << y << '\n';

  return 0;
}


int
OutlinePrinter::LineToFunction(const FT_Vector *to,
                               void *user)
{
  OutlinePrinter *self = static_cast<OutlinePrinter *>(user);

  FT_Pos x = to->x;
  FT_Pos y = to->y;

  self->m_path << "           "
                  "L " << x << ' ' << y << '\n';

  return 0;
}


int
OutlinePrinter::ConicToFunction(const FT_Vector *control,
                                const FT_Vector *to,
                                void *user)
{
  OutlinePrinter *self = static_cast<OutlinePrinter *>(user);

  FT_Pos controlX = control->x;
  FT_Pos controlY = control->y;

  FT_Pos x = to->x;
  FT_Pos y = to->y;

  self->m_path << "           "
                  "Q " << controlX << ' ' << controlY << ", "
                       << x << ' ' << y << '\n';

  return 0;
}


int
OutlinePrinter::CubicToFunction(const FT_Vector *controlOne,
                                const FT_Vector *controlTwo,
                                const FT_Vector *to,
                                void *user)
{
  OutlinePrinter *self = static_cast<OutlinePrinter *>(user);

  FT_Pos controlOneX = controlOne->x;
  FT_Pos controlOneY = controlOne->y;

  FT_Pos controlTwoX = controlTwo->x;
  FT_Pos controlTwoY = controlTwo->y;

  FT_Pos x = to->x;
  FT_Pos y = to->y;

  self->m_path << "           "
                  "C " << controlOneX << ' ' << controlOneY << ", "
                       << controlTwoX << ' ' << controlTwoY << ", "
                       << x << ' ' << y << '\n';

  return 0;
}


int
main(int argc,
     char **argv)
{
  if (argc != 3)
  {
    const char *program = argv[0];

    cerr << "This program prints a single character's outline"
            " in the SVG format to stdout.\n"
            "Usage: " << program << " font symbol\n"
            "Example: " << program << " LiberationSerif-Bold.ttf A" << endl;

    return 1;
  }

  const char *symbol = argv[2];

  // For simplicity, only accept single-byte characters like `A'.
  if (strlen(symbol) != 1 || isspace(*symbol))
  {
    cerr << "Error: '" << symbol
         << "' is not a single printable character" << endl;

    return 2;
  }

  int status;

  try
  {
    const char *filename = argv[1];

    OutlinePrinter printer(filename);

    status = printer.Run(symbol);
  }
  catch (const exception &e)
  {
    cerr << "Error: " << e.what() << endl;

    status = 3;
  }

  return status;
}

// EOF
