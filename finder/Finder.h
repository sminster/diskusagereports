//*****************************************************************************
//
//  FILE: Finder.h
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

#include <QDialog>

#include <memory>

namespace Ui { class Finder; }

namespace dur {

class Feedback;
class FinderThread;

class Finder : public QDialog
{
  Q_OBJECT

public:
  Finder(Feedback* feedback, QWidget* parent = NULL);
  virtual ~Finder();

public slots:
  void startDir(const QString& baseDir);

  void setCurrentDir(const QString& dir);
  void progress(unsigned long long count, unsigned long long total);

private slots:
  void findDone();

private:
  std::auto_ptr<Ui::Finder> theUi;
  Feedback* theFeedback;
  FinderThread* theWorker;
};

}

