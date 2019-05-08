class Wave {
  public:
      virtual float next() = 0;
      virtual void setLength(int length) = 0;
};
  
class CoreWave : public Wave {
  protected:
    int length;
    int pos;  
    float value;
  public:
    CoreWave();
    void setLength(int length);
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
      void setLength(int length);
};
