//*****************************************************************************
//
//  FILE: main.cc
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

#include "Directory.h"
#include "Feedback.h"
#include "Finder.h"
#include "FinderThread.h"

#include <QtGui/QApplication>

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  dur::Feedback fb;
  dur::Finder window(&fb);
  window.show();

  if (argc > 1) window.startDir(argv[1]);

#if 0
  // TODO
  QList<QThread*> workers;
  for(int a = 1; a < argc; ++a)
  {
    workers << new dur::FinderThread(argv[a], &fb);
    workers.back()->start();

#if 0
    dur::Directory dir(argv[a], &fb);
    cout << "Total bytes: " << dir.totalBytes() << endl;
    cout << "Total block usage: " << dir.totalBlocks() * 512 << endl;
#endif
  }
#endif

  return app.exec();
}
