#include <iostream>
#include <array>
#include <algorithm>
#include <string_view>
#include <variant>
#include <functional>
#include <cstdint>

enum class MonitorState : uint8_t { NOMINAL, WARNING, FAULT };

struct SignalThreshold {
    double low, high, pct, nominal_max, fault_max;
};

// interface is achieved via the abstract class
class IThresholdCheck {
public:
    virtual MonitorState check(double value) const noexcept = 0;
};

class BandCheck : public IThresholdCheck {
private:
    double band_pct;
    SignalThreshold th_;
public:
    explicit BandCheck(const SignalThreshold& th)
        : th_(th) {};
    MonitorState check(double value) const noexcept override {
        if (value >= th_.low && value <= th_.high)
            return MonitorState::NOMINAL;
        else if (value <= th_.low*(100+th_.pct)/100
                && value >= th_.high*(100+th_.pct)/100)
            return MonitorState::WARNING;
        else
            return MonitorState::FAULT;
    }
};

class MaxCheck : public IThresholdCheck {
private:
    SignalThreshold th_;
public:
    explicit MaxCheck(const SignalThreshold& th)
        : th_(th) {}
    MonitorState check(double value) const noexcept override {
        if (value <= th_.nominal_max)
            return MonitorState::NOMINAL;
        else if (value > th_.nominal_max && value <= th_.fault_max)
            return MonitorState::WARNING;
        else
            return MonitorState::FAULT;
    }
};

template<size_t N>
class SignalMonitor {
private:
    std::array<std::variant<std::string, MonitorState, SignalThreshold>, N> values_;
    std::array<SignalThreshold, N> thresholds_;
    std::array<IThresholdCheck*, N> check_ptr_;
public:
    // NOTE (dmitrii)
    // Because this class does not allocate past constructor
    // I have decided to make it super lean
    explicit SignalMonitor(const std::array<SignalThreshold, N>& thresholds)
        : thresholds_(thresholds) {}
    
    MonitorState update(std::string_view name, double value) {
        auto it = std::find_if(
            values_.begin(),
            values_.end(),
            [&name](std::variant<std::string, MonitorState> sig) {
                bool result = false;
                try {
                    result = std::get<std::string>(sig) == name;
                } catch (std::bad_variant_access const& ex) {
                    return false;
                }
                return true;
            }
        );
        if (it == values_.end()) {
            printf("Signal does not exist");
            return MonitorState::FAULT;
        }

        // check band
        size_t distance = std::distance()


        // check max
    }
};

int main(/*int argc, char * argv[]*/) {

    return 0;
}

