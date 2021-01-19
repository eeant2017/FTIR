//$file${./::run.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: state.qm
// File:  ${./::run.cpp}
//
// This code has been generated by QM 4.5.1 (https://www.state-machine.com/qm).
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
//$endhead${./::run.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$declare${AOs::ctrl} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//${AOs::ctrl} ...............................................................
class ctrl : public QP::QActive {
protected:
    Q_STATE_DECL(initial);
    Q_STATE_DECL(standby);
    Q_STATE_DECL(run);
};
//$enddecl${AOs::ctrl} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 650U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.5.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//$define${AOs::ctrl} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//${AOs::ctrl} ...............................................................
//${AOs::ctrl::SM} ...........................................................
Q_STATE_DEF(ctrl, initial) {
    //${AOs::ctrl::SM::initial}
    return tran(&standby);
}
//${AOs::ctrl::SM::standby} ..................................................
Q_STATE_DEF(ctrl, standby) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::ctrl::SM::standby::START}
        case START_SIG: {
            status_ = tran(&run);
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//${AOs::ctrl::SM::run} ......................................................
Q_STATE_DEF(ctrl, run) {
    QP::QState status_;
    switch (e->sig) {
        //${AOs::ctrl::SM::run::STOP}
        case STOP_SIG: {
            status_ = tran(&standby);
            break;
        }
        default: {
            status_ = super(&top);
            break;
        }
    }
    return status_;
}
//$enddef${AOs::ctrl} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^