/**
 * Generic LFO generators that simply output a float of -1 to 1 given a discrete
 * length for the cycle.
 */
class Wave {
  public:
      virtual float next() = 0;
      virtual float value() = 0;
      virtual void reset() = 0;
      virtual void setLength(int length) = 0;
};
  
class CoreWave : public Wave {
  protected:
    int length;
    int pos;  
    float _value;
  public:
    CoreWave();
    void setLength(int length);
    float value();
    void reset();
};
 
class SineWave : public CoreWave {
  private:
    float divs;
    
  public:
    SineWave();
    float next();
    void setLength(int length);
};

class SawWave : public CoreWave {
  private:
      float slope;
  public:
      SawWave();
      float next();
      void setLength(int length);
};

class TriWave : public CoreWave {
  private:
      float slope;
  public:
      TriWave();
      float next();
      void setLength(int length);
};

class SquareWave : public CoreWave {
  public:
    SquareWave();
    float next();
};

class NegWave : public Wave {
  private:
      Wave* wave;
  public:
      NegWave(Wave* wave);
      float next();
      float value();
      void setLength(int length);
      void reset();
};

class LFO {
  private:
    int _length;
    Wave* _wave;

    SineWave sine;
    SquareWave square;
    SawWave saw;
    SawWave up;
    SawWave innerDown;
    NegWave down = NegWave(&innerDown);
    Wave* _all[4] = {&sine, &square, &up,&down};

  public:
    LFO();
    Wave** all();
    float next();
    float value();
    void reset();
    void setLength(int length);
    void setWave(Wave* wave);
};
