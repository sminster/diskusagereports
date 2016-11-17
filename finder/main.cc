
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
