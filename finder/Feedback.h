//*****************************************************************************
//
//  FILE: Feedback.h
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

#include <QObject>

namespace dur {

class Feedback : public QObject
{
  Q_OBJECT

public:
  Feedback(QObject* parent = NULL);
  virtual ~Feedback();

  void addChildCount(const QString& dir, unsigned int count);
  void finishedStat();

signals:
  void dirChanged(const QString& dir);
  void update(unsigned long long count,
	      unsigned long long total);

private:
  unsigned long long theTotalChildren;
  unsigned long long theStats;
};

}
