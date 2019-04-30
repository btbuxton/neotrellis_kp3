class Context;
class Button;

class Layout {
  protected:
    Button *_all[32];
    Layout *_next;
  public:
    Layout();
    
    Button **all();
    void refresh(Context* context);
    void update(Context* context);
    Layout* next();
    void setNext(Layout *layout);
};
