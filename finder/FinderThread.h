
#include <QtCore/QThread>

namespace dur {

class Feedback;
class Directory;

class FinderThread : public QThread
{
  Q_OBJECT

public:
  FinderThread(const QString& baseDir, 
	       Feedback* feedback, 
	       QObject* parent = NULL);
  virtual ~FinderThread();

  const QString& baseDir() const { return theBaseDir; }
  Directory* dir() const { return theDirInfo; }

protected:
  virtual void run();

private:
  QString    theBaseDir;
  Feedback*  theFeedback;
  Directory* theDirInfo;
};

}
