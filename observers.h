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
#endif


class GraphicsDisplay : public Observer {
    Xwindow& xw;
    int gs = 800;
    int ss = 100;

    public:
    GraphicsDisplay(Xwindow& xw);
    ~GraphicsDisplay();
};


  