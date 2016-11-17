
#include "Directory.h"
#include "Feedback.h"

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "/home/scott/dev/debug.h"

namespace dur {

Directory::Directory(const QString& fullPath,
		     Feedback* feedback)
  :
     theDir(fullPath),
     theChildren(theDir.entryList()),
     theSubdirs(),
     theBytes(0),
     theBlocks(0),
     theNumFiles(0),
     theTotalBytes(0),
     theTotalBlocks(0),
     theTotalNumFiles(0),
     theSizeGroups(),
     theModifyGroups(),
     theTypes(),
     theOwners(),
     theGroups(),
     theFeedback(feedback)
{
}

Directory::~Directory()
{
}

QString Directory::hash() const
{
  // TODO
  return QString();
}

std::ostream& Directory::writeJson(std::ostream& os) const
{
  // TODO
  return os;
}

void Directory::analyze()
{
  std::cout << path().toStdString() << std::endl;

  // update the feedback object
  // subtract 2 to avoid "." and ".."
  if (theFeedback) theFeedback->addChildCount(path(), theChildren.size() - 2);

  foreach(const QString& info, theChildren)
  {
    if (info == "." || info == "..") continue;

    // also skip this file
    if (info == ".snapshot")
    {
      // fake stat
      if (theFeedback) theFeedback->finishedStat();
      continue;
    }

    QString child = theDir.filePath(info);

    struct stat s;
    memset(&s, 0, sizeof(s));
    if (lstat(child.toStdString().c_str(), &s) != 0)
    {
      static const QString ERR("Could not stat %1");
      perror(ERR.arg(child).toStdString().c_str());
      continue;
    }

    usleep(10000);

    if (theFeedback) theFeedback->finishedStat();

    theBytes += s.st_size;
    theTotalBytes += s.st_size;
    theBlocks += s.st_blocks;
    theTotalBlocks += s.st_blocks;

    if (S_ISDIR(s.st_mode))
    {
      theSubdirs.push_back(QSharedPointer<Directory>(new Directory(child, theFeedback)));
      theSubdirs.back()->analyze();
      theTotalNumFiles += theSubdirs.back()->totalNumFiles();
      theTotalBytes    += theSubdirs.back()->totalBytes();
      theTotalBlocks   += theSubdirs.back()->totalBlocks();

      // TODO -- other stats
    }
    else
    {
      theNumFiles++; theTotalNumFiles++;

      // TODO -- other stats
    }
  }
#if 0
  DVAR(theBytes);
  DVAR(theTotalBytes);
  DVAR(theBlocks * 512);
  DVAR(theTotalBlocks * 512);
  DVAR(theNumFiles);
  DVAR(theTotalNumFiles);
#endif
}

} // namespace dur
