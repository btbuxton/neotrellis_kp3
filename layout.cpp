#include <Arduino.h>
#include "layout.h"
#include "context.h"
#include "button.h"

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

void Layout::update(Context* context) {
  for (byte i=0; i < 32; i++) {
    _all[i]->update(i, context);
  }
}

Layout* Layout::next() {
  return _next;
}

void Layout::setNext(Layout *layout) {
  _next = layout;
}
