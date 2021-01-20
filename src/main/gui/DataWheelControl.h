#include "VmpcComponent.h"

#include <observer/Observer.hpp>
#include <hardware/DataWheel.hpp>

class DataWheelControl 
	: public VmpcComponent
	, public moduru::observer::Observer
{
public:
	DataWheelControl(std::weak_ptr<mpc::hardware::DataWheel> dataWheel);

	~DataWheelControl() override;
	void setImage(juce::Image image, int numFrames);
	int getFrameWidth() const { return frameWidth; }
	int getFrameHeight() const { return frameHeight; }
	void paint(juce::Graphics& g) override;

	void mouseDrag(const juce::MouseEvent& event) override;
	void mouseUp(const juce::MouseEvent& event) override;

	void update(moduru::observer::Observable* o, nonstd::any arg) override;

private:
    juce::Image filmStripImage;
	int numFrames;
	int frameWidth, frameHeight;

	int dataWheelIndex = 0;
	int lastDy = 0;
	std::weak_ptr<mpc::hardware::DataWheel> dataWheel;
};
