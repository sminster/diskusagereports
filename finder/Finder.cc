//*****************************************************************************
//
//  FILE: Finder.cc
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

#include "Finder.h"
#include "ui_Finder_gui.h"
#include "Feedback.h"
#include "FinderThread.h"
#include "Directory.h"

#include "/home/scott/dev/debug.h"

namespace dur {

Finder::Finder(Feedback* feedback, QWidget* parent)
 :
  QDialog(parent),
  theUi(new Ui::Finder()),
  theFeedback(feedback),
  theWorker(NULL)
{
  theUi->setupUi(this);

  theUi->theButtons->setEnabled(false);

  connect(feedback, SIGNAL(dirChanged(QString)),
	  this, SLOT(setCurrentDir(QString)));

  connect(feedback, SIGNAL(update(unsigned long long, unsigned long long)),
	  this, SLOT(progress(unsigned long long, unsigned long long)));
}

Finder::~Finder()
{
}

void Finder::setCurrentDir(const QString& dir)
{
  theUi->theCurDir->setText(dir);
}

void Finder::progress(unsigned long long count,
		      unsigned long long total)
{
  theUi->theProgress->setMaximum(total);
  theUi->theProgress->setValue(count);

  if (theWorker && theWorker->dir())
  {
    static const QString FILES("Files: %1");
    static const QString SIZES("Total Size: %1 B");
    theUi->theFilesLabel->setText(FILES.arg(theWorker->dir()->totalNumFiles()));
    theUi->theSizeLabel->setText(SIZES.arg(theWorker->dir()->totalBytes()));
  }
}

void Finder::startDir(const QString& baseDir)
{
  if (theWorker) return;

  theUi->theBaseDir->setText(baseDir);

  theWorker = new FinderThread(baseDir, theFeedback, this);
  connect(theWorker, SIGNAL(finished()), 
	  this,      SLOT(findDone()));

  theWorker->start();
}

void Finder::findDone()
{
  theUi->theButtons->setEnabled(true);

  // TODO
}

} // namespace dur
