#include "FlashFS.h"

// Konstruktor
FlashFS::FlashFS() {}

// Memulai LittleFS
String FlashFS::begin()
{
  if (!LittleFS.begin())
  {
    return "LittleFS mount failed";
  }
  return "LittleFS mounted";
}

// Menampilkan daftar file dalam direktori tertentu
String FlashFS::listDirectory(const String &dirname)
{
  String data = "Listing directory: " + dirname + "\n";
  Dir root = LittleFS.openDir(dirname);

  while (root.next())
  {
    File file = root.openFile("r");
    data += "  FILE: " + root.fileName() + "\n";
    data += "  SIZE: " + String(file.size()) + " bytes\n";

    time_t created = file.getCreationTime();
    time_t lastModified = file.getLastWrite();
    file.close();

    data += "  CREATED: " + formatTimestamp(created) + "\n";
    data += "  LAST MODIFIED: " + formatTimestamp(lastModified) + "\n";
  }

  return data;
}

// Memformat timestamp ke string
String FlashFS::formatTimestamp(time_t timestamp)
{
  struct tm *timeStruct = localtime(&timestamp);
  return String(timeStruct->tm_year + 1900) + "-" +
         String(timeStruct->tm_mon + 1) + "-" +
         String(timeStruct->tm_mday) + " " +
         String(timeStruct->tm_hour) + ":" +
         String(timeStruct->tm_min) + ":" +
         String(timeStruct->tm_sec);
}

// Membaca isi file
String FlashFS::readFile(const String &path)
{
  String data = "";
  File file = LittleFS.open(path.c_str(), "r");
  if (!file)
  {
    return "!";
  }
  while (file.available())
  {
    data += char(file.read());
  }
  file.close();
  return data;
}

// Menulis ke file (overwrite)
String FlashFS::writeFile(const String &path, const String &message)
{
  Serial.println("Menulis file: " + path);
  File file = LittleFS.open(path.c_str(), "w");

  if (!file)
  {
    return "Gagal membuka file";
  }
  if (file.print(message.c_str()))
  {
    file.close();
    return "Sukses menulis ke file";
  }
  file.close();
  return "Gagal menulis ke file";
}

// Mengganti nama file
String FlashFS::renameFile(const String &oldPath, const String &newPath)
{
  if (LittleFS.rename(oldPath.c_str(), newPath.c_str()))
  {
    return "Sukses mengganti nama file";
  }
  return "Gagal mengganti nama file";
}

// Menambahkan teks ke file (append)
String FlashFS::appendToFile(const String &path, const String &message)
{
  File file = LittleFS.open(path.c_str(), "a");
  if (!file)
  {
    return "Gagal membuka file";
  }
  if (file.print(message.c_str()))
  {
    file.close();
    return "Sukses menambahkan ke file";
  }
  file.close();
  return "Gagal menambahkan ke file";
}

// Menghapus file
String FlashFS::removeFile(const String &path)
{
  if (LittleFS.remove(path))
  {
    return "Sukses menghapus file";
  }
  return "Gagal menghapus file";
}
void FlashFS::replaceEmptyFiles( String path,const String pathTotalData)
{
  String initResult = begin();
  if (initResult != "LittleFS mounted")
  {
    Serial.println("LittleFS Mount Failed");
    return;
  }
  initResult = readFile(pathTotalData);
  int maxFiles = initResult.length() > 2 ? 0 : initResult.toInt() + 1;
  maxFiles = maxFiles > 20 ? 20 : maxFiles;

  for (int i = 1; i < maxFiles; i++)
  {
    String currentFile = path + String(i) + ".txt";
    String currentData = readFile(currentFile);

    if (currentData == "!")
    { // File kosong
      for (int j = i + 1; j <= maxFiles; j++)
      {
        String nextFile = path + String(j) + ".txt";
        String nextData = readFile(nextFile);

        if (nextData != "")
        {
          writeFile(currentFile, nextData);
          removeFile(nextFile);
          Serial.println("File " + nextFile + " dipindahkan ke " + currentFile);
          break;
        }
      }
    }
  }
  end();
}
// Memformat sistem file
void FlashFS::format()
{
  LittleFS.format();
}

// Menonaktifkan LittleFS
void FlashFS::end()
{
  LittleFS.end();
}