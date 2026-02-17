// IDigitizer.hh
#ifndef IDIGITIZER_HH
#define IDIGITIZER_HH

class IDigitizer {
public:
    virtual ~IDigitizer() = default;

    virtual void Initialize() = 0;
    virtual void Digitize() = 0;
    virtual void Reset() = 0;
};

#endif