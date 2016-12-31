/*
    Digital Clock: skin draw engine
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "raster_skin.h"

namespace skin_draw {

ISkin::QPixmapPtr RasterSkin::ResizeImage(const QString& str, int idx, qreal zoom)
{
  const QPixmap& original = orig_images_[str[idx]];
  if (original.isNull()) return QPixmapPtr();

  QPixmapPtr result(new QPixmap());
  *result = original.scaled(original.size() * zoom * GetDevicePixelRatio(),
                            Qt::KeepAspectRatio, Qt::SmoothTransformation);
  return result;
}

} // namespace skin_draw
