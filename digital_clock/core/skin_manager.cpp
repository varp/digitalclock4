#include "iclock_skin.h"
#include "skin_manager.h"

SkinManager::SkinManager(QObject* parent)
  : QObject(parent) {
}

void SkinManager::AddSkinDir(const QDir& dir) {
  skin_dirs_.append(dir);
}

void SkinManager::DelSkinDir(const QDir& dir) {
  skin_dirs_.removeOne(dir);
}

void SkinManager::ListSkins() {
  skins_.clear();
  for (auto& s_dir : skin_dirs_) {
    QStringList f_dirs = s_dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs);
    for (auto& f_dir : f_dirs) {
      QDir skin_root(s_dir.filePath(f_dir));
      IClockSkin* tmp = CreateSkin(skin_root);
      if (!tmp) continue;
      TSkinInfo info;
      tmp->GetInfo(&info);
      skins_[info[SI_NAME]] = skin_root;
      delete tmp;
    }
  }
  emit SearchFinished(skins_.keys());
}

void SkinManager::FindSkin(const QString& skin_name) {
  emit SkinFound(skins_[skin_name]);
}
