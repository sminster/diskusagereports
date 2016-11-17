//*****************************************************************************
//
//  FILE: FilterThread.h
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

#include <QtCore/QThread>

namespace dur {

class Feedback;
class Directory;

class FinderThread : public QThread
{
  Q_OBJECT

public:
  FinderThread(const QString& baseDir, 
	       Feedback* feedback, 
	       QObject* parent = NULL);
  virtual ~FinderThread();

  const QString& baseDir() const { return theBaseDir; }
  Directory* dir() const { return theDirInfo; }

protected:
  virtual void run();

private:
  QString    theBaseDir;
  Feedback*  theFeedback;
  Directory* theDirInfo;
};

}
