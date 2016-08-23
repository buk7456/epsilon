#ifndef ESCHER_APP_H
#define ESCHER_APP_H

#include <escher/responder.h>
#include <escher/view_controller.h>

/* An app is fed events and outputs drawing calls.
 *
 * To achieve this, it uses a View hierarchy, and modifies it according to the
 * events received. The View hierarchy keeps a memory of the area that needs to
 * be redrawn depending on how it's been modified.
 *
 * Multiple App can exist at once.
 * */

class App : public Responder {
public:
  App();
  void setWindow(Window * window);
  void focus(Responder * responder);
  void processEvent(Ion::Events::Event event);
protected:
  virtual ViewController * rootViewController() = 0;
private:
  Responder * m_focusedResponder;
};

#endif
