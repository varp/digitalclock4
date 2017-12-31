/*
    Digital Clock: skin draw engine
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vector_skin.h"

#include <QFile>
#include <QSvgRenderer>
#include <QPainter>

namespace skin_draw {

QPixmap VectorSkin::ResizeImage(const QString& str, int idx, Zoom zoom)
{
  const QString& img_file = image_files_[str[idx]];
  if (!QFile::exists(img_file)) return QPixmap();

  QSvgRenderer renderer(img_file);
  QPixmap result(renderer.defaultSize() * zoom * GetDevicePixelRatio());
  QPainter painter(&result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(result.rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  renderer.render(&painter);
  painter.end();
  return result;
}

} // namespace skin_draw
