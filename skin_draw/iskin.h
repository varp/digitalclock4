#ifndef ISKIN_H
#define ISKIN_H

#include <QPixmap>
#include <QSharedPointer>
#include "skin_draw_global.h"

namespace skin_draw {

/*!
 * @brief Skin interface.
 *
 * This class provides interface for all supported skin types:
 * @li raster image skin
 * @li vector image skin
 * @li text skin
 *
 * This interface is used by skin draw engine.
 * @see RasterSkin, VectorSkin, TextSkin, SkinDrawer
 */
class SKIN_DRAWSHARED_EXPORT ISkin {
public:
  /*! Virtual destructor. */
  virtual ~ISkin() {}

  /// skin pointer type
  typedef QSharedPointer<ISkin> SkinPtr;
  /// QPixmap shared pointer type
  typedef QSharedPointer<QPixmap> QPixmapPtr;

  /*!
   * Get image for specified symbol.
   * @param ch - symbol for which need to get image
   * @param zoom - skin zoom
   * @param cache - is image add to cache
   * @return pointer to result image
   */
  virtual QPixmapPtr GetImage(QChar ch, qreal zoom, bool cache) = 0;
  /*!
   * @brief Set device pixel ratio value.
   *
   * This value 1.0 for normal displays and 2.0 or higher for Retina displays.
   * See Qt documentation for details. Default value 1.0.
   * @param new_ratio - new device pixel ratio
   */
  virtual void SetDevicePixelRatio(qreal new_ratio) = 0;
};

} // namespace skin_draw

#endif // ISKIN_H
