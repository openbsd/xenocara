// example4.cpp

// This file holds the source code of a very simple application using the Qt
// framework to render a single glyph into a window.  For demonstration
// purposes, both direct rendering using a callback and rendering using a
// buffer are implemented (yielding the same output).
//
// Due to a bug in the FreeType's direct rendering support you should use
// version 2.4.3 or newer to get correct results.
//
// Written Sept. 2010 by Róbert Márki <gsmiko@gmail.com>,
// with slight modifications by Werner Lemberg
//
// Public domain.
//
//
// To compile this application, check the `example4.pro' proto-makefile
// whether all paths are fine, then say `qmake example4.pro' followed by
// `make'.
//
// It has been tested with Qt version 4.7.0.

#include <QtGui/QApplication>
#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QImage>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_TYPES_H
#include FT_OUTLINE_H
#include FT_RENDER_H


QString g_usageText =
  "usage:\n"
  "example4 FONT_PATH CHARACTER SIZE DIRECT_RENDERING_MODE(1|0)";


#define TRUNC(x) ((x) >> 6)


class Widget : public QWidget
{
  Q_OBJECT

public:
  Widget(const QString& fileName,
         QChar character,
         int pointSize,
         bool directRender,
         QWidget *parent = 0)
  : QWidget(parent), m_directRender(directRender)
  {
    FT_Error error = FT_Err_Ok;
    m_face = 0;
    m_library = 0;

    // For simplicity, the error handling is very rudimentary.
    error = FT_Init_FreeType(&m_library);
    if (!error)
    {
      error = FT_New_Face(m_library,
                          fileName.toAscii().constData(),
                          0,
                          &m_face);
      if (!error)
      {
        error = FT_Set_Char_Size(m_face,
                                 0,
                                 pointSize * 64,
                                 physicalDpiX(),
                                 physicalDpiY());

        if (!error)
        {
          FT_UInt glyph_index = 0;
          glyph_index = FT_Get_Char_Index(m_face,
                                          character.unicode());

          error = FT_Load_Glyph(m_face,
                                glyph_index,
                                FT_LOAD_DEFAULT);

          if (!error)
          {
            FT_Pos left = m_face->glyph->metrics.horiBearingX;
            FT_Pos right = left + m_face->glyph->metrics.width;
            FT_Pos top = m_face->glyph->metrics.horiBearingY;
            FT_Pos bottom = top - m_face->glyph->metrics.height;

            m_glyphRect = QRect(QPoint(TRUNC(left),
                                       -TRUNC(top) + 1),
                                QSize(TRUNC(right - left) + 1,
                                      TRUNC(top - bottom) + 1));
            setFixedSize(m_glyphRect.width(),
                         m_glyphRect.height());
          }
        }
      }
    }
  }

  ~Widget()
  {
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
  }

private:
  FT_Library m_library;
  FT_Face m_face;
  QRect m_glyphRect;
  bool m_directRender;

  // The callback function for direct rendering.
  static void graySpans(int y,
                        int count,
                        const FT_Span_ *spans,
                        void *user)
  {
    QPainter *painter = (QPainter *)user;
    y = -y;

    for (int i = 0; i < count; i++)
    {
      const FT_Span span = spans[i];
      qreal opacity = qreal(span.coverage) / 255.0;

      painter->setOpacity(opacity);

      if (span.len > 1)
        painter->drawLine(span.x, y, span.x + span.len - 1, y);
      else
        painter->drawPoint(span.x, y);
    }
  }

protected:
  void paintEvent(QPaintEvent *event)
  {
    QWidget::paintEvent(event);

    if (m_library && m_face)
    {
      FT_Error error = FT_Err_Ok;
      QPainter painter(this);

      painter.translate(-m_glyphRect.x(),
                        -m_glyphRect.y());

      if (m_directRender)
      {
        // Direct rendering.

        painter.setPen(Qt::black);

        FT_Raster_Params params;

        params.target = 0;
        params.flags = FT_RASTER_FLAG_DIRECT | FT_RASTER_FLAG_AA;
        params.user = &painter;
        params.gray_spans = &Widget::graySpans;
        params.black_spans = 0;
        params.bit_set = 0;
        params.bit_test = 0;

        FT_Outline* outline = &m_face->glyph->outline;

        FT_Outline_Render(m_library,
                          outline,
                          &params);
      }
      else
      {
        // Rendering using a buffer.

        error = FT_Render_Glyph(m_face->glyph,
                                FT_RENDER_MODE_NORMAL);

        QImage glyphImage(m_face->glyph->bitmap.buffer,
                          m_face->glyph->bitmap.width,
                          m_face->glyph->bitmap.rows,
                          m_face->glyph->bitmap.pitch,
                          QImage::Format_Indexed8);

        painter.translate(m_glyphRect.x(),
                          m_glyphRect.y());

        QVector<QRgb> colorTable;
        for (int i = 0; i < 256; ++i)
          colorTable << qRgba(0, 0, 0, i);
        glyphImage.setColorTable(colorTable);

        painter.drawImage(QPoint(0, 0),
                          glyphImage);
      }
    }
  }
};


int main(int argc,
         char **argv)
{
  bool status = false;

  if (argc == 5)
  {
    bool isSizeOk = false;
    QString path = argv[1];
    QChar character = *argv[2];
    int size = QString(argv[3]).toInt(&isSizeOk);
    bool directRender = QString(argv[4]).toInt();

    if (QFile::exists(path) && isSizeOk)
    {
      status = true;
      QApplication a(argc, argv);
      Widget w(path, character, size, directRender);
      w.show();
      return a.exec();
    }
  }

  if (!status)
  {
    std::cout << qPrintable(g_usageText) << std::endl;
    return 0;
  }
}

#include "example4.moc"

// Local Variables: 
// coding: utf-8
// End: 
