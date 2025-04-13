#ifndef FLASHFS_H
#define FLASHFS_H

#include <FS.h>
#include <LittleFS.h>

class FlashFS {
public:
  FlashFS();
  String begin(); 
  String listDirectory(const String &dirname);
  String formatTime(time_t time);
  String readFile(const String &path);
  String writeFile(const String &path, const String &message);
  String renameFile(const String &oldPath, const String &newPath);
  String appendToFile(const String &path, const String &message);
  String removeFile(const String &path);
  void replaceEmptyFiles( String path,const String pathTotalData);
  void format();
  void end();

private:
  String formatTimestamp(time_t timestamp);
};

#endif