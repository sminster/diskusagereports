#include <QDialog>

#include <memory>

namespace Ui { class Finder; }

namespace dur {

class Feedback;
class FinderThread;

class Finder : public QDialog
{
  Q_OBJECT

public:
  Finder(Feedback* feedback, QWidget* parent = NULL);
  virtual ~Finder();

public slots:
  void startDir(const QString& baseDir);

  void setCurrentDir(const QString& dir);
  void progress(unsigned long long count, unsigned long long total);

private slots:
  void findDone();

private:
  std::auto_ptr<Ui::Finder> theUi;
  Feedback* theFeedback;
  FinderThread* theWorker;
};

}

