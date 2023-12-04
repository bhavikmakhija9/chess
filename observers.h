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
    Xwindow& w;
    int gs = 400;
    int ss = 50;

    public:
    GraphicsDisplay(Xwindow& xw);
    void notify(Square &s) override;
    ~GraphicsDisplay();
};


  