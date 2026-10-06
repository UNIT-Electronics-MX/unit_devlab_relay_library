#include "DevLab_Relay.h"

bool DevLab_Relay::begin() {
    _verified = false;
    _bus.setClock(_clock);
    _busReady = _bus.begin();
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

bool DevLab_Relay::begin(uint8_t sdaPin, uint8_t sclPin, uint32_t clock) {
    _verified = false;
    _clock = clock;
    _bus.setClock(_clock);
    _busReady = _bus.begin(sdaPin, sclPin);
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

bool DevLab_Relay::beginRecovered(uint8_t sdaPin, uint8_t sclPin, uint32_t timeoutUs, bool restart) {
    _verified = false;
    _bus.setClock(_clock);
    _busReady = _bus.beginRecovered(sdaPin, sclPin, timeoutUs, restart);
    if (!_busReady) return false;
    return (_verified = _ddp.matchesExpectedDevice(_address, &_info));
}

void DevLab_Relay::printInfo(Print &out) const {
    DevLabDDP::printDeviceInfo(out, _address, _info, _ddp.expectedDeviceId());
}

bool DevLab_Relay::on(bool *state) {
    return _verified && _ddp.relayOn(_address, state);
}

bool DevLab_Relay::off(bool *state) {
    return _verified && _ddp.relayOff(_address, state);
}

bool DevLab_Relay::toggle(bool *state) {
    return _verified && _ddp.relayToggle(_address, state);
}

bool DevLab_Relay::set(bool enabled, bool *state) {
    return enabled ? on(state) : off(state);
}

bool DevLab_Relay::readState(bool &state) {
    return _verified && _ddp.readRelay(_address, state);
}
