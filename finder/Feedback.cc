//*****************************************************************************
//
//  FILE: Feedback.cc
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

#include "Feedback.h"

#include "/home/scott/dev/debug.h"

namespace dur {

Feedback::Feedback(QObject* parent)
 :
  QObject(parent),
  theTotalChildren(0),
  theStats(0)
{
}

Feedback::~Feedback()
{
}

void Feedback::addChildCount(const QString& dir,
			     unsigned int count)
{
  theTotalChildren += count;

  emit dirChanged(dir);
  emit update(theStats, theTotalChildren);
}

void Feedback::finishedStat()
{
  theStats++;
  emit update(theStats, theTotalChildren);
}

} // namespace dur
