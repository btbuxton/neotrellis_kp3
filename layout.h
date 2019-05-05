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
    virtual void update(Context* context);
    Layout* next();
    void setNext(Layout *layout);
    virtual void pressed(byte key, Context* context);
    virtual void released(byte key, Context* context);
};
