#ifndef COMMONDEFINES_H
#define COMMONDEFINES_H

#define AUTO_PROPERTY(TYPE, NAME)\
    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME##Changed)\
    public:\
    TYPE NAME() const {return a_##NAME;}\
    inline void NAME(TYPE value){\
        a_##NAME = value;\
        emit NAME##Changed(value);\
    }\
    Q_SIGNAL void NAME##Changed(TYPE value);\
    private: \
        TYPE a_##NAME;

#define CHECKABLE_PROPERTY(TYPE, NAME)\
    Q_PROPERTY(TYPE NAME READ NAME WRITE NAME NOTIFY NAME##Changed)\
    public:\
    TYPE NAME() const {return a_##NAME;}\
    inline void NAME(TYPE value){\
        if (a_##NAME == value) return; \
        a_##NAME = value;\
        emit NAME##Changed(value);\
    }\
    Q_SIGNAL void NAME##Changed(TYPE value);\
    private: \
        TYPE a_##NAME;

#define READONLY_PROPERTY(TYPE, NAME)\
    Q_PROPERTY(TYPE NAME READ NAME CONSTANT)\
    public: \
        TYPE NAME() const {return a_##NAME;}\
    private: \
        void NAME(TYPE value) {a_##NAME = value;}\
        TYPE a_##NAME;


#endif // COMMONDEFINES_H

