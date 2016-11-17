//*****************************************************************************
//
//  FILE: Directory.h
//
//  Copyright 2016 Scott Minster
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//  History:
//   16NOV16  S. Minster  Initial Creation
//
//*****************************************************************************

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QPair>

#include <string>
#include <vector>

namespace dur {

class Feedback;

class Directory
{
public:
  enum ModGroup
  {
    YEARS_TEN_PLUS = 0,
    YEARS_FIVE_TEN,
    YEARS_TWO_FIVE,
    YEARS_ONE_TWO,
    DAYS_270_365,
    DAYS_180_270,
    DAYS_90_180,
    DAYS_60_90,
    DAYS_30_60,
    DAYS_15_30,
    DAYS_7_15,
    DAYS_1_7,
    TODAY,
    FUTURE,

    NUM_TIMES
  };

  enum SizeGroup
  {

    GB_1_PLUS = 0,
    MB_500_1024,
    MB_250_500,
    MB_125_250,
    MB_75_125,
    MB_25_75,
    MB_10_25,
    MB_5_10,
    MB_1_5,
    KB_500_1024,
    KB_250_500,
    KB_100_250,
    KB_50_100,
    KB_25_50,
    KB_10_25,
    KB_5_10,
    KB_1_5,
    KB_LESS_1,

    NUM_SIZES
  };

  Directory(const QString& fullpath, Feedback* feedback = NULL);
  ~Directory();

  void analyze();

  QString hash() const;

  QString path() const { return theDir.path(); }
  QString name() const { return theDir.dirName(); }

  size_t childSize() const { return theChildren.size(); }
  size_t subdirSize() const { return theSubdirs.size(); }

  unsigned long long bytes() const { return theBytes; }
  unsigned long long blocks() const { return theBlocks; }
  unsigned long      numFiles() const { return theNumFiles; }
  unsigned long long totalBytes() const { return theTotalBytes; }
  unsigned long long totalBlocks() const { return theTotalBlocks; }
  unsigned long      totalNumFiles() const { return theTotalNumFiles; }


  std::ostream& writeJson(std::ostream& os) const;
  //  void writeJsonFile();

private:
  QDir theDir;
//  QFileInfoList theChildren;
  QStringList theChildren;
  std::vector<QSharedPointer<Directory> > theSubdirs;

  unsigned long long theBytes;
  unsigned long long theBlocks;
  unsigned int       theNumFiles;

  unsigned long long theTotalBytes;
  unsigned long long theTotalBlocks;
  unsigned int       theTotalNumFiles;

  typedef QPair<unsigned long long, unsigned int> SizeCount;

  QMap<SizeGroup, SizeCount> theSizeGroups;
  QMap<ModGroup,  SizeCount> theModifyGroups;
  QMap<QString,   SizeCount> theTypes;
  QMap<QString,   SizeCount> theOwners;
  QMap<QString,   SizeCount> theGroups;

  // TODO -- top100?
  // TODO -- parent(s)?

  Feedback* theFeedback;
};

}
