
#include <QObject>

namespace dur {

class Feedback : public QObject
{
  Q_OBJECT

public:
  Feedback(QObject* parent = NULL);
  virtual ~Feedback();

  void addChildCount(const QString& dir, unsigned int count);
  void finishedStat();

signals:
  void dirChanged(const QString& dir);
  void update(unsigned long long count,
	      unsigned long long total);

private:
  unsigned long long theTotalChildren;
  unsigned long long theStats;
};

}
