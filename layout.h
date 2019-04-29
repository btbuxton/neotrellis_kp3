class Button;
class Context;

class Layout {
  protected:
    Button *_all[32];
    Layout *_next;
  public:
    Layout();
    
    Button **all();
    void refresh(Context* context);
    Layout* next();
    void setNext(Layout *layout);
};
