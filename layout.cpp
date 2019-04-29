#include <Arduino.h>
#include "layout.h"
#include "button.h"
#include "context.h"

Layout::Layout() {  
}

Button** Layout::all() {
  return _all;
}

void Layout::refresh(Context* context) {
  for (byte i=0; i < 32; i++) {
    _all[i]->refresh(i, context);
  }
}

Layout* Layout::next() {
  return _next;
}

void Layout::setNext(Layout *layout) {
  _next = layout;
}
