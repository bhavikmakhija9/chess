#ifndef OBSERV_H
#define OBSERV_H
#include <iostream>
#include <vector>
#include "window.h"

class Square;

class Observer
{
public:
  virtual void notify(Square &s) = 0;
  virtual ~Observer() = default;
};

class TextDisplay : public Observer
{
  std::vector<std::vector<char>> theDisplay;

public:
  TextDisplay();

  void notify(Square &s) override;

  ~TextDisplay();

  friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

class GraphicsDisplay: public Observer{
  Xwindow *window;
  public:
    GraphicsDisplay(XWindow *w);
    void notify(Square &s) override;
    ~GraphicsDisplay();
};

#endif
