
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
