
#include "FinderThread.h"
#include "Directory.h"

#include "/home/scott/dev/debug.h"

namespace dur {

FinderThread::FinderThread(const QString& baseDir, 
			   Feedback* feedback,
			   QObject* parent)
 :
  QThread(parent),
  theBaseDir(baseDir),
  theFeedback(feedback),
  theDirInfo(NULL)
{
}

FinderThread::~FinderThread()
{
}

void FinderThread::run()
{
  theDirInfo = new Directory(theBaseDir, theFeedback);
  HEXVAR(theDirInfo);
  theDirInfo->analyze();
}

}
