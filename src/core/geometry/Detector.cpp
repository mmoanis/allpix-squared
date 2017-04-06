/**
 *  @author Koen Wolters <koen.wolters@cern.ch>
 */

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include "Detector.hpp"

using namespace allpix;

Detector::Detector(std::string name,
                   std::shared_ptr<DetectorModel> model,
                   ROOT::Math::XYZVector position,
                   ROOT::Math::EulerAngles orientation)
    : name_(std::move(name)), model_(std::move(model)), position_(std::move(position)), orientation_(orientation),
      electric_field_sizes_{{0, 0, 0}}, electric_field_(nullptr), external_models_() {
    if(model_ == nullptr) {
        throw std::invalid_argument("detector model cannot be null");
    }
}
Detector::Detector(std::string name, std::shared_ptr<DetectorModel> model)
    : Detector(std::move(name), std::move(model), ROOT::Math::XYZVector(), ROOT::Math::EulerAngles()) {}

// Set and get name of detector
std::string Detector::getName() const {
    return name_;
}

// Get the type of the model
std::string Detector::getType() const {
    return model_->getType();
}

// Return the model
const std::shared_ptr<DetectorModel> Detector::getModel() const {
    return model_;
}

// Get position of detector
ROOT::Math::XYZVector Detector::getPosition() const {
    return position_;
}

// Get orientation of model (FIXME: specify clearly which convention is used)
ROOT::Math::EulerAngles Detector::getOrientation() const {
    return orientation_;
}

// Get fields in detector
ROOT::Math::XYZVector Detector::getElectricField(const ROOT::Math::XYZVector& pos) {
    if(electric_field_ == nullptr) {
        // FIXME: determine what we should do if we have no external electric field...
        return ROOT::Math::XYZVector();
    }

    // compute indices
    int x_ind = static_cast<int>(static_cast<double>(electric_field_sizes_[0]) * (pos.x() - model_->getSensorMinX()) /
                                 model_->getSensorSizeX());
    int y_ind = static_cast<int>(static_cast<double>(electric_field_sizes_[1]) * (pos.y() - model_->getSensorMinY()) /
                                 model_->getSensorSizeY());
    int z_ind = static_cast<int>(static_cast<double>(electric_field_sizes_[2]) * (pos.z() - model_->getSensorMinZ()) /
                                 model_->getSensorSizeZ());

    // check for indices within the sensor
    if(x_ind < 0 || x_ind >= static_cast<int>(electric_field_sizes_[0]) || y_ind < 0 ||
       y_ind >= static_cast<int>(electric_field_sizes_[1]) || z_ind < 0 ||
       z_ind >= static_cast<int>(electric_field_sizes_[2])) {
        // FIXME: determine what to do here, throw an exception our return zero
        return ROOT::Math::XYZVector(0, 0, 0);
    }

    size_t tot_ind = static_cast<size_t>(x_ind) * electric_field_sizes_[1] * electric_field_sizes_[2] * 3 +
                     static_cast<size_t>(y_ind) * electric_field_sizes_[2] * 3 + static_cast<size_t>(z_ind) * 3;
    return ROOT::Math::XYZVector(
        (*electric_field_)[tot_ind], (*electric_field_)[tot_ind + 1], (*electric_field_)[tot_ind + 2]);
}
// FIXME: is that a good way to provide an electric field
void Detector::setElectricField(std::shared_ptr<std::vector<double>> field, std::array<size_t, 3> sizes) {
    if(sizes[0] * sizes[1] * sizes[2] * 3 != field->size()) {
        throw std::invalid_argument("electric field does not match the given sizes");
    }
    electric_field_ = std::move(field);
    electric_field_sizes_ = sizes;
}
