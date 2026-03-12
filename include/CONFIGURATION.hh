#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

class CONFIGURATION
{
public:
    CONFIGURATION() :
        set_cryOption(0),
        set_usePreshower(0),
        set_etSdOn(true),
        set_opticalOn(true),
        set_wrappingOn(true)
    {}

    int set_cryOption;    // 0=PbF2, 1=PWO
    int set_usePreshower; // 0=no, 1=yes
    bool set_etSdOn;      // sensitive detector on/off
    bool set_opticalOn;
    bool set_wrappingOn;
};

#endif
