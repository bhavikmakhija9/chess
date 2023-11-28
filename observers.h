#include <iostream>
#include <vector>

class Square;

class Observer {
 public:
  virtual void notify(Square &s) = 0;
  virtual ~Observer() = default;
};


class TextDisplay: public Observer {
  std::vector<std::vector<char>> theDisplay;

 public:
  TextDisplay();

  void notify(Square &s) override;
  
  ~TextDisplay();

  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};
