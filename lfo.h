class Wave {
  public:
      virtual float next() = 0;  
};
  
class CoreWave : public Wave {
  protected:
      int pos;
      int length;
      float value;
  public:
    CoreWave(int length);
};
 
class SineWave : public CoreWave {
  private:
    float divs;
    
  public:
    SineWave(int length);
    float next();
};

class SawWave : public CoreWave {
  private:
      float slope;
  public:
      SawWave(int length);
      float next();
};

class TriWave : public CoreWave {
  private:
      float slope;
  public:
      TriWave(int length);
      float next();
};

class SquareWave : public CoreWave {
  public:
    SquareWave(int length);
    float next();
};

class NegWave : public Wave {
  private:
      Wave* wave;
  public:
      NegWave(Wave* wave);
      float next();
};
