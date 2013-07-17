#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <QMap>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QPluginLoader>
#include "iclock_plugin.h"

class ClockSettings;
class DigitalClock;
class MainWindow;
class TrayControl;

/*! Data to init plugins. */
struct TPluginData {
  ClockSettings* settings;   /*!< clock settings object */
  DigitalClock* clock;       /*!< clock widget */
  MainWindow* window;        /*!< main window instance */
  TrayControl* tray;         /*!< tray control object */
};


/*!
 * @brief Plugin manager class.
 *
 * This class provides plugin management system. It loads/unloads plugins.
 * It distinguishes plugin types and int them with appropriate data.
 */
class PluginManager : public QObject {
  Q_OBJECT
public:
  /*!
   * Constructor.
   * @param parent - parent object
   */
  explicit PluginManager(QObject* parent = 0);
  /*!
   * Set data for plugin initialization.
   * @param data - data to set. @see TPluginData
   */
  void SetInitData(const TPluginData& data);

signals:
  /*!
   * Signal to notify that plugins search was finished.
   * @param plugins - list of found plugins
   */
  void SearchFinished(const QStringList& plugins);
  /*!
   * Signal to notify that info about plugin was got.
   * @param info - plugin info. @see TPluginInfo
   */
  void InfoGot(const TPluginInfo& info);

public slots:
  /*!
   * Add plugins search directory.
   * @param dir - search directory
   */
  void AddPluginsDir(const QDir& dir);
  /*!
   * Remove plugins search directory.
   * @param dir - directory to remove
   */
  void DelPluginsDir(const QDir& dir);
  /*!
   * List all available plugins. Signal @SearchFinished will emited with list of all plugins.
   */
  void ListAvailable();
  /*!
   * Load given plugins.
   * @param names - list of plugin names to load
   */
  void LoadPlugins(const QStringList& names);
  /*!
   * Enable/disable given plugin.
   * @param name - plugin name
   * @param enable - true to enable, false to disable plugin
   */
  void EnablePlugin(const QString& name, bool enable);
  /*!
   * Get plugin info. Info will be reported by @see InfoGot signal.
   * @param name - plugins name to get info
   */
  void GetPluginInfo(const QString& name);

private:
  /*!
   * Load given plugin.
   * @param name - plugin name to load
   */
  void LoadPlugin(const QString& name);
  /*!
   * Unload given plugin.
   * @param name - plugin name to unload
   */
  void UnloadPlugin(const QString& name);

  QList<QDir> plugins_dirs_;
  QMap<QString, QString> available_;
  QMap<QString, QPluginLoader*> loaded_;
  TPluginData data_;
};

#endif // PLUGIN_MANAGER_H
