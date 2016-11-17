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
